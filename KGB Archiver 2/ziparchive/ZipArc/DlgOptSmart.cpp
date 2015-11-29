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
#include "DlgOptSmart.h"
#include "BrowseForFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptSmart dialog


CDlgOptSmart::CDlgOptSmart(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgOptSmart::IDD, pParent)
{
	
	m_info.SetInfo(IDS_NOGLOBAL);
	m_bNeedDefault = true;
	//{{AFX_DATA_INIT(CDlgOptSmart)
	//}}AFX_DATA_INIT
}


void CDlgOptSmart::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptSmart)	
	DDX_Control(pDX, IDC_STORESMALL, m_storesmall);
	DDX_Control(pDX, IDC_INFOBUTTON, m_info);
	DDX_Control(pDX, IDC_TEMPPATH, m_temp);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CLPASS, m_pOptions->m_bClrPass);
	DDX_Check(pDX, IDC_STORESMALL, m_pOptions->m_bStoreSmall);
	DDX_Check(pDX, IDC_USETEMP, m_pOptions->m_bDefault);
	DDX_Control(pDX, IDC_SMART, m_smart);

}


BEGIN_MESSAGE_MAP(CDlgOptSmart, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CDlgOptSmart)
	ON_BN_CLICKED(IDC_TEMPBROWSE, OnTempBrowse)
	ON_BN_CLICKED(IDC_USETEMP, UpdateControls)
	ON_EN_KILLFOCUS(IDC_TEMPPATH, OnKillfocusTemppath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptSmart message handlers

void CDlgOptSmart::OnSetDefault()
{
	// SetDefault calls UpdateData (the user can hit Set As Default and then Cancel) we need to restore the values
	COptionsSmart tempOptions = *m_pOptions;
	UpdateData();
	m_pOptions->m_bSmart = m_smart.GetCheck();
// 	if (!m_pOptions->m_bDefault)  // already done in lost focus
// 		m_temp.GetWindowText(m_pOptions->m_szTemp);
	m_pOptions->MakeGlobal();
	*m_pOptions = tempOptions;
}

void CDlgOptSmart::OnTempBrowse() 
{
	CBrowseForFolder bf;	
	bf.strTitle.LoadString(IDS_SELECTTEMP);
	m_temp.GetWindowText(bf.strStartupDir);
	if (bf.GetFolder(m_pOptions->m_szTemp))
		m_temp.SetWindowText(m_pOptions->m_szTemp);	
}

void CDlgOptSmart::UpdateControls() 
{
	BOOL bSmart =  m_smart.GetCheck();
	if (bSmart)
	{
		m_storesmall.SetCheck(TRUE);
		m_storesmall.EnableWindow(FALSE);
	}
	else
		m_storesmall.EnableWindow();
	BOOL bUseSystDef = ((CButton*)GetDlgItem(IDC_USETEMP))->GetCheck();
	BOOL bEnableTempDir = bSmart && !bUseSystDef;
	m_temp.EnableWindow(bEnableTempDir);
	GetDlgItem(IDC_TEMPBROWSE)->EnableWindow(bEnableTempDir);
	CString sz;
		
	if (bUseSystDef)
		CZipArcApp::GetTempPath(sz);
	else
		sz = m_pOptions->m_szTemp;
	m_temp.SetWindowText(sz);
}

BOOL CDlgOptSmart::OnInitDialog() 
{
	CSAPrefsSubDlg::OnInitDialog();
	if (!m_bNeedDefault)
		m_info.ShowWindow(SW_HIDE);

	m_smart.Init(true);
	m_smart.SetCheck(m_pOptions->m_bSmart);
	UpdateControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOptSmart::OnOK() 
{
// 	UpdateData(); already called by parent (CDlgContainer or CSAPrefsDialog)
	m_pOptions->m_bSmart = m_smart.GetCheck();
	if (!m_pOptions->m_bDefault)
		m_temp.GetWindowText(m_pOptions->m_szTemp); // in case edit window has the focus and the user hit ENTER
	CSAPrefsSubDlg::OnOK();
}

BOOL CDlgOptSmart::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == STN_CLICKED && LOWORD(wParam) == IDC_SMART) 
		UpdateControls();	
	return CSAPrefsSubDlg::OnCommand(wParam, lParam);
}

void CDlgOptSmart::OnKillfocusTemppath() 
{
	if (!((CButton*)GetDlgItem(IDC_USETEMP))->GetCheck())
		m_temp.GetWindowText(m_pOptions->m_szTemp);	
}

