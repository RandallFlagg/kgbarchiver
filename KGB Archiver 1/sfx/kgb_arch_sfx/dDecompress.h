#pragma once
#include "afxwin.h"
#include "afxcmn.h"


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
	CStatic sFullSize;
	CStatic sCompressedSize;
	CStatic sRatio;
	CProgressCtrl pCurrent;
	CProgressCtrl pTotal;
	CStatic sFilename;
	CStatic sTime;
	CStatic sRemaining;
	afx_msg void OnBnClicked1014();
	CComboBox cPriority;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeCpriority();
};
