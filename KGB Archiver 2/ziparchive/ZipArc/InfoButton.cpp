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
#include "ziparc.h"
#include "InfoButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoButton

CInfoButton::CInfoButton()
{
	m_lpszStandardIcon = NULL;
}

CInfoButton::~CInfoButton()
{
}


BEGIN_MESSAGE_MAP(CInfoButton, CButton)
	//{{AFX_MSG_MAP(CInfoButton)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoButton message handlers

void CInfoButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_ICON);
	if (m_lpszStandardIcon)
		SetIcon(AfxGetApp()->LoadStandardIcon(m_lpszStandardIcon));
}

void CInfoButton::OnClicked() 
{
	AfxMessageBox(m_InfoID, MB_ICONINFORMATION);	
}

void CInfoButton::SetInfo(UINT nID, LPCTSTR lpszIconName)
{
	m_lpszStandardIcon = lpszIconName;
	m_InfoID = nID;
}
