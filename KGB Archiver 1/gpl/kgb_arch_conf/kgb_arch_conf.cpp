// kgb_arch_conf.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "kgb_arch_conf.h"
#include "kgb_arch_confDlg.h"
#include "main.h"
#include <atlbase.h>
#include "../common/homedir.cpp"
#include "../common/lang.cpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ckgb_arch_confApp

BEGIN_MESSAGE_MAP(Ckgb_arch_confApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Ckgb_arch_confApp construction

Ckgb_arch_confApp::Ckgb_arch_confApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Ckgb_arch_confApp object

Ckgb_arch_confApp theApp;


// Ckgb_arch_confApp initialization

BOOL Ckgb_arch_confApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	
	CWinApp::InitInstance();
	 AfxEnableControlContainer();
	 
	/*DWORD langID = 0x0409;
	CRegKey reg(HKEY_CURRENT_USER);
	if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
		//MessageBox(0, "", "", 0);
		reg.QueryDWORDValue("lang", langID);
	}

	//WORD lang = MAKELANGID(langID, 0x00);
	LCID loc = MAKELCID(langID, SORT_DEFAULT);
	//ConvertDefaultLocale(loc);
	SetThreadLocale(loc);*/
	 setLang();
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	char *cmd = theApp.m_lpCmdLine;
	Ckgb_arch_confDlg dlg;
	//dlg.ShowWindow(SW_HIDE);
	if(cmd[0] != '\0')
		dlg.Create(IDD_KGB_ARCH_CONF_DIALOG);
	dlg.ShowWindow(SW_HIDE);
	
	//dlg.cmdline = true;
	if(cmd[0] == '/' && cmd[1] == 'i'){
		CRegKey reg;
		if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
			theApp.ExitInstance();
			return false;
		}
		dlg.cContextMenu.SetCheck(1);
		dlg.cAttachFiles.SetCheck(1);
		dlg.cAssocZIP.SetCheck(1);
		dlg.cUpdates.SetCheck(1);
		dlg.cLang.SetCurSel(1);
		dlg.cStats.SetCheck(1);
		dlg.OnBnClickedButton3();
		theApp.ExitInstance();
		return false;
	}/*else if(cmd[0] == '/' && cmd[1] == 'u'){
		//domyslnie wy³aczone
		dlg.OnBnClickedButton3();
		theApp.ExitInstance();
		return false;
	}*/else if(cmd[0] == '/' && cmd[1] == 'v'){
		dlg.cmdline = true;
		dlg.OnBnClickedButton4();
		//dlg.EndDialog(IDOK);
		return false;
	}
	dlg.cmdline = false;//dla updatow
	dlg.CloseWindow();

	CMain dMain;
	//dMain.CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
	dMain.DoModal();

	/*Ckgb_arch_confDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
