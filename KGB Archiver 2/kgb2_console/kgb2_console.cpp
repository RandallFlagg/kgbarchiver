// kgb2_console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include <conio.h>
#include "..\\compress.cpp"

#define HEADER L"KGB Archiver 2 beta 1 console\t\t(c)2005-2007 Tomasz Pawlak\n\n"
#define USAGE L"To compress:\n\tkgb2_console [-ax] [-my] [-p] archive file1 file2 file3...\nTo decompress:\n\tkgb2_console archive\n\n a - number of algorithm, from 0 (no compression) to 7 (best compression)\n m - number of compression mode, see table below\n p - use password\n\n"
#define MEMORY_USAGE L"Memory usage\n-----------------------------------------\n-a1 -  49MB, -my option ignored (default)\n-a2 -  51MB, -my option ignored\n-a3 - 192MB, -my option ignored\n-a4 - 335MB, -my option ignored\n-a5 - 358MB, -my option ignored\n-a6 -m from 0 (4MB) to 9 (1600MB)\n-a7 -m from 0 (22MB) to 9 (940MB)\n"

void getPasswd(unsigned char *pass){
	int tmp;
	
	memset(pass, 0, 32);

	printf("Enter password: ");
	for(int i=0;i<32;i++){
		if(i < 0)
			i = 0;
		tmp = _getch();
		switch(tmp){
			case VK_RETURN: putchar('\n'); return; break;
			case VK_BACK:
				if(i > 0){
					pass[i-1] = '\0';
					printf("\b \b");
				}
				i-=2;
				break;
			default:
				pass[i] = tmp;
				putchar('*');
				break;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	compressInfo cmsi;
	clock_t benchmark;
	HANDLE hThread;
	DWORD exitCode;

	wprintf(HEADER);

	if(argc == 2){ //decompress
		if(readHeader((wstring)argv[1], &cmsi) != 0){
			wprintf(L"Archive is broken!\n");
			return 0;
		}
		if(cmsi.encryption.enabled)
			getPasswd(cmsi.encryption.pass);
		
		benchmark = clock();
		wprintf(L"Extracting archive %s...", cmsi.archiveName.c_str());
		hThread = CreateThread(NULL, NULL, &extractArchive, &cmsi, NULL, NULL);
		while(WaitForSingleObject(hThread, 500) == WAIT_TIMEOUT){
			wprintf(L"\rExtracting archive %s... %5.2f%%", cmsi.archiveName.c_str(), (float)(compressStatus.done_all*100)/(float)compressStatus.uncompressed_all);
		}
		GetExitCodeThread(hThread, &exitCode);
		switch(exitCode){
			case NULL: wprintf(L"\rArchive extracted, speed: %.2fKB/s   \n", ((float)compressStatus.uncompressed_all/1024.0f)/((float)(clock()-benchmark)/1000.0f)); break;
			case KGB_ERROR_WRONG_PASSWORD: wprintf(L"\rWrong password!                         \n"); break;
			default: wprintf(L"\rUndefinied error, code: %d                         \n", exitCode); break;
		}
	}else if(argc >= 3){//compress
		bool archiveSet = 0;
		for(int i=1;i<argc;i++){
			if(argv[i][0] == '-'){
				switch(argv[i][1]){
					case 'a': cmsi.algorithm = argv[i][2]-'0'; break;
					case 'm': cmsi.algoMode = argv[i][2]-'0'; break;
					case 'p':
						cmsi.encryption.enabled = true;
						getPasswd(cmsi.encryption.pass);
						break;
					default: wprintf(L"Unrecognized option: %s\n", argv[i]); return 0;
				}
			}else{
				if(!archiveSet){
					cmsi.archiveName = argv[i];
					archiveSet = 1;
				}else{
					cmsi.insertFile(argv[i]);
					//wprintf(L"%s\n", argv[i]);
				}
			}
		}
		wprintf(L"Creating archive %s...", cmsi.archiveName.c_str());
		hThread = CreateThread(NULL, NULL, &createArchive, &cmsi, NULL, NULL);
		benchmark = clock();
		while(WaitForSingleObject(hThread, 500) == WAIT_TIMEOUT){
			wprintf(L"\rCreating archive %s... %5.2f%%",  cmsi.archiveName.c_str(), (float)(compressStatus.done_all*100)/(float)compressStatus.uncompressed_all);
		}
		GetExitCodeThread(hThread, &exitCode);
		switch(exitCode){
			case NULL: wprintf(L"\rArchive created, ratio: %.2f%%, speed: %.2fKB/s\n", (float)(compressStatus.compressed_all*100)/(float)compressStatus.uncompressed_all, ((float)compressStatus.uncompressed_all/1024.0f)/((float)(clock()-benchmark)/1000.0f)); break;
			default: wprintf(L"\rUndefinied error, code: %d                         \n", exitCode); break;
		}
	}else
		wprintf(L"%s%s", USAGE, MEMORY_USAGE);

	return 0;
}

