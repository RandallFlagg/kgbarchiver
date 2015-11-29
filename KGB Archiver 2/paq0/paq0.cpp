// paq0.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" __declspec(dllexport) void kgbcompress(FILE *, char *, unsigned __int64);
extern "C" __declspec(dllexport) void setMode(char);
extern "C" __declspec(dllexport) void kgbdecompress(FILE *, char *, unsigned __int64);

void kgbcompress(FILE *archive, char *data, unsigned __int64 size){
	fwrite(data, 1, size, archive);
}

void setMode(char mode){
	//ignored
}

void kgbdecompress(FILE *archive, char *data, unsigned __int64 size){
	fread(data, 1, size, archive);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

