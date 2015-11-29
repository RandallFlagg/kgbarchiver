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
#include "DlgContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgContainer dialog


CDlgContainer::CDlgContainer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgContainer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgContainer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgContainer)
	DDX_Control(pDX, IDC_SETDEFAULT, m_default);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_DLG_FRAME, m_frame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgContainer, CDialog)
	//{{AFX_MSG_MAP(CDlgContainer)
	ON_BN_CLICKED(IDC_SETDEFAULT, OnSetDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgContainer message handlers

int CDlgContainer::DoModal(CSAPrefsSubDlg& sdlg,UINT nIDTitle, int iType)
{
	m_pDlg = &sdlg;
	m_szTitle.LoadString(nIDTitle);
	m_iType = iType;
	return CDialog::DoModal();
}

BOOL CDlgContainer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect r;
	m_frame.GetWindowRect(&r);
	ScreenToClient(&r);
	m_frame.ShowWindow(SW_HIDE);
	m_pDlg->Create(m_pDlg->GetID(), this);
	m_pDlg->MoveWindow(r.left, r.top, r.Width(), r.Height());
    m_pDlg->ShowWindow(SW_SHOW);
	m_pDlg->SetFocus();
	SetWindowText(m_szTitle);	
	if (m_iType != dcAll)
	{
		m_default.ShowWindow(SW_HIDE);
		if (m_iType == dcOKOnly)
		{
			m_default.GetWindowRect(&r);
			ScreenToClient(&r);
			m_cancel.ShowWindow(SW_HIDE);
			m_ok.MoveWindow(r.left, r.top, r.Width(), r.Height());
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgContainer::OnOK() 
{
	if (m_pDlg->UpdateData())
	{
		m_pDlg->OnOK();
		CDialog::OnOK();
	}
}



void CDlgContainer::OnSetDefault() 
{
	m_pDlg->OnSetDefault();	
}

void CDlgContainer::OnCancel() 
{
	m_pDlg->OnCancel();	
	CDialog::OnCancel();
}
