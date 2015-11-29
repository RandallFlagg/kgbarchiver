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
#if !defined(AFX_DLGCONTAINER_H__2B7585A7_C937_4D1C_9745_5BD2ABAB28E9__INCLUDED_)
#define AFX_DLGCONTAINER_H__2B7585A7_C937_4D1C_9745_5BD2ABAB28E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgContainer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgContainer dialog
#include "SAPrefsSubDlg.h"
class CDlgContainer : public CDialog
{
// Construction
public:
	enum 
	{
		dcOKOnly,
		dcOKCancel,
		dcAll
	};
	int DoModal(CSAPrefsSubDlg& sdlg, UINT nIDTitle, int iType = dcAll);
	CDlgContainer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgContainer)
	enum { IDD = IDD_CONTAINER };
	CButton	m_default;
	CButton	m_ok;
	CButton	m_cancel;
	CStatic	m_frame;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgContainer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iType;
	CSAPrefsSubDlg* m_pDlg;
	CString m_szTitle;

	// Generated message map functions
	//{{AFX_MSG(CDlgContainer)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSetDefault();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONTAINER_H__2B7585A7_C937_4D1C_9745_5BD2ABAB28E9__INCLUDED_)
