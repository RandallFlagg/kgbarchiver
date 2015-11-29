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
#include "DlgNew.h"
#include "DlgOptSmart.h"
#include "DlgContainer.h"
#include "..\ZipArchive\ZipPathComponent.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNew dialog


CDlgNew::CDlgNew(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNew)
	m_szFileName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgNew::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_SPANNING, m_span);
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_LEVEL, m_pOptionsDoc->m_iLevel);
	DDX_Radio(pDX, IDC_PKSPAN, m_optionsNew.m_iSegm);
	DDX_Check(pDX, IDC_RECURSIVE, m_pOptionsDoc->m_bRecursive);
	DDX_Text(pDX, IDC_VOL, m_optionsNew.m_iVolume);
	DDV_MinMaxUInt(pDX, m_optionsNew.m_iVolume, 0, 4294967295);
	//{{AFX_DATA_MAP(CDlgNew)
	DDX_Text(pDX, IDC_FILENAME, m_szFileName);	
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CDlgNew, CDialog)
	//{{AFX_MSG_MAP(CDlgNew)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_TDSPAN, UpdateControls)
	ON_BN_CLICKED(IDC_PKSPAN, UpdateControls)
	ON_BN_CLICKED(IDC_SMARTOPT, OnSmartOpt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNew message handlers

void CDlgNew::OnBrowse() 
{
	UpdateData();
	CZipPathComponent zpc(m_szFileName);
	CString szName = zpc.GetFileName();
	if (AfxGetApp()->DoPromptFileName(szName,
		  AFX_IDS_SAVEFILE, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, m_pTemplate))
	{
		m_szFileName = szName;
		UpdateData(FALSE);
	}
}

void CDlgNew::OnDefault() 
{
	UpdateData();
	m_optionsNew.m_bUseSegm = m_span.GetCheck();
	m_optionsNew.MakeGlobal();
	m_pOptionsDoc->MakeGlobal(COptionsDoc::glCompress);
}

void CDlgNew::UpdateControls() 
{
	BOOL bEnableVol = ((CButton*)GetDlgItem(IDC_TDSPAN))->GetCheck() && m_span.GetCheck();
 	GetDlgItem(IDC_VOL)->EnableWindow(bEnableVol);
	GetDlgItem(IDC_STATICVOL)->EnableWindow(bEnableVol);
}

BOOL CDlgNew::OnInitDialog() 
{
	m_optionsNew.GetFromGlobal();
	CDialog::OnInitDialog();
	m_span.Init(true);
	m_span.AddItem(IDC_PKSPAN);
	m_span.AddItem(IDC_TDSPAN);
	m_span.AddItem(IDC_VOL);
	m_span.AddItem(IDC_STATICVOL);
	m_span.SetCheck(m_optionsNew.m_bUseSegm);
	UpdateControls();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgNew::OnOK() 
{
	m_optionsNew.m_bUseSegm = m_span.GetCheck();	
	CDialog::OnOK();
}

void CDlgNew::OnSmartOpt() 
{
	CDlgOptSmart dlgsmart;
	dlgsmart.m_pOptions = m_pOptionsDoc->GetSmart();
	CDlgContainer dlg;
	dlg.DoModal(dlgsmart, IDS_PAGE2);
}



BOOL CDlgNew::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == STN_CLICKED && LOWORD(wParam) == IDC_SPANNING) 
		UpdateControls();
	return CDialog::OnCommand(wParam, lParam);
}
