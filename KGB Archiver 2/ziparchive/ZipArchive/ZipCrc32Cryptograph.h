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
* \file ZipCrc32Cryptograph.h
* Interface for the CZipCrc32Cryptograph class.
*
*/

#if !defined(ZIPARCHIVE_ZIPCRC32CRYPTOGRAPH_DOT_H)
#define ZIPARCHIVE_ZIPCRC32CRYPTOGRAPH_DOT_H

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "zlib/zlib.h"

#include "ZipCryptograph.h"
#include "ZipFileHeader.h"
#include "ZipStorage.h"


#define ZIPARCHIVE_ENCR_HEADER_LEN 12

/**
	Performs the traditional zip encryption.
	
	\see
		<a href="kb">0610201627|std</a>
*/
class ZIP_API CZipCrc32Cryptograph : public CZipCryptograph
{
public:
	CZipCrc32Cryptograph(){}
	
	bool InitDecode(CZipAutoBuffer& password, CZipFileHeader& currentFile, CZipStorage& storage);	
	void InitEncode(CZipAutoBuffer& password, CZipFileHeader& currentFile, CZipStorage& storage);	
	void Decode(char* pBuffer, DWORD uSize)
	{
		for (DWORD i = 0; i < uSize; i++)
			CryptDecode(pBuffer[i]);
	}
	void Encode(char* pBuffer, DWORD uSize)
	{
		for (DWORD i = 0; i < uSize; i++)
			CryptEncode(pBuffer[i]);
	}
	static DWORD GetEncryptedInfoSize() {return ZIPARCHIVE_ENCR_HEADER_LEN;}
	static const DWORD* GetCRCTable()
	{
			return get_crc_table();
	}
private:

	void CryptDecode(char &c)
	{
		c ^= CryptDecryptByte();
		CryptUpdateKeys(c);
	}

	char CryptDecryptByte()
	{
		int temp = (m_keys[2] & 0xffff) | 2;
		return (char)(((temp * (temp ^ 1)) >> 8) & 0xff);
	}
	void CryptInitKeys(CZipAutoBuffer& password);	
	void CryptUpdateKeys(char c);
	DWORD CryptCRC32(DWORD l, char c)
	{
		const DWORD *CRC_TABLE = get_crc_table();
		return CRC_TABLE[(l ^ c) & 0xff] ^ (l >> 8);
	}
	void CryptEncode(char &c)
	{
		char t = CryptDecryptByte();
		CryptUpdateKeys(c);
		c ^= t;
	}
	DWORD m_keys[3];
public:	
	~CZipCrc32Cryptograph(){}
};

#endif
