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

#ifndef ZIPARCHIVE_ZIPFILE_DOT_H
	#error Do not include this file directly. Include ZipFile.h instead
#endif

#include "ZipAbstractFile.h"
#include "ZipString.h"
#include "ZipExport.h"

#ifndef __GNUC__
	#include <io.h>
#else
	#include <unistd.h>
	#include <errno.h>	
#endif

#if !defined (_MSC_VER) || _MSC_VER < 1400	

// there seems to be a problem under Windows sometimes when using one of the functions below 
// without the underscore at the beginning	
#ifndef _lseek
	#define _lseek lseek
#endif

#ifndef _read
	#define _read read
#endif

#ifndef _close
	#define _close close
#endif

#ifndef _tell
	#define _tell tell
#endif

#ifndef _write
	#define _write write
#endif

#endif 
class ZIP_API CZipFile : public CZipAbstractFile
{
	void ThrowError() const;
public:
	int m_hFile;
	operator HANDLE();
	enum OpenModes
	{
		modeRead =          0x0001,
		modeWrite =         0x0002,
		modeReadWrite =     modeRead | modeWrite,
		shareDenyWrite =    0x0004,
		shareDenyRead =     0x0008,
		shareDenyNone =     0x0010,
		modeCreate =        0x0020,
		modeNoTruncate =    0x0040,
	};
	
	CZipFile(LPCTSTR lpszFileName, UINT openFlags)
	{
		m_hFile = -1;
		Open(lpszFileName, openFlags, true);
	}
	void Flush();
	ULONGLONG GetLength() const;
	CZipString GetFilePath() const {return m_szFileName;}
	bool IsClosed()const { return m_hFile == -1;}
	bool Open(LPCTSTR lpszFileName, UINT openFlags, bool bThrow);
	void Close() 
	{
		if (IsClosed())
			return;
		if (_close(m_hFile) != 0)
			ThrowError();
		else
		{
			m_szFileName.empty();
			m_hFile = -1;
		}
	}
	void Write(const void* lpBuf, UINT nCount)
	{
		if (_write(m_hFile, lpBuf, nCount) != (int) nCount)
			ThrowError();
	}
	ZIP_FILE_USIZE GetPosition() const
	{
	#ifndef __GNUC__
		ZIP_FILE_USIZE ret = _tell(m_hFile);
	#else
		ZIP_FILE_USIZE ret = lseek(m_hFile, 0, SEEK_CUR);
	#endif
		if (ret == -1L)
			ThrowError();
		return ret;
	}
	void SetLength(ZIP_FILE_USIZE uNewLen);
	UINT Read(void *lpBuf, UINT nCount)
	{
		errno = 0;
		int ret = _read(m_hFile, lpBuf, nCount);
		if (ret < (int) nCount && errno != 0)
			ThrowError();
		return ret;

	}

	ZIP_FILE_USIZE Seek(ZIP_FILE_SIZE dOff, int nFrom)
	{
		// restricted to signed
		ZIP_FILE_SIZE ret = (ZIP_FILE_SIZE)_lseek(m_hFile, (long)dOff, nFrom);
		if (ret == -1)
			ThrowError();
		return (ZIP_FILE_USIZE)ret;
	}
	CZipFile ();
	virtual ~CZipFile (){Close();};
protected:
	CZipString m_szFileName;

};
