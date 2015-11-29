// dlgPasswd.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_decompress.h"
#include "dlgPasswd.h"


// dlgPasswd dialog

IMPLEMENT_DYNAMIC(dlgPasswd, CDialog)
dlgPasswd::dlgPasswd(CWnd* pParent /*=NULL*/)
	: CDialog(dlgPasswd::IDD, pParent)
{
}

dlgPasswd::~dlgPasswd()
{
}

void dlgPasswd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EPASSWD, ePasswd);
}


BEGIN_MESSAGE_MAP(dlgPasswd, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// dlgPasswd message handlers

void dlgPasswd::OnBnClickedCancel()
{
	OnCancel();
}

void dlgPasswd::OnBnClickedOk()
{
	ePasswd.GetWindowText(_passwd, 32);
	OnOK();
}

void dlgPasswd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	ePasswd.SetFocus();
}
void dlgPasswd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	ePasswd.SetFocus();
}
