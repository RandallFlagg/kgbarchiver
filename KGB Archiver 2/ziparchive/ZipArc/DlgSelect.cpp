// DlgSelect.cpp : implementation file
//

#include "stdafx.h"
#include "ziparc.h"
#include "DlgSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelect dialog


CDlgSelect::CDlgSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelect::IDD, pParent)
{
	m_pOptions = CZipArcApp::GetOptions();
	m_info.SetInfo(IDS_PATTERN, IDI_QUESTION);
	//{{AFX_DATA_INIT(CDlgSelect)
	//}}AFX_DATA_INIT
}


void CDlgSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelect)
	DDX_Control(pDX, IDC_INFOBUTTON, m_info);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CS, m_pOptions->m_bSearchCaseSensitive);
	DDX_Check(pDX, IDC_FILENAMEONLY, m_pOptions->m_bSearchFileName);
	DDX_Text(pDX, IDC_PATTERN, m_pOptions->m_szSearchPattern);

}


BEGIN_MESSAGE_MAP(CDlgSelect, CDialog)
	//{{AFX_MSG_MAP(CDlgSelect)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelect message handlers
