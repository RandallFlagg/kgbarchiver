#pragma once
#include "afxwin.h"


// dPasswd dialog

class dPasswd : public CDialog
{
	DECLARE_DYNAMIC(dPasswd)

public:
	dPasswd(CWnd* pParent = NULL);   // standard constructor
	virtual ~dPasswd();

// Dialog Data
	enum { IDD = IDD_DIALOG_PASSWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit ePasswd;
	wchar_t passwd[32];
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
