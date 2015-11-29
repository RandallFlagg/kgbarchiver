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

#if !defined(AFX_DLGOPTSMART_H__BCCFC483_4EB5_4CFA_8225_182EB335EF24__INCLUDED_)
#define AFX_DLGOPTSMART_H__BCCFC483_4EB5_4CFA_8225_182EB335EF24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgOptSmart dialog
#include "SAPrefsSubDlg.h"
#include "CheckStatic.h"
#include "Options.h"
#include "InfoButton.h"
class CDlgOptSmart : public CSAPrefsSubDlg
{
// Construction
public:
	COptionsSmart* m_pOptions; 
	void OnSetDefault();
	CDlgOptSmart(CWnd* pParent = NULL);   // standard constructor
	CCheckStatic m_smart;
// Dialog Data
	//{{AFX_DATA(CDlgOptSmart)
	enum { IDD = IDD_OPTSMART };
	CButton	m_storesmall;
	CInfoButton	m_info;
	CEdit	m_temp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptSmart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgOptSmart)
	afx_msg void OnTempBrowse();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void UpdateControls();
	afx_msg void OnKillfocusTemppath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTSMART_H__BCCFC483_4EB5_4CFA_8225_182EB335EF24__INCLUDED_)
