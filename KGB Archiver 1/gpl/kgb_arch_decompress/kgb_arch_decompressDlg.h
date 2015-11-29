// kgb_arch_decompressDlg.h : header file
//

#pragma once
#include "afxwin.h"


// Ckgb_arch_decompressDlg dialog
class Ckgb_arch_decompressDlg : public CDialog
{
// Construction
public:
	Ckgb_arch_decompressDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_KGB_ARCH_DECOMPRESS_DIALOG };

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
	afx_msg void OnBnClickedButton2();
	CEdit eArchName;
	afx_msg void OnBnClickedBbrowse();
	CEdit eDestination;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBbrowse2();
	CListCtrl lFiles;
	afx_msg void OnBnClickedCheck1();
	CButton cSelect;
	afx_msg void OnEnChangeEarchname();
public:
	CStatic sArchiveInfo;
public:
	CButton bBrowse;
public:
	int bRecentArchives;
public:
	afx_msg void OnBnClickedBrecentarchives();
public:
	CButton bRecentFiles;
public:
	CButton bRecentDirs;
public:
//	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
public:
//	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
public:
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
public:
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
//	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
public:
	afx_msg void OnBnClickedBrecentdirs();
};
