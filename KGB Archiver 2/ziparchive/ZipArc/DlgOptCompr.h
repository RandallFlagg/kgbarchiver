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

#if !defined(AFX_DLGOPTCOMPR_H__66C625EE_BBB3_4B55_A250_4C5B338B78EB__INCLUDED_)
#define AFX_DLGOPTCOMPR_H__66C625EE_BBB3_4B55_A250_4C5B338B78EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SAPrefsSubDlg.h"
#include "InfoButton.h"
#include "CheckStatic.h"
class CDlgOptCompr : public CSAPrefsSubDlg
{
// Construction
public:
	virtual void OnOK();
	void OnSetDefault();
	CDlgOptCompr(CWnd* pParent = NULL);   // standard constructor
	COptionsCompress* m_pOptions;
	CCheckStatic m_replace;
// Dialog Data
	//{{AFX_DATA(CDlgOptCompr)
	enum { IDD = IDD_OPTCOMPR };
	CInfoButton	m_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptCompr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptCompr)
	virtual BOOL OnInitDialog();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTCOMPR_H__66C625EE_BBB3_4B55_A250_4C5B338B78EB__INCLUDED_)
