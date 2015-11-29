// kgb_arch_mfcDlg.h : header file
//

#pragma once
#include "afxwin.h"

// Ckgb_arch_mfcDlg dialog
class Ckgb_arch_mfcDlg : public CDialog
{
// Construction
public:
	Ckgb_arch_mfcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_KGB_ARCH_MFC_DIALOG };

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
	afx_msg void OnBnClickedBcancel();
	CButton bCancel;
	CEdit eArchName;
	afx_msg void OnBnClickedBbrowse();
	CComboBox cOptions;
	CListCtrl lFiles;
	afx_msg void OnBnClickedBadd2compress();
	afx_msg void OnBnClickedBremovefromcompress();
	afx_msg void OnBnClickedBnext();
	//afx_msg void OnCbnSelchangeCoptions();
	//afx_msg void OnEnChangeEarchname();
	//afx_msg void OnLbnSelchangeList1();
	CButton cFullPath;
	afx_msg void OnBnClickedBadd2compress2();
	CEdit ePassword;
	CButton cSFX;
	afx_msg void OnEnChangePassword();
	afx_msg void OnBnClickedCheck1();
public:
	CButton rKGB;
public:
	CButton rZIP;
public:
	afx_msg void OnBnClickedRadioKgb();
public:
	afx_msg void OnBnClickedRadioZip();
public:
	afx_msg void OnEnChangeEarchname();
public:
	CComboBox cSpan;
public:
//	afx_msg void OnCbnSelchangeComboSpan();
public:
	afx_msg void OnCbnEditchangeComboSpan();
public:
//	afx_msg void OnCbnSelchangeComboSpan();
	void setControls();
public:
//	afx_msg void OnHdnEnddragList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
public:
	afx_msg void OnBnClickedHelp();
public:
//	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CButton bRecentArch;
public:
	afx_msg void OnBnClickedBrecentarch();
public:
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
public:
	CButton cShowPass;
public:
	afx_msg void OnBnClickedCshowpass();
	char passwdchar;
};
