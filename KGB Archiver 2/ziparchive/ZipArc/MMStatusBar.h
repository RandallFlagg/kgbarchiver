/******************************************************
* MMStatusBar.h
* by Kiran Thonse Sanjeeva
* http://www.codeguru.com/statusbar/statdemo.html
*****************************************************/
/*
How to use:
1. Include MMStatusBar.cpp and MMStatusBar.h into your project. 

2. In the Mainframe , replace the CStatusBar with MMStatusBar. 

3. In the Mainframe Message Map, add a ON_UPDATE_COMMAND_UI_RANGE() to trap the range of pane_ids to be trapped for updating the pane texts/tooltips/icons. 

*/
#ifndef MM_STATUS_BAR_H
#define MM_STATUS_BAR_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int MM_STATUS_PANE_UPDATE		= 9999;

class MMStatusCmdUI : public CCmdUI      // class private to this file!
{
public: // re-implementations only
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText);
	virtual void SetToolTipText(LPCTSTR lpszText);
	virtual void SetWidth(int cxWidth);
	virtual void SetIcon(HICON hIcon);
};

class MMStatusBar : public CStatusBar
{
// Construction
public:
	MMStatusBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MMStatusBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bFirstTime;
	CToolTipCtrl m_oToolTip;
	virtual ~MMStatusBar();
	BOOL PreTranslateMessage(MSG *pMsg);
	void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual BOOL OnChildNotify(UINT message,WPARAM wParam,LPARAM lParam,LRESULT* pResult);
	// Generated message map functions
protected:
	//{{AFX_MSG(MMStatusBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // MM_STATUS_BAR_H
