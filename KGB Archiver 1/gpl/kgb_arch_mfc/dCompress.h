#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// dCompress dialog

class dCompress : public CDialog
{
	DECLARE_DYNAMIC(dCompress)
private:
	WINDOWPLACEMENT wp;

public:
	dCompress(CWnd* pParent = NULL);   // standard constructor
	virtual ~dCompress();

// Dialog Data
	enum { IDD = IDD_COMPRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl pTotal;
	CProgressCtrl pCurrent;
	CStatic sFilename;
	afx_msg void OnBnClickedButton1();
	CStatic sFullSize;
	CStatic sCompressedSize;
	CStatic sRatio;
	CStatic sTime;
	CStatic sRemaining;
	CComboBox cPriority;
	afx_msg void OnCbnSelchangeCpriority();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg LRESULT TrayIcon_HandleMessage(WPARAM, LPARAM);
public:	
	NOTIFYICONDATA tray;
public:
	CButton cShutdown;
public:
	afx_msg void OnClose();
public:
//	afx_msg void OnChangeUIState(UINT nAction, UINT nUIElement);
public:
//	afx_msg UINT OnQueryUIState();
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
public:
//	afx_msg UINT OnQueryUIState();
};
