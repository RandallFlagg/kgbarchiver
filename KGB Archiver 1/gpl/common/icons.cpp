#include <atlbase.h>

#pragma once

#ifndef icons_cpp
#define icons_cpp

HICON getIcon(string file){
	SHFILEINFO shfi;
	memset(&shfi,0,sizeof(shfi));
	SHGetFileInfo(file.c_str(), FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(shfi), SHGFI_ICON|SHGFI_USEFILEATTRIBUTES);
	return shfi.hIcon;
}

#endif