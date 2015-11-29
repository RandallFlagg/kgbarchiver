// kgb_arch_mfc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "kgb_arch_mfc.h"
#include "kgb_arch_mfcDlg.h"
#include "../common/lang.cpp"
//#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ckgb_arch_mfcApp

BEGIN_MESSAGE_MAP(Ckgb_arch_mfcApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// Ckgb_arch_mfcApp construction

Ckgb_arch_mfcApp::Ckgb_arch_mfcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Ckgb_arch_mfcApp object

Ckgb_arch_mfcApp theApp;


// Ckgb_arch_mfcApp initialization

BOOL Ckgb_arch_mfcApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	setLang();


	Ckgb_arch_mfcDlg dComp;
	m_pMainWnd = &dComp;
	dComp.DoModal(); 

	/*char *cmd = theApp.m_lpCmdLine;
	char mode=0;
	
	sscanf(cmd, "-%c", &mode);
	switch(mode){
		case 'c': m_pMainWnd = &dComp; dComp.DoModal(); break;
		case 'd': m_pMainWnd = &dDecompr; dDecompr.DoModal(); break;
		default: dComp.DoModal(); break;
	}*/


	/*INT_PTR nResponse = dlg.DoModal();
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
