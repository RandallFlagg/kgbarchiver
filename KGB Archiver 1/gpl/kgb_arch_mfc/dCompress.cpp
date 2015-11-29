// dCompress.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_mfc.h"
#include "dCompress.h"

#define WM_TRAY_ICON_ID 0x0001
#define WM_TRAY_ICON_MESSAGE (WM_USER+1997)

// dCompress dialog

IMPLEMENT_DYNAMIC(dCompress, CDialog)
dCompress::dCompress(CWnd* pParent /*=NULL*/)
	: CDialog(dCompress::IDD, pParent)
{
	
}

dCompress::~dCompress()
{
}

void dCompress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS2, pTotal);
	DDX_Control(pDX, IDC_PROGRESS1, pCurrent);
	DDX_Control(pDX, IDC_FILENAME, sFilename);
	DDX_Control(pDX, IDC_FULLSIZE, sFullSize);
	DDX_Control(pDX, IDC_COMPRESSEDSIZE, sCompressedSize);
	DDX_Control(pDX, IDC_RATIO, sRatio);
	DDX_Control(pDX, IDC_ELAPSEDTIME, sTime);
	DDX_Control(pDX, IDC_REMAININGTIME, sRemaining);
	DDX_Control(pDX, IDC_CPRIORITY, cPriority);
	DDX_Control(pDX, IDC_CHECK1, cShutdown);
}


BEGIN_MESSAGE_MAP(dCompress, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_CPRIORITY, OnCbnSelchangeCpriority)
//	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
//	ON_WM_CHANGEUISTATE()
//ON_WM_QUERYUISTATE()
ON_WM_WINDOWPOSCHANGED()
//ON_WM_QUERYUISTATE()
//    MESSAGE_HANDLER(WM_TRAY_ICON_MESSAGE, MSG, TrayIcon_HandleMessage)
	ON_MESSAGE(WM_TRAY_ICON_MESSAGE, dCompress::TrayIcon_HandleMessage)
END_MESSAGE_MAP()


// dCompress message handlers

void dCompress::OnBnClickedButton1()
{
	/*if(MessageBox(loadString(IDS_ARE_YOU_SURE_CANCEL), "KGB Archiver", MB_YESNO) == IDNO)
		return;
	EndDialog(IDCANCEL);*/
	OnClose();
}

void dCompress::OnCbnSelchangeCpriority()
{
	DWORD prio[] = {HIGH_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, IDLE_PRIORITY_CLASS};
	HANDLE hProc = GetCurrentProcess();

	if(!SetPriorityClass(hProc, prio[cPriority.GetCurSel()])){
		DWORD cPrio = GetPriorityClass(hProc);
		for(int i=0;i<sizeof(prio)/sizeof(prio[0]);i++){
			if(cPrio == prio[i]){
				cPriority.SetCurSel(i);
				break;
			}
		}
		MessageBox(loadString(IDS_CANT_CHANGE_PRIORITY), "KGB Archiver");
	}

}


void dCompress::OnShowWindow(BOOL bShow, UINT nStatus)
{
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), true);
	CDialog::OnShowWindow(bShow, nStatus);

	//cPriority.SetCurSel(2);
	DWORD prio[] = {HIGH_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, IDLE_PRIORITY_CLASS};
	HANDLE hProc = GetCurrentProcess();

	DWORD cPrio = GetPriorityClass(hProc);
	for(int i=0;i<sizeof(prio)/sizeof(prio[0]);i++){
		if(cPrio == prio[i]){
			cPriority.SetCurSel(i);
			break;
		}
	}
}

void dCompress::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(MessageBox(loadString(IDS_ARE_YOU_SURE_CANCEL), "KGB Archiver", MB_YESNO) == IDNO)
		return;
	EndDialog(IDCANCEL);
	CDialog::OnClose();
}


void dCompress::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);

	GetWindowPlacement(&wp);

	if(wp.showCmd & SW_MINIMIZE){
		//dorabiamy tray'a
		
		// add the program's icon to the tray
		memset(&tray, 0, sizeof(tray));
		tray.cbSize          = sizeof(tray) ;
		tray.hWnd            = m_hWnd; // form handle gets notification message
		tray.uID             = WM_TRAY_ICON_ID; // id of icon - passed back in wParam of message
		tray.uCallbackMessage= WM_TRAY_ICON_MESSAGE; // our notification message
		tray.hIcon           = AfxGetApp()->LoadIcon(IDI_TRAY0); // icon to display
		strncpy(tray.szTip, "KGB Archiver", sizeof(tray.szTip)); // set tool tip text
		tray.szTip[sizeof(tray.szTip)-1]='0';
		tray.uFlags          =NIF_MESSAGE | NIF_ICON | NIF_TIP; // indicate modifications
		Shell_NotifyIcon(NIM_ADD, &tray); // add it

		ShowWindow(SW_HIDE);
	
	}else{
		//usuwamy tray'a
		Shell_NotifyIcon(NIM_DELETE, &tray);
	}
	

	// TODO: Add your message handler code here
}

afx_msg LRESULT dCompress::TrayIcon_HandleMessage(WPARAM wparam, LPARAM lparam)
{
  switch (lparam)
  {
    case WM_LBUTTONDBLCLK:
      // handle left mouse button double click
	  wp.showCmd = SW_RESTORE;
	  SetWindowPlacement(&wp);
	  ShowWindow(SW_SHOW);
	  CDialog::SetForegroundWindow();
	  break;
    case WM_RBUTTONDOWN:
      // handle right mouse button click
      break;
    
    // ... and so on ...

    case WM_RBUTTONDBLCLK:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
      break;
  }
  return true;
}
