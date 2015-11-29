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

#if !defined(AFX_DLGNEW_H__AB49ACF3_A96F_40A9_BFE8_EEF8BE9BCCF1__INCLUDED_)
#define AFX_DLGNEW_H__AB49ACF3_A96F_40A9_BFE8_EEF8BE9BCCF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CheckStatic.h"
#include "Options.h"
class CDlgNew : public CDialog
{
// Construction
public:
	CDlgNew(CWnd* pParent = NULL);   // standard constructor
	CDocTemplate* m_pTemplate;
	CCheckStatic m_span;
	COptionsNew m_optionsNew;
	COptionsDoc* m_pOptionsDoc;
// Dialog Data
	//{{AFX_DATA(CDlgNew)
	enum { IDD = IDD_NEW };
	CString	m_szFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNew)
	afx_msg void OnBrowse();
	afx_msg void OnDefault();
	afx_msg void UpdateControls();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSmartOpt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEW_H__AB49ACF3_A96F_40A9_BFE8_EEF8BE9BCCF1__INCLUDED_)
