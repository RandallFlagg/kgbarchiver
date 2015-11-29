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

#if !defined(ZIPARCHIVE_STDAFX_DOT_H)
#define ZIPARCHIVE_STDAFX_DOT_H

#include "_features.h"
#include "_platform.h"

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

// uncomment to disable compiling standard error messages into the library
#define ZIP_ENABLE_ERROR_DESCRIPTION


#if _MSC_VER < 1300 && !defined __BORLANDC__ && !defined (__GNUC__)
	#define ZIPINLINE inline
#else
	#define ZIPINLINE
#endif

#ifdef ZIP_ARCHIVE_STL
	#include "std_stl.h"
#else
	#include "std_mfc.h"
#endif

#ifdef TRACE
	#if _MSC_VER >= 1300
		#define ZIPTRACE(f) TRACE(f,__FILE__,__LINE__)
	#else
		#define ZIPTRACE(f) TRACE(_T(f))
	#endif
#else
	#define ZIPTRACE(f)
	#define NOZIPTRACE
#endif

	#define ZIP_U16_U64 WORD
	#define ZIP_U16_U32 WORD
	#define ZIP_U32_U64 DWORD
	#define ZIP_32_64 int

#define ZIP_FILE_INDEX_NOT_FOUND ZIP_U16_U64(-1)
#define ZIP_FILE_INDEX_UNSPECIFIED ZIP_FILE_INDEX_NOT_FOUND
#define ZIP_DISK_NUMBER_UNSPECIFIED ZIP_U16_U32(-1)

#endif // !defined(ZIPARCHIVE_STDAFX_DOT_H)
 
