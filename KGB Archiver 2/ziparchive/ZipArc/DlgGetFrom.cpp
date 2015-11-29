// DlgGetFrom.cpp : implementation file
//

#include "stdafx.h"
#include "ziparc.h"
#include "DlgGetFrom.h"
#include "ZipArcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGetFrom dialog


CDlgGetFrom::CDlgGetFrom(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGetFrom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGetFrom)
	m_szPrompt = _T("");
	//}}AFX_DATA_INIT
}


void CDlgGetFrom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGetFrom)
	DDX_Control(pDX, IDC_GET_FROM, m_combo);
	DDX_Text(pDX, IDC_PROMPT_GET_FROM_ARCH, m_szPrompt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGetFrom, CDialog)
	//{{AFX_MSG_MAP(CDlgGetFrom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGetFrom message handlers

BOOL CDlgGetFrom::OnInitDialog() 
{
	ASSERT(m_pDoc);
	m_szPrompt.Format(IDS_GETFROM_PROMPT, m_pDoc->GetFileName());
	CDialog::OnInitDialog();
	int iSize = m_docs.GetSize();
	ASSERT(iSize);
	for (int i = 0; i < iSize; i++)
	{
		CZipArcDoc* pDoc = m_docs[i];
		int idx = m_combo.AddString(pDoc->GetFileName());
		m_combo.SetItemData(idx, (DWORD)pDoc);
	}
	m_combo.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGetFrom::OnOK() 
{
	int idx = m_combo.GetCurSel();
	if (idx != CB_ERR)
		m_pDoc = (CZipArcDoc*)m_combo.GetItemData(idx);
	else
		m_pDoc = NULL;
	CDialog::OnOK();
}
