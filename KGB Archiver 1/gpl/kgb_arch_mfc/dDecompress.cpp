// dDecompress.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_mfc.h"
#include "dDecompress.h"
//#include "FolderDlg.cpp"


// dDecompress dialog

IMPLEMENT_DYNAMIC(dDecompress, CDialog)
dDecompress::dDecompress(CWnd* pParent /*=NULL*/)
	: CDialog(dDecompress::IDD, pParent)
{
}

dDecompress::~dDecompress()
{
}

void dDecompress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LFILES, lFiles);
	DDX_Control(pDX, IDC_EARCHNAME, eArchName);
	DDX_Control(pDX, IDC_EDESTINATION, eDestination);
}


BEGIN_MESSAGE_MAP(dDecompress, CDialog)
	ON_BN_CLICKED(IDC_BCANCEL, OnBnClickedBcancel)
	ON_BN_CLICKED(IDC_BNEXT, OnBnClickedBnext)
	ON_BN_CLICKED(IDC_BBROWSE2, OnBnClickedBbrowse2)
	ON_BN_CLICKED(IDC_BBROWSE, OnBnClickedBbrowse)
END_MESSAGE_MAP()


// dDecompress message handlers

void dDecompress::OnBnClickedBcancel()
{
	dDecompress::EndDialog(IDCANCEL);
}

void dDecompress::OnBnClickedBnext()
{
	if(eArchName.GetWindowTextLength() == 0 || eDestination.GetWindowTextLength() == 0){
		MessageBox("Podaj nazwê archiwum i/lub folder, do którego je rozpakowaæ!");
		return;
	}
}

void dDecompress::OnBnClickedBbrowse2()
{
	CFileDialog dOpen(true, "kgb", 0, OFN_HIDEREADONLY|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, "Archiwa KGB (*.kgb)|*.kgb|");
	if(dOpen.DoModal() == IDOK){
		eArchName.SetWindowText(dOpen.GetPathName());
	}
}


void dDecompress::OnBnClickedBbrowse()
{
	/*if(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) != S_OK){
		MessageBox("Nie mo¿na za³adowaæ modelu COM!");
		return;
	}*/
	
	//char foldername[MAX_PATH];

	BROWSEINFO bInfo;
	bInfo.hwndOwner = m_hWnd;
	bInfo.lpfn = NULL;
	bInfo.lParam = NULL;
	bInfo.lpszTitle = "Wybierz folder dla dekompresji";
	bInfo.pidlRoot = NULL;
	bInfo.ulFlags = BIF_USENEWUI | BIF_VALIDATE;
	bInfo.pszDisplayName = NULL;//foldername;

	LPITEMIDLIST pidl;

	if((pidl = SHBrowseForFolder(&bInfo)) != NULL){
		char buff[MAX_PATH];
		SHGetPathFromIDList(pidl, buff);
		//GetCurrentDirectory(MAX_PATH, foldername);
		//eDestination.SetWindowText(foldername);
		eDestination.SetWindowText(buff);
	}

}
