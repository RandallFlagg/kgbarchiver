#include <windows.h>

#pragma argsused

//------------------------------------------------------------------------//
/*
int WINAPI _export DllEntryPoint( HINSTANCE hinst, DWORD fdwReason,
                                  PVOID pvReserved )
{
   return TRUE;
}
*/
int far pascal LibMain( HANDLE hInstance, WORD wDataSeg, WORD wHeapSize,
                        LPSTR lpszCmdLine )
{
   return 1;
}

//------------------------------------------------------------------------//
