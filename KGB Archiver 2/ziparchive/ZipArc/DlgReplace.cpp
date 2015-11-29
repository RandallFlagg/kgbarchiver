// DlgReplace.cpp : implementation file
//

#include "stdafx.h"
#include "ziparc.h"
#include "DlgReplace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReplace dialog


CDlgReplace::CDlgReplace(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgReplace::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgReplace)
	m_bDontAsk = FALSE;
	m_szPrompt = _T("");
	m_iReplace = 0;
	//}}AFX_DATA_INIT
}


void CDlgReplace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgReplace)
	DDX_Control(pDX, IDC_REPLACE_ACTION, m_combo);
	DDX_Control(pDX, IDC_ICON_Q, m_icon);
	DDX_Check(pDX, IDC_DONT_ASK, m_bDontAsk);
	DDX_Text(pDX, IDC_PROMPT, m_szPrompt);
	DDX_CBIndex(pDX, IDC_REPLACE_ACTION, m_iReplace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReplace, CDialog)
	//{{AFX_MSG_MAP(CDlgReplace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReplace message handlers

void CDlgReplace::OnCancel() 
{
	
	//	CDialog::OnCancel();
}

BOOL CDlgReplace::OnInitDialog() 
{
	m_szPrompt.Format(IDS_REPLACE_PROMPT, m_szFileName);
	if (m_bSegm)
	{
		CString szTemp;
		szTemp.LoadString(IDS_REPLACE_PROMPT_SPAN);
		m_szPrompt += szTemp;
	}
	CDialog::OnInitDialog();
	
	m_icon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_WARNING));
	SetWindowText(m_szZipName);
	if (m_bSegm)
	{
		m_combo.DeleteString(2);
		m_combo.DeleteString(0);
		UpdateData(FALSE); // deleted 0 index, if m_iReplaceIndex was 0 then update
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
