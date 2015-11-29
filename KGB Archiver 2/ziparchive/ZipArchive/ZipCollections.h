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

#ifndef ZIPARCHIVE_ZIPCOLLECTIONS_DOT_H
#define ZIPARCHIVE_ZIPCOLLECTIONS_DOT_H

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "_platform.h"
#include "ZipExport.h"

#ifdef ZIP_ARCHIVE_STL
	#include "ZipCollections_stl.h"
#else
	#include "ZipCollections_mfc.h"	
#endif

typedef CZipArray<ZIP_U16_U64> CZipIndexesArray;

#endif  /* ZIPARCHIVE_ZIPCOLLECTIONS_DOT_H */

