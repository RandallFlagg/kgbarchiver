/*
* This source file is part of the ZipArc project source distribution and
* is Copyrighted 2000 - 2006 by Tadeusz Dracz (http://www.artpol-software.com/)
*
* This code may be used in compiled form in any way you desire PROVIDING 
* it is not sold for profit as a stand-alone application.
*
* This file may be redistributed unmodified by any means providing it is
* not sold for profit without the authors written consent, and
* providing that this notice and the authors name and all copyright 
* notices remains intact. 
*
* This file is provided 'as is' with no expressed or implied warranty.
* The author accepts no liability if it causes any damage to your computer.
*
*****************************************************/


#include "stdafx.h"
#include "ZipArc.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ZipArcDoc.h"
#include "ZipArcView.h"
#include "DlgOptAbout.h"
#include "DlgContainer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZipArcApp

BEGIN_MESSAGE_MAP(CZipArcApp, CWinApp)
	//{{AFX_MSG_MAP(CZipArcApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZipArcApp construction

CZipArcApp::CZipArcApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CZipArcApp object

CZipArcApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {737CCB50-E9A0-46FB-AC0C-409B5E943C40}
static const CLSID clsid =
{ 0x737ccb50, 0xe9a0, 0x46fb, { 0xac, 0xc, 0x40, 0x9b, 0x5e, 0x94, 0x3c, 0x40 } };

/////////////////////////////////////////////////////////////////////////////
// CZipArcApp initialization

BOOL CZipArcApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
#if _MSC_VER < 1300
	#ifdef _AFXDLL
		Enable3dControls();			// Call this when using MFC in a shared DLL
	#else
		Enable3dControlsStatic();	// Call this when linking to MFC statically
	#endif
#endif

	SetRegistryKey(_T("Artpol Software"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	m_options.SetInfo(_T("Settings"), _T("General"));
	// TODO: [doc] if ZipArc reports an exception under Borland, remove the registry key
	m_options.Load();
 

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_ZIPARCTYPE,
		RUNTIME_CLASS(CZipArcDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CZipArcView));
	AddDocTemplate(pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfoZipArc cmdInfo;
	ParseCommandLine(cmdInfo);

	// Check to see if launched as OLE server
	if ((cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated) 
		&& cmdInfo.m_nShellCommand != CCommandLineInfo::AppUnregister
		&& !cmdInfo.m_bRegister)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
		UnregisterFileTypes();
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	if (cmdInfo.m_bRegister)
		m_options.m_bAssociate = true;

	if (m_options.m_bAssociate) // if false, do not unregister
	{
		Register(true, cmdInfo.m_bRunEmbedded != 0); 
		if (cmdInfo.m_bRunEmbedded)
			return FALSE;
	}


	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	

	return TRUE;
}


// App command to run the dialog
void CZipArcApp::OnAppAbout()
{
	CDlgOptAbout aboutDlg;
	CDlgContainer dlg;
	dlg.DoModal(aboutDlg, IDS_PAGE5, CDlgContainer::dcOKOnly);
}

/////////////////////////////////////////////////////////////////////////////
// CZipArcApp message handlers


int CZipArcApp::ExitInstance() 
{
	m_options.Save();	
	return CWinApp::ExitInstance();
}

bool CZipArcApp::Register(bool bRegister, bool bQuiet)
{
	
	if (bRegister)
	{

		POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
		while (pos)
		{
			CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
			CString strFileTypeId, strFilterExt;			
			if (pTemplate->GetDocString(strFileTypeId, CDocTemplate::regFileTypeId)
				&& pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt)
				&& !strFileTypeId.IsEmpty() && !strFilterExt.IsEmpty())
			{
				CRegKey reg;
				CString szTemp;				
				DWORD p = _MAX_PATH;

				bool bRegister = true;
				if (reg.Open(HKEY_CLASSES_ROOT, strFilterExt) == ERROR_SUCCESS &&
#if _MSC_VER < 1300
				  reg.QueryValue(szTemp.GetBuffer(_MAX_PATH), NULL, &p) 
#else
				  reg.QueryStringValue(NULL, szTemp.GetBuffer(_MAX_PATH), &p) 
#endif
						== ERROR_SUCCESS)
				{
					
					szTemp.ReleaseBuffer();
					if (!szTemp.IsEmpty() && szTemp != strFileTypeId)
					{
						CString sz;
						sz.Format(IDS_ASSOCEXISTS, strFilterExt, szTemp);
						if (bQuiet || AfxMessageBox(sz, MB_ICONWARNING | MB_YESNO) == IDYES)
						{
							WriteProfileString(CString((LPCTSTR)IDS_UNREG), strFilterExt, szTemp);
							reg.DeleteValue(_T(""));
						}
						else
						{
							m_options.m_bAssociate = FALSE;
							bRegister = false;
						}
					}
				}
				if (bRegister)
					RegisterShellFileTypes(TRUE);
				return true;

			}
		}
	}
	else
	{
		UnregisterFileTypes();			
		UnregisterShellFileTypes();
		return true;
	}
	m_options.m_bAssociate = FALSE;
	m_options.Save();
	return false;
}

void CZipArcApp::UnregisterFileTypes()
{
	POSITION pos = m_pDocManager->GetFirstDocTemplatePosition();
	while (pos)
	{
		CDocTemplate* pTemplate = m_pDocManager->GetNextDocTemplate(pos);
		CString strFilterExt;
		
	
		if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt))
		{
			CString unreg ((LPCTSTR)IDS_UNREG);
			CString szUnreg = GetProfileString(unreg, strFilterExt);
			if (!szUnreg.IsEmpty())
			{
				CRegKey reg;
				if (reg.Open(HKEY_CLASSES_ROOT, strFilterExt) == ERROR_SUCCESS)
#if _MSC_VER < 1300
				  reg.SetValue(szUnreg);
#else
					reg.SetStringValue(NULL, szUnreg);
#endif

					
				WriteProfileString(unreg, strFilterExt, _T(""));
			}
		}

	}

}

bool CZipArcApp::GetTempPath(CString &szTemp)
{
	DWORD d = ::GetTempPath(0, NULL);
	if (d)
	{
		d = ::GetTempPath(d, szTemp.GetBuffer(d + 1));
		szTemp.ReleaseBuffer();
		if (d)
			return true;
	}
	szTemp.Empty();
	return false;

}




BOOL CZipArcApp::SaveAllModified() 
{
	CString sz;
	if (!GetFrame()->CanClose(sz))
	{
		CString szMess;
		szMess.Format(IDS_CANTINTERRUPT, sz);
		AfxMessageBox(szMess, MB_ICONWARNING);
		return FALSE;
	}
	return CWinApp::SaveAllModified();
}

CString CZipArcApp::FormatSize(ZIP_U32_U64 uSize)
{
	CString sz;
#ifdef _ZIP64
	sz.Format(_T("%I64u"), uSize);	
#else
	sz.Format(_T("%lu"), uSize);
#endif
	int iT = 0;
	for (int i = sz.GetLength() - 1; i > 0; i--)
		if (!(++iT % 3))
			sz.Insert(i, _T(" "));
		return sz;
}


