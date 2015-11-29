// kgb_arch_confDlg.cpp : implementation file
//

#include "stdafx.h"
#include "kgb_arch_conf.h"
#include "kgb_arch_confDlg.h"
#include <atlbase.h>
#include <psapi.h>
#include <wininet.h>
//#include "about.cpp"

#define KGB_COMPRESS "\\kgb_arch_compress.exe \"%1\""
#define KGB_COMPRESS_DIR "\\kgb_arch_compress.exe \"%1\\\""
#define KGB_DECOMPRESS "\\kgb_arch_decompress.exe \"%1\""
#define KGB_DECOMPRESS_HERE "\\kgb_arch_decompress.exe /s\"%1\""

DWORD lang[] = {0x0415, 0x0409, 0x040c, 0x0416, 0x0c0a, 0x340a, 0x0418, 0x0410, 0x040e, 0x0413, 0x0405, 0x041f, 0x0427, 0x041d, 0x0425, 0x0407};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


Ckgb_arch_confDlg::Ckgb_arch_confDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ckgb_arch_confDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ckgb_arch_confDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, cAttachFiles);
	DDX_Control(pDX, IDC_CHECK2, cContextMenu);
	DDX_Control(pDX, IDC_CHECK3, cUpdates);
	DDX_Control(pDX, IDC_CHECK4, cAssocZIP);
	DDX_Control(pDX, IDC_LANG, cLang);
	DDX_Control(pDX, IDC_CHECK5, cStats);
}

BEGIN_MESSAGE_MAP(Ckgb_arch_confDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON3, &Ckgb_arch_confDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &Ckgb_arch_confDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &Ckgb_arch_confDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &Ckgb_arch_confDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// Ckgb_arch_confDlg message handlers

BOOL Ckgb_arch_confDlg::OnInitDialog()
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
	SetIcon(m_hIcon, FALSE);		// Set small icon

	cmdline = false;
	/*char *cmd = theApp.m_lpCmdLine;

	if(cmd[0] == '/' && cmd[1] == 'i'){
		cContextMenu.SetCheck(1);
		cAttachFiles.SetCheck(1);
		cAssocZIP.SetCheck(1);
		cUpdates.SetCheck(1);
		cLang.SetCurSel(1);
		OnBnClickedButton3();
		theApp.ExitInstance();
		return false;
	}else if(cmd[0] == '/' && cmd[1] == 'u'){
		//domyslnie wy³aczone
		OnBnClickedButton3();
		theApp.ExitInstance();
		return false;
	}else if(cmd[0] == '/' && cmd[1] == 'v'){
		cmdline = true;
		OnBnClickedButton4();
		EndDialog(IDOK);
		return false;
	}
	cmdline = false;//dla updatow*/
	
	CRegKey reg(HKEY_CLASSES_ROOT);
	
	if(reg.Open(HKEY_CLASSES_ROOT, "*\\shell\\compress\\commmand") == ERROR_SUCCESS || reg.Open(HKEY_CLASSES_ROOT, "Directory\\shell\\compress\\command") == ERROR_SUCCESS){
		cContextMenu.SetCheck(1);
	}

	if(reg.Open(HKEY_CLASSES_ROOT, "kgb_arch\\shell\\open\\command") == ERROR_SUCCESS || reg.Open(HKEY_CLASSES_ROOT, "kgb_arch_enc\\shell\\open\\command") == ERROR_SUCCESS){
		cAttachFiles.SetCheck(1);
	}

	if(reg.Open(HKEY_CLASSES_ROOT, "kgb_arch_zip\\shell\\open\\command") == ERROR_SUCCESS){
		cAssocZIP.SetCheck(1);
	}

	if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
		DWORD update = 0;
		reg.QueryDWORDValue("check_for_updates", update);
		cUpdates.SetCheck((int)update);
	}

	if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
		DWORD stats = 0;
		reg.QueryDWORDValue("show_stats", stats);
		cStats.SetCheck((int)stats);
	}

	cLang.SetCurSel(1);
	if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
		DWORD _lang;
		reg.QueryDWORDValue("lang", _lang);

		for(int i=0;i<sizeof(lang)/sizeof(*lang);i++){
			if(lang[i] == _lang){
				cLang.SetCurSel(i);
				break;
			}
		}
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ckgb_arch_confDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Ckgb_arch_confDlg::OnPaint()
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
HCURSOR Ckgb_arch_confDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Ckgb_arch_confDlg::OnBnClickedButton3()
{
	HANDLE hProcess = GetCurrentProcess();
	HMODULE hMod;
    DWORD cbNeeded;
	char processName[MAX_PATH];

	//ustawiamy jezyk
	LCID loc = MAKELCID(lang[cLang.GetCurSel()], SORT_DEFAULT);
	SetThreadLocale(loc);

    if(EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)){
        GetModuleFileNameEx(hProcess, hMod, processName, sizeof(processName)/sizeof(char));
    }

	for(int i=strlen(processName)-1;i>=0;i--){
		if(processName[i] == '\\' || processName[i] == '/'){
			processName[i] = 0;
			break;
		}
	}
	//MessageBox(processName);

	SetCurrentDirectory(processName);


	char ico[MAX_PATH];
	char shell[MAX_PATH];

	CRegKey reg(HKEY_CLASSES_ROOT);

	if(cAttachFiles.GetCheck()){

		GetCurrentDirectory(MAX_PATH-sizeof("kgb.ico"), ico);
		strcat(ico, "\\kgb.ico");

		GetCurrentDirectory(MAX_PATH-sizeof(KGB_DECOMPRESS), shell);
		strcat(shell, KGB_DECOMPRESS);

		reg.Create(HKEY_CLASSES_ROOT, ".kgb");
		reg.SetValue(HKEY_CLASSES_ROOT, ".kgb", "kgb_arch");
		
		reg.Create(HKEY_CLASSES_ROOT, "kgb_arch");
		reg.SetKeyValue("DefaultIcon", ico);

		reg.Create(HKEY_CLASSES_ROOT, "kgb_arch\\shell\\open");
		reg.SetKeyValue("command", shell);

		if(cContextMenu.GetCheck()){
			GetCurrentDirectory(MAX_PATH-sizeof(KGB_DECOMPRESS_HERE), shell);
			strcat(shell, KGB_DECOMPRESS_HERE);

			reg.Create(HKEY_CLASSES_ROOT, "kgb_arch\\shell\\decompress_here");
			reg.SetValue(HKEY_CLASSES_ROOT, "kgb_arch\\shell\\decompress_here", loadString(IDS_DECOMPRESS_HERE));
			reg.SetKeyValue("command", shell);
		}

		GetCurrentDirectory(MAX_PATH-sizeof(KGB_DECOMPRESS), shell);
		strcat(shell, KGB_DECOMPRESS);

		GetCurrentDirectory(MAX_PATH-sizeof("kge.ico"), ico);
		strcat(ico, "\\kge.ico");
		
		reg.Create(HKEY_CLASSES_ROOT, ".kge");
		reg.SetValue(HKEY_CLASSES_ROOT, ".kge", "kgb_arch_enc");
		
		reg.Create(HKEY_CLASSES_ROOT, "kgb_arch_enc");
		reg.SetKeyValue("DefaultIcon", ico);

		reg.Create(HKEY_CLASSES_ROOT, "kgb_arch_enc\\shell\\open");
		reg.SetKeyValue("command", shell);

		if(cContextMenu.GetCheck()){
			GetCurrentDirectory(MAX_PATH-sizeof(KGB_DECOMPRESS_HERE), shell);
			strcat(shell, KGB_DECOMPRESS_HERE);

			reg.Create(HKEY_CLASSES_ROOT, "kgb_arch_enc\\shell\\decompress_here");
			reg.SetValue(HKEY_CLASSES_ROOT, "kgb_arch_enc\\shell\\decompress_here", loadString(IDS_DECOMPRESS_HERE));
			reg.SetKeyValue("command", shell);
		}

	}else{
		reg.Open(HKEY_CLASSES_ROOT, "");
		reg.RecurseDeleteKey("kgb_arch");

		reg.Open(HKEY_CLASSES_ROOT, "");
		reg.RecurseDeleteKey("kgb_arch_enc");
	}

	
	if(cAssocZIP.GetCheck()){

		GetCurrentDirectory(MAX_PATH-sizeof("zip.ico"), ico);
		strcat(ico, "\\zip.ico");

		GetCurrentDirectory(MAX_PATH-sizeof(KGB_DECOMPRESS), shell);
		strcat(shell, KGB_DECOMPRESS);

		reg.Create(HKEY_CLASSES_ROOT, ".zip");
		reg.SetValue(HKEY_CLASSES_ROOT, ".zip", "kgb_arch_zip");
		
		reg.Create(HKEY_CLASSES_ROOT, "kgb_arch_zip");
		reg.SetKeyValue("DefaultIcon", ico);

		reg.Create(HKEY_CLASSES_ROOT, "kgb_arch_zip\\shell\\open");
		reg.SetKeyValue("command", shell);

		if(cContextMenu.GetCheck()){
			GetCurrentDirectory(MAX_PATH-sizeof(KGB_DECOMPRESS_HERE), shell);
			strcat(shell, KGB_DECOMPRESS_HERE);

			reg.Create(HKEY_CLASSES_ROOT, "kgb_arch_zip\\shell\\decompress_here");
			reg.SetValue(HKEY_CLASSES_ROOT, "kgb_arch_zip\\shell\\decompress_here", loadString(IDS_DECOMPRESS_HERE));
			reg.SetKeyValue("command", shell);
		}

	}else{
		reg.Open(HKEY_CLASSES_ROOT, "");
		reg.RecurseDeleteKey("kgb_arch_zip");
	}

	//kompresja

	if(cContextMenu.GetCheck()){

		GetCurrentDirectory(MAX_PATH-sizeof(KGB_COMPRESS), shell);
		strcat(shell, KGB_COMPRESS);

		reg.Create(HKEY_CLASSES_ROOT, "*\\shell\\compress");
		reg.SetValue(HKEY_CLASSES_ROOT, "*\\shell\\compress", loadString(IDS_COMPRESS2KGB));
		reg.SetKeyValue("command", shell);

		GetCurrentDirectory(MAX_PATH-sizeof(KGB_COMPRESS), shell);
		strcat(shell, KGB_COMPRESS_DIR);

		/*reg.Create(HKEY_CLASSES_ROOT, "Folder\\shell\\compress");
		reg.SetValue(HKEY_CLASSES_ROOT, "Folder\\shell\\compress", "Kompresuj do KGB");
		reg.SetKeyValue("command", shell);*/

		reg.Create(HKEY_CLASSES_ROOT, "Directory\\shell\\compress");
		reg.SetValue(HKEY_CLASSES_ROOT, "Directory\\shell\\compress", loadString(IDS_COMPRESS2KGB));
		reg.SetKeyValue("command", shell);
	}else{
		reg.Open(HKEY_CLASSES_ROOT, "*\\shell");
		reg.RecurseDeleteKey("compress");

		reg.Open(HKEY_CLASSES_ROOT, "Directory\\shell");
		reg.RecurseDeleteKey("compress");

		reg.Open(HKEY_CLASSES_ROOT, "kgb_arch\\shell");
		reg.RecurseDeleteKey("decompress_here");

		reg.Open(HKEY_CLASSES_ROOT, "kgb_arch_enc\\shell");
		reg.RecurseDeleteKey("decompress_here");

		reg.Open(HKEY_CLASSES_ROOT, "kgb_arch_zip\\shell");
		reg.RecurseDeleteKey("decompress_here");
	}

	reg.Create(HKEY_CURRENT_USER, "Software\\KGB Archiver");
	reg.SetDWORDValue("check_for_updates", cUpdates.GetCheck());

	reg.Create(HKEY_CURRENT_USER, "Software\\KGB Archiver");
	reg.SetDWORDValue("show_stats", cStats.GetCheck());

	//DWORD lang[] = {0x0415, 0x0409, 0x040c, 0x0416, 0x340a, 0x0418};
	reg.SetDWORDValue("lang", lang[cLang.GetCurSel()]);
	
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);	

	EndDialog(IDOK);
}

void Ckgb_arch_confDlg::OnBnClickedButton2()
{
	EndDialog(IDCANCEL);// TODO: Add your control notification handler code here
}

void Ckgb_arch_confDlg::OnBnClickedButton1()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void download(char *url, char *buffer, DWORD &size){
    HINTERNET internet = InternetOpen("KGB Archiver", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
    if(internet == NULL)
                return;
    HINTERNET urlHandle = InternetOpenUrl(internet, url, NULL, 0, 0, 0); 
    if(urlHandle == NULL)
                 return;
    InternetReadFile(urlHandle, buffer, size, &size);
    InternetCloseHandle(internet);
}

void Ckgb_arch_confDlg::OnBnClickedButton4()
{
	char ver[64] = "";
	DWORD verSize = sizeof(ver);
	char cVer[] = "KGB1.2.1.24";

	download("http://kgbarchiver.sourceforge.net/version.txt", ver, verSize);
	if(ver[0] != 'K' || ver[1] != 'G' || ver[2] != 'B'){
		!cmdline ? MessageBox(loadString(IDS_CANT_CONNECT_TO_SERVER), "KGB Archiver") : NULL;
		return;
	}
	
	if(strcmp(cVer, ver)){
		if(MessageBox(loadString(IDS_NEW_VERSION_AVAILABLE), "KGB Archiver", MB_YESNO) == IDYES){
			ShellExecute(NULL, "open", "http://kgbarchiver.sourceforge.net/?page=download.php", NULL, NULL, SW_SHOWDEFAULT);
		}
	}else if(!cmdline){
		MessageBox(loadString(IDS_NO_UPDATES), "KGB Archiver");
	}

}
