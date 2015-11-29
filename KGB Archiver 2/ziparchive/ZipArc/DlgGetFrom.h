#if !defined(AFX_DLGGETFROM_H__AF6B7BA9_66DD_4770_8A5A_1144433875F4__INCLUDED_)
#define AFX_DLGGETFROM_H__AF6B7BA9_66DD_4770_8A5A_1144433875F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGetFrom.h : header file
//

class CZipArcDoc;
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CDlgGetFrom dialog

class CDlgGetFrom : public CDialog
{
// Construction
public:
	CDlgGetFrom(CWnd* pParent = NULL);   // standard constructor
	CTypedPtrArray<CPtrArray, CZipArcDoc*> m_docs;
	CZipArcDoc* m_pDoc;
// Dialog Data
	//{{AFX_DATA(CDlgGetFrom)
	enum { IDD = IDD_GETFROM };
	CComboBox	m_combo;
	CString	m_szPrompt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGetFrom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGetFrom)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGETFROM_H__AF6B7BA9_66DD_4770_8A5A_1144433875F4__INCLUDED_)
