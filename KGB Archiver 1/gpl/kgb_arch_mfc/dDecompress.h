#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// dDecompress dialog

class dDecompress : public CDialog
{
	DECLARE_DYNAMIC(dDecompress)

public:
	dDecompress(CWnd* pParent = NULL);   // standard constructor
	virtual ~dDecompress();

// Dialog Data
	enum { IDD = IDD_DECOMPRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBcancel();
	CListCtrl lFiles;
	afx_msg void OnBnClickedBnext();
	CEdit eArchName;
	CEdit eDestination;
	afx_msg void OnBnClickedBbrowse2();
	afx_msg void OnBnClickedBbrowse();
};
