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

#include "_platform.h"

#ifdef ZIP_ARCHIVE_STL

#include "stdafx.h"
#include "ZipFile.h"
#include "ZipException.h"
#include "ZipPlatform.h"

#include <fcntl.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZipFile::CZipFile()
{
	m_hFile = -1;
}

void CZipFile::ThrowError() const
{
	CZipException::Throw(errno, m_szFileName);
}


ULONGLONG CZipFile::GetLength() const
{
	// cannot use Seek here, Seek is not const
	ZIP_U32_U64 lLen, lCur;
	lCur = _lseek(m_hFile, 0, current);
	if (lCur == -1)
		ThrowError();
	lLen = _lseek(m_hFile, 0, end);

	// first go back
	bool err = _lseek(m_hFile, lCur, begin) == -1;

	if (err || lLen == -1)
		ThrowError();
	return lLen;

}


bool CZipFile::Open(LPCTSTR lpszFileName, UINT openFlags, bool bThrow)
{
	if (!IsClosed())
		Close();

#ifdef O_BINARY
	UINT iNewFlags = O_BINARY;
#else
	UINT iNewFlags = 0;
#endif

	bool bReadOnly = false;
	if (openFlags & CZipFile::modeCreate)
		iNewFlags |= O_CREAT;
	if ((openFlags & CZipFile::modeReadWrite) == CZipFile::modeReadWrite)
		iNewFlags |= O_RDWR;
	else if (openFlags & CZipFile::modeRead)
	{
		// O_RDONLY is defined as 0
		bReadOnly = true;
		iNewFlags |= O_RDONLY;
	}
	else if (openFlags & CZipFile::modeWrite)
		iNewFlags |= O_WRONLY;

	if (!(openFlags & CZipFile::modeNoTruncate) && !bReadOnly)
		iNewFlags |= O_TRUNC;
	m_hFile = ZipPlatform::OpenFile(lpszFileName, iNewFlags, openFlags & 0x1C);
	if (m_hFile == -1)
		if (bThrow)
			ThrowError();
		else
			return false;
	m_szFileName = lpszFileName;
	return true;
}


void CZipFile::SetLength(ULONGLONG uNewLen)
{
	ZipPlatform::TruncateFile(m_hFile, uNewLen);
}


void  CZipFile::Flush()
{
	if (!ZipPlatform::FlushFile(m_hFile)) 
		ThrowError();
}

CZipFile::operator HANDLE()
{
	intptr_t fh = ZipPlatform::GetFileSystemHandle(m_hFile);
	if (fh == -1)
		ThrowError();
#if _MSC_VER >= 1300
	return (HANDLE)fh;
#else
	return (HANDLE)fh;
#endif
}

#endif
