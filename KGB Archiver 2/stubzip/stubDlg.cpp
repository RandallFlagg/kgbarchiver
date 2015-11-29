// stubDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stub.h"
#include "stubDlg.h"
#include "../compress.cpp"
#include "dPasswd.h"
#include "../ziparchive/ziparchive/ziparchive.h"

#define ZIP_ERROR_BROKEN_ARCHIVE_OR_PASS -10
#define ZIP_ERROR_BROKEN_ARCHIVE KGB_ERROR_BROKEN_ARCHIVE

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

wchar_t *archive_name;
bool semaphore = false;

DWORD WINAPI extractZIPArchive(LPVOID lpParam){
		compressInfo cmsi = *(compressInfo*)lpParam;
		compressStatus.init(cmsi);

		CZipArchive zip;
		CZipFileHeader header;
		_wfinddatai64_t fi;
		char tmpName[MAX_PATH];
		
		zip.Open(cmsi.archiveName.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
			for(int i=0;i<zip.GetCount(false);i++){
				if(zip[i]->IsDirectory())
					continue;
				compressStatus.uncompressed_all += zip[i]->m_uUncomprSize;
			}
		zip.Close();


		zip.Open(cmsi.archiveName.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
		
		if(cmsi.encryption.enabled){
			wchar_t pass[32];
			mbstowcs(pass, (char *)cmsi.encryption.pass, 32);
			
			zip.SetPassword(pass);
		}
		
		try{
			for(int i=0;i<zip.GetCount(false);i++){
				if(zip[i]->IsDirectory())
					continue;
				
				compressStatus.filename = zip[i]->GetFileName();
				compressStatus.uncompressed_cur = zip[i]->m_uUncomprSize;
				compressStatus.done_cur = 0;

				wcstombs(tmpName, zip[i]->GetFileName().c_str(), MAX_PATH);
				/*if(_access(tmpName, 0) == 0){
					wcscpy(fi.name, zip[i]->GetFileName().c_str());
					fi.size = zip[i]->m_uUncomprSize;
					fi.time_write = zip[i]->GetTime();
					if(!overwriteFile(&fi))
						continue;
				}*/

				zip.ExtractFile(i, cmsi.destination.c_str());

				compressStatus.done_cur += zip[i]->m_uUncomprSize;
				compressStatus.done_all += zip[i]->m_uUncomprSize;
			}
		}catch(.../*Exception^*/){
			if(zip[0]->IsEncrypted()){
				zip.Close();
				return ZIP_ERROR_BROKEN_ARCHIVE_OR_PASS;
			}
			zip.Close();
			return ZIP_ERROR_BROKEN_ARCHIVE;
		}

		zip.Close();

		return 0;
	}

// CstubDlg dialog

CstubDlg::CstubDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CstubDlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CstubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, eDestination);
	DDX_Control(pDX, IDC_PROGRESS1, pProgress);
	DDX_Control(pDX, IDC_EXTRACT_TO_FRAME, fExtract);
	DDX_Control(pDX, IDC_BUTTON2, bCancel);
	DDX_Control(pDX, IDC_BUTTON3, bExtract);
	DDX_Control(pDX, IDC_BUTTON1, bBrowse);
}

BEGIN_MESSAGE_MAP(CstubDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CstubDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CstubDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CstubDlg::onButtonExtract)
END_MESSAGE_MAP()


// CstubDlg message handlers

BOOL CstubDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//archive_name = theApp.m_lpCmdLine;
	archive_name = (wchar_t*)malloc(sizeof(wchar_t)*MAX_PATH);
	GetModuleFileName(NULL, archive_name, MAX_PATH);

	for(int i=wcslen(archive_name)-1;i>=2;i--){
		if(archive_name[i] == L'/' || archive_name[i] == L'\\'){
			archive_name[i] = L'\0';
			eDestination.SetWindowText(archive_name);
			archive_name[i] = L'\\';
			break;
		}
	}
	

	//MessageBox(archive_name);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CstubDlg::OnPaint()
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
HCURSOR CstubDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CstubDlg::OnBnClickedButton1()
{
	BROWSEINFO bInfo;
	bInfo.hwndOwner = m_hWnd;
	bInfo.lpfn = NULL;
	bInfo.lParam = NULL;
	bInfo.lpszTitle = L"Please select destination";
	bInfo.pidlRoot = NULL;
	bInfo.ulFlags = BIF_USENEWUI | BIF_VALIDATE;
	bInfo.pszDisplayName = NULL;//foldername;

	LPITEMIDLIST pidl;

	if((pidl = SHBrowseForFolder(&bInfo)) != NULL){
		wchar_t buff[MAX_PATH];
		SHGetPathFromIDList(pidl, buff);
		//GetCurrentDirectory(MAX_PATH, foldername);
		//eDestination.SetWindowText(foldername);
		eDestination.SetWindowText(buff);
	}
}

void CstubDlg::OnBnClickedButton2()
{
	CstubDlg::EndDialog(0);
	
	//theApp.ExitInstance();
}

DWORD WINAPI stats(LPVOID lpParam){
	HANDLE hThread = (HANDLE)lpParam;
	//DWORD exitCode;
	while(true/*WaitForSingleObject(hThread, 100) == WAIT_TIMEOUT*/){
		dlg.pProgress.SetRange32(0, compressStatus.uncompressed_all);
		dlg.pProgress.SetPos(compressStatus.done_all);
		Sleep(100);
	}
	return 0;
}

/*DWORD WINAPI CstubDlg::wait(LPVOID lpParam){
	
}*/
DWORD WINAPI extract(LPVOID lpParam){
	
	while(!semaphore)
		Sleep(100);
	
	compressInfo cmsi;
	wchar_t tmp[1024];
	HANDLE hThread;
	DWORD exitCode;

	cmsi.archiveName = archive_name;
	CZipArchive zip;
	zip.Open(cmsi.archiveName.c_str(), CZipArchive::OpenMode::zipOpenReadOnly);
	
	/*if(readHeader(archive_name, &cmsi) != 0){
		MessageBox(0,L"Broken archive!", L"KGB Archiver 2",0);
		dlg.EndDialog(0);
	}*/

	if(cmsi.encryption.enabled=zip[0]->IsEncrypted()){
		dPasswd pass;
		if(!pass.DoModal()){
			zip.Close();
			return 0;
		}
		cmsi.encryption.setPassword(pass.passwd);
	}
	zip.Close();

	dlg.eDestination.GetWindowText(tmp, sizeof(tmp)/sizeof(*tmp));
	cmsi.destination = tmp;

	//wchar_t tmp[1024];
	/*int err;
	cmsi.destination = cmsi.destination + L'\\';
	if(cmsi.destination.length() >= 2){
		for(int j=(cmsi.destination.c_str()[1] == L':'?3:0);j<cmsi.destination.length();j++){
			if(cmsi.destination.c_str()[j] == L'\\' || cmsi.destination.c_str()[j] == L'/'){
				wcscpy_s(tmp, sizeof(tmp)/sizeof(*tmp), cmsi.destination.c_str());
				tmp[j] = L'\0';
				//MessageBox::Show(gcnew String(tmp));
				err = NULL;
				if(CreateDirectory(tmp, NULL) == NULL && (err=GetLastError()) != ERROR_ALREADY_EXISTS){
					MessageBox(0,L"Cannot create directory to extract to!", L"KGB Archiver 2",0);
					return 0;
				}
			}
		}
	}
	SetCurrentDirectory(cmsi.destination.c_str());*/
	//MessageBox(cmsi.destination.c_str(), cmsi.archiveName.c_str());
	hThread = CreateThread(NULL, NULL, &extractZIPArchive, &cmsi, NULL, NULL);
	CreateThread(NULL, NULL, &stats, &hThread, NULL, NULL);

	WaitForSingleObject(hThread, INFINITE);

	GetExitCodeThread(hThread, &exitCode);
	switch(exitCode){
		case NULL: MessageBox(0,L"Decompression succeded!", L"KGB Archiver 2",0); break;
		case KGB_ERROR_WRONG_PASSWORD: MessageBox(0,L"Wrong password!", L"KGB Archiver 2",0); break;
		case KGB_ERROR_BROKEN_ARCHIVE: MessageBox(0,L"Broken archive!", L"KGB Archiver 2",0); break;
		case KGB_ERROR_SFX_NOT_FOUND: MessageBox(0,L"SFX module not found!", L"KGB Archiver 2",0); break;
		//case ZIP_ERROR_BROKEN_ARCHIVE_OR_PASS: MessageBox(L"Broken archive or wrong password!", L"KGB Archiver 2"); break; 
		case ERROR_ACCESS_DENIED: MessageBox(0,L"Access is denied!", L"KGB Archiver 2",0); break; //occurs sometimes on vista 
		default:
			wchar_t err[1024];
			wsprintf(err, L"Undefinied error code %d", exitCode);
			MessageBox(0,err, L"KGB Archiver 2",0);
			break;
	}
	dlg.EndDialog(0);
	return 0;
}

void CstubDlg::onButtonExtract()
{
	semaphore = true;

	eDestination.EnableWindow(false);
	bBrowse.EnableWindow(false);
	bExtract.EnableWindow(false);
	
	pProgress.ShowWindow(true);

	/*DWORD exitCode;
	GetExitCodeThread(hThread, &exitCode);
	switch(exitCode){
		case NULL: MessageBox(L"Decompression succeded!", L"KGB Archiver 2"); break;
		case KGB_ERROR_WRONG_PASSWORD: MessageBox(L"Wrong password!", L"KGB Archiver 2"); break;
		case KGB_ERROR_BROKEN_ARCHIVE: MessageBox(L"Broken archive!", L"KGB Archiver 2"); break;
		case KGB_ERROR_SFX_NOT_FOUND: MessageBox(L"SFX module not found!", L"KGB Archiver 2"); break;
		//case ZIP_ERROR_BROKEN_ARCHIVE_OR_PASS: MessageBox(L"Broken archive or wrong password!", L"KGB Archiver 2"); break; 
		case ERROR_ACCESS_DENIED: MessageBox(L"Access is denied!", L"KGB Archiver 2"); break; //occurs sometimes on vista 
		default:
			wchar_t err[1024];
			wsprintf(err, L"Undefinied error code %d", exitCode);
			MessageBox(err, L"KGB Archiver 2");
			break;
	}*/

	//WaitForSingleObject(hThread, INFINITE);
}
