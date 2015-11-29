//#include "..\kgb_arch_mfc\kgb_arch_mfcdlg.cpp"
// kgb_arch_decompressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_decompress.h"
#include "kgb_arch_decompressDlg.h"
#include "dDecompress.h"
#include "../compress/compress_decompress.cpp"
#include "io.h"
#include "dlgPasswd.h"
#include "../zip/zipArchive.h"
#include "../common/units.cpp"
#include "../common/lang.cpp"

#define BASE_MENU 0xF000

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

dDecompress _dDecompress;
bool _shutdown;

/*bool zipIsEncrypted(char *arch){
	CZipArchive zip;
	CZipFileHeader fhInfo;

	zip.Open(arch);
	zip.GetFileInfo(fhInfo, 0);
	zip.Close();
	
	return fhInfo.IsEncrypted();
}

bool zipCheckPasswd(char *arch, char *pass){
	CZipArchive zip;
	CZipFileHeader fhInfo;

	zip.Open(arch);
	zip.GetFileInfo(fhInfo, 0);
	zip.Close();

	if(strcmp(zip.GetPassword().GetString(), pass) == 0)
		return true;
	return false;
}*/

void zipGetFiles(char *arch, vector<string>&filenames, vector<__int64>&filesizes){
	CZipArchive zip;
	zip.Open(arch, CZipArchive::zipOpenReadOnly);
	for(int i=0;i<zip.GetCount(true);i++){
		CZipFileHeader fhInfo;
		zip.GetFileInfo(fhInfo, i);
		filenames.push_back(fhInfo.GetFileName().GetBuffer());
		filesizes.push_back(fhInfo.GetEffComprSize() != 0 ? __int64((float)fhInfo.GetEffComprSize()*100.0f/fhInfo.GetCompressionRatio()) : 0);
	}
	zip.Close();
}

DWORD WINAPI zipDecompress(LPVOID lpParam){
	_filename = DECOMPRESS_PREPARING;
	sTime = _time64(NULL);

	setLang();

	char overwrite_mode = OVERWRITE_YES;

	CZipArchive zip;

	vector<string>filenames;
	vector<__int64>filesizes;

	FILE *archive = fopen(archive_name, "rb");
	if(!archive){
		MessageBox(0, loadString(IDS_ARCHIVE_DOESNT_EXIST), "KGB Archiver", 0);
		return false;
	}
	_fseeki64(archive, 0LL, SEEK_END);
	_compressed = _ftelli64(archive);
	fclose(archive);

	zipGetFiles(archive_name, filenames, filesizes);

	//char buff[123];

	for(int i=0;i<filesizes.size();i++){
		_size_all += filesizes[i];
		/*if(filesizes[i] <= 0){
		sprintf(buff, "%s %lld", filenames[i].c_str(), filesizes[i]);
		MessageBox(0, buff, "", 0);}*/
	}

	
	/*sprintf(buff, "%lld", _size_all);
	MessageBox(0, buff, "", 0);*/

	zip.Open(archive_name, CZipArchive::zipOpenReadOnly);

	for(int i=0;i<filenames.size();i++){
		if(files4decompress.size() != 0){
			for(int j=0;j<files4decompress.size();j++){
				if(filenames[i] == files4decompress[j]){
					_filename = filenames[i];
					
					try{
						if(overwrite_mode != OVERWRITE_YES_ALL && overwrite_mode != OVERWRITE_NO_ALL)
							overwrite_mode = OVERWRITE_YES;
						if(_access(filenames[i].c_str(), 4) == NULL){
							if(overwrite_mode != OVERWRITE_YES_ALL && overwrite_mode != OVERWRITE_NO_ALL){
								CZipFileHeader fhInfo;
								zip.GetFileInfo(fhInfo, i);
								overwrite_mode = overwrite(filenames[i], filesizes[i], (__time64_t)fhInfo.GetTime());
							}
						}
						if((overwrite_mode == OVERWRITE_YES || overwrite_mode == OVERWRITE_YES_ALL) || _access(filenames[i].c_str(), 4) != NULL)
							zip.ExtractFile(i, "");
					}catch(...){
						CZipFileHeader fhInfo;
						zip.GetFileInfo(fhInfo, i);
						if(fhInfo.IsEncrypted()){
							dlgPasswd dlgPass;
							if(dlgPass.DoModal() != IDOK){
								_dDecompress.EndDialog(IDCANCEL);
								return false;
							}
							zip.SetPassword(dlgPass._passwd);
							j--;
							continue;
						}
					}

					break;
				}
			}
		}else{
			_filename = filenames[i];
			try{
				if(overwrite_mode != OVERWRITE_YES_ALL && overwrite_mode != OVERWRITE_NO_ALL)
					overwrite_mode = OVERWRITE_YES;
				if(_access(filenames[i].c_str(), 4) == NULL){
					if(overwrite_mode != OVERWRITE_YES_ALL && overwrite_mode != OVERWRITE_NO_ALL){
						CZipFileHeader fhInfo;
						zip.GetFileInfo(fhInfo, i);
						overwrite_mode = overwrite(filenames[i], filesizes[i], (__time64_t)fhInfo.GetTime());
					}
				}
				if((overwrite_mode == OVERWRITE_YES || overwrite_mode == OVERWRITE_YES_ALL) || _access(filenames[i].c_str(), 4) != NULL)
					zip.ExtractFile(i, "");
			}catch(...){
				CZipFileHeader fhInfo;
				zip.GetFileInfo(fhInfo, i);
				if(fhInfo.IsEncrypted()){
					dlgPasswd dlgPass;
					if(dlgPass.DoModal() != IDOK){
						_dDecompress.EndDialog(IDCANCEL);
						return false;
					}
					zip.SetPassword(dlgPass._passwd);
					i--;
					continue;
				}
			}
		}
		_done_all += filesizes[i];
	}
	zip.Close();
	return true;
}

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


// Ckgb_arch_decompressDlg dialog


Ckgb_arch_decompressDlg::Ckgb_arch_decompressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ckgb_arch_decompressDlg::IDD, pParent)
	, bRecentArchives(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ckgb_arch_decompressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EARCHNAME, eArchName);
	DDX_Control(pDX, IDC_EARCHNAME2, eDestination);
	DDX_Control(pDX, IDC_LIST1, lFiles);
	DDX_Control(pDX, IDC_CHECK1, cSelect);
	DDX_Control(pDX, IDC_ARCHIVE_INFO, sArchiveInfo);
	DDX_Control(pDX, IDC_BBROWSE, bBrowse);
	DDX_Control(pDX, IDC_BRECENTARCHIVES, bRecentFiles);
	DDX_Control(pDX, IDC_BRECENTDIRS, bRecentDirs);
}

BEGIN_MESSAGE_MAP(Ckgb_arch_decompressDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BBROWSE, OnBnClickedBbrowse)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BBROWSE2, OnBnClickedBbrowse2)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_EN_CHANGE(IDC_EARCHNAME, OnEnChangeEarchname)
	ON_BN_CLICKED(IDC_BRECENTARCHIVES, &Ckgb_arch_decompressDlg::OnBnClickedBrecentarchives)
	ON_WM_MENUSELECT()
//	ON_WM_MENUCHAR()
ON_WM_MENUSELECT()
//ON_WM_CONTEXTMENU()
//ON_WM_MENUCHAR()
ON_BN_CLICKED(IDC_BRECENTDIRS, &Ckgb_arch_decompressDlg::OnBnClickedBrecentdirs)
END_MESSAGE_MAP()


// Ckgb_arch_decompressDlg message handlers

BOOL Ckgb_arch_decompressDlg::OnInitDialog()
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

	/*DWORD updates = 0;

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
	}*/
	
	lFiles.InsertColumn(0, loadString(IDS_FILE), LVCFMT_LEFT, 300);
	lFiles.InsertColumn(1, loadString(IDS_SIZE), LVCFMT_RIGHT, 100);

	char *cmd = theApp.m_lpCmdLine;
	char temp[MAX_PATH];
	bool isArchName = true;

	for(unsigned long i=0;i<strlen(cmd);i++){
		if(cmd[i] == '"'){
			for(unsigned long j=0;i+j+1<strlen(cmd);j++){
				if(cmd[i+j+1] != '"')
					temp[j] = cmd[i+j+1];
				else if(cmd[i+j+1] == '"'){
					temp[j] = 0;
					i += j+1;
					GetLongPathName(temp, temp, sizeof(temp));
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
	}

	for(unsigned long i=0;i<strlen(cmd)-1;i++){ //-1 zeby sie nie przepe³nic za chwile
		if(cmd[i] == '"')
			break;
		if(cmd[i] == '/'){
			if(cmd[i+1] == 's'){//tryb silent
				OnBnClickedButton1();
			}
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ckgb_arch_decompressDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else
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

void Ckgb_arch_decompressDlg::OnBnClickedBbrowse()
{
	char fileTypes[1024];
	strcpy(fileTypes, loadString(IDS_FILETYPE));
	CFileDialog dOpen(true, "kgb", 0, OFN_HIDEREADONLY|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST, fileTypes);
	if(dOpen.DoModal() == IDOK){
		eArchName.SetWindowText(dOpen.GetPathName());
	}
}

DWORD WINAPI updateForm(LPVOID lpParam){
	char buffer[1024];
	unsigned __int64 time_sTime;
	unsigned __int64 temp;

	setLang();

	while(1){
		try{		
			if(_size_all != 0 && /*_size != 0 && _done != 0 && _done_all != 0 && */_dDecompress.m_hWnd != NULL){
				sprintf(buffer, "%s %.1f%%", loadString(IDS_DECOMPRESS_PROGRESS), (float)((_done_all+_done)*100)/(float)(_size_all));
				_dDecompress.SetWindowText(buffer);

				strncpy(_dDecompress.tray.szTip, buffer, sizeof(_dDecompress.tray.szTip));
				switch((int)((float)((_done_all+_done)*100)/(float)(_size_all))/19){
					case 0: _dDecompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY0); break;
					case 1: _dDecompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY1); break;
					case 2: _dDecompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY2); break;
					case 3: _dDecompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY3); break;
					case 4: _dDecompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY4); break;
					case 5: _dDecompress.tray.hIcon = AfxGetApp()->LoadIcon(IDI_TRAY5); break;
				}
				Shell_NotifyIcon(NIM_MODIFY, &_dDecompress.tray);

				_dDecompress.pTotal.SetRange32(0, 100);
				_dDecompress.pTotal.SetPos(((_done_all+_done)*100)/(_size_all));
				
				_dDecompress.pCurrent.SetRange32(0, 100);
				_dDecompress.pCurrent.SetPos((_done*100)/(_size+1));

				if(_filename.c_str()[0] > 32)
					_dDecompress.sFilename.SetWindowText(_filename.c_str());
				else
					_dDecompress.sFilename.SetWindowText(loadString(IDS_DECOMPRESS_STATUS+_filename.c_str()[0]-1));

				sprintf(buffer, "%s", convertUnits(_size_all));
				_dDecompress.sFullSize.SetWindowText(buffer);

				sprintf(buffer, "%s", convertUnits(_compressed));
				_dDecompress.sCompressedSize.SetWindowText(buffer);

				sprintf(buffer, "%.1f%%", ((double)_compressed*100.0)/(double)(_size_all));
				_dDecompress.sRatio.SetWindowText(buffer);

				buffer[0] = '\0';
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

				_shutdown = _dDecompress.cShutdown.GetCheck();

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
	if(eArchName.GetWindowTextLength() == 0 || eDestination.GetWindowTextLength() == 0){
		MessageBox(loadString(IDS_TYPE_ARCHIVE_NAME), "KGB Archiver");
		return;
	}
	
	resetSettings();

	archive_name = (char *)malloc(sizeof(char)*MAX_PATH);
	eArchName.GetWindowText(archive_name, MAX_PATH);

	if(strlen(archive_name) < 4)
		return;

	if(_access(archive_name, 4) != 0){
		MessageBox(loadString(IDS_ARCHIVE_DOESNT_EXIST), "KGB Archiver");
		return;
	}

	char buffer[MAX_PATH];
	eDestination.GetWindowText(buffer, MAX_PATH);
	CreateDirectory(buffer, NULL);
	if(!SetCurrentDirectory(buffer)){
		MessageBox(loadString(IDS_CANT_EXTRACT_DIR_NOT_FOUND), "KGB Archiver");
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

	/*if(zipIsEncrypted(archive_name)){
		dlgPasswd dlgPass;
		if(dlgPass.DoModal() != IDOK)
			return;
		if(zipCheckPasswd(archive_name, dlgPass._passwd))
			strcpy(passwd, dlgPass._passwd);
		else{
			MessageBox(loadString(IDS_WRONG_PASSWD), "KGB Archiver");
			return;
		}
	}*/
	
	ShowWindow(SW_HIDE);

	HANDLE hDecompress;
	DWORD hDecompressId;

	int archive_type = tolower(archive_name[strlen(archive_name)-4]) == '.' && tolower(archive_name[strlen(archive_name)-3]) == 'k' && tolower(archive_name[strlen(archive_name)-2]) == 'g' && tolower(archive_name[strlen(archive_name)-1]) == 'b' ? 0 : -1;
	archive_type = tolower(archive_name[strlen(archive_name)-4]) == '.' && tolower(archive_name[strlen(archive_name)-3]) == 'z' && tolower(archive_name[strlen(archive_name)-2]) == 'i' && tolower(archive_name[strlen(archive_name)-1]) == 'p' && archive_type == -1 ? 1 : -1;
	
	archive_type = checkArchiveFormat(archive_name) && archive_type == -1 ? 0 : -1;
	if(archive_type == -1){
		try{
			CZipArchive zip;
			zip.Open(archive_name, CZipArchive::zipOpenReadOnly);
			zip.Close();
		}catch(...){return;}
		archive_type = 1;
	}

	if(archive_type == 0){
		MEMORYSTATUS RAM;
		__int64 RAM_table[] = {9, 11, 15, 27, 66, 154, 202, 402, 800, 1600};
		GlobalMemoryStatus(&RAM);

		if(RAM_table[getArchOption(archive_name)]*1024*1024 > (__int64)RAM.dwAvailPageFile){
			MessageBox(loadString(IDS_OUT_OF_MEMORY), "KGB Archiver");
			ShowWindow(SW_SHOW);
			return;
		}

		hDecompress = CreateThread(NULL, 0, decompress, 0, 0, &hDecompressId);
	}else if(archive_type == 1){
		hDecompress = CreateThread(NULL, 0, zipDecompress, 0, 0, &hDecompressId);
	}

	SetThreadPriority(hDecompress, THREAD_PRIORITY_LOWEST);

	HANDLE hUpdate;
	DWORD hUpdateId;
	hUpdate = CreateThread(NULL, 0, updateForm, 0, 0, &hUpdateId);
	
	if(_dDecompress.DoModal() == IDCANCEL){
		TerminateThread(hDecompress, 0);
		MessageBox(loadString(IDS_EXTRACT_CANCELED), "KGB Archiver");
	}else{
		CRegKey reg(HKEY_CURRENT_USER);
		if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
			char _temp[MAX_PATH];
			char val[32];
			char destination[MAX_PATH];
			ULONG size;
			bool contains = false;
			vector<string>files;

			eDestination.GetWindowText(destination, MAX_PATH);

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

			files.clear();
			contains = false;

			for(int i=0;i<10;i++){
				size = sizeof(_temp);
				sprintf(val, "recentdir%d", i);
				if(reg.QueryStringValue(val, _temp, &size) == ERROR_SUCCESS){
					files.push_back(_temp);
				}
				if((string)destination == (string)_temp){
					contains = true;
					break;
				}
			}
			if(!contains){
				reg.SetStringValue("recentdir0", destination);
				for(int i=0;i<(files.size() < 9 ? files.size() : 9);i++){
					sprintf(val, "recentdir%d", i+1);
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
		
		char buff[1024];
		__int64 time_sTime, temp;
		char _size_all_buff[16], _compressed_buff[16];
		
		DWORD stats = 1;
		strcpy(_size_all_buff, convertUnits(_size_all));
		strcpy(_compressed_buff, convertUnits(_compressed));
		sprintf(buff, loadString(IDS_EXTRACT_SUCCED_STATS), _size_all_buff, _compressed_buff, ((float)_compressed*100.0)/(float)_size_all);
		
		time_sTime = time(NULL)-sTime;
		if((temp = time_sTime/3600) < 10)
			sprintf(buff, "%s0", buff);
		sprintf(buff, "%s%ld:", buff, temp);
		if((temp = (time_sTime%3600)/60) < 10)
			sprintf(buff, "%s0", buff);
		sprintf(buff, "%s%ld:", buff, temp);
		if((temp = time_sTime%60) < 10)
			sprintf(buff, "%s0", buff);
		sprintf(buff, "%s%ld", buff, temp);

		if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
			reg.QueryDWORDValue("show_stats", stats);
		}
		if(stats)
			::MessageBox(NULL, buff, "KGB Archiver", MB_TASKMODAL);
		else
			MessageBeep(MB_OK);
	}

	TerminateThread(hUpdate, 0);

	Shell_NotifyIcon(NIM_DELETE, &_dDecompress.tray);
    
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

void Ckgb_arch_decompressDlg::OnEnChangeEarchname()
{
		CString _arch;
		string arch;
		eArchName.GetWindowText(_arch);
		arch = _arch;
		vector<string> files;
		vector<__int64> filesizes;
		
		lFiles.DeleteAllItems();
		sArchiveInfo.SetWindowText("");

		if(arch.length() < 4)
			return;

		if(fopen(arch.c_str(), "rb") == 0)
			return;
		_fcloseall();

		if(eDestination.GetWindowTextLength() == 0){
			char tmp[MAX_PATH];
			strcpy(tmp, arch.c_str());
			for(int i=strlen(tmp)-1;i>=0;i--){
				if(tmp[i] == '\\' || tmp[i] == '/'){
					tmp[i] = 0;
					break;
				}
			}
			eDestination.SetWindowText(tmp);
		}

		if(tolower(arch.c_str()[arch.length()-4]) == '.' && tolower(arch.c_str()[arch.length()-3]) == 'z' && tolower(arch.c_str()[arch.length()-2]) == 'i' && tolower(arch.c_str()[arch.length()-1]) == 'p'){
			zipGetFiles((char*)arch.c_str(), files, filesizes);
			goto fillTable;
		}
		
		//dekodowanie archiwum
		if(tolower(arch.c_str()[arch.length()-4]) == '.' && tolower(arch.c_str()[arch.length()-3]) == 'k' && tolower(arch.c_str()[arch.length()-2]) == 'g' && tolower(arch.c_str()[arch.length()-1]) == 'e'){
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
			GetLongPathName(arch2, arch2, sizeof(arch2));
			strcat(arch2, "temp.kgb");
			//arch2[strlen(arch2)-1] = 'b';

			aes_ctx ctx[1];
			ctx->n_rnd = 0; // ensure all flags are initially set to zero
			ctx->n_blk = 0;
			aes_dec_key((const unsigned char*)passwd, 32, ctx);
			
			CDialog dProgress;
			dProgress.Create(IDD_DECODING);
			dProgress.ShowWindow(SW_SHOW);
			dProgress.UpdateWindow();
			
			FILE *fin = fopen(arch.c_str(), "rb");
			if(fin == 0)
				return;
			FILE *fout = fopen(arch2, "wb");
			if(fout == 0)
				return;
			decfile(fin, fout, ctx, arch.c_str(), arch2);

			fclose(fin);
			fclose(fout);
			dProgress.ShowWindow(SW_HIDE);
			dProgress.~CDialog();
			if(!checkArchiveFormat(arch2)){
				_fcloseall();
				DeleteFile(arch2);
				eArchName.SetWindowText("");
				MessageBox(loadString(IDS_WRONG_PASSWD), "KGB Archiver");
				return;
			}

			eArchName.SetWindowText(arch2);
			return;
		}
		
		if(tolower(arch.c_str()[arch.length()-4]) == '.' && tolower(arch.c_str()[arch.length()-3]) == 'e' && tolower(arch.c_str()[arch.length()-2]) == 'x' && tolower(arch.c_str()[arch.length()-1]) == 'e'){
			try{
				zipGetFiles((char*)arch.c_str(), files, filesizes); // sprawdzamy czy zip
				goto fillTable;
			}catch(...){}
			if(isSFXencrypted((char *)arch.c_str())){
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
				GetLongPathName(arch2, arch2, sizeof(arch2));
				strcat(arch2, "temp.kgb");
				//arch2[strlen(arch2)-1] = 'b';

				aes_ctx ctx[1];
				ctx->n_rnd = 0; // ensure all flags are initially set to zero
				ctx->n_blk = 0;
				aes_dec_key((const unsigned char*)passwd, 32, ctx);
				
				CDialog dProgress;
				dProgress.Create(IDD_DECODING);
				dProgress.ShowWindow(SW_SHOW);
				dProgress.UpdateWindow();
				
				FILE *fin = fopen(arch.c_str(), "rb");
				if(fin == 0)
					return;
				FILE *fout = fopen(arch2, "wb");
				if(fout == 0)
					return;
				_fseeki64(fin, isSFXencrypted((char *)arch.c_str()), SEEK_SET);
				decfile(fin, fout, ctx, arch.c_str(), arch2);

				fclose(fin);
				fclose(fout);
				dProgress.ShowWindow(SW_HIDE);
				dProgress.~CDialog();
				if(!checkArchiveFormat(arch2)){
					_fcloseall();
					DeleteFile(arch2);
					eArchName.SetWindowText("");
					MessageBox(loadString(IDS_WRONG_PASSWD), "KGB Archiver");
					return;
				}

				eArchName.SetWindowText(arch2);
				return;
			}
			if(getFilesFromArchive((char *)arch.c_str(), files, filesizes))
				goto fillTable;
			MessageBox(loadString(IDS_BROKEN_ARCHIVE), "KGB Archiver");
			eArchName.SetWindowText("");
			return;
		}
		
		if(!getFilesFromArchive((char *)arch.c_str(), files, filesizes)){
			MessageBox(loadString(IDS_BROKEN_ARCHIVE), "KGB Archiver");
			eArchName.SetWindowText("");
			return;
		}

fillTable:

		_size_all = 0;
		for(unsigned long i=0;i<files.size();i++){
			char buffer[32];
			_size_all += filesizes[i];
			//sprintf(buffer, "%d", filesizes.size());
			//MessageBox(buffer);
			/*if(filesizes[i] < 1024)
				sprintf(buffer, "%dB", filesizes[i]);
			else if(filesizes[i] < 1024*1024)
				sprintf(buffer, "%dKB", filesizes[i]/1024);
			else
				sprintf(buffer, "%dMB", filesizes[i]/1024/1024);
			*/
			//MessageBox(files[i].c_str());
			sprintf(buffer, "%s", convertUnits(filesizes[i]));
			//MessageBox(buffer);
			lFiles.InsertItem(0, i, 0, 0, 0, 0, 0);
			
			lFiles.SetItemText(i, 0, files[i].c_str());
			lFiles.SetItemText(i, 1, buffer);
		}
		FILE *f = fopen(arch.c_str(), "rb");
		if(f){
			_fseeki64(f, 0LL, SEEK_END);
			_compressed = _ftelli64(f);
			fclose(f);
		}
		char buffer[1024];
		char _size_all_buff[16], _compressed_buff[16];
		strcpy(_size_all_buff, convertUnits(_size_all));
		strcpy(_compressed_buff, convertUnits(_compressed));
		sprintf(buffer, loadString(IDS_ARCHIVE_INFO), _compressed_buff, _size_all_buff, (double)_compressed*100.0f/(double)_size_all);
		sArchiveInfo.SetWindowText(buffer);
}


void Ckgb_arch_decompressDlg::OnBnClickedBrecentarchives()
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
	bRecentFiles.GetWindowRect(&rect);
	
	TrackPopupMenu(menu.m_hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.right, rect.top, 0, m_hWnd, NULL);
}


void Ckgb_arch_decompressDlg::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
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
		}else if(nItemID-BASE_MENU >= 10 && nItemID-BASE_MENU < 20){
			//MessageBox("");
			sprintf(val, "recentdir%d", nItemID-10-BASE_MENU);
			if(reg.QueryStringValue(val, buffer, &size) == ERROR_SUCCESS)
				eDestination.SetWindowText(buffer);
		}
	}
}


void Ckgb_arch_decompressDlg::OnBnClickedBrecentdirs()
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
			sprintf(val, "recentdir%d", i);
			if(reg.QueryStringValue(val, buffer, &size) == ERROR_SUCCESS){
				menu.AppendMenu(MF_STRING, BASE_MENU+10+i, buffer);
			}
		}
	}

	RECT rect;
	bRecentDirs.GetWindowRect(&rect);
	
	TrackPopupMenu(menu.m_hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON, rect.right, rect.top, 0, m_hWnd, NULL);
}
