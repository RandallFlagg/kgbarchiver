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

#if !defined(AFX_CHILDFRM_H__51E17F65_06A8_4B34_A14A_1A22DC4F4608__INCLUDED_)
#define AFX_CHILDFRM_H__51E17F65_06A8_4B34_A14A_1A22DC4F4608__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ProgressBar.h"
#include "MMStatusBar.h"
class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	void SetSelString(const CString sz);
	CChildFrame();

// Attributes
public:

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual CWnd* GetMessageBar();
	int GetMenuIndex();
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	HICON m_icons[5];
	bool m_bSpecialInitiated;
	CProgressBar m_progress;
	MMStatusBar  m_wndStatusBar;
	//{{AFX_MSG(CChildFrame)
	afx_msg LONG OnProgress( UINT uParam, LONG lParam );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void PaneReportClicked();
	afx_msg LONG OnFinished( UINT uParam, LONG lParam );
	afx_msg void OnUpdateSelPane(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStatusPanes(CCmdUI *pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__51E17F65_06A8_4B34_A14A_1A22DC4F4608__INCLUDED_)
