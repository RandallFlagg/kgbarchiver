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
#include "DlgExtract.h"
#include "BrowseForFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExtract dialog


CDlgExtract::CDlgExtract(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExtract::IDD, pParent)
{
	m_pOptions = CZipArcApp::GetOptions();
	//{{AFX_DATA_INIT(CDlgExtract)
	m_szPath = _T("");
	//}}AFX_DATA_INIT
}


void CDlgExtract::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExtract)
	DDX_Text(pDX, IDC_EXTRACT, m_szPath);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_EXFULLPATH, m_pOptions->m_bFullPath);
	DDX_Check(pDX, IDC_OVERWRITE, m_pOptions->m_bOverwrite);
	DDX_Radio(pDX, IDC_EXALL, m_pOptions->m_iExtract);
}


BEGIN_MESSAGE_MAP(CDlgExtract, CDialog)
	//{{AFX_MSG_MAP(CDlgExtract)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExtract message handlers

void CDlgExtract::OnBrowse() 
{
	CString sz;
	GetDlgItem(IDC_EXTRACT)->GetWindowText(sz);
	CBrowseForFolder bf;
	bf.strTitle.LoadString(IDS_EXTRACTTO);
	bf.strStartupDir = sz;
	if (bf.GetFolder(m_szPath))
		GetDlgItem(IDC_EXTRACT)->SetWindowText(m_szPath);
			
}


BOOL CDlgExtract::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//	if we are starting without selection, disable the radio
	if (!m_pOptions->m_iExtract)
		GetDlgItem(IDC_EXSEL)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
