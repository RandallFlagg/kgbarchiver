#pragma once
#include "top10.h"


// CMain dialog

class CMain : public CDialog
{
	DECLARE_DYNAMIC(CMain)

public:
	CMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMain();

// Dialog Data
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
public:
	CTop10 eTop10;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	int cCompress;
public:
	int cDecompress;
public:
	int cCustomize;
public:
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnBnClickedOk();
};
