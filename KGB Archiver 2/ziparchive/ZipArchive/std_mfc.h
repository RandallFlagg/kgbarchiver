////////////////////////////////////////////////////////////////////////////////
// This source file is part of the ZipArchive library source distribution and
// is Copyrighted 2000 - 2006 by Tadeusz Dracz (tdracz@artpol-software.com)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// For the licensing details refer to the License.txt file.
//
// Web Site: http://www.artpol-software.com
////////////////////////////////////////////////////////////////////////////////

#ifndef ZIPARCHIVE_STDAFX_DOT_H
	#error Do not include this file directly. Include stdafx.h instead
#endif

#if _MSC_VER > 1000
#ifndef WINVER
	#define WINVER 0x0400
#endif
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#ifndef VC_EXTRALEAN
	#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>

#if _MSC_VER >= 1300
	#define ZIP_FILE_USIZE ULONGLONG
	#define ZIP_FILE_SIZE LONGLONG
	#define ZIP_FILE_SIZEMAX _I64_MAX	
#else
	#define ZIP_FILE_USIZE DWORD
	#define ZIP_FILE_SIZE LONG
	#define ZIP_FILE_SIZEMAX MAXLONG		
#endif