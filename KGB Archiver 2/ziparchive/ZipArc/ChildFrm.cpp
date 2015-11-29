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
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT indicators[] =
{
	ID_INDICATOR_SEL,
	IDS_PANE_INFO1,
	IDS_PANE_INFO2,
	IDS_PANE_INFO3,
	IDS_PANE_INFO4,
	IDS_PANE_INFO5
};


/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_MESSAGE(WM_PROGRESS,OnProgress)
	ON_WM_CREATE()
	ON_MESSAGE(WM_FINISHED,OnFinished)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_COMMAND(IDS_PANE_INFO3, PaneReportClicked)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SEL, OnUpdateSelPane)
	ON_UPDATE_COMMAND_UI_RANGE(IDS_PANE_INFO1, IDS_PANE_INFO5, OnUpdateStatusPanes)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	m_bSpecialInitiated = false;
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers


int CChildFrame::GetMenuIndex()
{
	BOOL bMaximized = FALSE;// if maximized, there is one additional submenu
	((CMDIFrameWnd*)GetParentFrame())->MDIGetActive(&bMaximized); // are we maximized?
	return bMaximized ? 2 : 1;
}


void CChildFrame::OnUpdateSelPane(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CChildFrame::OnUpdateStatusPanes(CCmdUI *pCmdUI)
{

	//Update command can get called for 2 reasons, 
	//1. Just before a command is sent to check if the command needs to be handled
	//2. As a part of idle processing, in this case the CCmdUI's would be having
	//it's m_pMenu set to MM_STATUS_PANE_UPDATE. Check the mmstatusbar.cpp for
	//MMStatusBar::OnUpdateCmdUI to see this
	//we set the pane text, icon and tooltip when we come across situation 2 as listed above

	//check if it is an Idle processing request
	BOOL bIsToolTipUpdate = FALSE;
	if(MM_STATUS_PANE_UPDATE == (int)pCmdUI->m_pMenu)
		bIsToolTipUpdate = TRUE;//yes it is !!!

	
	MMStatusCmdUI* pStatusUI = (MMStatusCmdUI*)pCmdUI;
	CString szToolTipText;

	if (bIsToolTipUpdate)
	{
		CZipArcDoc* pDoc = (CZipArcDoc*)GetActiveDocument();
		if (!pDoc)
			return;
		bool bApply = false;
		switch (pCmdUI->m_nID)
		{
		case IDS_PANE_INFO1:
			bApply = pDoc->IsModified() != 0;
			break;
		case IDS_PANE_INFO2:
			bApply = pDoc->IsReadOnly();
			break;
		case IDS_PANE_INFO3:
			bApply = !pDoc->IsReportEmpty();
			break;
		case IDS_PANE_INFO4:
			bApply = pDoc->GetSegmMode() != 0;			
			break;
		case IDS_PANE_INFO5:
			bApply = pDoc->IsAfterException();
			break;

		}
		if (bApply)
		{
			
			pStatusUI->SetIcon(m_icons[pCmdUI->m_nID - IDS_PANE_INFO1]);
			szToolTipText.LoadString(pCmdUI->m_nID);
		}
		else
			pStatusUI->SetIcon(NULL);
	}

	//set the tooltip
	if(bIsToolTipUpdate)
	{
		CClientDC dc(NULL);
		HFONT hFont;
		HGDIOBJ hOldFont;
		hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
		hOldFont = dc.SelectObject(hFont);
		pStatusUI->SetToolTipText(szToolTipText);
		pStatusUI->SetWidth(GetSystemMetrics(SM_CXSMICON));
			
		dc.SelectObject(hOldFont);
	}
	else
		pCmdUI->ContinueRouting();

}


void CChildFrame::ActivateFrame(int nCmdShow) 
{
	if (CZipArcApp::GetOptions()->m_bMaximized)
		nCmdShow = SW_SHOWMAXIMIZED;	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	int iIndicatCount = sizeof(indicators)/sizeof(UINT);
	if (!m_wndStatusBar.Create(this)
		|| !m_wndStatusBar.SetIndicators(indicators,
		  iIndicatCount))
		return -1;
	m_wndStatusBar.SetPaneStyle(0, 	m_wndStatusBar.GetPaneStyle(0) | SBPS_STRETCH);
	int i;
	for (i = 1; i < iIndicatCount; i++)
	{
		m_wndStatusBar.SetPaneInfo(i, indicators[i], m_wndStatusBar.GetPaneStyle(i), 16);
		m_wndStatusBar.SetPaneText(i, NULL);
	}
	m_progress.Create(this, NULL, 100, 1, TRUE);
	UINT icons [] = {IDI_MODIFIED,IDI_READONLY,IDI_REPORT,IDI_MULTIDISK, IDI_EXCEPT};
	ASSERT(sizeof (m_icons) / sizeof (HICON) == sizeof (icons) / sizeof (UINT));
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_MAINFRAME),RT_GROUP_ICON);
	for (i = 0; i < sizeof (m_icons) / sizeof (HICON); i++)
		m_icons[i] = (HICON)LoadImage(hInst,MAKEINTRESOURCE(icons[i]),IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	return 0;
}

LONG CChildFrame::OnProgress(UINT uParam, LONG lParam)
{
	CZipArcDoc::CProgressData& data = * ((CZipArcDoc::CProgressData*)uParam);
	if (!data.GetDoc()->IsActionInProgress())
	{
		m_progress.Clear();
	}
	else 
	{
		if (data.m_iType == CZipArchive::cbNothing)
			return 0;

		if (data.IsInitNeed() || data.IsSubAction() && !m_bSpecialInitiated)
		{
			if (m_bSpecialInitiated && !data.IsSubAction())
				m_bSpecialInitiated = false;
			if (data.IsSubAction())
				m_bSpecialInitiated = true;

			m_progress.Show();
			CString szMess;
			szMess.LoadString(data.GetActionIDS());
			m_progress.SetText(szMess);
			m_progress.SetRange(0, data.GetMax());			
		}
		m_progress.SetPos(data.GetPos());
	}
	return 0;
}

void CChildFrame::SetSelString(const CString sz)
{
	m_wndStatusBar.SetPaneText(0, sz);
	m_wndStatusBar.UpdateWindow();
}

CWnd* CChildFrame::GetMessageBar()
{
	return GetDescendantWindow(AFX_IDW_STATUS_BAR, TRUE);
}


LONG CChildFrame::OnFinished( UINT uParam, LONG lParam)
{
	CString sz;
	CZipArcDoc* pDoc = (CZipArcDoc*)lParam;
	sz.Format(IDS_FINISHED, pDoc->GetFileName(), CString((LPCTSTR)uParam), CString((LPCTSTR)(pDoc->IsReportEmpty() ? IDS_FINOK : IDS_FINNO)));
	GetParentFrame()->SetMessageText(sz);
	return 0;

}


void CChildFrame::OnDestroy() 
{
	for (int i = 0; i < sizeof (m_icons) / sizeof (HICON); i++)
		DestroyIcon(m_icons[i]);
	CMDIChildWnd::OnDestroy();
	
}

void CChildFrame::PaneReportClicked()
{
	CZipArcDoc* pDoc = (CZipArcDoc*)GetActiveDocument();
	if (!pDoc || pDoc->IsReportEmpty())
		return;
	pDoc->ViewReport();
	
}

