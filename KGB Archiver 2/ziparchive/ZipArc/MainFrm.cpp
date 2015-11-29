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

#include "ZipArcDoc.h"
#include "MainFrm.h"
#include "ZipArcView.h"
#include "DlgOptAbout.h"
#include "DlgOptAppear.h"
#include "DlgOptGen.h"
#include "DlgOptSmart.h"
#include "DlgOptCompr.h"
#include "SAPrefsDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// int CMainFrame::m_imgX;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_OPTIONS, OnFileOptions)
	ON_COMMAND(ID_HELP_WEBHOMEPAGE, OnHelpWebhomepage)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	int iSize = GetWindowsDirectory(NULL, 0);
	CString sz;
	if (iSize && GetWindowsDirectory(sz.GetBuffer(iSize), iSize))
	{
		sz.ReleaseBuffer();
		CZipPathComponent zpc(sz);
		sz = zpc.GetFileDrive();
		CZipPathComponent::AppendSeparator(sz);
		SHFILEINFO    shfi;
		m_small.Attach((HIMAGELIST)SHGetFileInfo( 
			sz, // we know it exists
			0, 
			&shfi, 
			sizeof(SHFILEINFO), 
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON));
// 		m_large.Attach((HIMAGELIST)SHGetFileInfo( 
// 			sz, // we know it exists
// 			0, 
// 			&shfi, 
// 			sizeof(SHFILEINFO), 
// 			SHGFI_SYSICONINDEX | SHGFI_ICON));		

	}
	
	CenterWindow();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



bool CMainFrame::CanClose(CString & szDocWithAction)
{
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	bool bAsk = true;
	while (pos)
	{
		CDocTemplate* pTempl = AfxGetApp()->GetNextDocTemplate(pos);
		POSITION posDoc = pTempl->GetFirstDocPosition();
		while (posDoc)
		{
			CZipArcDoc* pDoc = (CZipArcDoc*)pTempl->GetNextDoc(posDoc);
			if (pDoc->IsActionInProgress())
			{
				szDocWithAction = pDoc->GetFileName();
				return false;
			}
		}
	}
	return true;
}



void CMainFrame::OnDestroy() 
{
	m_small.Detach();
// 	m_large.Detach();
	CMDIFrameWnd::OnDestroy();
	
}

void CMainFrame::OnFileOptions() 
{
	COptions* pOptions = CZipArcApp::GetOptions();
	CFrameWnd* pFrame = GetActiveFrame();
	COptionsDoc* pOptionsDoc;
	bool bNo = !pFrame || pFrame == this; // GetActiveFrame return this is there are no other frames
	if (bNo) 
		pOptionsDoc = pOptions->GetDoc();
	else
	{
		CZipArcDoc* pDoc = static_cast<CZipArcDoc*>(pFrame->GetActiveDocument());
		pOptionsDoc = &pDoc->m_options;
		if (pDoc->IsReadOnly())
			bNo = true; // don't allow modify, there is no need for SetDefault button (all changes are global)

	}
		

	CSAPrefsDialog dlg;
	dlg.SetStartPage(pOptions->m_iLastOptionsPage);

	CDlgOptCompr	page1;
	page1.m_pOptions = pOptionsDoc->GetCompress();
	if (bNo)
		page1.m_bNeedDefault = false;

	CDlgOptSmart	page2;
	page2.m_pOptions = pOptionsDoc->GetSmart();
	if (bNo)
		page2.m_bNeedDefault = false;


	CDlgOptGen		page3;
	CDlgOptAppear	page4;
	CDlgOptAbout	page5;

	page3.m_pOptions = page4.m_pOptions = pOptions;

	
	dlg.AddPage(page1, IDS_PAGE1);
	dlg.AddPage(page2, IDS_PAGE2);
	dlg.AddPage(page3, IDS_PAGE3);
	dlg.AddPage(page4, IDS_PAGE4);
	dlg.AddPage(page5, IDS_PAGE5);

	BOOL bOldSort = pOptions->m_bListSorted;
	BOOL bOldAss = pOptions->m_bAssociate;

	if (dlg.DoModal() == IDOK)
	{
		pOptions->m_iLastOptionsPage = dlg.GetCurPage();
		pOptions->Save();
		if (bOldSort != pOptions->m_bListSorted)
		{
			CFrameWnd* pFrame = GetActiveFrame();
			CZipArcDoc* pActive = NULL;
			if (pFrame)
				pActive = static_cast<CZipArcDoc*>(pFrame->GetActiveDocument());
			POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
			while (pos)
			{
				CDocTemplate* pTempl = AfxGetApp()->GetNextDocTemplate(pos);
				POSITION posDoc = pTempl->GetFirstDocPosition();
				while (posDoc)
				{
					CZipArcDoc* pDoc = static_cast<CZipArcDoc*>(pTempl->GetNextDoc(posDoc));
					if (pDoc != pActive)
						pDoc->SetSorted();
				}
			}		
			if (pActive)
			{
				if (pActive->SetSorted())
					pActive->GetView()->PostMessage(WM_REDISPLAY);
			}

		}
		if (bOldAss != pOptions->m_bAssociate)
			if (!(static_cast<CZipArcApp*>(AfxGetApp()))->Register(pOptions->m_bAssociate != 0))
				AfxMessageBox(IDS_NOTREG, MB_ICONSTOP);
	}
	
}


void CMainFrame::ChangeSaveToFinalize(bool bSaveToFinalize)
{
	const int iToolBarButtonSavePos = 2;
	m_wndToolBar.SetButtonInfo(iToolBarButtonSavePos, bSaveToFinalize ? ID_FILE_FINALIZE : ID_FILE_SAVE, TBBS_BUTTON, iToolBarButtonSavePos);
}


void CMainFrame::OnHelpWebhomepage() 
{
	ShellExecute(NULL,NULL,_T("http://www.artpol-software.com"),NULL,NULL,SW_SHOW);	
}
