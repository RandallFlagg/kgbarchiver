// Main.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_conf.h"
#include "Main.h"
#include "../common/homedir.cpp"
#include "kgb_arch_confDlg.h"
#include <atlbase.h>
//#include "about.cpp"

// CMain dialog

IMPLEMENT_DYNAMIC(CMain, CDialog)

CMain::CMain(CWnd* pParent /*=NULL*/)
	: CDialog(CMain::IDD, pParent)
	, cCompress(0)
	, cDecompress(0)
	, cCustomize(0)
{

}

CMain::~CMain()
{
}

void CMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOP10, eTop10);
}


BEGIN_MESSAGE_MAP(CMain, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_SHOWWINDOW()
//	ON_WM_CREATE()
ON_BN_CLICKED(IDOK, &CMain::OnBnClickedOk)
END_MESSAGE_MAP()


// CMain message handlers

void CMain::OnBnClickedButton1()
{
	CDialog dlg(IDD_ABOUTBOX);
	dlg.DoModal();

	// TODO: Add your control notification handler code here
}
void CMain::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	eTop10.Navigate("http://kgbarchiver.sourceforge.net/top10donations.php", NULL, NULL, NULL, NULL);
	if(!bShow)
		return;

	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
	DWORD updates = 0;

	CRegKey reg(HKEY_CURRENT_USER);
	if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
		reg.QueryDWORDValue("check_for_updates", updates);
	}

	if(updates){
		HANDLE hProc = GetCurrentProcess();
		HMODULE hMod;
        DWORD cbNeeded;
		char processName[MAX_PATH];

        EnumProcessModules(hProc, &hMod, sizeof(hMod), &cbNeeded);
        GetModuleFileNameEx(hProc, hMod, processName, MAX_PATH);
        
		for(int i=strlen(processName)-1;i>=0;i--){
			if(processName[i] == '\\' || processName[i] == '/'){
				processName[i+1] = '\0';
				sprintf(processName, "%skgb_arch_conf.exe", processName);
				break;
			}
		}
		ShellExecute(0, "open", processName, "/v", NULL, NULL);
	}
}

void CMain::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
	
	setHomeDir();
	if(GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3) == IDC_RADIO1){
		ShellExecute(0, "open", "kgb_arch_compress.exe", "", "", SW_SHOW);
	}else if(GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3) == IDC_RADIO2){
		ShellExecute(0, "open", "kgb_arch_decompress.exe", "", "", SW_SHOW);
	}else if(GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3) == IDC_RADIO3){
		Ckgb_arch_confDlg dlg;
		dlg.DoModal();
	}


	OnOK();
}
