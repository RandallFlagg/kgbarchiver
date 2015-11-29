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
* \file ZipAutoBuffer.h
* Interface for the CZipAutoBuffer class.
*
*/

#if !defined(ZIPARCHIVE_ZIPAUTOBUFFER_DOT_H)
#define ZIPARCHIVE_ZIPAUTOBUFFER_DOT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZipExport.h"
/**
	A smart buffer freeing its contents on destruction.
*/
class ZIP_API CZipAutoBuffer
{
public:
	operator char*()
	{
		return m_pBuffer;
	}
#ifndef __BORLANDC__
	operator const char*() const  // Borland seems to have problems with this
	{
		return m_pBuffer;
	}
#endif
	const char* GetBuffer() const {return m_pBuffer;}
	char* Allocate(DWORD iSize, bool bZeroMemory = false);
	void Release();
	DWORD GetSize() const 
	{
		return m_iSize;
	}
	bool IsAllocated() const
	{
		return (m_pBuffer != NULL);
	}
	CZipAutoBuffer(DWORD iSize, bool bZeroMemory = false);
	CZipAutoBuffer();
	CZipAutoBuffer(const CZipAutoBuffer& buffer);
	virtual ~CZipAutoBuffer();
	CZipAutoBuffer& operator=(const CZipAutoBuffer& buffer);
protected:
	char* m_pBuffer;
	DWORD m_iSize;
};

#endif // !defined(ZIPARCHIVE_ZIPAUTOBUFFER_DOT_H)
	
