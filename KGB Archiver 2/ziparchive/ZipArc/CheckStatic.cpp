/*
* CheckStatic.cpp
*
*
* CCheckStatic
* By Christian Graus 
* http://www.codeproject.com/staticctrl/
*****************************************************/
//
//	2001.10 Modified by Tadeusz Dracz
//

#include "stdafx.h"
#include "CheckStatic.h"
#include "resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckStatic

CCheckStatic::CCheckStatic()
{
	create = true;
}

CCheckStatic::~CCheckStatic()
{
}


BEGIN_MESSAGE_MAP(CCheckStatic, CStatic)
	//{{AFX_MSG_MAP(CCheckStatic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckStatic message handlers


bool CCheckStatic::Init(bool bFlat, bool bDoNotHandleDisabling)
{
	// Get the static size
	GetWindowRect(&m_rcStatic);
	ScreenToClient(&m_rcStatic);

	// Grab the caption off the static
	// We are going to put it on the button so it is aligned nicely
	CString caption;
	GetWindowText(caption);
	SetWindowText(_T(""));

	// Now figure out how long the string is
	CDC * dc = this->GetDC();
	CFont* pOldFont = dc->SelectObject(GetFont());
	CSize size = dc->GetTextExtent(caption);
	dc->SelectObject(pOldFont);

	// Add a bit for the button itself
	if (size.cy < 20) size.cy = 20;
	size.cx += 40;

	if(!m_Check.Create(caption, WS_CHILD | WS_VISIBLE  | WS_TABSTOP | BS_CHECKBOX | (bFlat ? BS_FLAT : 0), 
		CRect(m_rcStatic.left + 10, m_rcStatic.top , m_rcStatic.left + size.cx, m_rcStatic.top + size.cy),
		this, ID_STATICCHECKBOX))
		return false;

	// All of the above would be useless if we did not make sure both buttons had the same font in them...
	m_Check.SetFont(this->GetFont());
	
	// Assume that the dialog starts active, a SetCheck function is provided where this is not the case
	m_Check.SetCheck(true);

	// Build a list of dialog item ID's
	bool loop = bDoNotHandleDisabling;

	CWnd * pWnd = this;

	while (loop == false)
	{
		pWnd = GetParent()->GetNextDlgTabItem(pWnd, TRUE);
	
		int ID = ::GetDlgCtrlID(pWnd->m_hWnd);
	
		// If we're trying to add the same ID twice, we've gone right round the horn.
		for (int i = 0; i< m_IDList.GetSize(); i++)
			if (m_IDList[i] == ID)
				loop = true;

		if (!loop)
			m_IDList.Add(ID);
	}
	return true;
}


BOOL CCheckStatic::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (wParam == ID_STATICCHECKBOX)
	{
		BOOL check = m_Check.GetCheck();
		m_Check.SetCheck(!check);

		// If we have specified some items to include, step through them
		if (m_ItemID.GetSize() > 0)
		{
			for (int i = 0; i < m_ItemID.GetSize(); i++)
			{
				GetParent()->GetDlgItem(m_ItemID[i])->EnableWindow(!check);
			}
		}
		// Otherwise, we step through the entire dialog list generated earlier
		else
		{
			for (int i = 0; i < m_IDList.GetSize(); i++)
			{
				CWnd * pWnd = GetParent()->GetDlgItem(m_IDList[i]);

				// We grab the rect of the item, put it into client co-ord's and only modify items inside our static box
				CRect rc;

				pWnd->GetWindowRect(&rc);
				ScreenToClient(&rc);

				if (m_rcStatic.PtInRect(rc.TopLeft()) && m_rcStatic.PtInRect(rc.BottomRight()))
					pWnd->EnableWindow(!check);
			}		
		}
// 
// 2001.10
// Change by Tadeusz Dracz
//	
		GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), STN_CLICKED), (LPARAM)m_hWnd);
	}

	return CStatic::OnCommand(wParam, lParam);
}

int CCheckStatic::AddItem(int ID)
{
	m_ItemID.Add(ID);
	return m_ItemID.GetSize();
}

void CCheckStatic::ClearItems()
{
	m_ItemID.RemoveAll();
}

void CCheckStatic::SetCheck(BOOL check)
{
	m_Check.SetCheck(check);

	if (m_ItemID.GetSize() > 0)
	{
		for (int i = 0; i < m_ItemID.GetSize(); i++)
			GetParent()->GetDlgItem(m_ItemID[i])->EnableWindow(check);
	}
	else
	{
		for (int i = 0; i < m_IDList.GetSize(); i++)
		{
			CWnd * pWnd = GetParent()->GetDlgItem(m_IDList[i]);
			
			// We grab the rect of the item, put it into client co-ord's and only modify items inside our static box
			CRect rc;
			
			pWnd->GetWindowRect(&rc);
			ScreenToClient(&rc);
			
			if (m_rcStatic.PtInRect(rc.TopLeft()) && m_rcStatic.PtInRect(rc.BottomRight()))
				pWnd->EnableWindow(check);
		}
	}
}
