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
#include "DlgReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReport dialog


CDlgReport::CDlgReport(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgReport::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReport)
	m_szReport = _T("");
	//}}AFX_DATA_INIT
	m_iType = 0;
}


void CDlgReport::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReport)
	DDX_Text(pDX, IDC_COMMENT, m_szReport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReport, CResizableDialog)
	//{{AFX_MSG_MAP(CDlgReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReport message handlers

BOOL CDlgReport::OnInitDialog() 
{
	

	CResizableDialog::OnInitDialog();
	CString szError;
	szError.Format(IDS_REPORT, CString((LPCTSTR)m_iType));
	SetWindowText(szError);
	AddAnchor(IDC_COMMENT, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDOK, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDCANCEL,BOTTOM_RIGHT, BOTTOM_RIGHT);
	EnableSaveRestore(_T("Report Dialog"));
	((CEdit*)GetDlgItem(IDC_COMMENT))->SetLimitText(0x7FFFFFFE);
	CenterWindow();
	if (!m_bEnableEmpty)
		GetDlgItem(IDOK)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

