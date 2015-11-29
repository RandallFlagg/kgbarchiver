// MMStatusBar.cpp : implementation file
//
/******************************************************
* MMStatusBar.cpp
* by Kiran Thonse Sanjeeva
* http://www.codeguru.com/statusbar/statdemo.html
*****************************************************/
/*
How to use:
1. Include MMStatusBar.cpp and MMStatusBar.h into your project. 

2. In the Mainframe , replace the CStatusBar with MMStatusBar. 

3. In the Mainframe Message Map, add a UPDATE_COMMAND_UI_RANGE() to trap the range of pane_ids to be trapped for updating the pane texts/tooltips/icons. 

*/

#include "stdafx.h"
#include "MMStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MMStatusBar

MMStatusBar::MMStatusBar()
{
	m_bFirstTime = TRUE ;
}

MMStatusBar::~MMStatusBar()
{
}


BEGIN_MESSAGE_MAP(MMStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(MMStatusBar)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MMStatusBar message handlers

int MMStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_oToolTip.Create(this,TTS_ALWAYSTIP);
	m_oToolTip.Activate(TRUE);
	m_oToolTip.SetDelayTime(1000);
	
	return 0;
}

void MMStatusBar::OnDestroy() 
{
	CStatusBar::OnDestroy();
	
	if(::IsWindow(m_oToolTip.GetSafeHwnd()))
		m_oToolTip.DestroyWindow();
}

void MMStatusBar::OnSize(UINT nType, int cx, int cy) 
{
	CStatusBar::OnSize(nType, cx, cy);
	
	if(!IsWindow(m_oToolTip.GetSafeHwnd()))
		return ;

	BOOL bAdded = FALSE;
	
	CToolInfo oToolInfo;
	memset(&oToolInfo,0,sizeof(TOOLINFO));
	oToolInfo.cbSize = sizeof(TOOLINFO) ;

	CRect oRect;
	UINT uId;
	GetClientRect(&oRect);
	for(int i = 0 ; i < m_nCount ; i++)
	{
		uId = GetItemID(i);
		GetItemRect(i,&oRect);
		if(m_bFirstTime && uId != 0)
		{
			VERIFY(m_oToolTip.AddTool(this,_T(""),&oRect,uId));
			bAdded = TRUE ;
		}
		m_oToolTip.GetToolInfo((CToolInfo& )oToolInfo,this,uId);
		
		oToolInfo.hwnd = this->GetSafeHwnd();
		oToolInfo.uId = uId;
		oToolInfo.rect.left = oRect.left;
		oToolInfo.rect.right = oRect.right;
		oToolInfo.rect.top = oRect.top;
		oToolInfo.rect.bottom = oRect.bottom;
		oToolInfo.lParam = uId;

		m_oToolTip.SetToolInfo(&oToolInfo);
	}
	if(bAdded)
		m_bFirstTime = FALSE;
}

void MMStatusCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	CStatusBar* pStatusBar = (CStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT_KINDOF(CStatusBar, pStatusBar);
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pStatusBar->GetPaneStyle(m_nIndex) & ~SBPS_DISABLED;
	if (!bOn)
		nNewStyle |= SBPS_DISABLED;
	pStatusBar->SetPaneStyle(m_nIndex, nNewStyle);
}

void MMStatusCmdUI::SetCheck(int nCheck) // "checking" will pop out the text
{
	CStatusBar* pStatusBar = (CStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT_KINDOF(CStatusBar, pStatusBar);
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pStatusBar->GetPaneStyle(m_nIndex) & ~SBPS_POPOUT;
	if (nCheck != 0)
		nNewStyle |= SBPS_POPOUT;
	pStatusBar->SetPaneStyle(m_nIndex, nNewStyle);
}

void MMStatusCmdUI::SetText(LPCTSTR lpszText)
{
	MMStatusBar* pStatusBar = (MMStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT_KINDOF(CStatusBar, pStatusBar);
	ASSERT(m_nIndex < m_nIndexMax);

	pStatusBar->SetPaneText(m_nIndex, lpszText);

	CToolInfo oToolInfo;
	memset(&oToolInfo,0,sizeof(TOOLINFO));
	oToolInfo.cbSize = sizeof(TOOLINFO) ;

	CRect oRect;
	UINT uId = pStatusBar->GetItemID(m_nIndex);
	pStatusBar->GetItemRect(m_nIndex,&oRect);
	
	pStatusBar->m_oToolTip.GetToolInfo((CToolInfo& )oToolInfo,pStatusBar,uId);
		
	oToolInfo.hwnd = pStatusBar->GetSafeHwnd();
	oToolInfo.uId = uId;
	oToolInfo.rect.left = oRect.left;
	oToolInfo.rect.right = oRect.right;
	oToolInfo.rect.top = oRect.top;
	oToolInfo.rect.bottom = oRect.bottom;
	oToolInfo.lParam = uId;

	pStatusBar->m_oToolTip.SetToolInfo(&oToolInfo);
}

void MMStatusCmdUI::SetToolTipText(LPCTSTR lpszText)
{
	MMStatusBar* pStatusBar = (MMStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT_KINDOF(CStatusBar, pStatusBar);
	ASSERT(m_nIndex < m_nIndexMax);

	pStatusBar->m_oToolTip.UpdateTipText(lpszText,m_pOther,m_nID);
}

void MMStatusCmdUI::SetWidth(int cxWidth)
{
	MMStatusBar* pStatusBar = (MMStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT_KINDOF(CStatusBar, pStatusBar);
	ASSERT(m_nIndex < m_nIndexMax);
	UINT nID;
	UINT nStyle;
	int nWidth;
	pStatusBar->GetPaneInfo(m_nIndex,nID,nStyle,nWidth);
	pStatusBar->SetPaneInfo(m_nIndex,m_nID,nStyle,cxWidth);
}

void MMStatusCmdUI::SetIcon(HICON hIcon)
{
	MMStatusBar* pStatusBar = (MMStatusBar*)m_pOther;
	ASSERT(pStatusBar != NULL);
	ASSERT_KINDOF(CStatusBar, pStatusBar);
	ASSERT(m_nIndex < m_nIndexMax);
	pStatusBar->GetStatusBarCtrl().SetIcon(m_nIndex,hIcon);
}

void MMStatusBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	MMStatusCmdUI state;
	state.m_pOther = this;
	state.m_pMenu = (CMenu*)MM_STATUS_PANE_UPDATE;
	state.m_nIndexMax = (UINT)m_nCount;
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = GetItemID(state.m_nIndex);

		// allow the statusbar itself to have update handlers
		if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
			continue;

		// allow target (owner) to handle the remaining updates
		state.DoUpdate(pTarget, FALSE);
	}

	// update the dialog controls added to the status bar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

BOOL MMStatusBar::PreTranslateMessage(MSG *pMsg)
{
	switch(pMsg->message)
	{
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		// This will reactivate the tooltip
		m_oToolTip.Activate (TRUE);
		m_oToolTip.RelayEvent (pMsg);
		default :break;
	}
	return CStatusBar::PreTranslateMessage(pMsg);
}

BOOL MMStatusBar::OnChildNotify(UINT message,WPARAM wParam,LPARAM lParam,LRESULT* pResult)
{
	switch(message)
	{
	case WM_NOTIFY:
		{
			NMHDR* pNMHDR = (NMHDR*)lParam;
			if(NM_DBLCLK == pNMHDR->code)
			{
				NMMOUSE* pNMMouse = (NMMOUSE*)lParam;
				UINT uCommandId = GetItemID(pNMMouse->dwItemSpec);
				this->GetParent()->SendMessage(WM_COMMAND,uCommandId,0);
			}
		}
		break;
	default:break;
	}
	return CStatusBar::OnChildNotify(message,wParam,lParam,pResult);
}