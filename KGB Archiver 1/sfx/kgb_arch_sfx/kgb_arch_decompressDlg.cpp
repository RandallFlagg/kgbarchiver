//#include "..\kgb_arch_mfc\kgb_arch_mfcdlg.cpp"
// kgb_arch_decompressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_decompress.h"
#include "kgb_arch_decompressDlg.h"
#include "dDecompress.h"
#include "../compress/compress.cpp"
#include "io.h"
#include "dlgPasswd.h"
#include <psapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char globalARCH[MAX_PATH];
dDecompress _dDecompress;

// CAboutDlg dialog used for App About

/*class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()*/


// Ckgb_arch_decompressDlg dialog


Ckgb_arch_decompressDlg::Ckgb_arch_decompressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ckgb_arch_decompressDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ckgb_arch_decompressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EARCHNAME, eArchName);
	DDX_Control(pDX, IDC_EARCHNAME2, eDestination);
	DDX_Control(pDX, IDC_LIST1, lFiles);
	DDX_Control(pDX, IDC_CHECK1, cSelect);
}

BEGIN_MESSAGE_MAP(Ckgb_arch_decompressDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BBROWSE, OnBnClickedBbrowse)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BBROWSE2, OnBnClickedBbrowse2)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
//	ON_EN_CHANGE(IDC_EARCHNAME, OnEnChangeEarchname)
END_MESSAGE_MAP()


// Ckgb_arch_decompressDlg message handlers

BOOL Ckgb_arch_decompressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	/*ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);*/

	/*CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}*/

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, TRUE);		// Set small icon

	// TODO: Add extra initialization here
	
	lFiles.InsertColumn(0, loadString(IDS_FILE), LVCFMT_LEFT, 300);
	lFiles.InsertColumn(1, loadString(IDS_SIZE), LVCFMT_RIGHT, 100);

	
	char curDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curDir);
	eDestination.SetWindowText(curDir);

	vector<string>files;
	vector<unsigned long>fsize;
	char arch[MAX_PATH];

	HANDLE hProc = GetCurrentProcess();
	HMODULE hMod;
    DWORD cbNeeded;

    EnumProcessModules(hProc, &hMod, sizeof(hMod), &cbNeeded);
    GetModuleFileNameEx(hProc, hMod, arch, MAX_PATH); //pierwszy modu³ jest aplikacj¹
    
//MessageBox("");
	if(isSFXencrypted(arch)){
		//wo³amy o has³o
		dlgPasswd dlgPass;
		if(dlgPass.DoModal() != IDOK){
			theApp.ExitInstance();
			return false;
		}

		strcpy(passwd, dlgPass._passwd);
		for(int i=strlen(passwd);i<32;i++)
			passwd[i] = '\0';
		
		char arch2[MAX_PATH];
		//strcpy(arch2, arch.c_str());
		GetTempPath(MAX_PATH, arch2);
		GetLongPathName(arch2, arch2, sizeof(arch2));
		strcat(arch2, "temp.kgb");
		//arch2[strlen(arch2)-1] = 'b';

		aes_ctx ctx[1];
		ctx->n_rnd = 0; // ensure all flags are initially set to zero
		ctx->n_blk = 0;
		aes_dec_key((const unsigned char*)passwd, 32, ctx);
		
		/*CDialog dProgress;
		dProgress.Create(IDD_DECODING);
		dProgress.ShowWindow(SW_SHOW);
		dProgress.UpdateWindow();*/
		
		FILE *fin = fopen(arch, "rb");
		if(fin == 0)
			return false;
		fseek(fin, isSFXencrypted(arch), SEEK_SET);
		FILE *fout = fopen(arch2, "wb");
		if(fout == 0)
			return false;
		decfile(fin, fout, ctx, arch, arch2);

		fclose(fin);
		fclose(fout);
		/*dProgress.ShowWindow(SW_HIDE);
		dProgress.~CDialog();*/
		if(!checkArchiveFormat(arch2)){
			_fcloseall();
			DeleteFile(arch2);
			//eArchName.SetWindowText("");
			MessageBox(loadString(IDS_WRONG_PASSWD), "KGB Archiver");
			theApp.ExitInstance();
			return false;
		}

		strcpy(arch, arch2);
		getFilesFromArchive(arch, files, fsize);
	}else{
		getFilesFromArchive(arch, files, fsize);
	}
	strcpy(globalARCH, arch);
//MessageBox("");
	for(unsigned long i=0;i<files.size();i++){
		char buffer[32];
		/*if(filesizes[i] < 1024)
			sprintf(buffer, "%dB", filesizes[i]);
		else if(filesizes[i] < 1024*1024)
			sprintf(buffer, "%dKB", filesizes[i]/1024);
		else
			sprintf(buffer, "%dMB", filesizes[i]/1024/1024);
		*/
		sprintf(buffer, "%.1fKB", (double)fsize[i]/1024.0);
		lFiles.InsertItem(0, i, 0, 0, 0, 0, 0);
		
		lFiles.SetItemText(i, 0, files[i].c_str());
		lFiles.SetItemText(i, 1, buffer);
	}

	//MessageBox("");

	//delete curDir;

	/*char *cmd = theApp.m_lpCmdLine;
	char temp[MAX_PATH];
	bool isArchName = true;
	
	/*sscanf(cmd, "-%c", &mode);
	switch(mode){
		case 'c':
			cmd += 2; //przesówamy wskaŸnik* /

	for(unsigned long i=0;i<strlen(cmd);i++){
		if(cmd[i] == '"'){
			for(unsigned long j=0;i+j+1<strlen(cmd);j++){
				if(cmd[i+j+1] != '"')
					temp[j] = cmd[i+j+1];
				else if(cmd[i+j+1] == '"'){
					temp[j] = 0;
					i += j+1;
					if(!isArchName)
						//lFiles.AddString(temp);
						eDestination.SetWindowText(temp);
					else{
						eArchName.SetWindowText(temp);
						isArchName = false;
					}
					break;
				}
			}
		}
	}*/

	//MessageBox(cmd);

	/*if(strlen(temp) == 0 || strlen(cmd) <= 2)
		return TRUE;

	vector<string> files;
	vector<unsigned long> filesizes;
	if(!getFilesFromArchive(temp, files, filesizes)){
		MessageBox("Wybrany plik nie jest prawid³owym archiwum KGB!");
		eArchName.SetWindowText("");
		eDestination.SetWindowText("");
		return TRUE;
	}

	//lFiles.DeleteAllItems();

	for(unsigned long i=0;i<files.size();i++){
		char buffer[32];
		/*if(filesizes[i] < 1024)
			sprintf(buffer, "%dB", filesizes[i]);
		else if(filesizes[i] < 1024*1024)
			sprintf(buffer, "%dKB", filesizes[i]/1024);
		else
			sprintf(buffer, "%dMB", filesizes[i]/1024/1024);
		* /
		sprintf(buffer, "%.1fKB", (double)filesizes[i]/1024.0);
		lFiles.InsertItem(0, i, 0, 0, 0, 0, 0);
		
		lFiles.SetItemText(i, 0, files[i].c_str());
		lFiles.SetItemText(i, 1, buffer);
	}*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ckgb_arch_decompressDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
/*		CAboutDlg dlgAbout;
		dlgAbout.DoModal();*/
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Ckgb_arch_decompressDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Ckgb_arch_decompressDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Ckgb_arch_decompressDlg::OnBnClickedButton2()
{
	EndDialog(IDCANCEL);
}

/*void Ckgb_arch_decompressDlg::OnBnClickedBbrowse()
{
	CFileDialog dOpen(true, "kgb", 0, OFN_HIDEREADONLY|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, "Archiwa KGB (*.kgb, *.kge)|*.kgb; *.kge|");
	if(dOpen.DoModal() == IDOK){
		eArchName.SetWindowText(dOpen.GetPathName());
		/*string arch = dOpen.GetPathName();
		vector<string> files;
		if(!getFilesFromArchive((char *)arch.c_str(), files)){
			MessageBox("Wybrany plik nie jest prawid³owym archiwum KGB!");
			return;
		}
		while(lFiles.GetCount() != 0)
			lFiles.DeleteString(0);
		for(int i=0;i<files.size();i++){
			lFiles.AddString(files[i].c_str());
		}*/
		
		/*-----------------------------------------

		string arch = dOpen.GetPathName();
		vector<string> files;
		vector<unsigned long> filesizes;
		if(!getFilesFromArchive((char *)arch.c_str(), files, filesizes)){
			MessageBox("Wybrany plik nie jest prawid³owym archiwum KGB!");
			return;
		}

		lFiles.DeleteAllItems();

		for(unsigned long i=0;i<files.size();i++){
			char buffer[32];
			/*if(filesizes[i] < 1024)
				sprintf(buffer, "%dB", filesizes[i]);
			else if(filesizes[i] < 1024*1024)
				sprintf(buffer, "%dKB", filesizes[i]/1024);
			else
				sprintf(buffer, "%dMB", filesizes[i]/1024/1024);
			* /
			sprintf(buffer, "%.1fKB", (double)filesizes[i]/1024.0);
			lFiles.InsertItem(0, i, 0, 0, 0, 0, 0);
			
			lFiles.SetItemText(i, 0, files[i].c_str());
			lFiles.SetItemText(i, 1, buffer);
		}
		----------------------------------* /
	}
}*/

DWORD WINAPI updateForm(LPVOID lpParam){
	char buffer[1024];
	unsigned __int64 time_sTime;
	unsigned __int64 temp;
	while(1){
		try{		
			if(_size_all != 0 && /*_size != 0 && _done != 0 && _done_all != 0 && */_dDecompress.m_hWnd != NULL){
				sprintf(buffer, "%s %.1f%%", loadString(IDS_DECOMPRESS_PROGRESS), (float)((_done_all+_done)*100)/(float)(_size_all));
				_dDecompress.SetWindowText(buffer);
				
				_dDecompress.pTotal.SetRange32(0, 100);
				_dDecompress.pTotal.SetPos(((_done_all+_done)*100)/(_size_all));
				
				_dDecompress.pCurrent.SetRange32(0, 100);
				_dDecompress.pCurrent.SetPos((_done*100)/(_size+1));

				if(_filename.c_str()[0] > 32)
					_dDecompress.sFilename.SetWindowText(_filename.c_str());
				else
					_dDecompress.sFilename.SetWindowText(loadString(IDS_DECOMPRESS_STATUS+_filename.c_str()[0]-1));

				sprintf(buffer, "%.1fKB", (float)_size_all/1024.0);
				_dDecompress.sFullSize.SetWindowText(buffer);

				sprintf(buffer, "%.1fKB", (float)_compressed/1024.0);
				_dDecompress.sCompressedSize.SetWindowText(buffer);

				sprintf(buffer, "%.1f%%", ((float)_compressed*100.0)/(float)(_size_all));
				_dDecompress.sRatio.SetWindowText(buffer);

				time_sTime = time(NULL)-sTime;
				if((temp = time_sTime/3600) < 10)
					sprintf(buffer, "0");
				sprintf(buffer, "%s%ld:", buffer, temp);
				if((temp = (time_sTime%3600)/60) < 10)
					sprintf(buffer, "%s0", buffer);
				sprintf(buffer, "%s%ld:", buffer, temp);
				if((temp = time_sTime%60) < 10)
					sprintf(buffer, "%s0", buffer);
				sprintf(buffer, "%s%ld", buffer, temp);

				_dDecompress.sTime.SetWindowText(buffer);
				buffer[0] = '\0';

				time_sTime = (unsigned __int64)((double)time_sTime/((double)(_done+_done_all+1)/(double)(_size_all))) - time_sTime;
				if((temp = time_sTime/3600) < 10)
					sprintf(buffer, "0");
				sprintf(buffer, "%s%ld:", buffer, temp);
				if((temp = (time_sTime%3600)/60) < 10)
					sprintf(buffer, "%s0", buffer);
				sprintf(buffer, "%s%ld:", buffer, temp);
				if((temp = time_sTime%60) < 10)
					sprintf(buffer, "%s0", buffer);
				sprintf(buffer, "%s%ld", buffer, temp);

				_dDecompress.sRemaining.SetWindowText(buffer);
				

				//_dDecompress.UpdateWindow();

				//_shutdown = _dDecompress.cShutdown.GetCheck();

				if(_done_all == _size_all){
					_dDecompress.EndDialog(IDOK);
					//return 0;
				}
			}
		}catch(...){}
		Sleep(500);
	}
	free(buffer);
	return 0;
}

void Ckgb_arch_decompressDlg::OnBnClickedButton1()
{
	if(eDestination.GetWindowTextLength() == 0){
		MessageBox(loadString(IDS_CHOOSE_EXTRACT_DIR), "KGB Archiver");
		return;
	}
	
	resetSettings();

	archive_name = (char *)malloc(sizeof(char)*MAX_PATH);
	//eArchName.GetWindowText(archive_name, MAX_PATH);
	/*HANDLE hProc = GetCurrentProcess();
	HMODULE hMod;
    DWORD cbNeeded;*/

    /*EnumProcessModules(hProc, &hMod, sizeof(hMod), &cbNeeded);
    GetModuleFileNameEx(hProc, hMod, archive_name, MAX_PATH); //pierwszy modu³ jest aplikacj¹
    */
	strcpy(archive_name, globalARCH);

	if(strlen(archive_name) < 4)
		return;

	if(_access(archive_name, 4) != 0){
		MessageBox(loadString(IDS_ARCHIVE_NOT_FOUND), "KGB Archiver");
		return;
	}

	char buffer[MAX_PATH];
	eDestination.GetWindowText(buffer, MAX_PATH);
	CreateDirectory(buffer, NULL);
	if(!SetCurrentDirectory(buffer)){
		MessageBox(loadString(IDS_CANT_CREATE_EXTRACT_DIR), "KGB Archiver");
		return;
	}

	if(cSelect.GetCheck() == true){
		if(lFiles.GetSelectedCount() == 0){
			MessageBox(loadString(IDS_CHOOSE_FILES2DECOMPRESS), "KGB Archiver");
			return;
		}
		POSITION i = lFiles.GetFirstSelectedItemPosition();

		while(i != NULL){
			//lFiles.DeleteItem(lFiles.GetNextSelectedItem(i));
			//i = lFiles.GetFirstSelectedItemPosition();
			char buffer[MAX_PATH];
			lFiles.GetItemText(lFiles.GetNextSelectedItem(i), 0, buffer, MAX_PATH);
			files4decompress.push_back((string)buffer);
		}
	}

	ShowWindow(SW_HIDE);

	//MessageBox(archive_name);

	HANDLE hDecompress;
	DWORD hDecompressId;
	hDecompress = CreateThread(NULL, 0, decompress, 0, 0, &hDecompressId);
	SetThreadPriority(hDecompress, THREAD_PRIORITY_LOWEST);

	HANDLE hUpdate;
	DWORD hUpdateId;
	hUpdate = CreateThread(NULL, 0, updateForm, 0, 0, &hUpdateId);
	
	if(_dDecompress.DoModal() == IDCANCEL){
		TerminateThread(hDecompress, 0);
		MessageBox(loadString(IDS_EXTRACT_CANCELED), "KGB Archiver");
	}else{
		char buff[1024];
		sprintf(buff, loadString(IDS_EXTRACT_SUCCED), (float)_size_all/1024.0, (float)_compressed/1024.0, ((float)_compressed*100.0)/(float)_size_all);
		MessageBox(buff, "KGB Archiver");
	}

	TerminateThread(hUpdate, 0);
    
	EndDialog(IDOK);
}

void Ckgb_arch_decompressDlg::OnBnClickedBbrowse2()
{
	BROWSEINFO bInfo;
	bInfo.hwndOwner = m_hWnd;
	bInfo.lpfn = NULL;
	bInfo.lParam = NULL;
	bInfo.lpszTitle = loadString(IDS_CHOOSE_DIR2DECOMPRESS);
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

void Ckgb_arch_decompressDlg::OnBnClickedCheck1()
{
	lFiles.EnableWindow(cSelect.GetCheck());
}

/*void Ckgb_arch_decompressDlg::OnEnChangeEarchname()
{
		CString _arch;
		string arch;
		eArchName.GetWindowText(_arch);
		arch = _arch;
		vector<string> files;
		vector<unsigned long> filesizes;
		
		lFiles.DeleteAllItems();

		if(fopen(arch.c_str(), "r") == 0)
			return;
		_fcloseall();

		//dekodowanie archiwum
		if(arch.c_str()[arch.length()-1] == 'e' || arch.c_str()[arch.length()-1] == 'E'){
			dlgPasswd dlgPass;
			if(dlgPass.DoModal() != IDOK){
				eArchName.SetWindowText("");
				return;
			}
			
			strcpy(passwd, dlgPass._passwd);
			for(int i=strlen(passwd);i<32;i++)
				passwd[i] = '\0';
			
			char arch2[MAX_PATH];
			//strcpy(arch2, arch.c_str());
			GetTempPath(MAX_PATH, arch2);
			strcat(arch2, "temp.kgb");
			//arch2[strlen(arch2)-1] = 'b';

			aes_ctx ctx[1];
			ctx->n_rnd = 0; // ensure all flags are initially set to zero
			ctx->n_blk = 0;
			aes_dec_key((const unsigned char*)passwd, 32, ctx);
			
			FILE *fin = fopen(arch.c_str(), "rb");
			if(fin == 0)
				return;
			FILE *fout = fopen(arch2, "wb");
			if(fout == 0)
				return;
			decfile(fin, fout, ctx, arch.c_str(), arch2);

			fclose(fin);
			fclose(fout);
			if(!checkArchiveFormat(arch2)){
				_fcloseall();
				DeleteFile(arch2);
				eArchName.SetWindowText("");
				MessageBox("Nieprawid³owe has³o!");
				return;
			}

			eArchName.SetWindowText(arch2);
			return;
		}
		
		if(!getFilesFromArchive((char *)arch.c_str(), files, filesizes)){
			MessageBox("Wybrany plik nie jest prawid³owym archiwum KGB!");
			eArchName.SetWindowText("");
			return;
		}

		for(unsigned long i=0;i<files.size();i++){
			char buffer[32];
			/*if(filesizes[i] < 1024)
				sprintf(buffer, "%dB", filesizes[i]);
			else if(filesizes[i] < 1024*1024)
				sprintf(buffer, "%dKB", filesizes[i]/1024);
			else
				sprintf(buffer, "%dMB", filesizes[i]/1024/1024);
			* /
			sprintf(buffer, "%.1fKB", (double)filesizes[i]/1024.0);
			lFiles.InsertItem(0, i, 0, 0, 0, 0, 0);
			
			lFiles.SetItemText(i, 0, files[i].c_str());
			lFiles.SetItemText(i, 1, buffer);
		}
}*/
