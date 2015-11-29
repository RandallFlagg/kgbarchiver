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

#if !defined(AFX_DLGPASSWORD_H__92722911_267C_48DD_A5E8_5A407F6415AD__INCLUDED_)
#define AFX_DLGPASSWORD_H__92722911_267C_48DD_A5E8_5A407F6415AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgPassword dialog

class CDlgPassword : public CDialog
{
// Construction
public:
	CDlgPassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPassword)
	enum { IDD = IDD_PASSWORD };
	CString	m_szConfirm;
	CString	m_szPass;
	int m_iMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPassword)
	virtual void OnOK();
	afx_msg void OnClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPASSWORD_H__92722911_267C_48DD_A5E8_5A407F6415AD__INCLUDED_)
