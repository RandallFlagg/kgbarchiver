// stubDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CstubDlg dialog
class CstubDlg : public CDialog
{
// Construction
public:
	CstubDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_STUB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	CEdit eDestination;
	CProgressCtrl pProgress;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void onButtonExtract();
	//DWORD WINAPI stats(LPVOID);
	//DWORD WINAPI wait(LPVOID);
	CStatic fExtract;
	CButton bCancel;
	CButton bExtract;
	CButton bBrowse;
};

CstubDlg dlg;
DWORD WINAPI extract(LPVOID lpParam);