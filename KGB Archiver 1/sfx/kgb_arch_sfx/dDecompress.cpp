// dDecompress.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_decompress.h"
#include "dDecompress.h"


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
	DDX_Control(pDX, IDC_FULLSIZE, sFullSize);
	DDX_Control(pDX, IDC_COMPRESSEDSIZE, sCompressedSize);
	DDX_Control(pDX, IDC_RATIO, sRatio);
	DDX_Control(pDX, IDC_PROGRESS1, pCurrent);
	DDX_Control(pDX, IDC_PROGRESS2, pTotal);
	DDX_Control(pDX, IDC_FILENAME, sFilename);
	DDX_Control(pDX, IDC_ELAPSEDTIME, sTime);
	DDX_Control(pDX, IDC_REMAININGTIME, sRemaining);
	DDX_Control(pDX, IDC_CPRIORITY, cPriority);
}


BEGIN_MESSAGE_MAP(dDecompress, CDialog)
	ON_BN_CLICKED(1014, OnBnClicked1014)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_CPRIORITY, OnCbnSelchangeCpriority)
END_MESSAGE_MAP()


// dDecompress message handlers

void dDecompress::OnBnClicked1014()
{
	EndDialog(IDCANCEL);
}

void dDecompress::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	cPriority.SetCurSel(2);
}

void dDecompress::OnCbnSelchangeCpriority()
{
	DWORD prio[] = {HIGH_PRIORITY_CLASS, ABOVE_NORMAL_PRIORITY_CLASS, NORMAL_PRIORITY_CLASS, BELOW_NORMAL_PRIORITY_CLASS, IDLE_PRIORITY_CLASS};
	HANDLE hProc = GetCurrentProcess();

	if(!SetPriorityClass(hProc, prio[cPriority.GetCurSel()])){
		DWORD cPrio = GetPriorityClass(hProc);
		for(int i=0;i<sizeof(prio)/sizeof(prio[0]);i++){
			if(cPrio == prio[i]){
				cPriority.SetCurSel(i);
				break;
			}
		}
		MessageBox(loadString(IDS_CANT_CHANGE_PRIORITY), "KGB Archiver");
	}
}
