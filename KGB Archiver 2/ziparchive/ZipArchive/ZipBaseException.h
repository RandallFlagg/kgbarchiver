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

#ifndef ZIPARCHIVE_ZIPBASEEXCEPTION_DOT_H
#define ZIPARCHIVE_ZIPBASEEXCEPTION_DOT_H

#ifdef ZIP_ARCHIVE_STL
	typedef std::exception CZipBaseException;
#else
	typedef CException CZipBaseException;
#endif

#endif //ZIPARCHIVE_ZIPBASEEXCEPTION_DOT_H4
