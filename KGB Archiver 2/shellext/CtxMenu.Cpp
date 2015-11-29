#include "priv.h"
#include "shellext.h"
#include <string>
#include "archive.h"
#include <windows.h>
//#include "../home.cpp"

using namespace std;

#ifndef _delayimp_h
  extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

//
//  FUNCTION: CShellExt::QueryContextMenu(HMENU, UINT, UINT, UINT, UINT)
//
//  PURPOSE: Called by the shell just before the context menu is displayed.
//           This is where you add your specific menu items.
//
//  PARAMETERS:
//    hMenu      - Handle to the context menu
//    indexMenu  - Index of where to begin inserting menu items
//    idCmdFirst - Lowest value for new menu ID's
//    idCmtLast  - Highest value for new menu ID's
//    uFlags     - Specifies the context of the menu event
//
//  RETURN VALUE:
//
//
//  COMMENTS:
//

STDMETHODIMP CShellExt::QueryContextMenu(HMENU hMenu,
                                         UINT indexMenu,
                                         UINT idCmdFirst,
                                         UINT idCmdLast,
                                         UINT uFlags)
{
    ODS(L"CShellExt::QueryContextMenu()\r\n");

    UINT idCmd = idCmdFirst;
    wstring sCompress = L"Compress to ";
    wstring sDecompress = L"Decompress ";

    bool bAppendItems = true;
	bool allowCompress = true;
	bool allowDecompress = true;
	HKEY key;
	DWORD dwTmp;
	DWORD dwSize = sizeof(dwTmp);

	if(files.size() == 1){
		wstring wsTmp = files[0].substr(files[0].find_last_of(L"\\")+1);
		sCompress += wsTmp.substr(0, wsTmp.find_last_of(L".")) + L".kgb";
		sDecompress += files[0].substr(files[0].find_last_of(L"\\")+1);
	}else if(files.size() > 1){
		wstring wsTmp = files[0].substr(0, files[0].find_last_of(L"\\"));
		sCompress += wsTmp.substr(wsTmp.find_last_of(L"\\")+1) + L".kgb";
	}

	if(uFlags & CMF_DEFAULTONLY || uFlags & CMF_VERBSONLY)
		bAppendItems = false;
	
	RegOpenKey(HKEY_CURRENT_USER, L"Software\\KGB Archiver 2\\explorerExtension", &key);
	RegQueryValueEx(key, L"compress" , NULL, NULL, (LPBYTE)&dwTmp, &dwSize);
	allowCompress = dwTmp;
	RegQueryValueEx(key, L"decompress", NULL, NULL, (LPBYTE)&dwTmp, &dwSize);
	allowDecompress = dwTmp;
	RegCloseKey(key);

    if(bAppendItems && (allowCompress || allowDecompress)){
        InsertMenu(hMenu, indexMenu++, MF_SEPARATOR|MF_BYPOSITION, 0, NULL);
		/*_wfinddatai64_t * fh = (_wfinddatai64_t*)malloc(560);
		if(!fh)
			MessageBox(0, L"out of memory", L"", 0);
		fh->size = 10;
		SetLastError(0);
		if(isKGBArchive(files[0]))
			MessageBox(0, L"kgb archive !", L"", 0);
		wchar_t xx[32];
		_swprintf(xx, L"%d", GetLastError());
		MessageBox(0, xx, L"", 0);*/
		if(!(files.size() == 1 && (isKGBArchive(files[0]) || isZIPArchive(files[0]))) && allowCompress){
			//MessageBox(0, L"compress", L"", 0);
			m_mode = 0;
			InsertMenu(hMenu, indexMenu++, MF_STRING|MF_BYPOSITION, idCmd++, sCompress.c_str());
		}else if(allowDecompress){
			m_mode = 1;
			InsertMenu(hMenu, indexMenu++, MF_STRING|MF_BYPOSITION, idCmd++, sDecompress.c_str());
		}

        InsertMenu(hMenu, indexMenu++, MF_SEPARATOR|MF_BYPOSITION, 0, NULL);

        return ResultFromShort(idCmd-idCmdFirst); //Must return number of menu
                                                  //items we added.
   }

   return NOERROR;
}

//
//  FUNCTION: CShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO)
//
//  PURPOSE: Called by the shell after the user has selected on of the
//           menu items that was added in QueryContextMenu().
//
//  PARAMETERS:
//    lpcmi - Pointer to an CMINVOKECOMMANDINFO structure
//
//  RETURN VALUE:
//
//
//  COMMENTS:
//

STDMETHODIMP CShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi)
{
    ODS(L"CShellExt::InvokeCommand()\r\n");

    HRESULT hr = E_INVALIDARG;

    //If HIWORD(lpcmi->lpVerb) then we have been called programmatically
    //and lpVerb is a command that should be invoked.  Otherwise, the shell
    //has called us, and LOWORD(lpcmi->lpVerb) is the menu ID the user has
    //selected.  Actually, it's (menu ID - idCmdFirst) from QueryContextMenu().
    if (!HIWORD(lpcmi->lpVerb))
    {
        //UINT idCmd = LOWORD(lpcmi->lpVerb);

        switch (m_mode)
        {
            case 0:
                hr = DoCompress(lpcmi);
                break;

            case 1:
                hr = DoDecompress(lpcmi);
                break;
        }
    }
    return hr;
}


//
//  FUNCTION: CShellExt::InvokeCommand(LPCMINVOKECOMMANDINFO)
//
//  PURPOSE: Called by the shell after the user has selected on of the
//           menu items that was added in QueryContextMenu().
//
//  PARAMETERS:
//    lpcmi - Pointer to an CMINVOKECOMMANDINFO structure
//
//  RETURN VALUE:
//
//
//  COMMENTS:
//

STDMETHODIMP CShellExt::GetCommandString(UINT_PTR idCmd,
                                         UINT uFlags,
                                         UINT FAR *reserved,
                                         LPSTR pszName,
                                         UINT cchMax)
{
    ODS(L"CShellExt::GetCommandString()\r\n");

    switch (idCmd){
        case 0:
            strcpy_s(pszName, cchMax, "DoCompress");
            break;

        case 1:
            strcpy_s(pszName, cchMax, "DoDecompress");
            break;
    }

    return NOERROR;
}

STDMETHODIMP CShellExt::DoCompress(LPCMINVOKECOMMANDINFO lpcmi)
{
    ODS(L"CShellExt::DoCompress()\r\n");

    wchar_t wcTemp[MAX_PATH-14];
	wchar_t wcTempFile[MAX_PATH];
	wchar_t wcExePath[MAX_PATH];
	wchar_t wcCmdLine[MAX_PATH*2+9];
	FILE *fTemp;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si)); 
	si.cb = sizeof(si);

	GetTempPath(MAX_PATH-14, wcTemp);
	GetTempFileName(wcTemp, L"", 0, wcTempFile);
	
	GetModuleFileName(reinterpret_cast<HMODULE>(&__ImageBase), wcExePath, MAX_PATH-wcslen(L"KGB Archiver 2 .net.exe"));
	for(int i=wcslen(wcExePath)-1;i>=0;--i){
		if(wcExePath[i] == L'\\' || wcExePath[i] == L'/'){
			wcExePath[i+1] = L'\0';
			break;
		}
	}
	wcscat(wcExePath, L"KGB Archiver 2 .net.exe");

	fTemp = _wfopen(wcTempFile, L"wt");
	if(!fTemp)
		return ERROR_ACCESS_DENIED;

	for(int i=0;i<files.size();++i){
		fwprintf(fTemp, L"%s\n", files[i].substr(files[i].find_last_of(L"\\")+1).c_str());
	}

	fclose(fTemp);

	swprintf(wcCmdLine, MAX_PATH*2+9, L" -c \"%s\" \"%s\"", files[0].substr(0, files[0].find_last_of(L"\\")).c_str(), wcTempFile);
	CreateProcess(wcExePath, wcCmdLine, NULL, NULL, false, NULL, NULL, NULL, &si, &pi);;

    return NOERROR;
}

STDMETHODIMP CShellExt::DoDecompress(LPCMINVOKECOMMANDINFO lpcmi)
{
    ODS(L"CShellExt::DoDecompress()\r\n");
	
	wchar_t wcTemp[MAX_PATH];
	wchar_t wcTempFile[MAX_PATH];
	wchar_t wcExePath[MAX_PATH];
	wchar_t wcCmdLine[MAX_PATH*2+45];
	wchar_t wcDestination[MAX_PATH];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	LPITEMIDLIST pidl;
	BROWSEINFO bInfo;

	ZeroMemory(&si, sizeof(si)); 
	si.cb = sizeof(si);

	GetTempPath(MAX_PATH-14, wcTemp);
	GetTempFileName(wcTemp, L"", 0, wcTempFile);
	
	GetModuleFileName(reinterpret_cast<HMODULE>(&__ImageBase), wcExePath, MAX_PATH-wcslen(L"kgb2compressor.exe"));
	for(int i=wcslen(wcExePath)-1;i>=0;--i){
		if(wcExePath[i] == L'\\' || wcExePath[i] == L'/'){
			wcExePath[i+1] = L'\0';
			break;
		}
	}
	wcscat(wcExePath, L"kgb2compressor.exe");

	bInfo.hwndOwner = NULL;
	bInfo.lpfn = NULL;
	bInfo.lParam = NULL;
	bInfo.lpszTitle = L"Choose directory to decompress to";
	bInfo.pidlRoot = NULL;
	bInfo.ulFlags = BIF_USENEWUI | BIF_VALIDATE;
	bInfo.pszDisplayName = (LPWSTR)malloc(MAX_PATH*sizeof(wchar_t));
	wcscpy(bInfo.pszDisplayName, files[0].substr(0, files[0].find_last_of(L"\\")).c_str());
	wcscpy(wcDestination, bInfo.pszDisplayName);

	if((pidl = SHBrowseForFolder(&bInfo)) != NULL){
		char buff[MAX_PATH];
		SHGetPathFromIDList(pidl, wcDestination);
	}else
		return NOERROR;

	swprintf(wcCmdLine, MAX_PATH*2+45, L" -md \"%s\" \"%s\"", files[0].c_str(), wcDestination);
	CreateProcess(wcExePath, wcCmdLine, NULL, NULL, false, NULL, NULL, NULL, &si, &pi);

    return NOERROR;
}
