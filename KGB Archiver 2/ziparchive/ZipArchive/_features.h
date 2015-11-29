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

#if !defined(ZIPARCHIVE_FEATURES_DOT_H)
#define ZIPARCHIVE_FEATURES_DOT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __GNUC__

#ifndef __int64
	#define __int64 long long
#endif

#endif

/************ Feel free to adjust the definitions in the following block ************/
/************************************ BLOCK START ***********************************/

// comment this out, if you don't use ZIP64
#define _ZIP64
// comment this out, if you don't use AES
// #define _ZIP_AES

/************************************* BLOCK END ***********************************/
/********* The contents below this line are not intended for modification **********/


#if defined _ZIP64 && defined __BORLANDC__ 
	#undef _ZIP64
#endif

#endif // !defined(ZIPARCHIVE_FEATURES_DOT_H)
