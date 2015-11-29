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

/**
* \file BytesWriter.h
*	Interface for the CBytesWriter class.
*
*/

#if !defined(ZIPARCHIVE_BYTESWRITER_DOT_H)
#define ZIPARCHIVE_BYTESWRITER_DOT_H

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "ZipCompatibility.h"


/**
	Provides implementation for various 
	buffer operations depending on the current platform and configuration.
 */
class ZIP_API CBytesWriter
{
public:
	
#ifdef ZIP_ARCHIVE_LITTLE_ENDIAN
	/**
		Reads \a iCount bytes from \a pSource into \a pDestination.

		\param[out] pDestination
			The buffer to retrieve data with byte-ordering depending on the machine.

		\param[in] pSource
			The buffer with little-endian ordered data.

		\param iCount 
			The number of bytes to read.
	*/
	static void ReadBytes(void* pDestination, const char* pSource, int iCount)
	{
		memcpy(pDestination, pSource, iCount);
	}

	/**
		Writes \a iCount bytes from \a pSource into \a pDestination.

		\param[out] pDestination
			The buffer to retrieve little-endian ordered data.

		\param[in] pSource
			The buffer with byte-ordering depending on the machine.

		\param iCount 
			The number of bytes to write.
	*/
	static void WriteBytes(char* pDestination, const void* pSource, int iCount)
	{
		memcpy(pDestination, pSource, iCount);
	}

	/**
		Compares \a iCount bytes.

		\param[in] pBuffer
			The buffer with little-endian ordered data.

		\param[in] pBytes
			The buffer with byte-ordering depending on the machine.

		\param iCount 
			The number of bytes to compare.
	*/
	static bool CompareBytes(const char* pBuffer, const void* pBytes, int iCount)
	{
		return memcmp(pBuffer, pBytes, iCount) == 0;
	}
#else
	static void ReadBytes(void* pDestination, const char* pSource, int iCount)
	{
		for (int i = 0; i < iCount; i++)
			((char*)pDestination)[i] = pSource[iCount - i - 1];
	}

	static void WriteBytes(char* pDestination, const void* pSource, int iCount)
	{
		for (int i = 0; i < iCount; i++)
			pDestination[i] = ((char*)pSource)[iCount - i - 1];
	}

	static bool CompareBytes(const char* pBuffer, const void* pBytes, int iCount)
	{
		for (int i = 0; i < iCount; i++)
			if (pBuffer[i] != ((char*)pBytes)[iCount - i - 1])
				return false;
		return true;
	}

#endif
	
	static const DWORD& WriteSafeU32(const DWORD& uValue)
	{
		return uValue;
	}
	static const WORD& WriteSafeU16(const WORD& uValue)
	{
		return uValue;
	}


};

#endif
