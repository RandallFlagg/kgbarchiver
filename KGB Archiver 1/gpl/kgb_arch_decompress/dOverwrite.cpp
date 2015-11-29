// dOverwrite.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_decompress.h"
#include "dOverwrite.h"


// dOverwrite dialog

IMPLEMENT_DYNAMIC(dOverwrite, CDialog)

dOverwrite::dOverwrite(CWnd* pParent /*=NULL*/)
	: CDialog(dOverwrite::IDD, pParent)
{

}

dOverwrite::~dOverwrite()
{
}

void dOverwrite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SORIGINALFILE, sOverwrite);
	DDX_Control(pDX, IDC_SORIGINALSIZE, sOriginalSize);
	DDX_Control(pDX, IDC_SORIGINALMOD, sOriginalMod);
	DDX_Control(pDX, IDC_SNEWSIZE, sNewSize);
	DDX_Control(pDX, IDC_SNEWMOD, sNewMod);
	DDX_Control(pDX, IDC_BYES, bYes);
	DDX_Control(pDX, IDC_BYESALL, bYesAll);
	DDX_Control(pDX, IDC_NEWICON, iNew);
	DDX_Control(pDX, IDC_OLDICON, iOld);
}


BEGIN_MESSAGE_MAP(dOverwrite, CDialog)
	ON_BN_CLICKED(IDC_BYESALL, &dOverwrite::OnBnClickedByesall)
	ON_BN_CLICKED(IDC_BYES, &dOverwrite::OnBnClickedByes)
	ON_BN_CLICKED(IDC_BNO, &dOverwrite::OnBnClickedBno)
	ON_BN_CLICKED(IDC_BNOALL, &dOverwrite::OnBnClickedBnoall)
END_MESSAGE_MAP()


// dOverwrite message handlers

void dOverwrite::OnBnClickedByesall()
{
	EndDialog(0);
}

void dOverwrite::OnBnClickedByes()
{
	EndDialog(1);
}

void dOverwrite::OnBnClickedBno()
{
	EndDialog(2);
}

void dOverwrite::OnBnClickedBnoall()
{
	EndDialog(3);
}
