// dPasswd.cpp : implementation file
//

#include "stdafx.h"
#include "stub.h"
#include "dPasswd.h"


// dPasswd dialog

IMPLEMENT_DYNAMIC(dPasswd, CDialog)

dPasswd::dPasswd(CWnd* pParent /*=NULL*/)
	: CDialog(dPasswd::IDD, pParent)
{

}

dPasswd::~dPasswd()
{
}

void dPasswd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, ePasswd);
}


BEGIN_MESSAGE_MAP(dPasswd, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &dPasswd::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &dPasswd::OnBnClickedButton2)
END_MESSAGE_MAP()


// dPasswd message handlers

void dPasswd::OnBnClickedButton1()
{
	EndDialog(0);
}

void dPasswd::OnBnClickedButton2()
{
	ePasswd.GetWindowText(passwd, 32);
	EndDialog(1);
}
