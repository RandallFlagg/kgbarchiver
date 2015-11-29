// kgb_arch_confDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include <afxwin.h>


// Ckgb_arch_confDlg dialog
class Ckgb_arch_confDlg : public CDialog
{
// Construction
public:
	Ckgb_arch_confDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_KGB_ARCH_CONF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton cAttachFiles;
public:
	CButton cContextMenu;
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton1();
public:
	CButton cUpdates;
public:
	afx_msg void OnBnClickedButton4();
public:
	CButton cAssocZIP;
public:
	CComboBox cLang;
	bool cmdline;
public:
	CButton cStats;
};
