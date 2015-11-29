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
#include "DlgOptCompr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgOptCompr::CDlgOptCompr(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgOptCompr::IDD, pParent)
{
	m_info.SetInfo(IDS_NOGLOBAL);
	m_bNeedDefault = true;
	//{{AFX_DATA_INIT(CDlgOptCompr)
	//}}AFX_DATA_INIT
}


void CDlgOptCompr::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptCompr)
	DDX_Control(pDX, IDC_INFOBUTTON, m_info);
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_LEVEL, m_pOptions->m_iLevel);
	DDX_Check(pDX, IDC_RECURSIVE, m_pOptions->m_bRecursive);
	DDX_CBIndex(pDX, IDC_SYSTCOMP, m_pOptions->m_iSystem);
	DDX_CBIndex(pDX, IDC_FILENAMES, m_pOptions->m_iFileNames);
	DDX_CBIndex(pDX, IDC_REPLACE_ACTION, m_pOptions->m_iReplaceAction);
	DDX_Control(pDX, IDC_REPLACE, m_replace);
}


BEGIN_MESSAGE_MAP(CDlgOptCompr, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CDlgOptCompr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptCompr message handlers


void CDlgOptCompr::OnSetDefault()
{
	// SetDefault calls UpdateData (the user can hit Set As Default and then Cancel) we need to restore the values
	COptionsCompress tempOptions = *m_pOptions;	
	UpdateData();
	m_pOptions->m_bReplace = m_replace.GetCheck();
	m_pOptions->MakeGlobal();
	*m_pOptions = tempOptions;
}

BOOL CDlgOptCompr::OnInitDialog() 
{
	CSAPrefsSubDlg::OnInitDialog();
	if (!m_bNeedDefault)
		m_info.ShowWindow(SW_HIDE);
	m_replace.Init(true);
	m_replace.AddItem(IDC_REPLACE_ACTION);
	m_replace.AddItem(IDC_PROMPT);
	m_replace.SetCheck(m_pOptions->m_bReplace);
#ifndef _UNICODE
	GetDlgItem(IDC_FILENAMES)->ShowWindow(SW_HIDE);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOptCompr::OnOK() 
{
	m_pOptions->m_bReplace = m_replace.GetCheck();
	CSAPrefsSubDlg::OnOK();
}
