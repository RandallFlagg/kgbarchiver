#include <psapi.h>

static void setHomeDir(){
    HANDLE hProcess = GetCurrentProcess();
	HMODULE hMod;
    DWORD cbNeeded;
	char processName[MAX_PATH];
	
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
}