#pragma once
#include "afxwin.h"


// dlgPasswd dialog

class dlgPasswd : public CDialog
{
	DECLARE_DYNAMIC(dlgPasswd)

public:
	dlgPasswd(CWnd* pParent = NULL);   // standard constructor
	virtual ~dlgPasswd();

// Dialog Data
	enum { IDD = IDD_PASSWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CEdit ePasswd;
	afx_msg void OnBnClickedOk();
	char _passwd[32];
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
