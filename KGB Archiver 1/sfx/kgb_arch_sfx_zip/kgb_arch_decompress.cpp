// kgb_arch_decompress.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "kgb_arch_decompress.h"
#include "kgb_arch_decompressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ckgb_arch_decompressApp

BEGIN_MESSAGE_MAP(Ckgb_arch_decompressApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// Ckgb_arch_decompressApp construction

Ckgb_arch_decompressApp::Ckgb_arch_decompressApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Ckgb_arch_decompressApp object

Ckgb_arch_decompressApp theApp;


// Ckgb_arch_decompressApp initialization

BOOL Ckgb_arch_decompressApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	/*if(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) != S_OK){
		MessageBox(0, "Nie mo¿na za³adowaæ modelu COM!", "KGB Archiver", 0);
		return 0;
	}*/


	Ckgb_arch_decompressDlg dlg;
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
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
