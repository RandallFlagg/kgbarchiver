
#ifndef lang_cpp
#define lang_cpp

static void setLang(){
	DWORD langID = 0x0409;
	CRegKey reg(HKEY_CURRENT_USER);
	if(reg.Open(HKEY_CURRENT_USER, "Software\\KGB Archiver") == ERROR_SUCCESS){
		//MessageBox(0, "", "", 0);
		reg.QueryDWORDValue("lang", langID);
	}

	//WORD lang = MAKELANGID(langID, 0x00);
	LCID loc = MAKELCID(langID, SORT_DEFAULT);
	//ConvertDefaultLocale(loc);
	SetThreadLocale(loc);
}

#endif