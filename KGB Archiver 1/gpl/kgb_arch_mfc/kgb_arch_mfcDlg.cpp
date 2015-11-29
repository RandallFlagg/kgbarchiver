// kgb_arch_mfcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_mfc.h"
#include "kgb_arch_mfcDlg.h"
#include "../compress/compress_compress.cpp"
#include "dCompress.h"
#include <io.h>
#include "../zip/zipArchive.h"
#include "../common/units.cpp"
#include "../common/lang.cpp"
#include "../common/icons.cpp"

#define BASE_MENU 0xF000

void loadDirs(CListCtrl *, string);
bool _shutdown;
dCompress _dCompress;
int spanSize;

inline bool addFile(CListCtrl *lFiles, string filename, __int64 size){
	char buffer[16];
	HICON ico = NULL;
	int ico_id;
	CImageList *imgLst = NULL;

	if(size >= (__int64)4*1024*1024*1024){
		MessageBox(0, loadString(IDS_TOO_BIG_FILE), "KGB Archiver", 0);
		return false;
	}

	ico = getIcon(filename);

	if(ico != NULL){
		imgLst = lFiles->GetImageList(LVSIL_SMALL);
		if(imgLst == NULL){
			imgLst = new CImageList();
			imgLst->Create(16, 16, ILC_COLORDDB, 0, INT_MAX);
			imgLst->SetBkColor(0x00FFFFFF);
		}
		ico_id = imgLst->Add(ico);
		lFiles->SetImageList(imgLst, LVSIL_SMALL);
		lFiles->InsertItem(lFiles->GetItemCount(), "", ico_id);
	}else
		lFiles->InsertItem(lFiles->GetItemCount(), ""); 


	lFiles->SetItemText(lFiles->GetItemCount()-1, 0, filename.c_str());
	
	sprintf(buffer, "%s", convertUnits(size));
	lFiles->SetItemText(lFiles->GetItemCount()-1, 1, buffer);

	return true;
}

DWORD WINAPI zipCompress(LPVOID lpParam){
	_filename = COMPRESS_PREPARING;
	sTime = time(NULL);

	CZipArchive zip;
	vector<__int64>filesize;
	_size_all = 1;

	/*char b[123];
	sprintf(b, "%d", spanSize);
	MessageBox(0, b, "", 0);*/
	
	if(spanSize == 0)
		zip.Open(archive_name, CZipArchive::OpenMode::zipCreate);
	else
		zip.Open(archive_name, CZipArchive::OpenMode::zipCreateSpan, spanSize);
	zip.SetPassword(passwd);
	//zip.SetAutoFlush();
	
	_filename = COMPRESS_GETTING_FILE_SIZES;
	for (int i=0; i<int(filename.size()); ++i) {
      FILE* f=fopen(filename[i].c_str(), "rb");
      if(f){
		_fseeki64(f, 0LL, SEEK_END);
		filesize.push_back(_ftelli64(f));
        _size_all += _ftelli64(f);
        fclose(f);
      }
    }

	for(int i=0;i<filename.size();i++){
		_filename = filename[i];
		//zip.AddNewFile(filename[i].c_str(), MEM);
		zip.AddNewFile(filename[i].c_str(), filename[i].c_str(), MEM);
		_done_all += filesize[i];
		
		CZipFileHeader fhInfo;
		zip.GetFileInfo(fhInfo, i);
		_compressed += fhInfo.GetEffComprSize() + fhInfo.GetSize() + fhInfo.GetExtraFieldSize();
		
	}
	zip.Close();

	/*FILE *f = fopen(archive_name, "rb");
	if(f){
		_fseeki64(f, 0LL, SEEK_END);
		_compressed = _ftelli64(f);
		fclose(f);
	}*/

	if(sfx_arch){ //do³¹czamy modu³ sfx
		_filename = COMPRESS_ADDING_SFX_MOD;
		
		/*archive_name[strlen(archive_name)-3] = '\0';
		sprintf(archive_name, "%sexe", archive_name);*/

		//odczytujemy œcie¿kê do modu³u
		HANDLE hProc = GetCurrentProcess();
		HMODULE hMod;
        DWORD cbNeeded;
		char processName[MAX_PATH];

        EnumProcessModules(hProc, &hMod, sizeof(hMod), &cbNeeded);
        GetModuleFileNameEx(hProc, hMod, processName, MAX_PATH);
        
		for(int i=strlen(processName)-1;i>=0;i--){
			if(processName[i] == '\\' || processName[i] == '/'){
				processName[i+1] = '\0';
				sprintf(processName, "%skgb_arch_sfx_zip.mod", processName);
				break;
			}
		}

		FILE *sfx = fopen(processName, "rb");

		//MessageBox(0, "", "", 0);
		if(sfx == NULL){
			MessageBox(0, "An internal error has occured, please reinstall KGB Archiver!", "KGB Archiver", 0);
			_fcloseall();
			DeleteFile(archive_name);
			return false;
		}

		//MessageBox(0, archive_name, "", 0);

		FILE *archive = fopen(archive_name, "rb");
		if(archive == NULL){
			_fcloseall();
			return false;
		}
		//MessageBox(0, "", "", 0);

		archive_name[strlen(archive_name)-3] = '\0';
		sprintf(archive_name, "%sexe", archive_name);

		//MessageBox(0, archive_name, "", 0);

		FILE *archive_sfx = fopen(archive_name, "wb");
		if(archive_sfx == NULL){
			_fcloseall();
			return false;
		}

		//MessageBox(0, "2", "", 0);

		int count=0;
		char buffer[65536];
		while(!feof(sfx)){
			count = fread(buffer, sizeof(char), sizeof(buffer), sfx);
			fwrite(buffer, sizeof(char), count, archive_sfx);
		}
		while(!feof(archive)){
			count = fread(buffer, sizeof(char), sizeof(buffer), archive);
			fwrite(buffer, sizeof(char), count, archive_sfx);
		}
		fclose(archive);
		fclose(sfx);
		fclose(archive_sfx);

		archive_name[strlen(archive_name)-3] = '\0';
		sprintf(archive_name, "%szip", archive_name);
		DeleteFile(archive_name);
	}

	_done_all++;

	return true;
}

/*char *loadString(int ID){
	CString cs;
	cs.LoadString(ID);
	return (char *)cs.GetString();
}*/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Ckgb_arch_mfcDlg dialog



Ckgb_arch_mfcDlg::Ckgb_arch_mfcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ckgb_arch_mfcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ckgb_arch_mfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BCANCEL, bCancel);
	DDX_Control(pDX, IDC_EARCHNAME, eArchName);
	DDX_Control(pDX, IDC_COPTIONS, cOptions);
	DDX_Control(pDX, IDC_LIST1, lFiles);
	DDX_Control(pDX, IDC_CFULLPATH, cFullPath);
	DDX_Control(pDX, IDC_PASSWORD, ePassword);
	DDX_Control(pDX, IDC_CHECK1, cSFX);
	DDX_Control(pDX, IDC_RADIO_KGB, rKGB);
	DDX_Control(pDX, IDC_RADIO_ZIP, rZIP);
	DDX_Control(pDX, IDC_COMBO_SPAN, cSpan);
	DDX_Control(pDX, IDC_BRECENTARCH, bRecentArch);
	DDX_Control(pDX, IDC_CSHOWPASS, cShowPass);
}

BEGIN_MESSAGE_MAP(Ckgb_arch_mfcDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BCANCEL, OnBnClickedBcancel)
	ON_BN_CLICKED(IDC_BBROWSE, OnBnClickedBbrowse)
	ON_BN_CLICKED(IDC_BADD2COMPRESS, OnBnClickedBadd2compress)
	ON_BN_CLICKED(IDC_BREMOVEFROMCOMPRESS, OnBnClickedBremovefromcompress)
	ON_BN_CLICKED(IDC_BNEXT, OnBnClickedBnext)
	ON_BN_CLICKED(IDC_BADD2COMPRESS2, OnBnClickedBadd2compress2)
	ON_EN_CHANGE(IDC_PASSWORD, OnEnChangePassword)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_RADIO_KGB, &Ckgb_arch_mfcDlg::OnBnClickedRadioKgb)
	ON_BN_CLICKED(IDC_RADIO_ZIP, &Ckgb_arch_mfcDlg::OnBnClickedRadioZip)
	ON_EN_CHANGE(IDC_EARCHNAME, &Ckgb_arch_mfcDlg::OnEnChangeEarchname)
//	ON_CBN_SELCHANGE(IDC_COMBO_SPAN, &Ckgb_arch_mfcDlg::OnCbnSelchangeComboSpan)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPAN, &Ckgb_arch_mfcDlg::OnCbnEditchangeComboSpan)
//	ON_CBN_SELCHANGE(IDC_COMBO_SPAN, &Ckgb_arch_mfcDlg::OnCbnSelchangeComboSpan)
//ON_NOTIFY(HDN_ENDDRAG, 0, &Ckgb_arch_mfcDlg::OnHdnEnddragList1)
ON_WM_DROPFILES()
ON_BN_CLICKED(IDC_HELP, &Ckgb_arch_mfcDlg::OnBnClickedHelp)
//ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, &Ckgb_arch_mfcDlg::OnLvnColumnclickList1)
ON_BN_CLICKED(IDC_BRECENTARCH, &Ckgb_arch_mfcDlg::OnBnClickedBrecentarch)
ON_WM_MENUSELECT()
ON_BN_CLICKED(IDC_CSHOWPASS, &Ckgb_arch_mfcDlg::OnBnClickedCshowpass)
END_MESSAGE_MAP()


// Ckgb_arch_mfcDlg message handlers

BOOL Ckgb_arch_mfcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, TRUE);		// Set small icon

	// TODO: Add extra initialization here

	DWORD curlev = 3;
	DWORD sfx = 0;
	DWORD keepFullPath = 0;
	//DWORD updates = 0;
	DWORD format = 0;

	CRegKey reg(HKEY_CURRENT_USER);
	if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
		reg.QueryDWORDValue("level", curlev);
		reg.QueryDWORDValue("sfx_module", sfx);
		reg.QueryDWORDValue("keep_full_path", keepFullPath);
		//reg.QueryDWORDValue("check_for_updates", updates);
		reg.QueryDWORDValue("format", format);
	}
	
	cOptions.SetCurSel((int)curlev);
	cSFX.SetCheck((int)sfx);
	cFullPath.SetCheck((int)keepFullPath);
	
	switch(format){
		case 0: /*rKGB.SetCheck(1); */OnBnClickedRadioKgb(); break;
		case 1: /*rZIP.SetCheck(1); */OnBnClickedRadioZip(); break;
	}

	/*if(updates){
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
	}*/
		

	//MessageBox(loadString(IDS_FILE));
	lFiles.InsertColumn(0, loadString(IDS_FILE), LVCFMT_LEFT, 300);
	lFiles.InsertColumn(1, loadString(IDS_SIZE), LVCFMT_RIGHT, 100);
	
	cSpan.SetCurSel(0);

	char *cmd = theApp.m_lpCmdLine;
	//char mode=0;
	char temp[MAX_PATH];
	bool isArchName = false;

	//MessageBox(cmd);

	for(unsigned long i=0;i<strlen(cmd);i++){
		if(cmd[i] == '"'){
			for(unsigned long j=0;i+j+1<strlen(cmd);j++){
				if(cmd[i+j+1] != '"')
					temp[j] = cmd[i+j+1];
				else if(cmd[i+j+1] == '"'){
					temp[j] = 0;
					i += j+1;

					GetLongPathName(temp, temp, sizeof(temp));

					//MessageBox(temp);

					if(temp[strlen(temp)-1] == '\\'){
						temp[strlen(temp)-1] = 0;
						loadDirs(&lFiles, temp);
						if(!isArchName){
							sprintf(temp, "%s.kgb", temp);
							eArchName.SetWindowText(temp);
						}
						continue;
					}

					//if(!isArchName){
						//lFiles.AddString(temp);
						
						FILE *f = fopen(temp, "rb");
						if(f == NULL){
							char msg[1024];
							sprintf(msg, "%s %s!", loadString(IDS_CANT_ACCESS_FILE), temp);
							MessageBox(msg, "KGB Archiver");
							break;
						}

						//lFiles.InsertItem(0, lFiles.GetItemCount(), 0, 0, 0, 0, 0);
						//lFiles.SetItemText(lFiles.GetItemCount()-1, 0, temp);
						_fseeki64(f, 0LL, SEEK_END);
						addFile(&lFiles, temp, _ftelli64(f));
						fclose(f);

						if(!isArchName){
							int k;
							for(k=strlen(temp)-1;k>=0;k--){
								if(temp[k] == '.')
									break;
								else if(temp[k] == '\\' || temp[k] == '/')
									k = -1;
							}
							if(k == -1)
								k = strlen(temp);
							temp[k] = 0;
							sprintf(temp, "%s.kgb", temp);
							eArchName.SetWindowText(temp);
							isArchName = true;
						}


						
						//sprintf(temp, "%s", convertUnits(_ftelli64(f)));
						//lFiles.SetItemText(lFiles.GetItemCount()-1, 1, temp);
		
					/*}else{
						eArchName.SetWindowText(temp);
						isArchName = false;
					}*/
					break;
				}
			}
		}
	}
			
	passwdchar = '•';
	ePassword.SetPasswordChar(passwdchar);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ckgb_arch_mfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Ckgb_arch_mfcDlg::OnPaint() 
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
HCURSOR Ckgb_arch_mfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Ckgb_arch_mfcDlg::OnBnClickedBcancel()
{
	EndDialog(IDCANCEL);
	// TODO: Add your control notification handler code here
}

void Ckgb_arch_mfcDlg::OnBnClickedBbrowse()
{
	char fileTypes[1024];
	strcpy(fileTypes, loadString(IDS_FILETYPES));
	CFileDialog fDialog(false, "kgb", 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_EXPLORER, fileTypes);
	if(fDialog.DoModal() == IDOK)
		eArchName.SetWindowText(fDialog.GetPathName());
}

void Ckgb_arch_mfcDlg::OnBnClickedBadd2compress()
{
	char fileTypes[1024];
	strcpy(fileTypes, loadString(IDS_FILETYPES_ALL));
	CFileDialog fDialog(true, 0, 0, OFN_HIDEREADONLY|OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, fileTypes);
	if(fDialog.DoModal() == IDOK){
		for(POSITION i=fDialog.GetStartPosition();i!=NULL;){
			//lFiles.AddString(fDialog.GetNextPathName(i));
			string file = fDialog.GetNextPathName(i);
			
			//sprawdzamy czy wpis nie istnieje juz
			bool exists = false;
			for(long j=0;j<lFiles.GetItemCount();j++){
				if(file == (string)lFiles.GetItemText(j, 0)){
					exists = true;
					break;
				}
			}

			if(exists)
				continue;
			
			
			//unsigned long fSize;
			//char buffer[32];
			FILE *f = fopen(file.c_str(), "rb");

			if(f == NULL){
				char msg[1024];
				sprintf(msg, "%s %s!", loadString(IDS_CANT_ACCESS_FILE), file.c_str());
				MessageBox(msg, "KGB Archiver");
				continue;
			}

			
			_fseeki64(f, 0L, SEEK_END);
			addFile(&lFiles, file, _ftelli64(f));
			fclose(f);
		}
	}

}

void Ckgb_arch_mfcDlg::OnBnClickedBremovefromcompress()
{
	//lFiles.DeleteString(lFiles.GetCurSel());
	//lFiles.DeleteItem(lFiles.GetSelectionMark());
	
	POSITION i = lFiles.GetFirstSelectedItemPosition();

	while(i != NULL){
		lFiles.DeleteItem(lFiles.GetNextSelectedItem(i));
		i = lFiles.GetFirstSelectedItemPosition();
	}

}

DWORD WINAPI updateForm(LPVOID lpParam){
	char buffer[1024];
	__int64 time_sTime;
	__int64 temp;

	setLang();

	while(1){
		try{		
			if(_size_all != 0 && /*_size != 0 && _done_all != 0 && _done != 0 && */_dCompress.m_hWnd != NULL){
				sprintf(buffer, "%s %.1f%%", loadString(IDS_COMPRESS_PROGRESS), _filename != COMPRESS_ENCODING ? (float)(100*(_done+_done_all))/(float)(_size_all) : 100);
				_dCompress.SetWindowText(buffer);
				
				strncpy(_dCompress.tray.szTip, buffer, sizeof(_dCompress.tray.szTip));
				switch((_filename != COMPRESS_ENCODING ? (int)((float)(100*(_done+_done_all))/(float)(_size_all)) : 100)/19){
					case 0: _dCompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY0); break;
					case 1: _dCompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY1); break;
					case 2: _dCompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY2); break;
					case 3: _dCompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY3); break;
					case 4: _dCompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY4); break;
					case 5: _dCompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY5); break;
				}
				Shell_NotifyIcon(NIM_MODIFY, &_dCompress.tray);
				

				_dCompress.pTotal.SetRange32(0, 100);
				_dCompress.pTotal.SetPos(((_done_all+_done)*100)/(_size_all));
				
				_dCompress.pCurrent.SetRange32(0, 100);
				_dCompress.pCurrent.SetPos((_done*100)/(_size+1));

				if(_filename.c_str()[0] > 32)
					_dCompress.sFilename.SetWindowText(_filename.c_str());
				else
					_dCompress.sFilename.SetWindowText(loadString(IDS_COMPRESS_STATUS+_filename.c_str()[0]-1));

				sprintf(buffer, "%s", convertUnits(_size_all));
				_dCompress.sFullSize.SetWindowText(buffer);
				
				strcpy(buffer, convertUnits(_compressed));
				sprintf(buffer, "%s (max %s)", buffer, convertUnits(_compressed+_size_all-_done_all-_done));
				_dCompress.sCompressedSize.SetWindowText(buffer);

				sprintf(buffer, "%.1f%%", ((float)_compressed*100.0)/(float)(_done_all+_done+1));
				_dCompress.sRatio.SetWindowText(buffer);

				time_sTime = time(NULL)-sTime;
				if((temp = time_sTime/3600) < 10)
					sprintf(buffer, "0");
				sprintf(buffer, "%s%lld:", buffer, temp);
				if((temp = (time_sTime%3600)/60) < 10)
					sprintf(buffer, "%s0", buffer);
				sprintf(buffer, "%s%lld:", buffer, temp);
				if((temp = time_sTime%60) < 10)
					sprintf(buffer, "%s0", buffer);
				sprintf(buffer, "%s%lld", buffer, temp);

				_dCompress.sTime.SetWindowText(buffer);
				buffer[0] = '\0';

				if(_filename.c_str()[0] > 32)
					time_sTime = (__int64)((double)time_sTime/((double)(_done+_done_all+1)/(double)(_size_all))) - time_sTime;
				else
					time_sTime = 0;
				if((temp = time_sTime/3600) < 10)
					sprintf(buffer, "0");
				sprintf(buffer, "%s%lld:", buffer, temp);
				if((temp = (time_sTime%3600)/60) < 10)
					sprintf(buffer, "%s0", buffer);
				sprintf(buffer, "%s%lld:", buffer, temp);
				if((temp = time_sTime%60) < 10)
					sprintf(buffer, "%s0", buffer);
				sprintf(buffer, "%s%lld", buffer, temp);

				_dCompress.sRemaining.SetWindowText(buffer);
				

				//_dCompress.UpdateWindow();

				_shutdown = _dCompress.cShutdown.GetCheck();

				if(_done_all == _size_all){
					_dCompress.EndDialog(IDOK);
					//return 0;
				}
			}/*else if(_filename.c_str()[0] == COMPRESS_NOTHING2COMPRESS[0]){
				MessageBox(0, loadString(IDS_NOTHING2COMPRESS), "KGB Archiver", 0);
				_dCompress.EndDialog(100);
				return 0;
			}*/
		}catch(...){}
		Sleep(500);
	}
	free(buffer);
	return 0;
}

void Ckgb_arch_mfcDlg::OnBnClickedBnext()
{
	if(eArchName.GetWindowTextLength() == 0 || lFiles.GetItemCount() == 0){
		MessageBox(loadString(IDS_TYPE_ARCHIVE_NAME), "KGB Archiver");
		return;
	}

	if(ePassword.GetWindowTextLength() != 0 && (ePassword.GetWindowTextLength() < 4 || ePassword.GetWindowTextLength() > 32)){
		MessageBox(loadString(IDS_PASSWDLEN), "KGB Archiver");
		return;
	}

	for(int i=0;i<lFiles.GetItemCount();i++){
		CString _archname;
		eArchName.GetWindowText(_archname);
		if(lFiles.GetItemText(i, 0) == _archname){
			MessageBox(loadString(IDS_WRONG_ARCHIVE_NAME), "KGB Archiver");
			return;
		}
	}

	if(rKGB.GetCheck()){
		MEMORYSTATUS RAM;
		__int64 RAM_table[] = {9, 11, 15, 27, 66, 154, 202, 402, 800, 1600};
		GlobalMemoryStatus(&RAM);

		if(RAM_table[cOptions.GetCurSel()]*1024*1024 > (__int64)RAM.dwAvailPageFile){
			MessageBox(loadString(IDS_OUT_OF_MEMORY), "KGB Archiver");
			cOptions.SetFocus();
			return;
		}

		if(RAM_table[cOptions.GetCurSel()]*1024*1024 > (__int64)RAM.dwTotalPhys*3/4){
			if(MessageBox(loadString(IDS_NOT_ENOUGH_RAM), "KGB Archiver", MB_YESNO | MB_DEFBUTTON2) == IDNO){
				cOptions.SetFocus();
				return;
			}
		}
	}

	CDialog::ShowWindow(SW_HIDE);

	//kompresja
	char buff[1024];
	resetSettings();
	MEM = cOptions.GetCurSel();

	cSpan.GetWindowText(buff, sizeof(buff));
	if(strlen(buff) == 0)
		strcpy(buff, "0");
	sscanf(buff, "%d", &spanSize);

	archive_name = (char *)malloc(sizeof(char)*MAX_PATH);
	eArchName.GetWindowText(archive_name, MAX_PATH);
	sfx_arch = cSFX.GetCheck();

	char tab[3][MAX_PATH];
	bool isDiff = false;
	unsigned long diffIdx=0;

	//lFiles.GetText(0, tab[0]);
	string tmp = lFiles.GetItemText(0, 0);
	strcpy(tab[0], tmp.c_str());

	if(lFiles.GetItemCount() > 1){
	for(unsigned long i=0;i<strlen(tab[0]);i++){
		for(unsigned long j=0;j<lFiles.GetItemCount()-1;j++){
			//lFiles.GetText(j, tab[1]);
			//lFiles.GetText(j+1, tab[2]);
			tmp = lFiles.GetItemText(j, 0);
			strcpy(tab[1], tmp.c_str());
			tmp = lFiles.GetItemText(j+1, 0);
			strcpy(tab[2], tmp.c_str());
			if(tab[1][i] != tab[2][i]){
				isDiff = true;
				for(unsigned long k=i;k>0;k--){
					if((tab[1][k] == '\\' || tab[1][k] == '/') && (tab[2][k] == '\\' || tab[2][k] == '/')){
						diffIdx = k+1;
						tab[2][diffIdx]=0;
						break;
					}
				}
				break;
			}
		}
		if(isDiff)
			break;
	}
	}else if(lFiles.GetItemCount() == 1){
		for(unsigned long k=strlen(tab[0])-1;k>=0;k--){
			if(tab[0][k] == '\\' || tab[0][k] == '/'){
				diffIdx = k+1;
				//lFiles.GetText(0, tab[2]);
				tmp = lFiles.GetItemText(0, 0);
				strcpy(tab[2], tmp.c_str());
				tab[2][diffIdx]=0;
				break;
			}
		}
	}

	if((long)diffIdx < 0){
		diffIdx = 0;
		tab[2][0] = 0;
	}

	if(cFullPath.GetCheck() == BST_CHECKED){
		diffIdx = 0;
	}

	//char *buffer/*[MAX_PATH]*/;
	string buffer2;

	for(unsigned long i=0;i<lFiles.GetItemCount();i++){
		//lFiles.GetText(i, buffer);
		buffer2 = /*tmp =*/ lFiles.GetItemText(i, 0);
		//buffer = (char *)tmp.c_str();
		//buffer2 = buffer;
		/*sprintf(buffer, "%u %d", diffIdx, buffer2.length());
		MessageBox(buffer);*/
		//if(diffIdx >= 3)
			buffer2 = buffer2.substr(diffIdx);
		//MessageBox(buffer2.c_str());
		filename.push_back(buffer2);
	}

	//MessageBox(tab[2]);

	SetCurrentDirectory(tab[2]);
	
	
	/*SECURITY_ATTRIBUTES *security = (SECURITY_ATTRIBUTES *)malloc(sizeof(SECURITY_ATTRIBUTES));
	security->nLength = sizeof(SECURITY_ATTRIBUTES);
	security->lpSecurityDescriptor = (LPVOID)THREAD_ALL_ACCESS;
	security->bInheritHandle = true;*/

	//przygotowanie do szyfrowania:
	if(ePassword.GetWindowTextLength() != 0){
		ePassword.GetWindowText(passwd, 32);
		for(int i=strlen(passwd);i<32;i++){
			passwd[i] = '\0';
		}
	}

	/*if(rKGB.GetCheck() && sfx_arch)
		passwd[0] = '\0';*/

	HANDLE hCompressThread;
	DWORD dwCompressThreadId;
	
	if(rKGB.GetCheck()){
		hCompressThread = CreateThread(/*security*/NULL, 0, compress, 0, 0, &dwCompressThreadId);
	}else if(rZIP.GetCheck()){
		hCompressThread = CreateThread(/*security*/NULL, 0, zipCompress, 0, 0, &dwCompressThreadId);
	}

	SetThreadPriority(hCompressThread, THREAD_PRIORITY_LOWEST);


	HANDLE hUpdateThread;
	DWORD dwUpdateThreadId;
	hUpdateThread = CreateThread(/*security*/NULL, 0, updateForm, 0, 0, &dwUpdateThreadId);
	//SetThreadPriority(hUpdateThread, THREAD_PRIORITY_HIGHEST);

	//int modalMode;
	if(/*modalMode = */_dCompress.DoModal() == IDCANCEL/* || modalMode == 100*/){
		TerminateThread(hCompressThread, 0);
		_fcloseall();
		DeleteFile(archive_name);
		//if(modalMode == IDCANCEL)
			MessageBox(loadString(IDS_COMPRESSION_CANCEL), "KGB Archiver");
	}else{
		//char buff[1024];
		CRegKey reg(HKEY_CURRENT_USER);
		reg.Create(HKEY_CURRENT_USER, "Software\\KGB Archiver");
		reg.SetDWORDValue("level", cOptions.GetCurSel());
		reg.SetDWORDValue("sfx_module", cSFX.GetCheck());
		reg.SetDWORDValue("keep_full_path", cFullPath.GetCheck());
		if(rKGB.GetCheck()){
			reg.SetDWORDValue("format", 0);
		}else if(rZIP.GetCheck()){
			reg.SetDWORDValue("format", 1);
		}

		if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
			char _temp[MAX_PATH];
			char val[32];
			ULONG size ;
			bool contains = false;
			vector<string>files;

			for(int i=0;i<10;i++){
				size = sizeof(_temp);
				sprintf(val, "recentarch%d", i);
				if(reg.QueryStringValue(val, _temp, &size) == ERROR_SUCCESS){
					files.push_back(_temp);
				}
				if((string)archive_name == (string)_temp){
					contains = true;
					break;
				}
			}
			if(!contains){
				reg.SetStringValue("recentarch0", archive_name);
				for(int i=0;i<(files.size() < 9 ? files.size() : 9);i++){
					sprintf(val, "recentarch%d", i+1);
					reg.SetStringValue(val, files[i].c_str());
				}
			}
		}
		
		if(_shutdown){
			HANDLE hToken; 
            TOKEN_PRIVILEGES tkp;
            OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

            LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);

            tkp.PrivilegeCount = 1;  // one privilege to set
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

            ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
		}
		
		char _size_all_buff[16], _compressed_buff[16];
		DWORD stats = 1;
		strcpy(_size_all_buff, convertUnits(_size_all));
		strcpy(_compressed_buff, convertUnits(_compressed));
		sprintf(buff, loadString(IDS_COMPRESSION_SUCCED_STATS), _size_all_buff, _compressed_buff, ((float)_compressed*100.0)/_size_all);
		__int64 time_sTime, temp;
		time_sTime = time(NULL)-sTime;
		if((temp = time_sTime/3600) < 10)
			sprintf(buff, "%s0", buff);
		sprintf(buff, "%s%lld:", buff, temp);
		if((temp = (time_sTime%3600)/60) < 10)
			sprintf(buff, "%s0", buff);
		sprintf(buff, "%s%lld:", buff, temp);
		if((temp = time_sTime%60) < 10)
			sprintf(buff, "%s0", buff);
		sprintf(buff, "%s%lld", buff, temp);

		if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
			reg.QueryDWORDValue("show_stats", stats);
		}
		if(stats)
			::MessageBox(NULL, buff, "KGB Archiver", MB_TASKMODAL);
		else
			MessageBeep(MB_OK);
		
	}

	TerminateThread(hUpdateThread, 0);

	Shell_NotifyIcon(NIM_DELETE, &_dCompress.tray);

	CDialog::EndDialog(IDOK);
}

void loadDirs(CListCtrl *lFiles, string dir){
	struct _finddatai64_t c_file;
	long hFile;
	char buff2[MAX_PATH];
	string dir2;
	bool exists;

	SetCurrentDirectory(dir.c_str());

	if((hFile = _findfirsti64("*.*", &c_file)) == -1L){
		//katalog jest pusty
	}else{
		//c_file.attrib & _A_RDONLY;
        //c_file.attrib & _A_SYSTEM;
        //c_file.attrib & _A_HIDDEN;
        //c_file.attrib & _A_ARCH;
		//c_file.attrib & _A_SUBDIR;
		//c_file.attrib = _A_HIDDEN;

		while(_findnexti64(hFile, &c_file) == 0){
			if((c_file.name[0] == '.' && c_file.name[1] == '.') || c_file.name[0] == '.')
				continue;
			if(c_file.attrib & _A_SUBDIR){
				dir2 = dir + '\\' + c_file.name;
				loadDirs(lFiles, dir2);
				continue;
			}

			sprintf(buff2, "%s\\%s", dir.c_str(), c_file.name);

			//sprawdzamy czy juz nie jes na liscie
			exists = false;
			for(int i=0;i<lFiles->GetItemCount();i++){
				if(strcmp(lFiles->GetItemText(i, 0).GetString(), buff2) == 0){
					exists = true;
				}
			}
			if(exists)
				continue;

			
			/*lFiles->InsertItem(0, lFiles->GetItemCount(), "", 0, 0, 0, 0);
			//sprintf(buff2, "%s\\%s", dir.c_str(), c_file.name);
			lFiles->SetItemText(lFiles->GetItemCount()-1, 0, buff2);
			sprintf(buff2, "%s", convertUnits(c_file.size));
			lFiles->SetItemText(lFiles->GetItemCount()-1, 1, buff2);*/
			addFile(lFiles, buff2, c_file.size);
        }

	}
}

void Ckgb_arch_mfcDlg::OnBnClickedBadd2compress2()
{
	BROWSEINFO bInfo;
	bInfo.hwndOwner = m_hWnd;
	bInfo.lpfn = NULL;
	bInfo.lParam = NULL;
	bInfo.lpszTitle = loadString(IDS_CHOOSE_DIR2COMPRESS);
	bInfo.pidlRoot = NULL;
	bInfo.ulFlags = BIF_USENEWUI | BIF_VALIDATE;
	bInfo.pszDisplayName = NULL;//foldername;

	LPITEMIDLIST pidl;

	if((pidl = SHBrowseForFolder(&bInfo)) != NULL){
		char buff[MAX_PATH];
		SHGetPathFromIDList(pidl, buff);
		//GetCurrentDirectory(MAX_PATH, foldername);
		//eDestination.SetWindowText(foldername);
		//eDestination.SetWindowText(buff);
		
		
		loadDirs(&lFiles, buff);
	}
}

void Ckgb_arch_mfcDlg::OnEnChangePassword()
{
	/*if(ePassword.GetWindowTextLength() != 0){
		cSFX.EnableWindow(false);
		cSFX.SetCheck(0);
	}else{
		cSFX.EnableWindow(true);
	}*/
	setControls();	
}

void Ckgb_arch_mfcDlg::OnBnClickedCheck1()
{
	/*if(cSFX.GetCheck()){
		ePassword.EnableWindow(false);
		ePassword.SetWindowText("");
	}else
		ePassword.EnableWindow(true);*/
	setControls();
}

void Ckgb_arch_mfcDlg::setControls(){
	if(rKGB.GetCheck()){
		cSpan.EnableWindow(0);
		//cSFX.EnableWindow(ePassword.GetWindowTextLength() == 0 ? 1 : 0);
		//ePassword.EnableWindow(cSFX.GetCheck() == 1 ? 0 : 1);
	}else if(rZIP.GetCheck()){
		cSpan.EnableWindow(1);
		//cSFX.EnableWindow(0);
		//ePassword.EnableWindow(1);
	}
}

void Ckgb_arch_mfcDlg::OnBnClickedRadioKgb()
{
	rKGB.SetCheck(1);
	rZIP.SetCheck(0);
	
	setControls();

	if(eArchName.GetWindowTextLength() < 4)
		return;
	char arch[MAX_PATH];
	eArchName.GetWindowText(arch, sizeof(arch));
	arch[strlen(arch)-4] = '\0';
	sprintf(arch, "%s.kgb", arch);
	eArchName.SetWindowText(arch);
}

void Ckgb_arch_mfcDlg::OnBnClickedRadioZip()
{
	rKGB.SetCheck(0);
	rZIP.SetCheck(1);
	
	setControls();

	if(eArchName.GetWindowTextLength() < 4)
		return;
	char arch[MAX_PATH];
	eArchName.GetWindowText(arch, sizeof(arch));
	arch[strlen(arch)-4] = '\0';
	sprintf(arch, "%s.zip", arch);
	eArchName.SetWindowText(arch);
}

void Ckgb_arch_mfcDlg::OnEnChangeEarchname()
{
	char arch[MAX_PATH];
	if(eArchName.GetWindowTextLength() < 4)
		return;

	eArchName.GetWindowText(arch, sizeof(arch));
	if(tolower(arch[strlen(arch)-4]) == '.' && tolower(arch[strlen(arch)-3]) == 'k' && tolower(arch[strlen(arch)-2]) == 'g' && tolower(arch[strlen(arch)-1]) == 'b'){
		rKGB.SetCheck(1);
		rZIP.SetCheck(0);
	}else if(tolower(arch[strlen(arch)-4]) == '.' && tolower(arch[strlen(arch)-3]) == 'z' && tolower(arch[strlen(arch)-2]) == 'i' && tolower(arch[strlen(arch)-1]) == 'p'){
		rKGB.SetCheck(0);
		rZIP.SetCheck(1);
	}else{
		sprintf(arch, "%s.kgb", arch);
		eArchName.SetWindowText(arch);
		eArchName.SetSel(strlen(arch)-4, strlen(arch)-4, 0);
		rKGB.SetCheck(1);
		rZIP.SetCheck(0);
	}
	setControls();

}


void Ckgb_arch_mfcDlg::OnCbnEditchangeComboSpan()
{
	char buffer[32];
	cSpan.GetWindowText(buffer, sizeof(buffer));

	for(int i=0;i<strlen(buffer);i++){
		if(!(buffer[i] >= '0' && buffer[i] <= '9')){
			MessageBox(loadString(IDS_ONLY_NUMBERS_ACCEPTED), "KGB Archiver");
			cSpan.SetWindowText("0");
			return;
		}
	}
}

void Ckgb_arch_mfcDlg::OnDropFiles(HDROP hDropInfo)
{
	//MessageBox("");
	// TODO: Add your message handler code here and/or call default
	
	UINT  uNumFiles;
	TCHAR szNextFile[MAX_PATH];

    // Get the # of files being dropped.
    uNumFiles = DragQueryFile(hDropInfo, -1, NULL, 0);

    for(UINT uFile = 0; uFile < uNumFiles; uFile++)
        {
        // Get the next filename from the HDROP info.
        if (DragQueryFile(hDropInfo, uFile, szNextFile, MAX_PATH) > 0)
            {
				//sprawdzamy czy wpis nie istnieje juz
				bool exists = false;
				for(long j=0;j<lFiles.GetItemCount();j++){
					if((string)szNextFile == (string)lFiles.GetItemText(j, 0)){
						exists = true;
						break;
					}
				}

				if(exists)
					continue;
				
				char buffer[32];
				FILE *f = fopen((char *)szNextFile, "rb");

				if(f == NULL){
					if(SetCurrentDirectory(szNextFile)){
						loadDirs(&lFiles, szNextFile);
						continue;
					}
					char msg[1024];
					sprintf(msg, "%s %s!", loadString(IDS_CANT_ACCESS_FILE), (char *)szNextFile);
					MessageBox(msg, "KGB Archiver");
					continue;
				}

				//lFiles.InsertItem(0, lFiles.GetItemCount(), 0, 0, 0, 0, 0); 
				//lFiles.SetItemText(lFiles.GetItemCount()-1, 0, (char *)szNextFile);

				_fseeki64(f, 0L, SEEK_END);
				//sprintf(buffer, "%s", convertUnits(_ftelli64(f)));
				addFile(&lFiles, szNextFile, _ftelli64(f));
				fclose(f);
				//lFiles.SetItemText(lFiles.GetItemCount()-1, 1, buffer);
            }
        }

    // Free up memory.
    //DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void Ckgb_arch_mfcDlg::OnBnClickedHelp()
{
	MessageBox(loadString(IDS_MEMORY_USAGE), "KGB Archiver");
	//MessageBox("Memory usage:\nKGB Compression:\nMinimal\t\t\t9MB\nVery weak\t\t11MB\nWeak\t\t\t15MB\nNormal\t\t\t27MB\nMedium\t\t\t66MB\nAbove medium\t\t154MB\nGood\t\t\t202MB\nVery good\t\t402MB\nExtreme\t\t\t800MB\nMaximum\t\t\t1600MB\n\nZIP Compression:\nAll modes take about 10MB");
}


void Ckgb_arch_mfcDlg::OnBnClickedBrecentarch()
{
	CMenu menu;
	menu.CreatePopupMenu();
	
	CRegKey reg(HKEY_CURRENT_USER);

	if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
		char val[32];
		char buffer[MAX_PATH];
		ULONG size;
		for(unsigned char i=0;i<10;i++){
			size = sizeof(buffer);
			sprintf(val, "recentarch%d", i);
			if(reg.QueryStringValue(val, buffer, &size) == ERROR_SUCCESS){
				menu.AppendMenu(MF_STRING, BASE_MENU+i, buffer);
			}
		}
	}

	RECT rect;
	bRecentArch.GetWindowRect(&rect);
	
	TrackPopupMenu(menu.m_hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.right, rect.top, 0, m_hWnd, NULL);
}

void Ckgb_arch_mfcDlg::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CDialog::OnMenuSelect(nItemID, nFlags, hSysMenu);

	// TODO: Add your message handler code here
	CRegKey reg(HKEY_CURRENT_USER);
	if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
		char val[32];
		char buffer[MAX_PATH];
		ULONG size = sizeof(buffer);
		
		/*sprintf(buffer, "%d", nItemID-BASE_MENU);
		MessageBox(buffer);*/

		if(nItemID-BASE_MENU >= 0 && nItemID-BASE_MENU < 10){
			sprintf(val, "recentarch%d", nItemID-BASE_MENU);
			if(reg.QueryStringValue(val, buffer, &size) == ERROR_SUCCESS)
				eArchName.SetWindowText(buffer);
		}
	}
}

void Ckgb_arch_mfcDlg::OnBnClickedCshowpass()
{
	cShowPass.GetCheck() ? ePassword.SetPasswordChar(0) : ePassword.SetPasswordChar(passwdchar);
	ePassword.RedrawWindow();
	ePassword.SetFocus();
}
