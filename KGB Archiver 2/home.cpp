#include <windows.h>
#include <tlhelp32.h> 
#include <string>

using namespace std;

wstring getHome(){
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE; 
  MODULEENTRY32 me32; 
 
  //  Take a snapshot of all modules in the specified process. 
  hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, GetCurrentProcessId() ); 
  if( hModuleSnap == INVALID_HANDLE_VALUE ) 
  { 
    //printError( "CreateToolhelp32Snapshot (of modules)" ); 
    return NULL; 
  } 
 
  //  Set the size of the structure before using it. 
  me32.dwSize = sizeof( MODULEENTRY32 ); 
 
  //  Retrieve information about the first module, 
  //  and exit if unsuccessful 
  if( !Module32First( hModuleSnap, &me32 ) ) 
  { 
    //printError( "Module32First" );  // Show cause of failure 
    CloseHandle( hModuleSnap );     // Must clean up the snapshot object! 
    return NULL; 
  } 

  for(int i=wcslen(me32.szExePath)-2;i>=0;i--){
	  if(me32.szExePath[i] == '\\'){
		  me32.szExePath[i+1] = '\0';
		  return (wstring)me32.szExePath;
	  }
  }
  return NULL;
}