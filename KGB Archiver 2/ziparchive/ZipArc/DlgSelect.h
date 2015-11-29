#if !defined(AFX_DLGSELECT_H__48C1E4D1_5397_42E4_A2AA_D49A1C9735A3__INCLUDED_)
#define AFX_DLGSELECT_H__48C1E4D1_5397_42E4_A2AA_D49A1C9735A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelect dialog
#include "InfoButton.h"

class CDlgSelect : public CDialog
{
// Construction
public:
	CDlgSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelect)
	enum { IDD = IDD_SELECT };
	CInfoButton	m_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COptions* m_pOptions;
	// Generated message map functions
	//{{AFX_MSG(CDlgSelect)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECT_H__48C1E4D1_5397_42E4_A2AA_D49A1C9735A3__INCLUDED_)
