#pragma once
#include "afxwin.h"


// dOverwrite dialog

class dOverwrite : public CDialog
{
	DECLARE_DYNAMIC(dOverwrite)

public:
	dOverwrite(CWnd* pParent = NULL);   // standard constructor
	virtual ~dOverwrite();

// Dialog Data
	enum { IDD = IDD_OVERWRITE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic sOverwrite;
public:
	CStatic sOriginalSize;
public:
	CStatic sOriginalMod;
public:
	CStatic sNewSize;
public:
	CStatic sNewMod;
public:
	afx_msg void OnBnClickedByesall();
public:
	afx_msg void OnBnClickedByes();
public:
	afx_msg void OnBnClickedBno();
public:
	afx_msg void OnBnClickedBnoall();
public:
	CButton bYes;
public:
	CButton bYesAll;
public:
	CStatic iNew;
public:
	CStatic iOld;
};
