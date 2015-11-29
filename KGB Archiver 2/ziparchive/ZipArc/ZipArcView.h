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


#if !defined(AFX_ZIPARCVIEW_H__C887E369_3DA6_4BC2_BF24_2A58D6F6A367__INCLUDED_)
#define AFX_ZIPARCVIEW_H__C887E369_3DA6_4BC2_BF24_2A58D6F6A367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define NCOLS 6

class CZipArcView : public CListView
{

protected:

	bool m_bHidden;
	static int m_iColums[NCOLS];
	struct CZipArcViewCache
	{
		CString m_sz[NCOLS];
		int m_iImage;
		CString& operator[](int i)
		{
			return m_sz[i];

		}
	};
	bool FillAvWithFh(CZipArcViewCache& av, ZIP_U16_U64 idx, bool bFast = false);
	CMap<int, int, CZipArcViewCache, CZipArcViewCache&> m_cache;
	CZipArcView();
	DECLARE_DYNCREATE(CZipArcView)

// Attributes
public:
	
	void SelectItem(ZIP_U16_U64 iItem, bool bSelect);
	CZipArcDoc* GetDocument();

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZipArcView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool CanProcessDEL();
	
	void SelectArray(CZipIndexesArray& ar);
	void SetView(int iView);
	
	enum
	{
		vsReport = LVS_REPORT,
		vsIcon = LVS_ICON,
		vsSmall = LVS_SMALLICON,
		vsList = LVS_LIST
	};
	void ClearCache();
	void UpdateItems();
	void UpdateSelText();
	bool m_bRefreshNeeded;
	void DeselectAllItems(); // more often used

	void BuildSelectedArray(CZipIndexesArray &dwArray);
	int InsertColumn(int nCol, UINT nIDColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	virtual ~CZipArcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	bool m_bHoldUpdate;
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	//{{AFX_MSG(CZipArcView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG OnRedisplay( UINT uParam, LONG lParam );
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdcachehint(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewRefresh();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LONG OnSetreport( UINT uParam, LONG lParam );
	afx_msg void OnSelectall();
	afx_msg LONG OnShowhideview( UINT uParam, LONG lParam );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnUpdateViewFileinfo(CCmdUI* pCmdUI);
	afx_msg LONG OnModifymenu( UINT uParam, LONG lParam );
	afx_msg void OnUpdateSelectall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNotHidden(CCmdUI* pCmdUI);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCommandRename();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ZipArcView.cpp
inline CZipArcDoc* CZipArcView::GetDocument()
   { return (CZipArcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZIPARCVIEW_H__C887E369_3DA6_4BC2_BF24_2A58D6F6A367__INCLUDED_)
