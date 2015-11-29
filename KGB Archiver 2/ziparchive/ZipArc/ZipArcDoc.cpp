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


#include "stdafx.h"
#include "ZipArc.h"
#include <afxtempl.h>

#include "ZipArcDoc.h"
#include "ZipArcView.h"
#include "DlgPassword.h"
#include "DlgReport.h"
#include "DlgNew.h"
#include "DlgExtract.h"
#include "DlgFileInfo.h"
#include "DlgSelect.h"
#include "BrowseForFolder.h"
#include "DlgSave.h"
#include "ChildFrm.h"
#include "DropFiles.h"
#include "DlgReplace.h"
#include "DlgGetFrom.h"
#include "..\ZipArchive\ZipPlatform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZipArcDoc

IMPLEMENT_DYNCREATE(CZipArcDoc, CDocument)

BEGIN_MESSAGE_MAP(CZipArcDoc, CDocument)
	//{{AFX_MSG_MAP(CZipArcDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_FINALIZE, OnUpdateFileSave)
	ON_COMMAND(ID_COMMAND_SETPASSWORD, OnCommandSetpassword)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_TEST_SELECTED, OnUpdateCommandRead)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_EXTRACT, OnUpdateCommandExtract)
	ON_COMMAND(ID_COMMAND_ADD_FILES, OnCommandAddFiles)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_ADD_FILES, OnUpdateCommandModify)
	ON_COMMAND(ID_COMMAND_ADD_FOLDERS, OnCommandAddFolders)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REPORT, OnUpdateViewTestreport)
	ON_COMMAND(ID_VIEW_REPORT, OnViewReport)
	ON_COMMAND(ID_COMMAND_TEST_SELECTED, OnCommandTestSelected)
	ON_COMMAND(ID_COMMAND_EXTRACT, OnCommandExtractSelected)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_DELETE_SELECTED, OnUpdateCommandDeleteSelected)
	ON_COMMAND(ID_COMMAND_STOP, OnCommandStop)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_STOP, OnUpdateCommandStop)
	ON_COMMAND(ID_COMMAND_ABORT, OnCommandAbort)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_ABORT, OnUpdateCommandAbort)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REFRESH, OnUpdateNoActionAllowed)
	ON_COMMAND(ID_VIEW_FILEINFO, OnViewFileinfo)
	ON_COMMAND(ID_COMMAND_DELETE_SELECTED, OnCommandDeleteSelected)
	ON_COMMAND(ID_FILE_INFORMATION, OnFileInformation)
	ON_COMMAND(ID_COMMAND_PASTEFILES, OnCommandPasteFiles)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_PASTEFILES, OnUpdateCommandPasteFiles)
	ON_COMMAND(ID_COMMAND_SEARCHSELECT, OnCommandSearchSelect)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_RENAME, OnUpdateCommandRename)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_ADD_FOLDERS, OnUpdateCommandModify)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPTIONS, OnUpdateNoActionAllowed)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateNoActionAllowed)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_GETFROMARCHIVE, OnUpdateCommandModify)
	ON_COMMAND(ID_COMMAND_GETFROMARCHIVE, OnCommandGetfromarchive)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_FILE_SETPRIORITY_IDLE, ID_FILE_SETPRIORITY_TIMECRITICAL, OnFileSetPriority)
	ON_UPDATE_COMMAND_UI_RANGE(ID_FILE_SETPRIORITY_IDLE, ID_FILE_SETPRIORITY_TIMECRITICAL, OnUpdateFileSetPriotity)
	ON_COMMAND(ID_FILE_FINALIZE, OnFileSaveNew)
	ON_COMMAND(ID_FILE_SAVE, OnFileSaveNew)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CZipArcDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CZipArcDoc)
	DISP_FUNCTION(CZipArcDoc, "Close", Close, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CZipArcDoc, "OpenZipFile", OpenZipFile, VT_BOOL, VTS_BSTR VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IZipArc to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {CF49F80B-86E3-46ED-BF1F-B0D01B558200}
static const IID IID_IZipArc =
{ 0xcf49f80b, 0x86e3, 0x46ed, { 0xbf, 0x1f, 0xb0, 0xd0, 0x1b, 0x55, 0x82, 0x0 } };

BEGIN_INTERFACE_MAP(CZipArcDoc, CDocument)
	INTERFACE_PART(CZipArcDoc, IID_IZipArc, Dispatch)
END_INTERFACE_MAP()




/////////////////////////////////////////////////////////////////////////////
// CZipArcDoc construction/destruction

CZipArcDoc::CZipArcDoc():m_progress(this)
{
	m_bAfterException = false;
	EnableAutomation();
	AfxOleLockApp();
	m_zip.SetSpanCallback(&m_changeCallback);

	m_zip.SetCallback(&m_progress, CZipArchive::cbAll);
	m_iPriority = THREAD_PRIORITY_NORMAL;
	m_CommandMap.SetAt(ID_FILE_SETPRIORITY_IDLE,THREAD_PRIORITY_IDLE);
	m_CommandMap.SetAt(ID_FILE_SETPRIORITY_LOWEST,THREAD_PRIORITY_LOWEST); 
	m_CommandMap.SetAt(ID_FILE_SETPRIORITY_BELOWNORMAL,THREAD_PRIORITY_BELOW_NORMAL);
	m_CommandMap.SetAt(ID_FILE_SETPRIORITY_NORMAL,THREAD_PRIORITY_NORMAL);
	m_CommandMap.SetAt(ID_FILE_SETPRIORITY_ABOVENORMAL,THREAD_PRIORITY_ABOVE_NORMAL);
	m_CommandMap.SetAt(ID_FILE_SETPRIORITY_HIGHEST,THREAD_PRIORITY_HIGHEST);
	m_CommandMap.SetAt(ID_FILE_SETPRIORITY_TIMECRITICAL,THREAD_PRIORITY_TIME_CRITICAL);
	m_szLastExtractPath = CZipArcApp::GetOptions()->m_szLastExtractFolder;
	m_pView = NULL;	
}

CZipArcDoc::~CZipArcDoc()
{
	AfxOleUnlockApp();
}

CString CZipArcDoc::GetFileName()
{
	CZipPathComponent zpc(GetPathName());
	return zpc.GetFileName();
}






/////////////////////////////////////////////////////////////////////////////
// CZipArcDoc serialization


/////////////////////////////////////////////////////////////////////////////
// CZipArcDoc diagnostics

#ifdef _DEBUG
void CZipArcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CZipArcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


ZIP_U16_U64 CZipArcDoc::GetIndex(ZIP_U16_U64 nListIndex)
{
	return m_bSorted ? m_zip.GetFindFastIndex(nListIndex) : nListIndex;
}

bool CZipArcDoc::SetSorted()
{
	CWaitCursor cw;
	bool bSorted = CZipArcApp::GetOptions()->m_bListSorted != 0;
	if (m_bSorted == bSorted)
		return false;
	m_bSorted = bSorted;
	m_pView->m_bRefreshNeeded = true;
	m_pView->DeselectAllItems();
	m_zip.EnableFindFast(m_bSorted);
	return true;

}

void CZipArcDoc::Init(CZipArcView *pView)
{
	m_pView = pView;
	m_progress.SetFrame(pView->GetParentFrame());
	m_bSorted = CZipArcApp::GetOptions()->m_bListSorted == 0; // opposite for SetSorted to work
	m_menu.LoadMenu(IDR_ZIPARCTYPE);
	MenuSaveFinalizeChange(m_zip.GetSegmMode() != 0, ModifyMenu(false));
	
	SetSorted();
}

CZipFileHeader* CZipArcDoc::GetFileInfo(ZIP_U16_U64 idx)
{
	return m_zip.GetFileInfo(GetIndex(idx));

}

/////////////////////////////////////////////////////////////////////////////
// CZipArcDoc commands

int CZipArcDoc::ModifyMenu(bool bAction)
{
	
	CMenu menu;
	menu.LoadMenu(bAction ? IDR_COMMAND_STOP : IDR_COMMAND_ACTION);

	CChildFrame* pFrame = ((CChildFrame*)m_pView->GetParentFrame());
	int iSubMenu = pFrame->GetMenuIndex();
	CMenu* pm = m_menu.GetSubMenu(iSubMenu);
	if (pm && IsMenu(pm->m_hMenu))
		pm->DestroyMenu();
	m_menu.ModifyMenu(iSubMenu, MF_BYPOSITION|MF_POPUP, (UINT) menu.Detach(), CString((LPCTSTR)IDS_COMMAND));
	CZipArcApp::GetFrame()->DrawMenuBar();
	return iSubMenu;
}

void CZipArcDoc::MenuSaveFinalizeChange(bool bSaveToFinalize, int iSubMenu)
{
	const int iMenuItemSavePos = 3;
	if (iSubMenu == -1)
	{
		CChildFrame* pFrame = ((CChildFrame*)m_pView->GetParentFrame());
		iSubMenu = pFrame->GetMenuIndex();
	}

	CMenu* pMenu = m_menu.GetSubMenu(iSubMenu - 1);
	if (pMenu)
		if (bSaveToFinalize)
			pMenu->ModifyMenu(iMenuItemSavePos, MF_BYPOSITION|MF_STRING, ID_FILE_FINALIZE, CString((LPCTSTR)IDS_FINALIZE));
		else
			pMenu->ModifyMenu(iMenuItemSavePos, MF_BYPOSITION|MF_STRING, ID_FILE_SAVE, CString((LPCTSTR)IDS_FILE_SAVE));
	CZipArcApp::GetFrame()->ChangeSaveToFinalize(bSaveToFinalize);
}

void CZipArcDoc::OnCloseDocument() 
{
	Close(m_bAfterException);
	if (m_bOpenOK)
		CZipArcApp::GetFrame()->SetMessageText(AFX_IDS_IDLEMESSAGE);
	CDocument::OnCloseDocument();
}

BOOL CZipArcDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	DeleteContents();
	SetModifiedFlag(FALSE);
	m_options.GetFromGlobal();
	return OpenZipFile(lpszPathName, CZipArchive::zipOpen, 
		CZipArcApp::GetOptions()->m_bTDOnRemovable ? 1 : 0);
}


void CZipArcDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	BOOL bEnable = !m_zip.IsReadOnly() && !IsActionInProgress() && IsModified();
	pCmdUI->Enable(bEnable);
}



void CZipArcDoc::OnUpdateCommandRead(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_zip.GetSegmMode() <= 0 && m_zip.GetCount() > 0 && m_pView->GetListCtrl().GetSelectedCount() && !IsActionInProgress());
}

void CZipArcDoc::OnUpdateCommandExtract(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_zip.GetSegmMode() <= 0 && m_zip.GetCount() > 0  && !IsActionInProgress());
}


void CZipArcDoc::OnUpdateCommandModify(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_zip.IsReadOnly() && !IsActionInProgress());
	
}

void CZipArcDoc::OnUpdateCommandRename(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_zip.IsReadOnly() && !IsActionInProgress() && m_pView->GetListCtrl().GetSelectedCount() == 1);
	
}


void CZipArcDoc::OnUpdateCommandPasteFiles(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_zip.IsReadOnly() && !IsActionInProgress() && IsClipboardFormatAvailable(CF_HDROP));	
}

void CZipArcDoc::OnUpdateViewTestreport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_progress.m_report.IsEmpty() && !IsActionInProgress());	
}

void CZipArcDoc::OnUpdateCommandDeleteSelected(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_zip.GetSegmMode() && !m_zip.IsReadOnly() && m_pView->GetListCtrl().GetSelectedCount() && !IsActionInProgress());
}


void CZipArcDoc::OnUpdateFileSetPriotity(CCmdUI* pCmdUI) 
{
	// there is no point in stopping when there is aborting in progress
	if (IsActionInProgress())
		pCmdUI->Enable(FALSE);
	else
	{
		int iPriority;
		if (m_CommandMap.Lookup(pCmdUI->m_nID, iPriority))
		{
			pCmdUI->SetCheck(iPriority == m_iPriority);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}

void CZipArcDoc::OnUpdateCommandStop(CCmdUI* pCmdUI) 
{
	// there is no point in stopping when there is aborting in progress
	pCmdUI->Enable(!m_progress.IsAbortOnly() 
		&& !m_progress.IsStopping() && !m_progress.IsAborting() && IsActionInProgress());
}
void CZipArcDoc::OnUpdateCommandAbort(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_progress.IsStopOnly() && !m_progress.IsAborting() && IsActionInProgress());
}

void CZipArcDoc::OnUpdateNoActionAllowed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IsActionInProgress());	
}



void CZipArcDoc::OnCommandStop() 
{
	m_progress.Stop();
}


void CZipArcDoc::OnCommandAbort() 
{
	if (m_progress.m_iType == CZipArchive::cbSave && m_zip.GetSegmMode()
		&& AfxMessageBox(IDS_SAVNOTSAFE, MB_ICONINFORMATION|MB_YESNO) == IDNO)
		return;
	m_progress.Abort();	
}



BOOL CZipArcDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	if (IsActionInProgress())
	{
		AfxMessageBox(IDS_CANNOTCLOSE, MB_ICONWARNING);
		return FALSE;
	}
	else 
		return CDocument::CanCloseFrame(pFrame);
}




void CZipArcDoc::BuildSelectedIndexes(CZipIndexesArray &indexes)
{
	CZipIndexesArray selected;
	m_pView->BuildSelectedArray(selected);
	for (ZIP_ARRAY_SIZE_TYPE i = 0; i < selected.GetSize(); i++)
		indexes.Add(GetIndex(selected[i]));
}

void CZipArcDoc::OnFileSetPriority(UINT nID)
{
// menu dissappears anyway
// 	CMenu* pMenu = CZipArcApp::GetFrame()->GetMenu()->GetSubMenu(0)->
// 		GetSubMenu(6);
// 	ASSERT(pMenu);
// 	if (!pMenu)
// 		return;
// 	pMenu->CheckMenuRadioItem(ID_FILE_SETPRIORITY_IDLE, ID_FILE_SETPRIORITY_TIMECRITICAL, 
//       nID, MF_BYCOMMAND);
// 	
	m_CommandMap.Lookup(nID, m_iPriority);
}

void CZipArcDoc::OnViewReport() 
{
	CDlgReport dlg;
	dlg.m_bEnableEmpty = !IsActionInProgress();
	dlg.m_iType = m_progress.GetActionIDS();
	dlg.m_szReport = m_progress.m_report;
	if (dlg.DoModal() == IDOK)
		SetReport(false);
}



void CZipArcDoc::DisplayErrorMsg()
{
	AfxMessageBox(IDS_ERR, MB_ICONSTOP);
}


void CZipArcDoc::SetAfterException()
{
	DisplayErrorMsg();
	if (!m_bAfterException)
		m_bAfterException = true;

}

void CZipArcDoc::SetReport(bool bSet)
{
	if (!bSet && m_progress.m_report.IsEmpty())
		return;
	if (!bSet)
		m_progress.m_report.Empty();
	SetTitle (GetTitle());
}

void CZipArcDoc::CProgressData::Start(bool bModifyMenu)
{

	if (bModifyMenu)
	{
		// must be send otherwise it may be called after a call to CZipArcDoc::CProgressData::Finish clearing this way the error report 
		m_pDoc->GetView()->SendMessage(WM_MODIFYMENU, 1);
		m_pDoc->GetView()->SendMessage(WM_SETREPORT, 0);
	}
	m_sAction.Activate();
	m_bRet = true;
}

void CZipArcDoc::CProgressData::Begin(ZIP_U32_U64 uMax)
{
	m_uPos = 0; 
	m_uMax = uMax;
	m_iType = CZipArchive::cbNothing;
	m_bInit = true;
	m_sStop.Deactivate();
	m_sAbort.Deactivate();
	//	Send();
}

void CZipArcDoc::CProgressData::SetTotal(ZIP_U32_U64 uTotalToDo)
{
	CZipActionCallback::SetTotal(uTotalToDo);
	if (IsSubAction())
		m_bInit = true;
	else if (m_iType == CZipArchive::cbDelete) // we hide it here, because earlier we had only counting sub-action
		m_pDoc->GetView()->PostMessage(WM_SHOWHIDEVIEW, 0);
}


void CZipArcDoc::CProgressData::Send()
{
	m_pFrame->PostMessage(WM_PROGRESS, (WPARAM)this);
}

void CZipArcDoc::CProgressData::Finish(bool bModifyMenu)
{

	m_sAction.Deactivate();
	Send();
	if (m_iType == CZipArchive::cbAddTmp || m_iType == CZipArchive::cbAddStore)
		m_iType = CZipArchive::cbAdd;


	if (!m_iType || m_iType == CZipArchive::cbAdd || m_iType == CZipArchive::cbDelete
		|| m_iType == CZipArchive::cbRename || m_iType == CZipArchive::cbReplace || m_iType == CZipArchive::cbGet)
		m_pDoc->GetView()->PostMessage(WM_SHOWHIDEVIEW, 1);
	if (bModifyMenu)	
	{
		m_pDoc->GetView()->PostMessage(WM_MODIFYMENU, 0);
		m_pFrame->PostMessage(WM_FINISHED, GetActionIDS(), 
			 (LPARAM)m_pDoc);
		if (!m_report.IsEmpty())

			m_pDoc->GetView()->PostMessage(WM_SETREPORT, 1);
	}

}
 

bool CZipArcDoc::CProgressData::StepIt(ZIP_U32_U64 uStep) // detects not safe stop
{
	if (IsAborting())
		return false;
	if (!IsSubAction())
		m_uPos += uStep;
	Send();
	return true;
}

void CZipArcDoc::CReport::Append(LPCTSTR lpszMessage)
{
	if (lpszMessage)
		*this += lpszMessage;
	*this += _T("\r\n");
}
void CZipArcDoc::CReport::AppendFileName(LPCTSTR lpszFileName)
{
	if (!lpszFileName)
		return;
	CString sz;
	sz.Format(IDS_ERRINFILE, lpszFileName);
	Append(sz);
	*this += _T("\r\n"); // one more time to separate multiline errors (one from exception, one from this)
}
bool CZipArcDoc::CReport::GetFromException(CException* e)
{
	ASSERT(e);
	if (!e)
		return false;

	const int iBufSize = 512;
	TCHAR buf[iBufSize];
	e->GetErrorMessage(buf, iBufSize);
	Append(buf);
	if (CZipArcDoc::IsSafeAbort(e))
		return true;
	return false;

}
bool CZipArcDoc::RemoveFile(LPCTSTR szFile, bool bDisplay)
{
	CString sz = szFile;
	if (sz.IsEmpty())
		return false;
	TRY
	{
		ZipPlatform::RemoveFile(sz);
	}
	CATCH(CException, pEx)
	{
		if (bDisplay)
		{
			CString sz;
			sz.Format(IDS_NOTREMOVED, szFile);
			AfxMessageBox(sz, MB_ICONSTOP);
		}
		return false;	
	}
	END_CATCH
	return true;
}


void CZipArcDoc::OnCommandAddFiles() 
{

	CFileDialog cFileDlg (
	TRUE, 
	(LPCTSTR)NULL, 
	(LPCTSTR)NULL, 
	OFN_ENABLESIZING|OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_LONGNAMES|OFN_OVERWRITEPROMPT|OFN_CREATEPROMPT, 
	_T("All Files (*.*)|*.*|"));
	cFileDlg.m_ofn.lpstrTitle = _T("Add Files To Archive");
	if (cFileDlg.DoModal() != IDOK)
		return;
	POSITION pp = cFileDlg.GetStartPosition();
	CStringArray* pArray = new CStringArray;
	pArray->SetSize(1); // for SetRootPath
	while (pp)
		pArray->Add(cFileDlg.GetNextPathName(pp));
	if (pArray->GetSize() > 1)
	{
		CZipPathComponent zpc(pArray->GetAt(1));
		pArray->SetAt(0, zpc.GetFilePath());
	}
	else
		return;
	OnCommandAdd(pArray);
}

void CZipArcDoc::OnCommandPasteFiles() 
{
	
	CStringArray* pArray = new CStringArray;
	CDropFiles df;
	df.m_pszaFiles = pArray;
	if (df.CreateFromClipboard())
	{
		pArray->InsertAt(0, _T("")); // for SetRootPath
		OnCommandAdd(pArray);
	}
	else
		delete pArray;
}


void CZipArcDoc::OnCommandAddFolders() 
{
	CBrowseForFolder bf;	
	bf.hWndOwner = m_pView->m_hWnd;
	bf.strTitle.LoadString(IDS_ADDFOLDER);
	CString sz;
	if (!bf.GetFolder(sz))
		return;
	CStringArray* pArray = new CStringArray;
	pArray->Add(sz); // for SetRootPath

	OnCommandAdd(pArray);
}


void CZipArcDoc::OnFileInformation() 
{
	CDlgFileInfo dlg;
	dlg.m_pArchive = &m_zip;
	dlg.m_bReadOnly = m_zip.IsReadOnly() || IsActionInProgress();
	if (dlg.DoModal() == IDOK && dlg.m_szComment != m_zip.GetGlobalComment())
	{
		m_zip.SetGlobalComment(dlg.m_szComment);
		SetModifiedFlag();
	}
}



void CZipArcDoc::OnViewFileinfo() 
{
	CZipIndexesArray dw;
	BuildSelectedIndexes(dw);
	ZIP_ARRAY_SIZE_TYPE iSize = dw.GetSize();
	if (iSize == 0)
		return;
	CDlgFileInfo dlg;
	CZipString szComment;
	if (iSize == 1)
	{
		dlg.m_pHeader = m_zip.GetFileInfo(dw[0]);
		szComment = dlg.m_pHeader->GetComment();
	}
	dlg.m_bReadOnly = m_zip.IsReadOnly() || IsActionInProgress();
	if (dlg.DoModal() == IDOK)
	{
		// is comment modified
		if (iSize == 1 && dlg.m_szComment != szComment || iSize > 1 && !dlg.m_szComment.IsEmpty())
		{
			for (int i = 0; i < iSize; i++)
				m_zip.SetFileComment(dw[i], dlg.m_szComment);
			SetModifiedFlag();
			m_pView->ClearCache();
			m_pView->UpdateItems();
		}
	}
}


HMENU CZipArcDoc::GetDefaultMenu()
{
	return m_menu.GetSafeHmenu();
}

void CZipArcDoc::OnFileSaveNew()
{
	// we want to avoid calling by OnFileSave - DoFileSave which checks whether the file
	// exists and if not, then display a prompt dialog. This would happen 
	// in TD spanning archive where files have no zip extension, but the document name has
	DoSave(m_strPathName);
}

///////////////////// Zip operations /////////////////////
//////////////////////////////////////////////////////////



BOOL CZipArcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// prompt for the file name
	if (!DoSave(NULL))
		return FALSE;
	m_options.GetFromGlobal();
	CDlgNew dlg;
	dlg.m_pOptionsDoc = &m_options;
	dlg.m_pTemplate = GetDocTemplate();
	dlg.m_szFileName = m_strPathName;

	BOOL b = dlg.DoModal() == IDOK;
	if (b)
	{
		if (ZipPlatform::FileExists(dlg.m_szFileName) == 1)
			RemoveFile(dlg.m_szFileName);
		if (!OpenZipFile(dlg.m_szFileName, dlg.m_optionsNew.m_bUseSegm ? CZipArchive::zipCreateSegm : CZipArchive::zipCreate,
			dlg.m_optionsNew.m_iSegm == 0 ? 0 : dlg.m_optionsNew.m_iVolume))
				b = false;
		else
		{
			
			SetModifiedFlag();
		}
	}

	return b;
}

BOOL CZipArcDoc::OpenZipFile(LPCTSTR lpszZipFileName, long iMode, long iVolumeSize) 
{
	bool bRep = m_bOpenOK = true;

	do 
	{
		TRY
		{
			// TODO: [doc] to use zip64 (lots of files) disable built-in windows xp zip support or it will severly slow down opening the files
			m_bAfterException = false;
			m_zip.SetAdvanced(1500000);
			m_zip.Open(lpszZipFileName, iMode, iVolumeSize);
			CZipPathComponent zpc(lpszZipFileName);
			if (!zpc.GetFileExt().CompareNoCase(_T("jar")))
				m_zip.SetIgnoreCRC();


		}
		CATCH(CException, pEx)
		{
			if (iMode == CZipArchive::zipOpen)
			{
				if (pEx->IsKindOf(RUNTIME_CLASS(CFileException))
					&& (((CFileException*)pEx)->m_cause == CFileException::accessDenied
						|| ((CFileException*)pEx)->m_cause == CFileException::sharingViolation))
				{
					// we may have no write acces, try to open in read-only mode
					iMode = CZipArchive::zipOpenReadOnly;
					continue;
				}
				else if (pEx->IsKindOf(RUNTIME_CLASS(CZipException)) && 
				((CZipException*)pEx)->m_iCause == CZipException::cdirNotFound &&
				ZipPlatform::IsDriveRemovable(lpszZipFileName))
				{
					if (AfxMessageBox(IDS_NOCDONREMOV, MB_YESNO|MB_ICONWARNING) == IDYES)
					{
						// we must close here
						m_zip.Close(CZipArchive::afAfterException);
						continue;
					}
					else 
						bRep = false;
				}
			}
			m_bOpenOK = false;
			CReport rep;
			rep.GetFromException(pEx);
			if (bRep)
				MessageBox(AfxGetMainWnd()->m_hWnd, rep, CString((LPCTSTR)IDS_OPENFAILED), MB_ICONSTOP);
			break;			
			
		}
		END_CATCH
		break;
	} while (true);

	
	return m_bOpenOK != 0;
}

BOOL CZipArcDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	if (!m_zip.IsClosed() && IsModified())
	{
		bool bIsSegmBefore = m_zip.GetSegmMode() > 0;
		m_progress.Start();
		AfxBeginThread(Flush, &m_progress, m_iPriority);
		m_progress.WaitForNoAction();  // can be called when exiting so we need it
		if (bIsSegmBefore && !m_zip.GetSegmMode())
		{
			// a segmented archive after finalizing turned out to be a normal archive (one-segment only)
			// so we update the menus
			MenuSaveFinalizeChange(false);
		}
		if (m_progress.m_bRet)
		{
			m_bAfterException = false;
			SetModifiedFlag(FALSE);
		}
		else
			return FALSE;
	}
	
	return TRUE;
}


BOOL CZipArcDoc::SaveModified() 
{
// 	return CDocument::SaveModified();
	if (IsModified())
	{
		if (!CZipArcApp::GetOptions()->m_bAlwaysSave)
		{
			CDlgSave dlg;
			dlg.m_szTitle = GetFileName();
			int iRet = dlg.DoModal();
			if (iRet == IDNO)
			{
				m_zip.Close(CZipArchive::afAfterException); // do not write anything
				return TRUE;
			}
			else if (iRet == IDCANCEL)
				return FALSE;
		}
		return OnSaveDocument(NULL);
	}
	return TRUE;
}
void CZipArcDoc::Close(BOOL bAfterException) 
{
	
	if (!m_bOpenOK)
		return;
	// should already be flushed or closed (when user says "no" to save)
	ASSERT(!IsModified() || m_zip.IsClosed());
	TRY
	{
		m_zip.Close(bAfterException ? CZipArchive::afWriteDir : CZipArchive::afNoException, true);

	}
	CATCH(CException, pEx)
	{
		m_zip.Close(CZipArchive::afAfterException);		// this time for sure	
	}
	END_CATCH

}

UINT CZipArcDoc::Flush( LPVOID pParam )
{
	CProgressData& data = *((CProgressData*)pParam);
	CZipArchive& zip = data.GetDoc()->m_zip;
	ASSERT(zip.GetSegmMode() >= 0);
	data.Begin(zip.GetCount());
	TRY
	{

		zip.Flush();

	}
	CATCH(CException, pEx)
	{
		data.m_report.GetFromException(pEx);
		if (!IsSafeAbort(pEx))
			data.GetDoc()->SetAfterException();
		data.m_bRet = false;
	}
	END_CATCH

	
	data.Finish();
	return 0;
}



void CZipArcDoc::OnCommandSetpassword() 
{
	CDlgPassword dlg;
	dlg.m_szConfirm = dlg.m_szPass = m_zip.GetPassword();
	dlg.m_iMethod = m_zip.GetEncryptionMethod();
	if (dlg.DoModal() == IDOK)
	{
		m_zip.SetPassword(dlg.m_szPass);
		m_zip.SetEncryptionMethod(dlg.m_iMethod);
	}	
}

void CZipArcDoc::PrepareData(bool bAll) 
{
	m_progress.Start();
	CZipIndexesArray* pdw = new CZipIndexesArray;
	ZIP_U16_U64 iCount = m_zip.GetCount();
	if (bAll || iCount == m_pView->GetListCtrl().GetSelectedCount())
	{
		for (ZIP_U16_U64 i = 0; i < iCount; i++)
			pdw->Add(i);
	}
	else
		BuildSelectedIndexes(*pdw);
	m_progress.m_pData = (void*)pdw;
	
}


void CZipArcDoc::OnCommandTestSelected() 
{
	PrepareData();
	AfxBeginThread(Test, &m_progress, m_iPriority);	
}



UINT CZipArcDoc::Test( LPVOID pParam )
{
	CProgressData& data = *((CProgressData*)pParam);
	CZipArchive& zip = data.GetDoc()->m_zip;
	
	// counting bytes to test
	CZipIndexesArray* pdw = (CZipIndexesArray*)data.m_pData;
	ZIP_ARRAY_SIZE_TYPE iCount = pdw->GetSize();
	data.Begin(iCount);	
	data.m_iType = CProgressData::cbCounting;
	ZIP_U32_U64 iMax = 0;
	pdw->Sort(true);
	const int iStep = data.GetStep();	
	INT_PTR i;
	for (i = 0; i < iCount; i++)
	{
		iMax += zip.GetFileInfo((*pdw)[i])->m_uUncomprSize;
		if (!(i % iStep))
		{
			data.StepIt(iStep); // always safe break (no data is lost)
			if (data.IsStopping()) 
			{
				delete pdw;
				data.m_report.Append(CString((LPCTSTR)IDS_SAFEABORT));
				data.Finish();
				return 0;
			}
		}
	}

	// testing
	data.Continue();

	data.Begin(iMax);

	for (i = 0; i < iCount; i++)
	{
		ZIP_U16_U64 nIndex = (*pdw)[i];
		bool bAddToReport = false;
		TRY
		{
			bAddToReport = !zip.TestFile(nIndex);
			if (bAddToReport)
			{
				data.m_uTotalSoFar = 0;
				data.m_uTotalToDo = zip.GetFileInfo(nIndex)->m_uUncomprSize;// for GoOn
				data.m_report.Append(CString((LPCTSTR)IDS_NOTSUCCESS));
			}
		}
		CATCH(CException, pEx)
		{			
			bAddToReport = true;
			data.m_report.GetFromException(pEx);
		}
		END_CATCH
		bool bBreak = !data.GoOn();
		bool bSafeBreak = false;
		if (!bAddToReport && bBreak)
				bSafeBreak = true;
		if (!bBreak && !bSafeBreak && !bAddToReport)
			bSafeBreak = data.IsStopping();
		

		if (bSafeBreak)
		{
			data.m_report.Append(CString((LPCTSTR)IDS_SAFEABORT));
			bBreak = bAddToReport = true;
		}

		if (bAddToReport)
			data.m_report.AppendFileName(data.m_szFileInZip);

		if (bBreak)
			break;			
	}

	delete pdw;
	data.Finish();
	return 0;
}


void CZipArcDoc::OnCommandAdd(CStringArray *pArray)
{
	ASSERT(pArray);
	m_progress.Start();
	m_progress.m_pData = (void*)pArray;
	AfxBeginThread(Add, (LPVOID)&m_progress, m_iPriority);
}

int CZipArcDoc::SortInfo(CAddInfo* p1, CAddInfo* p2, WPARAM)
{
	if (p1->m_bDir != p2->m_bDir)
		return p1->m_bDir ? -1 : 1;
	else
		return p1->m_szName.CollateNoCase(p2->m_szName);
}

bool CZipArcDoc::AddFolder(CString szFolder, CAddInfoArray &array, CProgressData& data)
{
	szFolder.TrimRight(_T("\\"));
	CFileFindEnh ff;
	BOOL b = ff.FindFile(szFolder + _T("\\*"));
	CAddInfoArray folders;
	CAddInfoArray files;
	while (b)
	{
		b = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
			folders.Add(new CAddInfo(ff.GetFilePath(), 0, true));
		else
			files.Add(new CAddInfo(ff.GetFilePath(), ff.GetInfo()->nFileSizeLow));
		if (data.IsStopping())
			return false;
	}
	files.SetSortFunction(SortInfo, NULL);
	folders.SetSortFunction(SortInfo, NULL);
	files.Sort();
	files.m_bDestroyOnDelete = false;	
	array.Append(files);
	folders.Sort();
	bool bRecursive = data.GetDoc()->m_options.m_bRecursive != 0;
	for (int i = 0; i < folders.GetSize(); i++)
	{
		CAddInfo* pInfo = folders[i];
		array.Add(pInfo); // add the folder before adding its files											// it is not needed to add the root folder
		folders[i] = NULL;
		if (bRecursive && !AddFolder(pInfo->m_szName, array, data))
			return false;
	}
	return true;

}

UINT CZipArcDoc::Add( LPVOID pParam )
{
	CProgressData& data = *((CProgressData*)pParam);
	CZipArchive& zip = data.GetDoc()->m_zip;
	CStringArray* csa = (CStringArray*)data.m_pData;
	CAddInfoArray array;
	data.Begin(0);
	data.m_iType = CProgressData::cbCounting;
	data.Send();
	INT_PTR iSize = csa->GetSize();
	bool bContinue = true;

	// counting the sizes
	CAddInfoArray folders;
	CAddInfoArray files;
	bool bAddingFolder = iSize == 1 && !(*csa)[0].IsEmpty();
	INT_PTR i;
	for (i = 1; i < iSize; i++) // at 0 is the root path
	{
		CString sz = (*csa)[i];
		CFileStatus fs;
		if (!CFile::GetStatus(sz, fs))
			continue;
		CAddInfo* ai = new CAddInfo(sz, fs.m_size, (fs.m_attribute & FILE_ATTRIBUTE_DIRECTORY) != 0);
		if (ai->m_bDir)
		{
			ai->m_uSize = 0;
			folders.Add(ai);
		}
		else
			files.Add(ai);
		if (data.IsStopping())
		{
			bContinue = false;
			break;
		}
	}
	if (bContinue)
	{
		files.SetSortFunction(SortInfo, NULL);
		folders.SetSortFunction(SortInfo, NULL);
		files.Sort();
		files.m_bDestroyOnDelete = false;
		array.Append(files);
		folders.Sort();
	
		if (bAddingFolder) // do not add folder itself (because the root path is set, it will be empty)
			AddFolder((*csa)[0], array, data);
		else 
			for (i = 0; i < folders.GetSize(); i++)
			{
				CAddInfo* pInfo = folders[i];
				array.Add(pInfo); // add the folder before adding its files											// it is not needed to add the root folder
				folders[i] = NULL;
				bContinue = AddFolder(pInfo->m_szName, array, data);
				if (!bContinue)
					break;
			}
	}
	if (bContinue)
	{
		zip.SetRootPath(csa->GetAt(0));
		data.Continue();
	}
	delete csa;				 // delete this anyway

	if (!bContinue)
	{
		data.m_report.Append(CString((LPCTSTR)IDS_SAFEABORT));
		data.Finish();
		return 0;
	}
	
	// counting bytes to add
	iSize = array.GetSize();
	ZIP_U32_U64 iMax = 0;
	for (i = 0; i < iSize; i++)
		iMax += (ZIP_U32_U64)array[i]->m_uSize;
	COptionsDoc& options = data.GetDoc()->m_options;
	// adding
	data.Begin(iMax);
	data.GetDoc()->GetView()->PostMessage(WM_SHOWHIDEVIEW, 0);
	int iSmart =  CZipArchive::zipsmCPassDir; // that's the ususal thing
	if (options.m_bStoreSmall)
		iSmart |= CZipArchive::zipsmNotCompSmall;
	if (options.m_bClrPass)
		iSmart |= CZipArchive::zipsmCPFile0;
	if (options.m_bSmart)
		iSmart |= CZipArchive::zipsmCheckForEff;
	const int iSystems[] = {ZipCompatibility::zcDosFat, ZipCompatibility::zcUnix};
	if (data.GetDoc()->m_options.m_iSystem < sizeof (iSystems)/ sizeof(int))
		zip.SetSystemCompatibility(iSystems[data.GetDoc()->m_options.m_iSystem]);
	ASSERT(data.GetDoc()->GetSegmMode() >= 0);
	bool bClearTempPath = false;
	if (data.GetDoc()->GetSegmMode() != 0 && options.m_bSmart && !options.m_bDefault) // if use not default path for smart compression
	{
		zip.SetTempPath(options.m_szTemp);
		bClearTempPath = true;
	}
	int iLastReplaceAction = 2;
	bool bCallbackAbort = false;
	if (options.m_iFileNames == 0)
		zip.ResetStringStoreSettings();
	else if (options.m_iFileNames == 1)
		zip.SetStringStoreSettings(CP_UTF8);
	else if (options.m_iFileNames == 2)
		zip.SetStringStoreSettings(CP_UTF8, true);

	for (i = 0; i < iSize; i++)
	{
		bool bAddToReport = true; 
		bool bSafeBreak = false;

		const CAddInfo* ai = array[i];
		TRY
		{
			CZipAddNewFileInfo zanfi(ai->m_szName, false); // we have a root path set, so set it to false
			zanfi.m_iComprLevel = options.m_iLevel;
			zanfi.m_iSmartLevel = iSmart;
			bool bAdd = data.GetDoc()->UpdateReplaceIndex(zanfi.m_uReplaceIndex, ai->m_szName, iLastReplaceAction);
					
			if (bAdd)
				bAddToReport = !zip.AddNewFile(zanfi);

			if (bAddToReport)
			{
				if (!bAdd)
				{
					data.m_szFileInZip.Empty(); // may be from previous adding
					data.m_report.Append(CString((LPCTSTR)IDS_DUPLICATED));
				}
				else
					data.m_report.Append(CString((LPCTSTR)IDS_NOTSUCCESS));

				data.m_szExternalFile = ai->m_szName;
				data.m_uTotalSoFar = 0;
				data.m_uTotalToDo = (ZIP_U32_U64)ai->m_uSize;
			}

		}
		CATCH(CException, pEx)
		{
			bAddToReport = true;
			if (!CZipArcDoc::IsSafeAbort(pEx))
				data.GetDoc()->m_bAfterException = true;
			else
				bSafeBreak = true;
			data.m_report.GetFromException(pEx);
			zip.CloseNewFile(true); // allow reusing the archive
			if (IsAbort(pEx))
				bCallbackAbort = true;			
		}
		END_CATCH


		bool bBreak = !data.GoOn();
		if (!bAddToReport && bBreak)
			bSafeBreak = true;
		if (!bBreak && !bSafeBreak && !bAddToReport)
			bSafeBreak = data.IsStopping();
		
		
		if (bSafeBreak)
		{
			data.m_report.Append(CString((LPCTSTR)IDS_SAFEABORT));
			bBreak = bAddToReport = true;
		}
		
		if (bAddToReport || bSafeBreak)
			data.m_report.AppendFileName(data.m_szExternalFile + _T(" -> ") + data.m_szFileInZip);
		
		if (bBreak || bCallbackAbort)
			break;			
		
	}
	zip.SetRootPath(); // clear the root path
	if (bClearTempPath) // the options may change, so clear the temp path as well
		zip.SetTempPath();
	data.GetDoc()->SetModifiedFlag();
	data.Finish();
	return 0;
}


void CZipArcDoc::OnCommandDeleteSelected() 
{
	if (!GetView()->CanProcessDEL()) // ignore del key
		return;
	PrepareData();
	m_pView->DeselectAllItems();
	AfxBeginThread(Delete, &m_progress, m_iPriority);

}

UINT CZipArcDoc::Delete( LPVOID pParam )
{
	CProgressData& data = *((CProgressData*)pParam);
	CZipArchive& zip = data.GetDoc()->m_zip;
	
	
	CZipIndexesArray* pdw = (CZipIndexesArray*)data.m_pData;
	bool bSafeBreak = false;
	data.Begin(0); // will be initialized later
	TRY
	{
		zip.DeleteFiles((*pdw));
	}
	CATCH(CException, pEx)
	{
		if (IsSafeAbort(pEx))
			bSafeBreak = true;

		if (data.m_report.GetFromException(pEx))
			bSafeBreak = true;
		else
			data.GetDoc()->SetAfterException();
	}
	END_CATCH

	delete pdw;
	data.Finish();
	if (!bSafeBreak)
		data.GetDoc()->SetModifiedFlag();
	
	return 0;
	
}

void CZipArcDoc::Rename(int iItem, LPCTSTR lpszNewName)
{	
	ASSERT(lpszNewName);
	CZipFileHeader fh;
	ZIP_U16_U64 uRealIndex = GetIndex(iItem);
	// we need uRealIndex once more - we don't use CZipArcDoc::GetFileInfo	
	if (m_zip.GetFileInfo(uRealIndex)->GetFileName().Collate(lpszNewName) == 0)
		return;
	m_progress.Start();
	m_szLastRenameTo = lpszNewName;
	CZipIndexesArray* pdw = new CZipIndexesArray;
	pdw->Add(uRealIndex);
	m_progress.m_pData = (void*)pdw;
	AfxBeginThread(Rename, &m_progress, m_iPriority);
}




UINT CZipArcDoc::Rename( LPVOID pParam )
{
	CProgressData& data = *((CProgressData*)pParam);
	CZipArchive& zip = data.GetDoc()->m_zip;
	
	
	CZipIndexesArray* pdw = (CZipIndexesArray*)data.m_pData;
	data.Begin(0); // will be initialized later
	data.m_iType = CZipArchive::cbRename; // it is only initialized by ZipArchive library if the length of the filenames is different
	data.GetDoc()->GetView()->PostMessage(WM_SHOWHIDEVIEW, 0);
	TRY
	{
		zip.RenameFile((*pdw)[0], data.GetDoc()->m_szLastRenameTo);
	}
	CATCH(CException, pEx)
	{
		
		data.m_report.GetFromException(pEx);
		data.GetDoc()->SetAfterException();
	}
	END_CATCH
		
	delete pdw;
	data.Finish();
	return 0;
}

void CZipArcDoc::OnCommandExtractSelected() 
{
	CDlgExtract dlg;
	dlg.m_szPath = m_szLastExtractPath;
	int iTemp = CZipArcApp::GetOptions()->m_iExtract;
	CZipArcApp::GetOptions()->m_iExtract = GetView()->GetListCtrl().GetSelectedCount() ? 1 : 0;
	if (dlg.DoModal() != IDOK)
	{
		CZipArcApp::GetOptions()->m_iExtract = iTemp;
		return;
	}		
	// global and local paths (one for session , one on new session)
	CZipArcApp::GetOptions()->m_szLastExtractFolder = m_szLastExtractPath = dlg.m_szPath;
	PrepareData(CZipArcApp::GetOptions()->m_iExtract == 0);
	AfxBeginThread(Extract, &m_progress, m_iPriority);
}

UINT CZipArcDoc::Extract( LPVOID pParam )
{
	CProgressData& data = *((CProgressData*)pParam);
	CZipArchive& zip = data.GetDoc()->m_zip;
	CZipIndexesArray* pdw = (CZipIndexesArray*)data.m_pData;

	
	// convert UNC path to be understand by Windows API
	CString szExtractTo = data.GetDoc()->m_szLastExtractPath;
	if (szExtractTo.Left(2) == _T("\\\\"))
		szExtractTo = _T("\\\\?\\UNC\\") + szExtractTo.Mid(2);
	ZIP_ARRAY_SIZE_TYPE iCount = pdw->GetSize();
	ZIP_U32_U64 iMax = 0;

	// counting bytes to extract
	data.Begin(iCount);	
	data.m_iType = CProgressData::cbCounting;
	
	const int iStep = data.GetStep();	
	ZIP_ARRAY_SIZE_TYPE i;
	for (i = 0; i < iCount; i++)
	{
		iMax += zip.GetFileInfo((*pdw)[i])->m_uUncomprSize;
		if (!(i % iStep))
		{
			data.StepIt(iStep); // always safe break (no data is lost)
			if (data.IsStopping()) 
			{
				delete pdw;
				data.m_report.Append(CString((LPCTSTR)IDS_SAFEABORT));
				data.Finish();
				return 0;
			}
		}

	}
	
	// extract
	data.Continue();

	data.Begin(iMax);
	BOOL bFullPath = CZipArcApp::GetOptions()->m_bFullPath;
	BOOL bOverwrite = CZipArcApp::GetOptions()->m_bOverwrite;
	pdw->Sort(true); // for faster operations
	bool bCallbackAbort = false;
	for (i = 0; i < iCount; i++)
	{
		ZIP_U16_U64 nIndex = (*pdw)[i];
		bool bAddToReport = false; 
		bool bSafeBreak = false;
		TRY
		{
			bool bSkip = false;
			CZipFileHeader* pFh = zip.GetFileInfo(nIndex);
			VERIFY(pFh);
			data.m_uTotalSoFar = 0;
			data.m_uTotalToDo = pFh->m_uUncomprSize;// for GoOn
			if (!bOverwrite)
			{
				
				CString szPredicted = zip.PredictExtractedFileName(pFh->GetFileName(), szExtractTo, bFullPath != 0);
				if (ZipPlatform::FileExists(szPredicted))
				{
					data.m_report.Append(CString((LPCTSTR)IDS_EXISTS));	
					data.m_szExternalFile = szPredicted;
					data.m_szFileInZip = pFh->GetFileName();
					if (data.m_iType == CZipArchive::cbNothing)
						data.m_iType = CZipArchive::cbExtract;
					bSkip = bAddToReport = true;
				}
			}
			if (!bSkip)
			{
				bAddToReport = !zip.ExtractFile(nIndex,szExtractTo, bFullPath != 0);
				if (bAddToReport)
					data.m_report.Append(CString((LPCTSTR)IDS_NOTSUCCESS));
			}

		}
		CATCH(CException, pEx)
		{
			bAddToReport = true;
			data.m_report.GetFromException(pEx);
			zip.CloseFile(NULL, true); // allow reusing the archive
			if (!IsSafeAbort(pEx))
				RemoveFile(data.m_szExternalFile);  // remove partially extracted file
			else
				bSafeBreak = true;
			if (IsAbort(pEx))
				bCallbackAbort = true;
		}
		END_CATCH
		bool bBreak = !data.GoOn();
		if (!bAddToReport && bBreak)
			bSafeBreak = true;
		if (!bBreak && !bSafeBreak && !bAddToReport)
			bSafeBreak = data.IsStopping();
		

		if (bSafeBreak)
		{
			data.m_report.Append(CString((LPCTSTR)IDS_SAFEABORT));
			bBreak = bAddToReport = true;
		}

		if (bAddToReport)
			data.m_report.AppendFileName(data.m_szFileInZip + _T(" -> ") + data.m_szExternalFile);

		if (bBreak || bCallbackAbort)
			break;			
	}

	delete pdw;
	data.Finish();
	return 0;
}

void CZipArcDoc::OnCommandSearchSelect() 
{
	CDlgSelect dlg;
	if (dlg.DoModal())
	{
		CZipIndexesArray ar;
		COptions* pOptions = CZipArcApp::GetOptions();
		bool bCS = pOptions->m_bSearchCaseSensitive != 0;
		if (bCS)
			m_zip.SetCaseSensitivity(true); // the default for Windows is false
		m_zip.FindMatches(pOptions->m_szSearchPattern, ar, !pOptions->m_bSearchFileName);
		if (bCS)
			m_zip.SetCaseSensitivity(false); // restore it
		if (m_bSorted)
		{
			CZipIndexesArray arr;
			ZIP_U16_U64 iCount = m_zip.GetCount();
			ZIP_ARRAY_SIZE_TYPE iSize = ar.GetSize();
			for (ZIP_ARRAY_SIZE_TYPE i =0; i < iSize; i++)
			{
				ZIP_U16_U64 nIndexToSelect = ar[i];
				for (ZIP_U16_U64 j = 0; j < iCount; j++)
					if (m_zip.GetFindFastIndex(j) == nIndexToSelect)
					{
						arr.Add(j);
						break;
					}
			}
			ASSERT(arr.GetSize() == iSize);
			GetView()->SelectArray(arr);
		}
		else
			GetView()->SelectArray(ar);
	}
}





bool CZipArcDoc::UpdateReplaceIndex(ZIP_U16_U64& uReplaceIndex, const CString &szFileName, int& iLastReplaceAction)
{
	
	bool bAdd = true;
	bool bIsSegm = m_zip.GetSegmMode() != 0;
	if (m_options.m_bReplace)
	{
		ZIP_U16_U64 idx = m_zip.WillBeDuplicated(szFileName, false);
		if (idx != ZIP_FILE_INDEX_NOT_FOUND)
		{
			int iAction = m_options.m_iReplaceAction;
			if (bIsSegm) 
			{
				if (iAction == 0 || iAction == 2)
					iAction = 1;
				if (iLastReplaceAction > 1)
					iLastReplaceAction = 0;
			}
				
			if (iAction == 1) // ask
			{
				CDlgReplace dlg;
				dlg.m_iReplace = iLastReplaceAction;
				dlg.m_szZipName = GetFileName();
				dlg.m_szFileName = m_zip.GetFileInfo(idx)->GetFileName();
				dlg.m_bSegm = bIsSegm;
				if (dlg.DoModal() == IDOK)
				{
					iLastReplaceAction = dlg.m_iReplace;
					iAction = dlg.m_iReplace;
					if (bIsSegm)
					{
						// only two choices available
						if (iAction == 0)
							iAction = 1;
						else
						{
							ASSERT(iAction == 1);
							iAction = 3;
						}
					}
					if (iAction == 1)
					{
						if (dlg.m_bDontAsk)
							m_options.m_bReplace = false;
					}
					else
					{
						if (dlg.m_bDontAsk)
							m_options.m_iReplaceAction = iAction;
					}
					
				}
			}
			
			if (iAction == 0) // replace
				uReplaceIndex = idx;
			else if (iAction == 2) // if older
			{
				time_t ttime;
				if (!ZipPlatform::GetFileModTime(szFileName, ttime)
					|| m_zip.GetFileInfo(idx)->GetTime() >= ttime)
					bAdd = false;
				else
					uReplaceIndex = idx;
			}
			else if (iAction == 3) // skip
				bAdd = false;
		}
	}
	return bAdd;
}

void CZipArcDoc::OnCommandGetfromarchive() 
{
	CWinApp* pApp= AfxGetApp();
	if (!pApp)
		return;
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	CDlgGetFrom dlg;
	dlg.m_pDoc = this;
	while(pos)
	{
		CDocTemplate* pTemplate = pApp->GetNextDocTemplate(pos);
		ASSERT(pTemplate);
		POSITION posTempl = pTemplate->GetFirstDocPosition();
		while (posTempl)
		{
			CZipArcDoc* pDoc = (CZipArcDoc*)pTemplate->GetNextDoc(posTempl);
			ASSERT(pDoc);
			if (pDoc == this || pDoc->IsReadOnly() 
				|| pDoc->IsActionInProgress() || pDoc->GetCount() == 0 
				|| pDoc->GetView()->GetListCtrl().GetSelectedCount() == 0)
				continue;
			else
				dlg.m_docs.Add(pDoc);
		}
	}
	if (dlg.m_docs.GetSize() == 0)
	{
		AfxMessageBox(IDS_NO_ZIP_TO_GET_FROM, MB_OK|MB_ICONWARNING);
		return;
	}
	if (dlg.DoModal()!=IDOK || !dlg.m_pDoc || dlg.m_pDoc == this)
		return;

	m_progress.Start();
	m_progress.m_pData = (void*)dlg.m_pDoc;		
	AfxBeginThread(GetFromArchive, &m_progress, m_iPriority);
}

UINT CZipArcDoc::GetFromArchive(LPVOID pParam)
{
	CProgressData& data = *((CProgressData*)pParam);
	CZipArchive& zip = data.GetDoc()->m_zip;
	
	
	CZipArcDoc* pDoc = (CZipArcDoc*)data.m_pData;
	
	
	CZipIndexesArray dw;
	pDoc->BuildSelectedIndexes(dw);
	ZIP_ARRAY_SIZE_TYPE iSize = dw.GetSize();
	if (!iSize)
	{
		// it should have already been checked
		ASSERT(FALSE);
		return 1;
	}
	int iLastReplaceAction = 2;
	CZipArchive& zipFrom = pDoc->m_zip;
	
	bool bSafeBreak = false;
	ZIP_U32_U64 iMax = 0;
	CZipArray<ZIP_U32_U64> sizes;
	ZIP_ARRAY_SIZE_TYPE i;
	for (i = 0; i < iSize; i++)
	{
		
		ZIP_U16_U64 uIndex = dw[i];
		ZIP_U32_U64 uEndOffset;
		if (uIndex < zipFrom.GetCount() - 1)
			uEndOffset = zipFrom.GetFileInfo(uIndex+1)->m_uOffset;
		else
		{
			CZipCentralDir::Info info;
			zipFrom.GetCentralDirInfo(info);
			if (info.m_bOnDisk)
				uEndOffset = info.m_uOffset;
			else
				uEndOffset = (ZIP_U32_U64)zipFrom.GetStorage()->m_pFile->GetLength();
		}
		ZIP_U32_U64 uSize  = uEndOffset - zipFrom.GetFileInfo(uIndex)->m_uOffset;
		iMax += uSize;
		sizes.Add(uSize);
	}
	
	data.Begin(iMax); 
	data.m_iType = CZipArchive::cbGet; 
	data.GetDoc()->GetView()->PostMessage(WM_SHOWHIDEVIEW, 0);
	
	bool bCallbackAbort = false;
	for (i = 0; i < iSize; i++)
	{
		bool bAddToReport = true; 
		bool bSafeBreak = false;
		TRY
		{
			ZIP_U16_U64 uReplaceIndex = ZIP_FILE_INDEX_UNSPECIFIED;
			ZIP_U16_U64 uIndex = dw[i];
			CZipFileHeader* pFh = zipFrom.GetFileInfo(uIndex);
			VERIFY(pFh);
			bool bAdd = data.GetDoc()->UpdateReplaceIndex(uReplaceIndex, pFh->GetFileName(), iLastReplaceAction);
			
					
			if (bAdd)
				bAddToReport = !zip.GetFromArchive(zipFrom, uIndex, NULL, uReplaceIndex);

			if (bAddToReport)
			{
				if (!bAdd)
				{
					data.m_szFileInZip.Empty(); // may be from previous adding
					data.m_report.Append(CString((LPCTSTR)IDS_DUPLICATED));
				}
				else
					data.m_report.Append(CString((LPCTSTR)IDS_NOTSUCCESS));
				
				data.m_szExternalFile = pFh->GetFileName();
				data.m_uTotalSoFar = 0;
				data.m_uTotalToDo = sizes[i];
			}
			
		}
		CATCH(CException, pEx)
		{
			bAddToReport = true;
			if (!CZipArcDoc::IsSafeAbort(pEx))
				data.GetDoc()->m_bAfterException = true;
			else
				bSafeBreak = true;
			data.m_report.GetFromException(pEx);
			zip.CloseNewFile(true); // allow reusing the archive
			if (IsAbort(pEx))
				bCallbackAbort = true;			
			
		}
		END_CATCH
			
		bool bBreak = !data.GoOn();
		if (!bAddToReport && bBreak)
			bSafeBreak = true;
		if (!bBreak && !bSafeBreak && !bAddToReport)
			bSafeBreak = data.IsStopping();
		
		
		if (bSafeBreak)
		{
			data.m_report.Append(CString((LPCTSTR)IDS_SAFEABORT));
			bBreak = bAddToReport = true;
		}
		
		if (bAddToReport || bSafeBreak)
			data.m_report.AppendFileName(data.m_szExternalFile + _T(" -> ") + data.m_szFileInZip);
		
		if (bBreak || bCallbackAbort)
			break;						
	}

	data.GetDoc()->SetModifiedFlag();		
	data.Finish();
	
	return 0;
}
