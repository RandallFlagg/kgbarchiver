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
#include "DlgSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSave dialog


CDlgSave::CDlgSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSave::IDD, pParent)
{
	m_pOptions = CZipArcApp::GetOptions();
	//{{AFX_DATA_INIT(CDlgSave)
	//}}AFX_DATA_INIT
}


void CDlgSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSave)
	DDX_Control(pDX, IDC_QUESTION, m_icon);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_ALWAYS, m_pOptions->m_bAlwaysSave);
}


BEGIN_MESSAGE_MAP(CDlgSave, CDialog)
	//{{AFX_MSG_MAP(CDlgSave)
	ON_BN_CLICKED(IDNO, OnNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSave message handlers

void CDlgSave::OnNo() 
{
	EndDialog(IDNO);	
}

BOOL CDlgSave::OnInitDialog() 
{
	
	CDialog::OnInitDialog();
	
	m_icon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_WARNING));
	CString szTemp;
	szTemp.Format(IDS_SAVETITLE, m_szTitle);
	SetWindowText(szTemp);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSave::OnOK() 
{
	CDialog::OnOK();
	m_pOptions->Save();
}
