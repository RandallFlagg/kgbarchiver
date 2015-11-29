// sfx_kgb.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "sfx_kgb.h"
#include <stdio.h>

bool cmp(char *a, char *b){
	for(int i=0;b[i] != '\0'/* && a[i] != '\0'*/;i++){
		if(a[i] != b[i])
			return false;
	}
	return true;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	wchar_t arch_name[1024];
	wchar_t temp[1024];
	wchar_t stub_name[1024];
	wchar_t dll_name[1024];
	wchar_t cmd[1024];
	FILE *archive;
	FILE *stub;
	FILE *dll;
	char buffer[512];
	char done = 0;
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	int size;


	if(!GetModuleFileName(NULL, arch_name, sizeof(arch_name)/sizeof(*arch_name))){
		MessageBox(0, L"Initialization failed!", L"KGB Archiver 2", 0);
		return 0;
	}
	GetTempPath(sizeof(temp)/sizeof(*temp), temp);
	//GetLongPathName(temp, temp, sizeof(temp)/sizeof(*temp));
	GetTempFileName(temp, L"", 0, stub_name);
	wsprintf(dll_name, L"%s\\decompressor.dll", temp);

	archive = _wfopen(arch_name, L"rb");
	if(archive == NULL){
		MessageBox(0, L"Access denied to archive!", L"KGB Archiver", 0);
		return 0;
	}
	
	wsprintf(cmd, L"\"%s\" %s", stub_name, arch_name);

	for(__int64 i=0;fread(buffer, sizeof(*buffer), sizeof(buffer)/sizeof(*buffer), archive) && done < 2;i = _ftelli64(archive)){
	//while(!feof(archive)){
		//MessageBox(0, L"", L"", 0);
		//fread(buffer, sizeof(*buffer), sizeof(buffer)/sizeof(*buffer), archive);
		//buffer[9] = 0;
		//printf("%s", buffer);
		if(cmp(buffer, "KGBSFXST")){
			stub = _wfopen(stub_name, L"wb");
			if(stub == NULL){
				MessageBox(0, L"Initialization failed!", L"KGB Archiver 2", 0);
				fclose(archive);
				return 0;
			}
			_fseeki64(archive, i+8, SEEK_SET);
			fread(buffer, sizeof(*buffer), 8, archive);
			while(!cmp(buffer, "KGBSFXEN")){
				fwrite(buffer, sizeof(*buffer), 8, stub);
				fread(buffer, sizeof(*buffer), 8, archive);
			}
			fclose(stub);
			if((size=_ftelli64(archive)%512) != 0){//skip some garbage ;)
				size = 512-size;
				fread(buffer, sizeof(*buffer), size, archive);
			}
			done++;
		}else if(cmp(buffer, "KGBSFXDL")){
			dll = _wfopen(dll_name, L"wb");
			if(dll == NULL){
				MessageBox(0, L"Initialization failed!", L"KGB Archiver 2", 0);
				fclose(archive);
				return 0;
			}
			_fseeki64(archive, i+8, SEEK_SET);
			fread(buffer, sizeof(*buffer), 8, archive);
			while(!cmp(buffer, "KGBSFXEN")){
				fwrite(buffer, sizeof(*buffer), 8, dll);
				fread(buffer, sizeof(*buffer), 8, archive);
			}
			fclose(dll);
			if((size=_ftelli64(archive)%512) != 0){//skip some garbage ;)
				size = 512-size;
				fread(buffer, sizeof(*buffer), size, archive);
			}
			done++;
		}
	}

	fclose(archive);
	
	ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

	if(done == 2)
		CreateProcess(NULL, cmd, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);

	return 0;
}
