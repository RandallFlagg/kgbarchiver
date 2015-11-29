#if !defined(AFX_DLGREPLACE_H__A3373527_ABC8_4DB2_B5B8_DD1CEE02FDC4__INCLUDED_)
#define AFX_DLGREPLACE_H__A3373527_ABC8_4DB2_B5B8_DD1CEE02FDC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReplace.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgReplace dialog

class CDlgReplace : public CDialog
{
// Construction
public:
	CDlgReplace(CWnd* pParent = NULL);   // standard constructor
	CString m_szZipName, m_szFileName;
	bool m_bSegm;
// Dialog Data
	//{{AFX_DATA(CDlgReplace)
	enum { IDD = IDD_REPLACE };
	CComboBox	m_combo;
	CStatic	m_icon;
	BOOL	m_bDontAsk;
	CString	m_szPrompt;
	int		m_iReplace;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReplace)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgReplace)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREPLACE_H__A3373527_ABC8_4DB2_B5B8_DD1CEE02FDC4__INCLUDED_)
