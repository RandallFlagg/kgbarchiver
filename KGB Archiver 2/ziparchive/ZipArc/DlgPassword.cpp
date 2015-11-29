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
#include "DlgPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPassword dialog


CDlgPassword::CDlgPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPassword::IDD, pParent)
	, m_iMethod(0)
{
	//{{AFX_DATA_INIT(CDlgPassword)
	m_szConfirm = _T("");
	m_szPass = _T("");
	m_iMethod = 0;
	//}}AFX_DATA_INIT
}


void CDlgPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPassword)
	DDX_Text(pDX, IDC_CONF, m_szConfirm);
	DDV_MaxChars(pDX, m_szConfirm, 255);
	DDX_Text(pDX, IDC_PASS, m_szPass);
	DDV_MaxChars(pDX, m_szPass, 255);
	DDX_Radio(pDX, IDC_RADIO_STD, m_iMethod);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CDlgPassword, CDialog)
	//{{AFX_MSG_MAP(CDlgPassword)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPassword message handlers

void CDlgPassword::OnOK() 
{
	if (!UpdateData())
		return;
	if (m_szPass.Collate(m_szConfirm) != 0)
	{
		AfxMessageBox(IDS_PASSNOTMATCH, MB_ICONWARNING);
		return;
	}
	CDialog::OnOK();
}

void CDlgPassword::OnClear() 
{
	m_szPass.Empty();
	m_szConfirm.Empty();
	UpdateData(FALSE);
}

