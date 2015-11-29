/*
* This source file is part of the ZipArc project source distribution and
* is Copyrighted 2000 - 2006 by Tadeusz Dracz (http://www.artpol-software.com/)
*
* This code may be used in compiled form in any way you desire PROVIDING 
* it is not sold for profit as a stand-alone application.
*
* This file may be redistributed unmodified by any means providing it is
* not sold for profit without the authors written consent, and
* providing that this notice and the authors name and all copyright 
* notices remains intact. 
*
* This file is provided 'as is' with no expressed or implied warranty.
* The author accepts no liability if it causes any damage to your computer.
*
*****************************************************/

#if !defined(AFX_ZIPARCDOC_H__325CC963_9B16_47BC_9CC8_099C68069E35__INCLUDED_)
#define AFX_ZIPARCDOC_H__325CC963_9B16_47BC_9CC8_099C68069E35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\ZipArchive\ZipArchive.h"
#include "Signal.h"
#include "SortableObArray.h"
#include <afxinet.h>

class CZipArcView;

class CZipArcDoc : public CDocument
{
public:
	struct CChangeDisks : public CZipSpanCallback
	{
		bool Callback(ZIP_U32_U64 uProgress)
		{
			// TODO: [doc] how to use CZipSpanCallback
			CString sz;
			if (m_iCode == scDiskNeededForWrite)
				sz.Format(IDS_CHGSPACE, CZipArcApp::FormatSize(uProgress));
			else if (uProgress == scFileNameDuplicated)
				sz.Format(IDS_CHGEX, m_szExternalFile);
			else if (uProgress == scCannotSetVolLabel)
				sz.LoadString(IDS_CHGWRPR);
			else if (uProgress == scFileCreationFailure)
				sz.LoadString(IDS_CHGNOTCR);
			
			CString szMess;
			szMess.Format(IDS_CHGDSK, m_uDiskNeeded, sz);
			if (uProgress == -2)
			{
				int iAnswer = AfxMessageBox(szMess, MB_YESNOCANCEL|MB_ICONQUESTION);
				if (iAnswer == IDCANCEL)
					return false;
				if (iAnswer == IDYES)
					CZipArcDoc::RemoveFile(m_szExternalFile, true);
				return true;
			}
			else
				return AfxMessageBox(szMess, MB_OKCANCEL|MB_ICONQUESTION) == IDOK;
		}
	};
	class CFileFindEnh : public CFileFind
	{
	public:
		LPWIN32_FIND_DATA GetInfo() 
		{
			ASSERT(m_pFoundInfo);
			return (LPWIN32_FIND_DATA) m_pFoundInfo;
		}
	};
	struct CAddInfo:public CObject
	{
		CAddInfo(){}
		CAddInfo(const CString& szName, ZIP_FILE_USIZE uSize, bool bDir = false)
			:m_szName(szName), m_uSize(uSize), m_bDir(bDir){}
		CString m_szName;
		bool m_bDir;
		ZIP_FILE_USIZE m_uSize;
		
	};
	typedef CTypedSortableObArray<CAddInfo*> CAddInfoArray;
	struct CReport : public CString
	{
		void Append(LPCTSTR lpszMessage);
		void AppendFileName(LPCTSTR lpszFileName);
		bool GetFromException(CException* e); // retrun true if aborted safely
	};

	struct CProgressData : public CZipActionCallback
	{

		enum 
		{
			cbCounting = CZipArchive::cbNextValue
		};
		
		CProgressData(CZipArcDoc* pDoc):m_pDoc(pDoc)
		{
			const int iActions[] = {CZipArchive::cbNothing, CZipArchive::cbAdd,CZipArchive::cbAddTmp,CZipArchive::cbAddStore,CZipArchive::cbExtract,CZipArchive::cbDeleteCnt,
				CZipArchive::cbDelete,CZipArchive::cbTest,CZipArchive::cbSave, cbCounting, CZipArchive::cbRename, CZipArchive::cbReplace, CZipArchive::cbGet};
			const UINT ids[] = {IDS_NOTHING, IDS_PRCOMPRESS,IDS_PRCOMPRESSMOVE,IDS_PRCOMPRESSSTORE,IDS_PREXTRACT,IDS_PRDELCHECK,
				IDS_PRDELETE,IDS_PRTEST,IDS_PRSAVING,IDS_COUNTING, IDS_RENAMING, IDS_MAKING_SPACE, IDS_GETTING_FROM_ARCHIVE};
			int iCount = sizeof(iActions)/sizeof(int);
			ASSERT(iCount == sizeof(ids)/sizeof(UINT));
			for (int i = 0; i < iCount; i++)
				m_mActionToIDS[iActions[i]] = ids[i];
			m_dFactor = 1.0;
		}
		
		void* m_pData;
		void Start(bool bModifyMenu = true); // called before counting iMax for Begin
		void Begin(ZIP_U32_U64 uMax);
		bool IsActive() {return m_sAction.IsActive();}
		void Finish(bool bModifyMenu = true);
		void Continue() {Finish(false); Start(false);}
		bool GoOn(){return StepIt(LeftToDo());} // advance the progress after an exception was thrown
		bool StepIt(ZIP_U32_U64 uStep = 1);
		
		void SetFrame(CFrameWnd* pFrame){m_pFrame = pFrame;}
		CZipArcDoc* GetDoc(){return m_pDoc;}
		void Send();
		UINT GetActionIDS()
		{
			UINT u;
			VERIFY(m_mActionToIDS.Lookup(m_iType, u));
			return u;
		}
		bool IsSubAction () 
		{
			return (m_iType & CZipArchive::cbSubActions) != 0;
		}
		int GetPos()
		{
			ZIP_U32_U64 uPos = IsSubAction() ? m_uTotalSoFar : m_uPos;
			if (m_dFactor == 1.0)
				return (int)uPos;
			else
#if _MSC_VER < 1300
				return (int)((double)(__int64)uPos * m_dFactor);
#else
				return (int)((double)uPos * m_dFactor);
#endif
		}
		int GetMax()
		{
			ZIP_U32_U64 uMax;
			if (m_bInit) // parent frame has received it
				m_bInit = false; 
			if (IsSubAction() || m_iType == CZipArchive::cbDelete || m_iType == CZipArchive::cbRename)
				uMax = m_uTotalToDo; 
			else
				uMax = m_uMax;
			if (uMax > INT_MAX)
			{
#if _MSC_VER < 1300
				m_dFactor = (double)INT_MAX / (double)(__int64)uMax;
#else
				m_dFactor = (double)INT_MAX / (double)uMax;
#endif
				return INT_MAX;
			}
			else
			{
				m_dFactor = 1.0;
				return (int)uMax;
			}
		}
		
		void Stop()
		{
			m_sStop.Activate();
			m_sAction.WaitForDeactivate();
		}
		void Abort()
		{
			m_sAbort.Activate();
			m_sAction.WaitForDeactivate();
		}
		bool IsStopOnly()
		{
			//counting before deleting, saving c.d. on non-segmented archive, other counting
			return m_iType == CZipArchive::cbDeleteCnt || m_iType == CZipArchive::cbSave && m_pDoc->GetSegmMode() == 0
				|| m_iType == cbCounting;
		}
		bool IsAbortOnly()
		{			
			return m_iType == CZipArchive::cbDelete || m_iType == CZipArchive::cbSave && m_pDoc->GetSegmMode() != 0 || m_iType == CZipArchive::cbRename || m_iType == CZipArchive::cbReplace;
		}
		void WaitForNoAction() {m_sAction.WaitForDeactivate();}
		bool IsAborting() {return m_sAbort.IsActive();}
		bool IsStopping() {return m_sStop.IsActive();}
		bool IsInitNeed(){return m_bInit;} // is first call
		CReport m_report;
		
		// initiate sub actions after calling SetTotal by ZipArchive
		void SetTotal(ZIP_U32_U64 uTotalToDo);
		// (the deleting or adding action has two stages progress)
		void CallbackEnd()
		{
			if (IsSubAction())
				m_bInit = true;
		}
		bool Callback(ZIP_U32_U64 uProgress)
		{
			if (IsStopOnly() && IsStopping())
				return false; // can break safely
			return StepIt(uProgress);
		}
		bool m_bRet;
	protected:
		CMap<int, int, UINT, UINT> m_mActionToIDS;
		bool m_bInit;
		double m_dFactor;
		ZIP_U32_U64 m_uPos, m_uMax;	
		CSignal m_sAction, m_sStop, m_sAbort;
		CZipArcDoc* m_pDoc;		// what has sent
		CFrameWnd* m_pFrame;	// where to send
	};


	friend class CZipArcView;
	COptionsDoc m_options;
protected: // create from serialization only
	CChangeDisks m_changeCallback;
	void PrepareData(bool bAll = false);
	CProgressData m_progress;
	CZipArcDoc();
	DECLARE_DYNCREATE(CZipArcDoc)
	static UINT Delete( LPVOID pParam );
	static UINT Rename( LPVOID pParam );
	static UINT Test( LPVOID pParam );
	static UINT Extract( LPVOID pParam );
	static UINT Add( LPVOID pParam );
	static UINT Flush( LPVOID pParam );
	static int SortInfo(CAddInfo* p1, CAddInfo* p2, WPARAM);
	
// Attributes
public:

// Operations
public:
	static bool IsSafeAbort(CException* pEx)
	{
		if (pEx->IsKindOf(RUNTIME_CLASS(CZipException))
			&& ((CZipException*)pEx)->m_iCause == CZipException::abortedSafely)
			return true;
		else
			return false;
	}
	static bool IsAbort(CException* pEx)
	{
		if (pEx->IsKindOf(RUNTIME_CLASS(CZipException))
			&& ((CZipException*)pEx)->m_iCause == CZipException::aborted)
			return true;
		else
			return false;
	}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZipArcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	void Rename(int iItem, LPCTSTR lpszNewName);
	int ModifyMenu(bool bAction);
	void MenuSaveFinalizeChange(bool bSaveToFinalize, int iSubMenu = -1);
	HMENU GetDefaultMenu();
	static bool RemoveFile(LPCTSTR szFile, bool bDisplay = false);
	CString GetFileName();
	CZipFileHeader* GetFileInfo(ZIP_U16_U64 idx);
	void Init(CZipArcView* pView);
	bool SetSorted();
	ZIP_U16_U64 GetIndex(ZIP_U16_U64 nListIndex);
	CZipArcView* GetView(){return m_pView;}
	bool IsReportEmpty(){return m_progress.m_report.IsEmpty() != 0;}
	void SetReport(bool bSet);

	void BuildSelectedIndexes(CZipIndexesArray &indexes);
	bool IsAfterException(){return m_bAfterException;}
	bool IsReadOnly(){return m_zip.IsReadOnly();}
	int GetSegmMode(){return m_zip.GetSegmMode();}
	ZIP_U16_U64 GetCount() {return m_zip.GetCount();} 
	bool IsActionInProgress()
	{
		return m_progress.IsActive();
	}
	bool IsModifyingInProgress()
	{
		return m_progress.IsActive() && (m_progress.m_iType == CZipArchive::cbAdd || 
			m_progress.m_iType == CZipArchive::cbDelete || m_progress.m_iType == CZipArchive::cbAddStore || m_progress.m_iType == CZipArchive::cbAddTmp
			|| m_progress.m_iType == CZipArchive::cbRename || m_progress.m_iType == CZipArchive::cbReplace || m_progress.m_iType == CZipArchive::cbGet);
	}

	CProgressData& GetProgress()
	{
		return m_progress;
	}
	
	virtual ~CZipArcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void ViewReport(){OnViewReport();}
protected:
	static UINT GetFromArchive( LPVOID pParam );
	bool UpdateReplaceIndex(ZIP_U16_U64 &uReplaceIndex, const CString & szFileName, int& iLastReplaceAction);
	CMenu m_menu;
	CZipArchive m_zip;
	bool m_bSorted;
	bool m_bOpenOK;
	static bool AddFolder(CString szFolder, CAddInfoArray &array, CProgressData& data);
	void OnCommandAdd(CStringArray* pArray);
	void DisplayErrorMsg();
	void SetAfterException();
	CString m_szLastExtractPath;
	CString m_szLastRenameTo;
	CZipArcView* m_pView;
	CMap<UINT, UINT, int, int> m_CommandMap;
	int m_iPriority;
	bool m_bAfterException;
	//{{AFX_MSG(CZipArcDoc)
	afx_msg void OnUpdateFileSetPriotity(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnCommandSetpassword();
	afx_msg void OnUpdateCommandRead(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCommandExtract(CCmdUI* pCmdUI);
	afx_msg void OnCommandAddFiles();
	afx_msg void OnUpdateCommandModify(CCmdUI* pCmdUI);
	afx_msg void OnCommandAddFolders();
	afx_msg void OnUpdateViewTestreport(CCmdUI* pCmdUI);
	afx_msg void OnViewReport();
	afx_msg void OnCommandTestSelected();
	afx_msg void OnCommandExtractSelected();
	afx_msg void OnUpdateCommandDeleteSelected(CCmdUI* pCmdUI);
	afx_msg void OnCommandStop();
	afx_msg void OnUpdateCommandStop(CCmdUI* pCmdUI);
	afx_msg void OnCommandAbort();
	afx_msg void OnUpdateCommandAbort(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNoActionAllowed(CCmdUI* pCmdUI);
	afx_msg void OnViewFileinfo();
	afx_msg void OnCommandDeleteSelected();
	afx_msg void OnFileInformation();
	afx_msg void OnCommandPasteFiles();
	afx_msg void OnUpdateCommandPasteFiles(CCmdUI* pCmdUI);
	afx_msg void OnCommandSearchSelect();
	afx_msg void OnUpdateCommandRename(CCmdUI* pCmdUI);
	afx_msg void OnCommandGetfromarchive();
	//}}AFX_MSG
	afx_msg void OnFileSaveNew();
	afx_msg void OnFileSetPriority(UINT nID);
	DECLARE_MESSAGE_MAP()
	
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CZipArcDoc)
	afx_msg void Close(BOOL bAfterException);
	afx_msg BOOL OpenZipFile(LPCTSTR lpszZipFileName, long iMode, long iVolumeSize);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZIPARCDOC_H__325CC963_9B16_47BC_9CC8_099C68069E35__INCLUDED_)
