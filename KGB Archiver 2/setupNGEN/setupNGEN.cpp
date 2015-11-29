// setupNGEN.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "setupNGEN.h"
#include "../home.cpp"
#include <shellapi.h>

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;

    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
  
    if (NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            // handle error
        }
    }
    return bIsWow64;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//wchar_t windir[MAX_PATH];
	wchar_t curdir[MAX_PATH];
	wchar_t cmddir[MAX_PATH];

	GetWindowsDirectory(cmddir, MAX_PATH);
	wcscpy(curdir, getHome().c_str());
	
	wcscat(cmddir, L"\\Microsoft.NET\\Framework\\v2.0.50727\\ngen.exe");

	if(wcscmp(lpCmdLine, L"-i") == 0){
		ShellExecute(NULL, L"open", cmddir, L"install \"KGB Archiver 2 .net.exe\"", curdir, SW_HIDE);
		ShellExecute(NULL, L"open", cmddir, L"install \"kgb2compressor.exe\"", curdir, SW_HIDE);
		
		//register shellext...
		if(IsWow64()){
			ShellExecute(NULL, L"open", L"regsvr32.exe", L"/s shellext64.dll", curdir, SW_NORMAL);
		}else{
			ShellExecute(NULL, L"open", L"regsvr32.exe", L"/s shellext.dll", curdir, SW_NORMAL);
		}
		
		Sleep(10000);
	}else if(wcscmp(lpCmdLine, L"-u") == 0){
		ShellExecute(NULL, L"open", cmddir, L"uninstall \"KGB Archiver 2 .net.exe\"", curdir, SW_HIDE);
		ShellExecute(NULL, L"open", cmddir, L"uninstall \"kgb2compressor.exe\"", curdir, SW_HIDE);
		
		if(IsWow64()){
			ShellExecute(NULL, L"open", L"regsvr32.exe", L"/u /s shellext64.dll", curdir, SW_NORMAL);
		}else{
			ShellExecute(NULL, L"open", L"regsvr32.exe", L"/u /s shellext.dll", curdir, SW_NORMAL);
		}

		Sleep(2500);
	}

	return 0;
}