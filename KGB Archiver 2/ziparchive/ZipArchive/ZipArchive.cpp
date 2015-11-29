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

#include "stdafx.h"
#include "ZipArchive.h"
#include "ZipPlatform.h"
#include "ZipCompatibility.h"
#include "BytesWriter.h"

#include <time.h>

#ifndef DEF_MEM_LEVEL
#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const TCHAR CZipArchive::m_gszCopyright[] = {_T("ZipArchive library Copyright 2000 - 2006 Tadeusz Dracz")};

void CZipAddNewFileInfo::Defaults()
{
	m_iSmartLevel = CZipArchive::zipsmSafeSmart;
	m_uReplaceIndex = ZIP_FILE_INDEX_UNSPECIFIED;
	m_nBufSize = 65536;
	m_iComprLevel = -1; // default
}

CZipArchive::CZipArchive()
{

	m_bRemoveDriveLetter = m_bDetectZlibMemoryLeaks = true;
	m_bIgnoreCRC = m_bAutoFlush = false;
	m_centralDir.m_pStorage= &m_storage;	
	m_info.m_stream.zalloc = (alloc_func)_zliballoc;
	m_info.m_stream.zfree = (free_func)_zlibfree;
	m_iFileOpened = nothing;
	SetCaseSensitivity(ZipPlatform::GetSystemCaseSensitivity());
	m_iEncryptionMethod = CZipCryptograph::encStandard;
	m_pCryptograph = NULL;
}


CZipArchive::~CZipArchive()
{
	// 	Close(); // cannot be here: if an exception is thrown strange things can happen
	EmptyPtrList();	
	ClearCryptograph();
}



void CZipArchive::Open(LPCTSTR szPathName, int iMode, ZIP_U32_U64 uVolumeSize)
{
	if (!IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive already opened.\n");
		return;
	}
	m_storage.Open(szPathName, iMode, uVolumeSize);
	OpenInternal(iMode);
}

void CZipArchive::Open(CZipAbstractFile& af,int iMode)
{
	if (!IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive already opened.\n");
		return;
	}
	if (iMode != zipOpen && iMode != zipOpenReadOnly && iMode != zipCreate)
	{
		ZIPTRACE("%s(%i) : Mode not supported.\n");
		return;
	}
	m_storage.Open(af, iMode);
	OpenInternal(iMode);
}


void CZipArchive::OpenInternal(int iMode)
{
	m_pszPassword.Release();
	m_iFileOpened = nothing;
	m_iArchiveSystCompatib = ZipPlatform::GetSystemID();
	m_centralDir.Init(m_iArchiveSystCompatib);	
	m_szRootPath.Empty();

	if ((iMode == zipOpen) ||(iMode == zipOpenReadOnly))
	{
		m_centralDir.Read();
		// if there is at least one file, get system comp. from the first one
		if (m_centralDir.IsValidIndex(0))
		{			
			int iSystemComp = m_centralDir[0]->GetSystemCompatibility();
			if (ZipCompatibility::IsPlatformSupported(iSystemComp))
				m_iArchiveSystCompatib = iSystemComp;
		}
	}

}

void CZipArchive::ThrowError(int err, bool bZlib)
{
	if (bZlib)
		err = CZipException::ZlibErrToZip(err);
	CZipException::Throw(err, IsClosed() ? _T("") : (LPCTSTR)m_storage.m_pFile->GetFilePath());
}



bool CZipArchive::GetFileInfo(CZipFileHeader & fhInfo, ZIP_U16_U64 uIndex) const
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return false;
	}
	
	if (!m_centralDir.IsValidIndex(uIndex))
		return false;
	
	fhInfo = *(m_centralDir[uIndex]);
	return true;
}

CZipFileHeader* CZipArchive::GetFileInfo(ZIP_U16_U64 uIndex)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return NULL;
	}
	
	if (!m_centralDir.IsValidIndex(uIndex))
		return NULL;
	
	return m_centralDir[uIndex];
}

const CZipFileHeader* CZipArchive::GetFileInfo(ZIP_U16_U64 uIndex) const
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return NULL;
	}
	
	if (!m_centralDir.IsValidIndex(uIndex))
		return NULL;
	
	return m_centralDir[uIndex];
}



ZIP_U16_U64 CZipArchive::FindFile(LPCTSTR lpszFileName, int iCaseSensitive, bool bFileNameOnly)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return ZIP_FILE_INDEX_NOT_FOUND;
	}
	bool bCS;
	bool bSporadically;
	switch (iCaseSensitive)
	{
	case ffCaseSens:
		bCS = true;
		bSporadically = true;
		break;
	case ffNoCaseSens:
		bCS = false;
		bSporadically = true;
		break;
	default:
		bCS = m_bCaseSensitive;
		bSporadically = false;
	}
	return m_centralDir.FindFile(lpszFileName, bCS, bSporadically, bFileNameOnly);
}

bool CZipArchive::OpenFile(ZIP_U16_U64 uIndex)
{	
	if (!m_centralDir.IsValidIndex(uIndex))
	{
		ASSERT(FALSE);
		return false;
	}
	if (m_storage.IsSegmented() == 1)
	{
		ZIPTRACE("%s(%i) : You cannot extract from a segmented archive in creation.\n");
		return false;
	}	
	
	if (m_iFileOpened)
	{
		ZIPTRACE("%s(%i) : A file already opened.\n");
		return false;
	}
	
	m_info.Init();
	m_centralDir.OpenFile(uIndex);
	if (CurrentFile()->IsEncrypted())
	{		
		if (m_pszPassword.GetSize() == 0)
		{
			ZIPTRACE("%s(%i) : Password not set for the encrypted file.\n");
				ThrowError(CZipException::badPassword);
		}
		CreateCryptograph(CurrentFile()->m_uEncryptionMethod);
		if (!m_pCryptograph->InitDecode(m_pszPassword, *CurrentFile(), m_storage))
			ThrowError(CZipException::badPassword); 

	}
	else if (m_pszPassword.GetSize() != 0)
	{
		ZIPTRACE("%s(%i) : Password set for a not encrypted file. Ignoring password.\n");
	}
	
	// should be validated already in CZipFileHeader::ReadLocal (from CZipCentralDir::OpenFile)
	ASSERT(!CurrentFile()->m_uMethod || CurrentFile()->m_uMethod == Z_DEFLATED);

	if (CurrentFile()->m_uMethod == Z_DEFLATED)
	{
		m_info.m_stream.opaque =  m_bDetectZlibMemoryLeaks ? &m_list : 0;
		int err = inflateInit2(&m_info.m_stream, -MAX_WBITS);
		//			* windowBits is passed < 0 to tell that there is no zlib header.
		//          * Note that in this case inflate *requires* an extra "dummy" byte
		//          * after the compressed stream in order to complete decompression and
		//          * return Z_STREAM_END. 
		CheckForError(err);
	}
	m_info.m_uComprLeft = CurrentFile()->GetDataSize(false, true);
	m_info.m_uUncomprLeft = CurrentFile()->m_uUncomprSize;
	m_info.m_uCrc32 = 0;
	m_info.m_stream.total_out = 0;
	m_info.m_stream.avail_in = 0;
	
	m_iFileOpened = extract;
	return true;
}

void* CZipArchive::_zliballoc(void* opaque, UINT items, UINT size)
{
	void* p = new char[size * items];
	if (opaque)
	{
		CZipPtrList<void*>* list  = (CZipPtrList<void*>*) opaque;
		list->AddTail(p);
	}
	return p;
}

void CZipArchive::_zlibfree(void* opaque, void* address)
{
	if (opaque)
	{
		CZipPtrList<void*>* list  = (CZipPtrList<void*>*) opaque;
		CZipPtrListIter iter = list->Find(address);
		if (list->IteratorValid(iter))
			list->RemoveAt(iter);
	}
	delete[] (char*) address;
}

void CZipArchive::CheckForError(int iErr)
{
	if ((iErr == Z_OK) ||(iErr == Z_NEED_DICT))
		return;
	
	ThrowError(iErr, true);
}

CZipFileHeader* CZipArchive::CurrentFile()
{
	ASSERT(m_centralDir.m_pOpenedFile);
	return m_centralDir.m_pOpenedFile;
}

DWORD CZipArchive::ReadFile(void *pBuf,     
                            DWORD uSize)
{
	if (m_iFileOpened != extract)
	{
		ZIPTRACE("%s(%i) : Current file must be opened.\n");
		return 0;
	}
	
	if (!pBuf || !uSize)
		return 0;
	
	m_info.m_stream.next_out = (Bytef*)pBuf;
	m_info.m_stream.avail_out = uSize > m_info.m_uUncomprLeft 
		? (DWORD)m_info.m_uUncomprLeft : uSize;
	
	
	DWORD iRead = 0;

	// may happen when the file is 0 sized
	bool bForce = m_info.m_stream.avail_out == 0 && m_info.m_uComprLeft > 0;
	while (m_info.m_stream.avail_out > 0 || (bForce && m_info.m_uComprLeft > 0))
	{
		if ((m_info.m_stream.avail_in == 0) &&
			(m_info.m_uComprLeft >= 0)) // Also when there are zero bytes left!
		{
			DWORD uToRead = m_info.m_pBuffer.GetSize();
			if (m_info.m_uComprLeft < uToRead)
				uToRead = (DWORD)m_info.m_uComprLeft;
			
			if (uToRead == 0)
			{
				uToRead = 1; // Add dummy byte at end of compressed data.
			}
			else
			{
				m_storage.Read(m_info.m_pBuffer, uToRead, false);
				if (m_pCryptograph)
					m_pCryptograph->Decode(m_info.m_pBuffer, uToRead);
			}

			m_info.m_uComprLeft -= uToRead;
			
			m_info.m_stream.next_in = (Bytef*)(char*)m_info.m_pBuffer;
			m_info.m_stream.avail_in = uToRead;
		}
		
		if (CurrentFile()->m_uMethod == 0)
		{
			DWORD uToCopy = m_info.m_stream.avail_out < m_info.m_stream.avail_in 
				? m_info.m_stream.avail_out : m_info.m_stream.avail_in;
			
			memcpy(m_info.m_stream.next_out, m_info.m_stream.next_in, uToCopy);
			
			m_info.m_uCrc32 = crc32(m_info.m_uCrc32, m_info.m_stream.next_out, uToCopy);
			
			m_info.m_uUncomprLeft -= uToCopy;
			m_info.m_stream.avail_in -= uToCopy;
			m_info.m_stream.avail_out -= uToCopy;
			m_info.m_stream.next_out += uToCopy;
			m_info.m_stream.next_in += uToCopy;
            m_info.m_stream.total_out += uToCopy;
			iRead += uToCopy;
		}
		else
		{
			ZIP_U32_U64 uTotal = m_info.m_stream.total_out;
			Bytef* pOldBuf =  m_info.m_stream.next_out;
			int err = inflate(&m_info.m_stream, Z_SYNC_FLUSH);
			// will not exceed DWORD
			DWORD uToCopy = (DWORD)(m_info.m_stream.total_out - uTotal);
			
			m_info.m_uCrc32 = crc32(m_info.m_uCrc32, pOldBuf, uToCopy);
			
			m_info.m_uUncomprLeft -= uToCopy;
			iRead += uToCopy;
            
			if (err == Z_STREAM_END)
				return iRead;
			
			CheckForError(err);
		}
	}
	
	return iRead;
}

void CZipArchive::Close(int iAfterException, bool bUpdateTimeStamp)
{
	// if after an exception - the archive may be closed, but the file may be opened
	if (IsClosed() && (!iAfterException || IsClosed(false)))
	{
		ZIPTRACE("%s(%i) : ZipArchive is already closed.\n");
		return;
	}

	if (m_iFileOpened == extract)
		CloseFile(NULL, iAfterException != afNoException);

	if (m_iFileOpened == compress)
		CloseNewFile(iAfterException != afNoException);

	if (iAfterException != afAfterException && !IsClosed(false)) // in disk spanning when user aborts 
		WriteCentralDirectory(false);  // we will flush in CZipStorage::Close

	time_t tNewestTime = 0;

	if (bUpdateTimeStamp)
	{
		ZIP_U16_U64 iSize = (ZIP_U16_U64)m_centralDir.m_headers.GetSize();
		for (ZIP_U16_U64 i = 0; i < iSize; i++)
		{
			time_t tFileInZipTime = m_centralDir[i]->GetTime();
			if (tFileInZipTime > tNewestTime)
				tNewestTime = tFileInZipTime;
		}
	}
	m_centralDir.Clear();
	ResetStringStoreSettings();
	CZipString szFileName = m_storage.Close(iAfterException == afAfterException);
	if (bUpdateTimeStamp && !szFileName.IsEmpty())
		ZipPlatform::SetFileModTime(szFileName, tNewestTime);
}

void CZipArchive::WriteCentralDirectory(bool bFlush)
{
	m_centralDir.Write(GetCallback(cbSave));
	if (bFlush)
		m_storage.Flush();
}

void CZipArchive::SetCallback(CZipActionCallback* pCallback, int iWhich)
{
	CallbackType cbs[] = {cbAdd, cbAddTmp, cbAddStore, cbExtract, cbDeleteCnt, cbDelete, cbTest, cbSave, cbGet, cbRename, cbReplace};
	int iCount = sizeof(cbs)/sizeof(CallbackType);
	for (int i = 0; i < iCount; i++)
	{
		CallbackType iCallback = cbs[i];
		if (iWhich & iCallback)
			m_callbacks.Set(pCallback, iCallback);
	}
}

void CZipArchive::SetAdvanced(int iWriteBuffer, int iGeneralBuffer, int iSearchBuffer)
{
	if (!IsClosed())
	{
		ZIPTRACE("%s(%i) : Set this options before opening the archive.\n");
		return;
	}
	
	m_storage.m_iWriteBufferSize = iWriteBuffer < 1024 ? 1024 : iWriteBuffer;
	m_info.m_iBufferSize = iGeneralBuffer < 1024 ? 1024 : iGeneralBuffer;
	m_storage.m_iLocateBufferSize = iSearchBuffer < 1024 ? 1024 : iSearchBuffer;
}

int CZipArchive::CloseFile(CZipFile &file)
{
	CZipString temp = file.GetFilePath();
	file.Close();
	return CloseFile(temp);
}

int CZipArchive::CloseFile(LPCTSTR lpszFilePath, bool bAfterException)
{
	if (m_iFileOpened != extract)
	{
		ZIPTRACE("%s(%i) : No opened file.\n");
		return 0;
	}

	int iRet = 1;
	if (!bAfterException)
	{
		if (m_info.m_uUncomprLeft == 0)
		{
			if (!m_bIgnoreCRC && !CurrentFile()->m_bIgnoreCrc32 && m_info.m_uCrc32 != CurrentFile()->m_uCrc32)
				ThrowError(CZipException::badCrc);
		}
		else
			iRet = -1;

		if (CurrentFile()->m_uMethod == Z_DEFLATED)
			inflateEnd(&m_info.m_stream);
		
		if (lpszFilePath)
		{			
			if (!ZipPlatform::SetFileModTime(lpszFilePath, CurrentFile()->GetTime())
				||!ZipPlatform::SetFileAttr(lpszFilePath, CurrentFile()->GetSystemAttr()))
					iRet = -2;
		}
		if (m_pCryptograph)
			m_pCryptograph->FinishDecode(*CurrentFile(), m_storage);
	}

	m_centralDir.CloseFile(bAfterException);
		
	m_info.ReleaseBuf();
	m_iFileOpened = nothing;	
	ClearCryptograph();
	EmptyPtrList();
	return iRet;
}

bool CZipArchive::OpenNewFile(CZipFileHeader & header, int iLevel, LPCTSTR lpszFilePath,
							  ZIP_U16_U64 uReplaceIndex)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return false;
	}
	
	if (m_iFileOpened)
	{
		ZIPTRACE("%s(%i) : A file already opened.\n");
		return false;
	}
	
	if (m_storage.IsSegmented() == -1)
	{
		ZIPTRACE("%s(%i) : You cannot add files to an existing segmented archive.\n");
		return false;
	}

	if (GetCount() ==(WORD)USHRT_MAX)
	{
		ZIPTRACE("%s(%i) : Maximum file count inside archive reached.\n");
		return false;
	}
	
	DWORD uAttr = 0;
	time_t ttime;
	if (lpszFilePath)
	{
		if (!ZipPlatform::GetFileAttr(lpszFilePath, uAttr))
			// do not continue - if the file was a directory then not recognizing it will cause 
			// serious errors (need uAttr to recognize it)
			return false;
		if (!ZipPlatform::GetFileModTime(lpszFilePath, ttime))
			ttime = time(NULL);
	}

	m_info.Init();
	
	
	if (lpszFilePath)
	{
		header.SetTime(ttime);
		SetFileHeaderAttr(header, uAttr); // set system compatibility as well
	}
	else
		header.SetSystemCompatibility(m_iArchiveSystCompatib);

	CZipString szFileName = header.GetFileName();
	

	bool bIsDirectory = header.IsDirectory();
	if (bIsDirectory)
	{
		int iNameLen = szFileName.GetLength();		
		if (!iNameLen || !CZipPathComponent::IsSeparator(szFileName[iNameLen-1]))
		{
			szFileName += CZipPathComponent::m_cSeparator;
			header.SetFileName(szFileName);
		}
	}

	if (szFileName.IsEmpty())
	{
		szFileName.Format(_T("file%i"), GetCount());
		header.SetFileName(szFileName);
	}

	bool bEncrypted = m_pszPassword.GetSize() != 0;

#if defined _DEBUG && !defined NOZIPTRACE
	if (bIsDirectory && bEncrypted)
		ZIPTRACE("%s(%i) : Encrypting a directory. You may want to consider clearing the password before adding a directory.\n");
#endif		
	
	bool bReplace = uReplaceIndex != ZIP_FILE_INDEX_UNSPECIFIED;

	if (iLevel < -1 || iLevel > 9)
		iLevel = -1;
	
	if (bEncrypted)
	{
		header.m_uEncryptionMethod = (BYTE)m_iEncryptionMethod;
		CreateCryptograph(m_iEncryptionMethod);
	}
	CZipFileHeader* pHeader = m_centralDir.AddNewFile(header, uReplaceIndex, iLevel, bEncrypted);	

	// replace can happen only from AddNewFile and the compressed size is already known and set (the file is stored, not compressed)
	if (bReplace)
	{
		// this will be used in GetLocalSize and WriteLocal
		pHeader->PrepareFileName();
		// we use the local size, because the real does not exist yet
		ZIP_U32_U64 uFileSize = pHeader->GetDataSize(true, false) + pHeader->GetLocalSize(false) + pHeader->GetDataDescriptorSize(&m_storage);
		MakeSpaceForReplace(uReplaceIndex, uFileSize, szFileName);
	}

	CurrentFile()->WriteLocal(&m_storage);

	if (m_pCryptograph)
		m_pCryptograph->InitEncode(m_pszPassword, *pHeader, m_storage);
	
	m_info.m_uComprLeft = 0;
    m_info.m_stream.avail_in = (uInt)0;
    m_info.m_stream.avail_out = (uInt)m_info.m_pBuffer.GetSize();
    m_info.m_stream.next_out = (Bytef*)(char*)m_info.m_pBuffer;
    m_info.m_stream.total_in = 0;
    m_info.m_stream.total_out = 0;
	
	if (bIsDirectory && (CurrentFile()->m_uMethod != 0))
		CurrentFile()->m_uMethod = 0;
	
	if (CurrentFile()->m_uMethod == Z_DEFLATED)
    {
        m_info.m_stream.opaque = m_bDetectZlibMemoryLeaks ? &m_list : 0;
		
        int err = deflateInit2(&m_info.m_stream, iLevel,
			Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);
		
		CheckForError(err);
    }
	m_iFileOpened = compress;
	return true;
}


bool CZipArchive::ExtractFile(ZIP_U16_U64 uIndex,                  
                              LPCTSTR lpszPath,             
                              bool bFullPath,              
                              LPCTSTR lpszNewName,          
                              DWORD nBufSize)
{
	
	if (!nBufSize && !lpszPath)
		return false;
	
	CZipFileHeader* pHeader = (*this)[uIndex];	
	CZipString szFileNameInZip = pHeader->GetFileName();
	CZipString szFile = PredictExtractedFileName(szFileNameInZip, lpszPath, bFullPath, lpszNewName);
	CZipActionCallback* pCallback = GetCallback(cbExtract);
	if (pCallback)
		pCallback->Init(szFileNameInZip, szFile);
	

	if (pHeader->IsDirectory())
	{
		if (pCallback)
			pCallback->SetTotal(0); // in case of calling LeftToDo afterwards

		ZipPlatform::ForceDirectory(szFile);
		ZipPlatform::SetFileAttr(szFile, pHeader->GetSystemAttr());

		if (pCallback)
			pCallback->CallbackEnd();
		return true;
	}
	else
	{
		if (pCallback)
			pCallback->SetTotal(pHeader->m_uUncomprSize);

		if (!OpenFile(uIndex))
			return false;

		CZipPathComponent zpc(szFile);
		ZipPlatform::ForceDirectory(zpc.GetFilePath());
		CZipFile f(szFile, CZipFile::modeWrite | 
			CZipFile::modeCreate | CZipFile::shareDenyWrite);
		DWORD iRead;
		CZipAutoBuffer buf(nBufSize);
		int iAborted = 0;
		do
		{
			iRead = ReadFile(buf, buf.GetSize());
			if (iRead)
			{	
				f.Write(buf, iRead);
				if (pCallback)
					if (!(*pCallback)(iRead))
					{
						if (iRead == buf.GetSize() && ReadFile(buf, 1) != 0) // test one byte if there is something left
							iAborted = CZipException::abortedAction; 
						else
							iAborted = CZipException::abortedSafely; // we did it!
						break;
					}

			}
		}
		while (iRead == buf.GetSize());
		bool bRet = CloseFile(f) == 1;
		if (!bRet && iAborted == CZipException::abortedSafely)
			iAborted = CZipException::abortedAction; // finished, but not successfull

		if (pCallback)
			pCallback->CallbackEnd();

		if (iAborted)
			CZipException::Throw(iAborted, szFile); // throw to distuingiush from other return codes
		return bRet;

	}	
}

bool CZipArchive::ExtractFile(ZIP_U16_U64 uIndex,                  
                              CZipMemFile& mf,
							  bool bRewind,
                              DWORD nBufSize)
{
	if (!nBufSize)
		return false;
	
	CZipFileHeader* pHeader = (*this)[uIndex];
	CZipActionCallback* pCallback = GetCallback(cbExtract);
	if (pCallback)
	{
		pCallback->Init(pHeader->GetFileName());
		pCallback->SetTotal(pHeader->m_uUncomprSize);
	}

	if (pHeader->IsDirectory() || !OpenFile(uIndex))
		return false;

	
	

	CZipAutoBuffer buf(nBufSize);
	//mf.SeekToEnd();
	ZIP_FILE_USIZE oldPos = 0;

	if (bRewind)
		oldPos = mf.GetPosition();

	DWORD iRead;
	int iAborted = 0;
	do
	{
		iRead = ReadFile(buf, buf.GetSize());
		if (iRead)
		{	
			mf.Write(buf, iRead);
			if (pCallback)
				if (!(*pCallback)(iRead))
				{
					if (iRead == buf.GetSize() && ReadFile(buf, 1) != 0) // test one byte if there is something left
						iAborted = CZipException::abortedAction; 
					else
						iAborted = CZipException::abortedSafely; // we did it!
					break;
				}
		}
	}
	while (iRead == buf.GetSize());
	bool bRet = CloseFile() == 1;
	if (!bRet && iAborted == CZipException::abortedSafely)
		iAborted = CZipException::abortedAction; // sorry, finished, but not successfull

	if (pCallback)
		pCallback->CallbackEnd();

	if (bRewind)
		mf.Seek(oldPos, true);

	if (iAborted)
		CZipException::Throw(iAborted); // throw to distuingiush from other return codes
	return bRet;
}


bool CZipArchive::WriteNewFile(const void *pBuf, DWORD iSize)
{
	if (m_iFileOpened != compress)
	{
		ZIPTRACE("%s(%i) : A new file must be opened.\n");
		return false;
	}
	
	
    m_info.m_stream.next_in = (Bytef*)pBuf;
    m_info.m_stream.avail_in = iSize;
    CurrentFile()->m_uCrc32 = crc32(CurrentFile()->m_uCrc32, (Bytef*)pBuf, iSize);
	
	
    while (m_info.m_stream.avail_in > 0)
    {
        if (m_info.m_stream.avail_out == 0)
        {
			if (m_pCryptograph)
				m_pCryptograph->Encode(m_info.m_pBuffer, (DWORD)m_info.m_uComprLeft);
			m_storage.Write(m_info.m_pBuffer, (DWORD)m_info.m_uComprLeft, false);
			m_info.m_uComprLeft = 0;
            m_info.m_stream.avail_out = m_info.m_pBuffer.GetSize();
            m_info.m_stream.next_out = (Bytef*)(char*)m_info.m_pBuffer;
        }
		
        if (CurrentFile()->m_uMethod == Z_DEFLATED)
        {
            ZIP_32_64 uTotal = m_info.m_stream.total_out;
            int err = deflate(&m_info.m_stream,  Z_NO_FLUSH);
			CheckForError(err);
            m_info.m_uComprLeft += m_info.m_stream.total_out - uTotal;
        }
        else
        {
            DWORD uToCopy = (m_info.m_stream.avail_in < m_info.m_stream.avail_out) 
				? m_info.m_stream.avail_in : m_info.m_stream.avail_out;
			
			memcpy(m_info.m_stream.next_out, m_info.m_stream.next_in, uToCopy);
			
            m_info.m_stream.avail_in -= uToCopy;
            m_info.m_stream.avail_out -= uToCopy;
            m_info.m_stream.next_in += uToCopy;
            m_info.m_stream.next_out += uToCopy;
            m_info.m_stream.total_in += uToCopy;
            m_info.m_stream.total_out += uToCopy;
            m_info.m_uComprLeft += uToCopy;
        }
    }
	
	return true;
}

bool CZipArchive::CloseNewFile(bool bAfterException)
{
	if (m_iFileOpened != compress)
	{
		ZIPTRACE("%s(%i) : A new file must be opened.\n");
		return false;
	}
	
    m_info.m_stream.avail_in = 0;
    if (!bAfterException)
	{
		int err = Z_OK;
		if (CurrentFile()->m_uMethod == Z_DEFLATED)
			while (err == Z_OK)
			{
				if (m_info.m_stream.avail_out == 0)
				{
					if (m_pCryptograph)
						m_pCryptograph->Encode(m_info.m_pBuffer, (DWORD)m_info.m_uComprLeft);
					m_storage.Write(m_info.m_pBuffer, (DWORD)m_info.m_uComprLeft, false);
					m_info.m_uComprLeft = 0;
					m_info.m_stream.avail_out = m_info.m_pBuffer.GetSize();
					m_info.m_stream.next_out = (Bytef*)(char*)m_info.m_pBuffer;
				}
				ZIP_U32_U64 uTotal = m_info.m_stream.total_out;
				err = deflate(&m_info.m_stream,  Z_FINISH);
				m_info.m_uComprLeft += m_info.m_stream.total_out - uTotal;
			}
			
		if (err == Z_STREAM_END)
			err = Z_OK;
		CheckForError(err);
		
		if (m_info.m_uComprLeft > 0)
		{
			if (m_pCryptograph)
				m_pCryptograph->Encode(m_info.m_pBuffer, (DWORD)m_info.m_uComprLeft);
			m_storage.Write(m_info.m_pBuffer, (DWORD)m_info.m_uComprLeft, false);
		}
		
		if (CurrentFile()->m_uMethod == Z_DEFLATED)
		{
			err = deflateEnd(&m_info.m_stream);
			CheckForError(err);
		}
		
		
		// it may be increased by the encrypted header size in CZipFileHeader::PrepareData
		CurrentFile()->m_uComprSize += m_info.m_stream.total_out;
		CurrentFile()->m_uUncomprSize = m_info.m_stream.total_in;

		if (m_pCryptograph)
			m_pCryptograph->FinishEncode(*CurrentFile(), m_storage);
		
		m_centralDir.CloseNewFile();
	}
	else
		m_centralDir.m_pOpenedFile = NULL;

	m_iFileOpened = nothing;
	m_info.ReleaseBuf();
	EmptyPtrList();
	ClearCryptograph();
	if (m_bAutoFlush && !bAfterException)
		Flush();

	return true;
}

void CZipArchive::DeleteFile(ZIP_U16_U64 uIndex)
{
	CZipIndexesArray indexes;
	indexes.Add(uIndex);
	DeleteFiles(indexes);
}

void CZipArchive::GetIndexes(const CZipStringArray &aNames, CZipIndexesArray& aIndexes)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return;
	}
	ZIP_U16_U64 uSize = (ZIP_U16_U64)aNames.GetSize();
	for (ZIP_U16_U64 i = 0; i < uSize; i++)
		aIndexes.Add(FindFile(aNames[(ZIP_ARRAY_SIZE_TYPE)i], ffDefault, false));			
}

void CZipArchive::DeleteFiles(const CZipStringArray &aNames)
{
	CZipIndexesArray indexes;
	GetIndexes(aNames, indexes);
	DeleteFiles(indexes);
}

void CZipArchive::DeleteFiles(CZipIndexesArray &aIndexes)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return;
	}
	
	if (m_storage.IsSegmented())
	{
		ZIPTRACE("%s(%i) : You cannot delete files from a segmented archive.\n");
		return;
	}
	
	if (m_iFileOpened)
	{
		ZIPTRACE("%s(%i) : You cannot delete files if there is a file opened.\n");
		return;
	}
	
	CZipActionCallback* pCallback = GetCallback(cbDeleteCnt);
	if (pCallback)
		pCallback->Init();

	ZIP_U16_U64 uSize = (ZIP_U16_U64)aIndexes.GetSize();
	if (!uSize)
	{
		ZIPTRACE("%s(%i) : The indekses array is empty.\n");
		return;
	}

	ZIP_U16_U64 i;
	
	aIndexes.Sort(true);
	// remove all - that's easy so don't waste the time
	if (uSize == GetCount())
	{		
		// check that the indexes are correct
		bool allIncluded = true;
		// iterate all indexes, if all are sorted then the condition should always be true
		for (i = 0; i < uSize; i++)
			if (aIndexes[(ZIP_ARRAY_SIZE_TYPE)i] != i)
			{
				allIncluded = false;
				break;
			}

		if (allIncluded)
		{
			pCallback = GetCallback(cbDelete);
			if (pCallback)
			{
				// do it right and sent the notification
				pCallback->Init();
				pCallback->SetTotal(uSize);
			}

			m_centralDir.RemoveFromDisk();
			m_storage.m_pFile->SetLength((ZIP_FILE_USIZE) m_storage.m_uBytesBeforeZip);
			m_centralDir.RemoveAll();
			if (m_bAutoFlush)
				Flush();
			if (pCallback)
				pCallback->CallbackEnd();
			return;
		}
	}	

	CZipArray<CZipDeleteInfo> aInfo;
	
	int iStep = 0; // for the compiler
	if (pCallback)
	{
		pCallback->SetTotal(GetCount());
		iStep = CZipActionCallback::m_iStep; // we don't want to wait forever
	}
	
	ZIP_U16_U64 uDelIndex = 0;
	ZIP_U16_U64 uMaxDelIndex = aIndexes[(ZIP_ARRAY_SIZE_TYPE)(uSize - 1)];
	ZIP_U16_U64 uStart = aIndexes[0];
	for (i = uStart; i < GetCount(); i++)
	{
		CZipFileHeader* pHeader = m_centralDir[i];
		bool bDelete;
		if (i <= uMaxDelIndex && i == aIndexes[(ZIP_ARRAY_SIZE_TYPE)uDelIndex])
		{
			uDelIndex++;
			bDelete = true;
		}
		else
			bDelete = false;
		aInfo.Add(CZipDeleteInfo(pHeader, bDelete));
		if (pCallback && !(i % iStep) && i != uStart)
			if (!(*pCallback)(iStep))
				ThrowError(CZipException::abortedSafely);
	}
	ASSERT(uDelIndex == uSize);

	if (pCallback)
		pCallback->CallbackEnd();

	uSize = (ZIP_U16_U64)aInfo.GetSize();
	if (!uSize) // it is possible
		return;

	// they should already be sorted after reading the in CZipCentralDir::ReadHeaders and when replacing, the index is placed at the same place as the old one
	//aInfo.Sort(true); // sort by offsets (when operators uncommented in CZipDeleteInfo)
	
	// now we start deleting (not safe to break)
	pCallback = GetCallback(cbDelete);
	if (pCallback)
		pCallback->Init();
	
	m_centralDir.RemoveFromDisk();

	ZIP_U32_U64 uTotalToMoveBytes = 0, uLastOffset = (ZIP_U32_U64)m_storage.m_pFile->GetLength() - m_storage.m_uBytesBeforeZip;
	// count the number of bytes to move
	i = uSize - 1;
	while (true)
	{
		// we alread know it is not 0
		// cannot use a decreasing loop because i is unsigned and instead negative at the end of the loop it will be maximum positive
		const CZipDeleteInfo& di = aInfo[(ZIP_ARRAY_SIZE_TYPE)i];
		if (!di.m_bDelete)
			uTotalToMoveBytes += uLastOffset - di.m_pHeader->m_uOffset;
		uLastOffset = di.m_pHeader->m_uOffset;
		if (i == 0)
			break;
		i--;
	}
	
	if (pCallback)
		pCallback->SetTotal(uTotalToMoveBytes);

	
	m_info.Init();

	ZIP_U32_U64 uMoveBy = 0, uOffsetStart = 0;
	for (i = 0; i < uSize; i++)
	{
		const CZipDeleteInfo& di = aInfo[(ZIP_ARRAY_SIZE_TYPE)i];
		
		if (di.m_bDelete)
		{
			// next hole
			ZIP_U32_U64 uTemp = di.m_pHeader->m_uOffset;
			m_centralDir.RemoveFile(di.m_pHeader); // first remove
			if (uOffsetStart)
			{
				// copy the files over a previous holes
				MovePackedFiles(uOffsetStart, uTemp, uMoveBy, pCallback);
				uOffsetStart = 0;  // never be at the beginning, because the first file is always to be deleted
			}
			if (i == uSize - 1)
				uTemp = ((ZIP_U32_U64)m_storage.m_pFile->GetLength() - m_storage.m_uBytesBeforeZip) - uTemp;
			else
				uTemp = aInfo[(ZIP_ARRAY_SIZE_TYPE)(i + 1)].m_pHeader->m_uOffset - uTemp;

			uMoveBy += uTemp;
			
		}
		else
		{
			if (uOffsetStart == 0) // find contiuos area to move
				uOffsetStart = di.m_pHeader->m_uOffset;
			di.m_pHeader->m_uOffset -= uMoveBy;
		}

	}
	if (uOffsetStart)
		MovePackedFiles(uOffsetStart,
			(ZIP_U32_U64)m_storage.m_pFile->GetLength() - m_storage.m_uBytesBeforeZip,
			uMoveBy, pCallback); 

	m_info.ReleaseBuf();
	if (uMoveBy) // just in case
		m_storage.m_pFile->SetLength((ZIP_FILE_USIZE)(m_storage.m_pFile->GetLength() - uMoveBy));

	if (pCallback)
		pCallback->CallbackEnd();

	if (m_bAutoFlush)
		Flush();
}



bool CZipArchive::AddNewFile(LPCTSTR lpszFilePath,
                             int iComprLevel,          
                             bool bFullPath,
							 int iSmartLevel,
                             unsigned long nBufSize)
{
	
	CZipAddNewFileInfo zanfi (lpszFilePath, bFullPath);
	zanfi.m_iComprLevel = iComprLevel;
	zanfi.m_iSmartLevel = iSmartLevel;
	zanfi.m_nBufSize = nBufSize;
	return AddNewFile(zanfi);	
}

bool CZipArchive::AddNewFile(LPCTSTR lpszFilePath,
							 LPCTSTR lpszFileNameInZip,
                             int iComprLevel,                                       
							 int iSmartLevel,
                             unsigned long nBufSize)
{
	CZipAddNewFileInfo zanfi(lpszFilePath, lpszFileNameInZip);
	zanfi.m_iComprLevel = iComprLevel;
	zanfi.m_iSmartLevel = iSmartLevel;
	zanfi.m_nBufSize = nBufSize;
	return AddNewFile(zanfi);	
}

bool CZipArchive::AddNewFile(CZipMemFile& mf,
							 LPCTSTR lpszFileNameInZip,
                             int iComprLevel,                                       
							 int iSmartLevel,
                             unsigned long nBufSize)
{
	CZipAddNewFileInfo zanfi(&mf, lpszFileNameInZip);
	zanfi.m_iComprLevel = iComprLevel;
	zanfi.m_iSmartLevel = iSmartLevel;
	zanfi.m_nBufSize = nBufSize;
	return AddNewFile(zanfi);
}

bool CZipArchive::AddNewFile(CZipAddNewFileInfo& info)
{
	// no need for ASSERT and TRACE here - it will be done by OpenNewFile
	
	if (!m_info.m_iBufferSize)
		return false;

	if (info.m_pFile)
		info.m_szFilePath = info.m_pFile->GetFilePath();
	else
	{
		CZipPathComponent::RemoveSeparators(info.m_szFilePath);
		if (info.m_szFilePath.IsEmpty())
			return false;	
	}

	bool bSegm = GetSegmMode() != 0;

	// checking the replace index
	if (!UpdateReplaceIndex(info.m_uReplaceIndex))
		return false;

	bool bReplace = info.m_uReplaceIndex != ZIP_FILE_INDEX_UNSPECIFIED;
	
	DWORD uAttr;
	time_t ttime;
	if (info.m_pFile)
	{
		uAttr = ZipPlatform::GetDefaultAttributes();
		ttime = time(NULL);
	}
	else
	{
		if (!ZipPlatform::GetFileAttr(info.m_szFilePath, uAttr))
			return false; // we don't know whether it is a file or a directory
		if (!ZipPlatform::GetFileModTime(info.m_szFilePath, ttime))
			ttime = time(NULL);
	}
	CZipFileHeader header;
	
	if (ZipPlatform::GetSystemID() != ZipCompatibility::zcUnix)
		uAttr |= ZipCompatibility::ConvertToSystem(uAttr, ZipPlatform::GetSystemID(), ZipCompatibility::zcUnix);  // make it readable under Unix as well, since it stores its attributes in HIWORD(uAttr)
	SetFileHeaderAttr(header, uAttr);
	if (info.m_szFileNameInZip.IsEmpty())
		info.m_szFileNameInZip = PredictFileNameInZip(info.m_szFilePath, info.m_bFullPath, header.IsDirectory() ? prDir : prFile);
	header.SetFileName(info.m_szFileNameInZip);
	header.SetTime(ttime);
	bool bInternal = (info.m_iSmartLevel & zipsmInternal01) != 0;
	CZipActionCallback* pCallback = NULL;
	if (!bInternal)
	{
		pCallback = GetCallback(cbAdd);
		if (pCallback)
			pCallback->Init(info.m_szFileNameInZip, info.m_szFilePath);
	}

	if (header.IsDirectory()) // will never be when m_pFile is not NULL, so we don't check it
	{
		ASSERT(!info.m_pFile); // should never happened
		ASSERT(!bInternal);

		if (pCallback)
			pCallback->SetTotal(0); // in case of calling LeftToDo afterwards		

		// clear password for a directory
		CZipSmClrPass smcp;
		if (info.m_iSmartLevel & zipsmCPassDir)
			smcp.ClearPasswordSmartly(this);

		bool bRet = OpenNewFile(header, 0, NULL, info.m_uReplaceIndex);
		
		CloseNewFile();
		if (pCallback)
			pCallback->CallbackEnd();
		
		return bRet;		
	}
	
	CZipSmClrPass smcp;
	bool bIsCompression = info.m_iComprLevel != 0;
	bool bEff = (info.m_iSmartLevel & zipsmCheckForEff)&& bIsCompression;
	bool bCheckForZeroSized = (info.m_iSmartLevel & zipsmCPFile0) && !GetPassword().IsEmpty();
	bool bCheckForSmallFiles = (info.m_iSmartLevel & zipsmNotCompSmall) && bIsCompression;
	ZIP_U32_U64 uFileSize = ZIP_U32_U64(-1);
	bool bNeedTempArchive = (bEff && bSegm) || (bReplace && bIsCompression);
	if (bCheckForSmallFiles || bCheckForZeroSized || bNeedTempArchive)
	{
		
		if (info.m_pFile)
			uFileSize = (ZIP_U32_U64)info.m_pFile->GetLength();
		else
		{
			if (!ZipPlatform::GetFileSize(info.m_szFilePath, uFileSize) && bEff)
				bEff = false; // the file size is needed only when efficient in a segmented archive
		}

		if (uFileSize !=  ZIP_U32_U64(-1))
		{
			if (bCheckForZeroSized && uFileSize == 0)
				smcp.ClearPasswordSmartly(this);			
			if (bCheckForSmallFiles && uFileSize < 5)
				info.m_iComprLevel = 0;			
		}
	}
	bool bEffInMem = bEff && (info.m_iSmartLevel & zipsmMemoryFlag);
	CZipString szTempFileName;
	if (bNeedTempArchive && (bEffInMem || 
		!(szTempFileName = ZipPlatform::GetTmpFileName(
			m_szTempPath.IsEmpty() ? NULL : (LPCTSTR)m_szTempPath, uFileSize)
		).IsEmpty()))
	{
		CZipMemFile* pmf = NULL;
		CZipArchive zip;
		try
		{
			// compress first to a temporary file, if ok - copy the data, if not - add storing
			
			if (bEffInMem)
			{
				pmf = new CZipMemFile;
				zip.Open(*pmf, zipCreate);
			}
			else
				zip.Open(szTempFileName, zipCreate);
			zip.SetRootPath(m_szRootPath);
			zip.SetPassword(GetPassword());
			zip.SetEncryptionMethod(m_iEncryptionMethod);
			zip.SetSystemCompatibility(m_iArchiveSystCompatib);
			zip.SetCallback(pCallback, cbAdd);
			// create a temporary file
			ZIP_U16_U64 uTempReplaceIndex = info.m_uReplaceIndex;
			info.m_iSmartLevel = zipsmLazy;
			info.m_uReplaceIndex = ZIP_FILE_INDEX_UNSPECIFIED;
			if (!zip.AddNewFile(info))
				throw false;
			info.m_uReplaceIndex = uTempReplaceIndex;

			// this may also happen when bReplace, but not in a segmented archive
			if (bEff)
			{
				if (!(*this)[0]->CompressionEfficient())
				{
					info.m_iComprLevel = 0;
					info.m_iSmartLevel = zipsmInternal01;
					// compression is not efficient, store instead
					throw AddNewFile(info);
				}
			}
			zip.m_storage.Flush();
			m_info.Init();
			throw GetFromArchive(zip, 0, NULL, info.m_uReplaceIndex, true, GetCallback(cbAddTmp));
		}
		catch (bool bRet)
		{
			zip.Close(!bRet); // that doesn't really matter how it will be closed
			if (pmf)
				delete pmf;
			if (!bEffInMem)
				ZipPlatform::RemoveFile(szTempFileName, false);
			m_info.ReleaseBuf();
			return bRet;
		}
		catch (...)
		{
			zip.Close(true);
			if (pmf)
				delete pmf;
			if (!bEffInMem)
				ZipPlatform::RemoveFile(szTempFileName, false);
			m_info.ReleaseBuf();
			throw;
		}
	}

	// try to open before adding
	CZipFile f;
	CZipAbstractFile *pf;
	if (info.m_pFile)
	{
		pf = info.m_pFile;
		pf->SeekToBegin();
	}
	else
	{
		// cannot be shareDenyWrite		
		// If you specify the GENERIC_READ and GENERIC_WRITE access modes along with the FILE_SHARE_READ and FILE_SHARE_WRITE sharing modes in your first call to CreateFile. If you specify the GENERIC_READ and GENERIC_WRITE access modes and the FILE_SHARE_READ sharing mode only in your second call to CreateFile, the function will fail with a sharing violation because the read-only sharing mode specified in the second call conflicts with the read/write access that has been granted in the first call.
		// Original information was here (but not any longer): http://msdn.microsoft.com/library/default.asp?url=/library/en-us/fileio/base/creating_and_opening_files.asp
		if (!f.Open(info.m_szFilePath, CZipFile::modeRead | CZipFile::shareDenyNone, false))
		{
			if (pCallback)
				pCallback->CallbackEnd();
			return false;
		}
		pf = &f;
	}

	ASSERT(pf);
	// call init before opening (in case of exception we have the names)
	uFileSize = (ZIP_U32_U64)pf->GetLength();
	
	// predict sizes in local header, so that zip64 can write extra header if needed
	header.m_uLocalUncomprSize = uFileSize;	
	if (!bIsCompression)
		header.m_uLocalComprSize = uFileSize;

	bool bRet;	
	if (bReplace)
	{
		ASSERT(!bIsCompression);		
		bRet = OpenNewFile(header, 0, NULL, info.m_uReplaceIndex);
	}
	else
		bRet = OpenNewFile(header, info.m_iComprLevel);
	if (!bRet)
	{
		if (pCallback)
			pCallback->CallbackEnd();
		
		return false;
	}
	if (bInternal) 
	{
		// we do it here, because if in OpenNewFile is replacing 
		// then we get called cbReplace callback before and it would 
		// overwrite callback information written in pCallback->Init
		pCallback = GetCallback(cbAddStore);
		if (pCallback)
			pCallback->Init(info.m_szFileNameInZip, info.m_szFilePath);
	}
	if (pCallback)
		pCallback->SetTotal(uFileSize);

	CZipAutoBuffer buf(info.m_nBufSize);
	DWORD iRead;
	int iAborted = 0;
	do
	{
		iRead = pf->Read(buf, info.m_nBufSize);
		if (iRead)
		{
			WriteNewFile(buf, iRead);
			if (pCallback)
				if (!(*pCallback)(iRead))
				{
					// we could remove here the bytes of the file partially added if the archive is not segmented - but we don't.... for now
					if (iRead == buf.GetSize() && pf->Read(buf, 1) != 0) // test one byte if there is something left
					{
						if (!m_storage.IsSegmented() && !bReplace)
						{
							RemoveLast(true);							
							CloseNewFile(true);
							iAborted = CZipException::abortedSafely;
						}
						else
							iAborted = CZipException::abortedAction; 
					}
					else
					{
						iAborted = CZipException::abortedSafely; // we did it!
						CloseNewFile();
					}
					break;
				}
				
		}
		
	}
	while (iRead == buf.GetSize());
	if (!iAborted)
		CloseNewFile();
	
	if (pCallback)
		pCallback->CallbackEnd();
	
	if (iAborted)
		CZipException::Throw(iAborted); // throw to distuinguish from other return codes
	
	if (bEff)
	{
		// remove the last file and add it without the compression if needed
		if (!info.m_pFile)
			f.Close();

		buf.Release();
		if (RemoveLast())
		{
			info.m_iComprLevel = 0;
			info.m_iSmartLevel = zipsmInternal01;
			return AddNewFile(info);
		}
	}
	return true;	
	
}

bool CZipArchive::RemoveLast(bool bRemoveAnyway)
{
	if (GetCount() == 0)
		return false;
	ZIP_U16_U64 uIndex = GetCount() - 1;
	CZipFileHeader* pHeader = m_centralDir[uIndex];

	if (!bRemoveAnyway && pHeader->CompressionEfficient())
		return false;

	m_centralDir.RemoveLastFile(pHeader, uIndex);
	return true;
}




CZipString CZipArchive::GetArchivePath() const
{
	if (IsClosed(false))
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return _T("");
	}
	return m_storage.m_pFile->GetFilePath();
}

CZipString CZipArchive::GetGlobalComment() const
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return _T("");
	}
	CZipString temp;
	m_centralDir.GetComment(temp);
	return temp;
}

bool CZipArchive::SetGlobalComment(LPCTSTR lpszComment)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return false;
	}
	if (m_storage.IsSegmented() == -1)
	{
		ZIPTRACE("%s(%i) : You cannot modify the global comment of an existing segmented archive.\n");
		return false;
	}

	m_centralDir.SetComment(lpszComment);
	if (m_bAutoFlush)
		Flush();

	return true;
}



ZIP_U16_U32 CZipArchive::GetCurrentDisk() const 
{
	return m_storage.GetCurrentDisk() + 1;
}

bool CZipArchive::SetFileComment(ZIP_U16_U64 uIndex, LPCTSTR lpszComment)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return false;
	}
	if (m_storage.IsSegmented() == -1)
	{
		ZIPTRACE("%s(%i) : You cannot modify the file comment in an existing segmented archive.\n");
		return false;
	}
	
	m_centralDir.SetFileComment(uIndex, lpszComment);
	if (m_bAutoFlush)
		Flush();
	return true;
}

bool CZipArchive::SetPassword(LPCTSTR lpszPassword)
{
	if (m_iFileOpened != nothing)
	{
		ZIPTRACE("%s(%i) : You cannot change the password when a file is opened.\n");
		return false; // it's important not to change the password when the file inside archive is opened
	}
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : Setting the password for a closed archive has no effect.\n");
	}
	if (lpszPassword)
		ZipCompatibility::ConvertStringToBuffer(lpszPassword, m_pszPassword, CP_ACP);
	else
		m_pszPassword.Release();
	return true;
}

bool CZipArchive::SetEncryptionMethod(int iEncryptionMethod)
{
	if (m_iFileOpened == compress)
	{
		ZIPTRACE("%s(%i) : You cannot change the encryption method when a file is opened for compression.\n");
		return false;
	}	
	if (!CZipCryptograph::IsEncryptionSupported(iEncryptionMethod))
		return false;
	m_iEncryptionMethod = iEncryptionMethod;
	return true;
}

CZipString CZipArchive::GetPassword()const 
{
	CZipString temp;
	ZipCompatibility::ConvertBufferToString(temp, m_pszPassword, CP_ACP);
	return temp;
}

bool CZipArchive::TestFile(ZIP_U16_U64 uIndex, DWORD uBufSize)
{
	if (m_storage.IsSegmented() == 1)
	{
		ZIPTRACE("%s(%i) : You cannot test a segmented archive in creation.\n");
		return false;
	}
	if (!uBufSize)
		return false;
	
	CZipFileHeader* pHeader = m_centralDir[uIndex];
	CZipActionCallback* pCallback = GetCallback(cbTest);
	if (pCallback)
	{
		pCallback->Init(pHeader->GetFileName());
	}

	if (pHeader->IsDirectory())
	{
		if (pCallback)
			pCallback->SetTotal(0);

		// we do not test whether the password for the encrypted directory
		// is correct, since it seems to be senseless (anyway password 
		// encrypted directories should be avoided - it adds 12 bytes)
		ZIP_U32_U64 uSize = pHeader->m_uComprSize;
		if ((uSize != 0 || uSize != pHeader->m_uUncomprSize)
			// different treating compressed directories
			&& !(pHeader->IsEncrypted() && uSize == 12 && !pHeader->m_uUncomprSize))
			CZipException::Throw(CZipException::dirWithSize);

		if (pCallback)
			pCallback->CallbackEnd();

		return true;
	}
	else
	{
		try
		{			
			if (pCallback)
				pCallback->SetTotal(pHeader->m_uUncomprSize);
			
			if (!OpenFile(uIndex))
				return false;
			CZipAutoBuffer buf(uBufSize);
			DWORD iRead;
			int iAborted = 0;
			do
			{	
				iRead = ReadFile(buf, buf.GetSize());
				if (pCallback && iRead)
					if (!(*pCallback)(iRead))
					{
						if (iRead == buf.GetSize() && ReadFile(buf, 1) != 0) // test one byte if there is something left
							iAborted = CZipException::abortedAction; 
						else
							iAborted = CZipException::abortedSafely; // we did it!
						break;
					}
			}
			while (iRead == buf.GetSize());
			bool bRet = CloseFile() != -1;
			if (!bRet && iAborted == CZipException::abortedSafely)
				iAborted = CZipException::abortedAction; // sorry, finished, but not successfull

			if (pCallback)
				pCallback->CallbackEnd();

			if (iAborted)
				CZipException::Throw(iAborted); // throw to distuingiush from other return codes
			if (bRet)
				return true;
			else
				CZipException::Throw(CZipException::badZipFile);
			return false; // to satisfy some compilers (some will complain)...
		}
		catch(...)
		{
			CloseFile(NULL, true);
			throw;
		}
	}
}

void CZipArchive::EmptyPtrList()
{
	if (m_list.GetCount())
	{
		// if some memory hasn't been freed due to an error in zlib, so free it now
		CZipPtrListIter iter = m_list.GetHeadPosition();
		while (m_list.IteratorValid(iter))
			delete[] (char*) m_list.GetNext(iter);
	}
	m_list.RemoveAll();
}



void CZipArchive::SetFileHeaderAttr(CZipFileHeader& header, DWORD uAttr)const
{
	header.SetSystemCompatibility(m_iArchiveSystCompatib);
	header.SetSystemAttr(uAttr);
}

void CZipArchive::EnableFindFast(bool bEnable)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : Set it after opening the archive.\n");
		return;
	}
	m_centralDir.EnableFindFast(bEnable, m_bCaseSensitive);

}

bool CZipArchive::SetSystemCompatibility(int iSystemComp)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : Set it after opening the archive.\n");
		return false;
	}

	if (m_iFileOpened == compress)
	{
		ZIPTRACE("%s(%i) : Set it before opening a file inside archive.\n");
		return false;
	}

	if (!ZipCompatibility::IsPlatformSupported(iSystemComp))
		return false;
	// change the name coding page, if it was not changed before
	if (m_centralDir.m_stringSettings.IsStandardNameCodePage(m_iArchiveSystCompatib))
		m_centralDir.m_stringSettings.SetDefaultNameCodePage(iSystemComp);	
	m_iArchiveSystCompatib = iSystemComp;
	
	return true;
}

void CZipArchive::SetRootPath(LPCTSTR szPath)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : Set it after opening the archive.\n");
		return;
	}

	if (m_iFileOpened != nothing)
	{
		ZIPTRACE("%s(%i) : Set it before opening a file inside archive.\n");
		return;
	}

	if (szPath)
	{
		m_szRootPath = szPath;
		CZipPathComponent::RemoveSeparators(m_szRootPath);
	}
	else
		m_szRootPath.Empty();
}

CZipString CZipArchive::TrimRootPath(CZipPathComponent &zpc)const 
{
	if (m_szRootPath.IsEmpty())
		return zpc.GetFileName();
	CZipString szPath = zpc.GetFullPath();
	return RemovePathBeginning(m_szRootPath, szPath, m_pZipCompare) ? szPath : zpc.GetFileName();
}

bool CZipArchive::RemovePathBeginning(LPCTSTR lpszBeginning, CZipString& szPath, ZIPSTRINGCOMPARE pCompareFunction)
{
	CZipString szBeginning(lpszBeginning);
	CZipPathComponent::RemoveSeparators(szBeginning);
	int iRootPathLength = szBeginning.GetLength();
	if (iRootPathLength && szPath.GetLength() >= iRootPathLength &&
		(szPath.Left(iRootPathLength).*pCompareFunction)(szBeginning) == 0)
	{		
		// the beginning is the same
		if (szPath.GetLength() == iRootPathLength)
		{
			szPath.Empty();
			return true;
		}
		// is the end of m_szPathRoot only a beginning of a directory name? 
		// check for a separator
		// we know the length is larger, so we can write:
		if (CZipPathComponent::IsSeparator(szPath[iRootPathLength]))
		{			
			szPath = szPath.Mid(iRootPathLength);
			CZipPathComponent::RemoveSeparatorsLeft(szPath);
			return true;
		}
	}
	return false;
}

void CZipArchive::SetTempPath(LPCTSTR lpszPath, bool bForce)
{
	m_szTempPath = lpszPath;
	if (lpszPath && bForce)
		ZipPlatform::ForceDirectory(lpszPath);
	CZipPathComponent::RemoveSeparators(m_szTempPath);
}

CZipString CZipArchive::PredictFileNameInZip(LPCTSTR lpszFilePath, 
						 bool bFullPath, int iWhat)const 
{
	CZipString sz = lpszFilePath;
	if (sz.IsEmpty())
		return _T("");
	bool bAppend;
	switch (iWhat)
	{
	case prFile:
		bAppend = false;
		break;
	case prDir:
		bAppend = true;
		break;
	default:
		bAppend = CZipPathComponent::IsSeparator(sz[sz.GetLength() - 1]);
	}
	
	// remove for CZipPathComponent treating last name as a file even if dir
	CZipPathComponent::RemoveSeparators(sz);
	// it may be empty after removing separators, e.g.: "/"
	if (sz.IsEmpty())
		return _T("");
	CZipPathComponent zpc(sz);

	if (bFullPath)
	{
		if (m_bRemoveDriveLetter)
			sz = zpc.GetNoDrive();
	}
	else
		sz = TrimRootPath(zpc);

	if (bAppend && !sz.IsEmpty())
		CZipPathComponent::AppendSeparator(sz);
	return sz;	
}

CZipString CZipArchive::PredictExtractedFileName(LPCTSTR lpszFileNameInZip, LPCTSTR lpszPath, bool bFullPath, LPCTSTR lpszNewName)const 
{
	CZipString szFile = lpszPath;
	CZipString sz = lpszNewName ? lpszNewName : lpszFileNameInZip;
	if (sz.IsEmpty())
		return szFile;
	if (!szFile.IsEmpty())
		CZipPathComponent::AppendSeparator(szFile);
	
	
	// remove for CZipPathComponent treating last name as a file even if dir
	CZipPathComponent::RemoveSeparators(sz);
	CZipPathComponent zpc(sz);
	szFile += bFullPath ? (m_bRemoveDriveLetter ? zpc.GetNoDrive() : sz) 
						  : TrimRootPath(zpc);	
	return szFile;
}

ZIP_U32_U64 CZipArchive::PredictMaximumFileSizeInArchive(LPCTSTR lpszFilePath, bool bFullPath) const
{
	DWORD attr;
	if (!ZipPlatform::GetFileAttr(lpszFilePath, attr))
		return 0;		
	CZipFileHeader fh;		
	SetFileHeaderAttr(fh, attr);
	if (!fh.IsDirectory())
		if (!ZipPlatform::GetFileSize(lpszFilePath, fh.m_uUncomprSize))
			return 0;	
	fh.SetFileName(PredictFileNameInZip(lpszFilePath, bFullPath, fh.IsDirectory() ? prDir  : prFile));
	return PredictMaximumFileSizeInArchive(fh);
}

ZIP_U32_U64 CZipArchive::PredictMaximumFileSizeInArchive(CZipFileHeader& fh) const
{
	fh.m_stringSettings = m_centralDir.m_stringSettings;
	fh.PrepareData(Z_DEFAULT_COMPRESSION, !GetPassword().IsEmpty(), m_storage.IsSegmented() != 0);
	DWORD uLocalSize = fh.GetLocalSize(true);
	return fh.GetSize() + uLocalSize + fh.GetDataSize(true, false) + fh.GetDataDescriptorSize(&m_storage);
}

void CZipArchive::SetAutoFlush(bool bAutoFlush)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive not yet opened.\n");
		return;
	}
	if (m_storage.IsSegmented() != 0)
	{
		ZIPTRACE("%s(%i) : Cannot set auto-flush for a segmented archive.\n");
		return;
	}
	m_bAutoFlush = bAutoFlush;
}

void CZipArchive::Flush()
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive not yet opened.\n");
		return;
	}

	if (m_storage.IsSegmented() < 0)
	{
		ZIPTRACE("%s(%i) : Cannot flush an existing segmented archive.\n");
		return;
	}
	WriteCentralDirectory();
	m_storage.FlushFile();
	if (m_storage.IsSegmented() > 0) // try to finalize a segmented archive without closing it
		m_storage.FinalizeSegm();
}


void CZipArchive::GetCentralDirInfo(CZipCentralDir::Info& info)const
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive not yet opened.\n");
		return;
		
	}
	m_centralDir.GetInfo(info);
	if (GetSegmMode() > 0)
		info.m_uDiskEntriesNo = m_storage.GetCurrentDisk();
}

bool CZipArchive::CWildcard::IsPatternValid(LPCTSTR lpszPattern, int* iErrorType)
{
	try
	{
		/* loop through pattern to EOS */
		while (*lpszPattern)
		{
			/* determine pattern type */
			switch (*lpszPattern)
			{
				/* check literal escape, it cannot be at end of pattern */
			case _T('\\'):
				if (!*++lpszPattern)
					throw patternEsc;
				lpszPattern++;
				break;
				
				/* the [..] construct must be well formed */
			case _T('['):
				lpszPattern++;
				
				/* if the next character is ']' then bad pattern */
				if (*lpszPattern == _T(']'))
					throw patternEmpty;
				
				/* if end of pattern here then bad pattern */
				if (!*lpszPattern)
					throw patternClose;
				
				/* loop to end of [..] construct */
				while (*lpszPattern != _T(']'))
				{
					/* check for literal escape */
					if (*lpszPattern == _T('\\'))
					{
						lpszPattern++;
						
						/* if end of pattern here then bad pattern */
						if (!*lpszPattern++)
							throw patternEsc;
					}
					else  lpszPattern++;
					
					/* if end of pattern here then bad pattern */
					if (!*lpszPattern)
						throw patternClose;
					
					/* if this a range */
					if (*lpszPattern == _T('-'))
					{
						/* we must have an end of range */
						if (!*++lpszPattern || *lpszPattern == ']')
							throw patternRange;
						else
						{
							
							/* check for literal escape */
							if (*lpszPattern == _T('\\'))
								lpszPattern++;
							
								/* if end of pattern here
							then bad pattern           */
							if (!*lpszPattern++)
								throw patternEsc;
						}
					}
				}
				break;
				
				/* all other characters are valid pattern elements */
			case '*':
			case '?':
			default:
				lpszPattern++;                              /* "normal" character */
				break;
			}
		}
		throw patternValid;
	}
	catch (int i)
	{
		if (iErrorType)
			*iErrorType = i;
		return i == patternValid;	
	}
	
	
}

bool CZipArchive::CWildcard::IsPattern(LPCTSTR lpszPattern)
{
	while (*lpszPattern)
	{
        switch (*lpszPattern++)
        {
        case _T('?'):
        case _T('*'):
        case _T('['):
        case _T('\\'):
			return true;
        }
	}
	return false;
	
}

bool CZipArchive::CWildcard::IsMatch(LPCTSTR lpszText, int *iRetCode)
{
	CZipString sz;
	if (!m_bCaseSensitive)
	{
		sz = lpszText;
		sz.MakeLower();
		lpszText = (LPCTSTR)sz;
	}
	int i = Match((LPCTSTR)m_szPattern, lpszText);
	if (iRetCode)
		*iRetCode = i;
	return i == matchValid;
}

int CZipArchive::CWildcard::MatchAfterStar(LPCTSTR p, LPCTSTR t)
{
      int iMatch = matchNone;
      TCHAR nextp;

      /* pass over existing ? and * in pattern */

      while ( *p == _T('?') || *p == _T('*') )
      {
            /* take one char for each ? and + */

            if (*p == _T('?'))
            {
                  /* if end of text then no match */
                  if (!*t++)
                        return matchAbort;
            }

            /* move to next char in pattern */

            p++;
      }

      /* if end of pattern we have matched regardless of text left */

      if (!*p)
            return matchValid;

      /* get the next character to match which must be a literal or '[' */

      nextp = *p;
      if (nextp == _T('\\'))
      {
            nextp = p[1];

            /* if end of text then we have a bad pattern */

            if (!nextp)
                  return matchPattern;
      }

      /* Continue until we run out of text or definite result seen */

      do
      {
            /* a precondition for matching is that the next character
               in the pattern match the next character in the text or that
               the next pattern char is the beginning of a range.  Increment
               text pointer as we go here */

            if (nextp == *t || nextp == _T('['))
                  iMatch = Match(p, t);

			/* try finding another precondition */
			if (iMatch == matchPattern)
				iMatch = matchNone;
            /* if the end of text is reached then no iMatch */

            if (!*t++)
                  iMatch = matchAbort;

      } while ( iMatch != matchValid && 
                iMatch != matchAbort);

      /* return result */

      return iMatch;
}


int CZipArchive::CWildcard::Match(LPCTSTR lpszPattern, LPCTSTR lpszText)
{
	TCHAR range_start, range_end;  /* start and end in range */	
	bool bInvert;             /* is this [..] or [!..] */
	bool bMemberMatch;       /* have I matched the [..] construct? */
	bool bLoop;               /* should I terminate? */
	
	for ( ; *lpszPattern; lpszPattern++, lpszText++)
	{
	/* if this is the end of the text
		then this is the end of the match */
		
		if (!*lpszText)
		{
			if ( *lpszPattern == _T('*') && *++lpszPattern == _T('\0') )
				return matchValid;
			else
				return matchAbort;
		}
		
		/* determine and react to pattern type */
		
		switch (*lpszPattern)
		{
		case _T('?'):                     /* single any character match */
			break;
			
		case _T('*'):                     /* multiple any character match */
			return MatchAfterStar (lpszPattern, lpszText);
			
			/* [..] construct, single member/exclusion character match */
		case _T('['):
			{
				/* move to beginning of range */
				
				lpszPattern++;
				
				/* check if this is a member match or exclusion match */
				
				bInvert = false;
				if (*lpszPattern == _T('!') || *lpszPattern == _T('^'))
				{
					bInvert = true;
					lpszPattern++;
				}
				
				/* if closing bracket here or at range start then we have a
				malformed pattern */
				
				if (*lpszPattern == _T(']'))
					return matchPattern;
				
				bMemberMatch = false;
				bLoop = true;
				
				while (bLoop)
				{
					/* if end of construct then bLoop is done */
					
					if (*lpszPattern == _T(']'))
					{
						bLoop = false;
						continue;
					}
					
					/* matching a '!', '^', '-', '\' or a ']' */
					
					if (*lpszPattern == _T('\\'))
						range_start = range_end = *++lpszPattern;
					else  
						range_start = range_end = *lpszPattern;
					
					/* if end of pattern then bad pattern (Missing ']') */
					
					if (!*lpszPattern)
						return matchPattern;
					
					/* check for range bar */
					if (*++lpszPattern == _T('-'))
					{
						/* get the range end */
						
						range_end = *++lpszPattern;
						
						/* if end of pattern or construct
						then bad pattern */
						
						if (range_end == _T('\0') || range_end == _T(']'))
							return matchPattern;
						/* special character range end */
						if (range_end == _T('\\'))
						{
							range_end = *++lpszPattern;
							
							/* if end of text then
							we have a bad pattern */
							if (!range_end)
								return matchPattern;
						}
						
						/* move just beyond this range */
						lpszPattern++;
					}
					
					/* if the text character is in range then match found.
					make sure the range letters have the proper
					relationship to one another before comparison */
					
					if (range_start < range_end)
					{
						if (*lpszText >= range_start && *lpszText <= range_end)
						{
							bMemberMatch = true;
							bLoop = false;
						}
					}
					else
					{
						if (*lpszText >= range_end && *lpszText <= range_start)
						{
							bMemberMatch = true;
							bLoop = false;
						}
					}
				}
				
				/* if there was a match in an exclusion set then no match */
				/* if there was no match in a member set then no match */
				
				if ((bInvert && bMemberMatch) || !(bInvert || bMemberMatch))
					return matchRange;
				
				/* if this is not an exclusion then skip the rest of
				the [...] construct that already matched. */
				
				if (bMemberMatch)
				{
					while (*lpszPattern != _T(']'))
					{
						/* bad pattern (Missing ']') */
						if (!*lpszPattern)
							return matchPattern;
						
						/* skip exact match */
						if (*lpszPattern == _T('\\'))
						{
							lpszPattern++;
							
							/* if end of text then
							we have a bad pattern */
							
							if (!*lpszPattern)
								return matchPattern;
						}
						
						/* move to next pattern char */
						
						lpszPattern++;
					}
				}
				break;
			}
			case _T('\\'):  /* next character is quoted and must match exactly */
				
				/* move pattern pointer to quoted char and fall through */
				
				lpszPattern++;
				
				/* if end of text then we have a bad pattern */
				
				if (!*lpszPattern)
					return matchPattern;
				
				/* must match this character exactly */
				
			default:
				if (*lpszPattern != *lpszText)
					return matchPattern;
			}
	  }
	  /* if end of text not reached then the pattern fails */
	
		if (*lpszText)
			return matchEnd;
		else  
			return matchValid;
}

void CZipArchive::FindMatches(LPCTSTR lpszPattern, CZipIndexesArray &ar, bool bFullPath)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return;
	}

	//	ar.RemoveAll(); don't do this
	ZIP_U16_U64 uCount = (ZIP_U16_U64)GetCount();
	CWildcard wc(lpszPattern, m_bCaseSensitive);
	for (ZIP_U16_U64 i = 0; i < uCount; i++)
	{
		CZipString sz = m_centralDir[i]->GetFileName();
		if (!bFullPath)
		{
			CZipPathComponent::RemoveSeparators(sz);
			CZipPathComponent zpc(sz);			
			sz = zpc.GetFileName();
		}
		if (wc.IsMatch(sz))
			ar.Add(i);
	}
}

ZIP_U16_U64 CZipArchive::WillBeDuplicated(LPCTSTR lpszFilePath, bool bFullPath, bool bFileNameOnly , int iWhat)
{
	CZipString szFile;
	if (bFileNameOnly)
	{
		CZipPathComponent zpc(lpszFilePath);
		szFile = PredictFileNameInZip(zpc.GetFileName(), false, iWhat); 
	}
	else
		szFile = PredictFileNameInZip(lpszFilePath, bFullPath, iWhat);
	return FindFile(szFile, ffDefault, bFileNameOnly);
}


bool CZipArchive::GetFromArchive(CZipArchive& zip, ZIP_U16_U64 uIndex, LPCTSTR lpszNewFileName, ZIP_U16_U64 uReplaceIndex, bool bKeepSystComp, CZipActionCallback* pCallback)
{
	if (this == &zip)
	{
		ZIPTRACE("%s(%i) : You cannot get files from the same archive.\n");
		return false;
	}

	if (IsClosed() || zip.IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return false;
	}
	
	if (m_iFileOpened || zip.m_iFileOpened)
	{
		ZIPTRACE("%s(%i) : You cannot get files from another archive if there is a file opened.\n");
		return false;
	}
	
	if (zip.m_storage.IsSegmented())
	{
		ZIPTRACE("%s(%i) : You cannot get files from a segmented archive.\n");
		return false;
	}

	if (m_storage.IsSegmented() == -1)
	{
		ZIPTRACE("%s(%i) : You cannot add files to an existing segmented archive.\n");
		return false;
	}

	ASSERT(m_info.m_pBuffer.GetSize() > 0);

	bool bSegm = m_storage.IsSegmented() == 1;

	if (!zip.m_centralDir.IsValidIndex(uIndex))
		return false;
	// update sizes of local filename and extra field - they may differ from the ones in the central directory
	zip.m_centralDir.OpenFile(uIndex);
	// skip checking the data descriptor, we are not there yet
	zip.m_centralDir.CloseFile(true);

	CZipFileHeader originalHeader;
	// we need a copy - we are going to modify this
	if (!zip.GetFileInfo(originalHeader, uIndex))
		return false;
	CZipAbstractFile* pFile = zip.m_storage.m_pFile;
	
	// we need the real local header size, we opened and closed the file before to update the local information
	ZIP_U32_U64 uStartOffset = zip.m_storage.m_uBytesBeforeZip + originalHeader.m_uOffset + originalHeader.GetLocalSize(true); 
	ZIP_U32_U64 uEndOffset = uStartOffset + originalHeader.m_uComprSize;

	ZIP_U32_U64 uTotalToMove = uEndOffset - uStartOffset, uTotalMoved = 0;			

	bool bConvertSystem = !bKeepSystComp && originalHeader.GetSystemCompatibility() != m_iArchiveSystCompatib;
	CZipString szFileName;
	if (lpszNewFileName != NULL)
	{
		szFileName = lpszNewFileName;
		originalHeader.SetFileName(lpszNewFileName);
	}
	else
		szFileName = originalHeader.GetFileName(); // empty

	if (bConvertSystem)
	{
		DWORD uAttr = originalHeader.GetSystemAttr();
		originalHeader.SetSystemCompatibility(m_iArchiveSystCompatib);
		originalHeader.SetSystemAttr(uAttr);
	}
	
	if (!UpdateReplaceIndex(uReplaceIndex))
		return false;

	bool bReplace = uReplaceIndex != ZIP_FILE_INDEX_UNSPECIFIED;
	if (bReplace && bSegm)
		return false;

	int iCallbackType = 0;
	if (pCallback)
		iCallbackType = pCallback->m_iType;

	// if the same callback is applied to cbReplace, then the previous information about the type will be lost
	CZipFileHeader* pHeader = m_centralDir.AddNewFile(originalHeader, uReplaceIndex, originalHeader.GetCompressionLevel(), originalHeader.IsEncrypted(), true);
	pHeader->m_stringSettings.m_uCommentCodePage = originalHeader.m_stringSettings.m_uCommentCodePage;
	
	// prepare this here: it will be used for GetLocalSize and WriteLocal
	pHeader->PrepareFileName();

	if (bReplace)
		MakeSpaceForReplace(uReplaceIndex, uTotalToMove + pHeader->GetLocalSize(false) + pHeader->GetDataDescriptorSize(&m_storage), szFileName);

	if (pCallback)
	{
		pCallback->m_iType = iCallbackType;
		pCallback->Init(szFileName, zip.GetArchivePath());
		pCallback->SetTotal(pHeader->m_uComprSize);
	}

	// must be written as not converted
	pHeader->WriteLocal(&m_storage); 

	// skip reading the local file header	
	pFile->Seek((ZIP_FILE_USIZE)uStartOffset);

	ZIP_U32_U64 uPack = uTotalToMove > m_info.m_pBuffer.GetSize() ? m_info.m_pBuffer.GetSize() : uTotalToMove;
	char* buf = (char*)m_info.m_pBuffer;

	DWORD size_read;
	
	int iAborted = 0;
	bool bBreak = false;
	if (uPack)
		do
		{
			size_read = (UINT)pFile->Read(buf, (UINT)uPack);
			if (!size_read)
				break;
			if (uTotalMoved + size_read > uTotalToMove)
			{
				size_read = (DWORD)(uTotalToMove - uTotalMoved);
				if (!size_read)  // this is for protection
					break;
				bBreak = true;
			}
			
			m_storage.Write(buf, size_read, false);
			uTotalMoved += size_read;
			if (pCallback)
				if (!(*pCallback)(size_read))
				{												
					if (uTotalToMove != uTotalMoved) 
					{							
						if (!bSegm && !bReplace)
						{							
							m_centralDir.RemoveLastFile();
							iAborted = CZipException::abortedSafely;
						}
						else
							iAborted = CZipException::abortedAction; 
					}
					else
						iAborted = CZipException::abortedSafely;
					break;
					
				}
		}
		while (!bBreak);

	if (iAborted)
		CZipException::Throw(iAborted); // throw to distinguish from other return codes					
		
	m_centralDir.m_pOpenedFile = NULL;
	// copying from a not segmented to a segmented archive so add the data descriptor

	// we want write the descriptor only if everything is all right, but we don't want to flush the storage before 
	// (and we want to flush the storage before throwing an exception, if something is wrong)
	if (uTotalMoved == uTotalToMove)
		// it will be written only if needed
		pHeader->WriteDataDescriptor(&m_storage);

	m_storage.Flush();
	if (uTotalMoved < uTotalToMove)
		ThrowError(CZipException::badZipFile);

	if (pCallback)
		pCallback->CallbackEnd();
				
	return true;
}

bool CZipArchive::GetFromArchive(CZipArchive& zip, CZipIndexesArray &aIndexes, bool bKeepSystComp)
{
	aIndexes.Sort(true);
	ZIP_U16_U64 uFiles = (ZIP_U16_U64)aIndexes.GetSize();		
	m_info.Init();
	try
	{
		for (ZIP_U16_U64 i = 0; i < uFiles; i++)
		{
			ZIP_U16_U64 uFileIndex = aIndexes[(ZIP_ARRAY_SIZE_TYPE)i];
			if (!m_centralDir.IsValidIndex(uFileIndex))
				if (!GetFromArchive(zip, uFileIndex, NULL, ZIP_FILE_INDEX_UNSPECIFIED, bKeepSystComp, GetCallback(cbGet)))
			{
				m_info.ReleaseBuf();
				return false;
			}
		}
	}
	catch (...)
	{
		m_info.ReleaseBuf();
		throw;
	}
	m_info.ReleaseBuf();
	if (m_bAutoFlush)
		Flush();
	return true;
}
bool CZipArchive::RenameFile(ZIP_U16_U64 uIndex, LPCTSTR lpszNewName)
{
	if (IsClosed())
	{
		ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
		return false;
	}
	
	if (m_storage.IsSegmented())
	{
		ZIPTRACE("%s(%i) : You cannot rename files in a segmented archive.\n");
		return false;
	}
	
	if (m_iFileOpened)
	{
		ZIPTRACE("%s(%i) : You cannot rename a file if there is a file opened.\n");
		return false;
	}
	CZipFileHeader* pHeader = (*this)[uIndex];
	if (pHeader == NULL)
		return false;

	CZipString szNewName(lpszNewName);
	if (pHeader->IsDirectory())
		CZipPathComponent::AppendSeparator(szNewName);
	else
		CZipPathComponent::RemoveSeparators(szNewName);
	CZipString szPreviousFileName = pHeader->GetFileName();
	if (szPreviousFileName.Collate(szNewName) == 0)
		return true;
	
	// don't copy the comment code page, it already could have been set using it's own code page (set CD CP, change comment, change CD CP)
	pHeader->m_stringSettings.m_bStoreNameInExtraData = m_centralDir.m_stringSettings.m_bStoreNameInExtraData;
	pHeader->m_stringSettings.m_uNameCodePage = m_centralDir.m_stringSettings.m_uNameCodePage;
	pHeader->SetFileName(szNewName);

	m_centralDir.RemoveFromDisk(); // does m_storage.Flush();
	// read local data - it may differ from central data
	char localInfo[4];
	m_storage.Seek(pHeader->m_uOffset + 26);
	m_storage.m_pFile->Read(localInfo, 4); // read at once
	WORD uFileNameLen, uExtraFieldSize;
	CBytesWriter::ReadBytes(&uFileNameLen, localInfo, 2);
	// skip endian issues - the variable will not be used, but written back as it is
	memcpy(&uExtraFieldSize, localInfo + 2, 2);
	
	// filename buffer already cleared (GetFileName), the conversion will take place
	pHeader->PrepareFileName();
	ASSERT(pHeader->m_pszFileNameBuffer.IsAllocated());
	WORD uNewFileNameLen = (WORD)pHeader->m_pszFileNameBuffer.GetSize();
	int iDelta = uNewFileNameLen - uFileNameLen;
	int iOffset = 0;
	CZipAutoBuffer buf, *pBuf;
	
	if (iDelta != 0)
	{
		// we need to make more or less space		
		m_info.Init();
		ZIP_U32_U64 uStartOffset = pHeader->m_uOffset + 30 + uFileNameLen;
		ZIP_U32_U64 uFileLen = (ZIP_U32_U64)m_storage.m_pFile->GetLength();
		ZIP_U32_U64 uEndOffset = uFileLen - m_storage.m_uBytesBeforeZip;
		CZipActionCallback* pCallback = GetCallback(cbRename);
		if (pCallback)
		{
			// do it right and sent the notification
			pCallback->Init(szPreviousFileName, GetArchivePath());
			pCallback->SetTotal(uEndOffset - uStartOffset);
		}
		bool bForward = iDelta > 0;
		if (bForward)
			m_storage.m_pFile->SetLength((ZIP_FILE_USIZE)(uFileLen + iDelta)); // ensure the seek is correct

		MovePackedFiles(uStartOffset, uEndOffset, abs(iDelta), pCallback, bForward);
		if (pCallback)
			pCallback->CallbackEnd();

		if (!bForward)
			m_storage.m_pFile->SetLength((ZIP_FILE_USIZE)(uFileLen + iDelta)); // delta < 0; shrink the file

		m_info.ReleaseBuf();	

		ZIP_U16_U64 uSize = (ZIP_U16_U64)GetCount();
		for (ZIP_U16_U64 i = uIndex + 1; i < uSize; i++)
			m_centralDir[i]->m_uOffset += iDelta;
		buf.Allocate(4 + uNewFileNameLen);
		CBytesWriter::WriteBytes(buf, &uNewFileNameLen, 2);
		// the variable was not used - we write it back as it was (no endian issues)
		// to write everything at once
		memcpy(buf + 2, &uExtraFieldSize, 2);
		memcpy(buf + 4, pHeader->m_pszFileNameBuffer, uNewFileNameLen);	
		pBuf = &buf;
		iOffset = -4;
	}
	else
		pBuf = &pHeader->m_pszFileNameBuffer;
	
	m_storage.Seek(pHeader->m_uOffset + 30 + iOffset);
	m_storage.m_pFile->Write(*pBuf, pBuf->GetSize());
	m_centralDir.RebuildFindFastArray();
	if (m_bAutoFlush)
		Flush();
	
	return true;
}

bool CZipArchive::UpdateReplaceIndex(ZIP_U16_U64& uReplaceIndex)
{
	if (uReplaceIndex == ZIP_FILE_INDEX_UNSPECIFIED)
		return true;

	if (GetSegmMode() != 0)
	{
		ZIPTRACE("%s(%i) : You cannot replace files in a segmented archive.\n");		
		return false;
	}
	
	if (!m_centralDir.IsValidIndex(uReplaceIndex))
	{
		ZIPTRACE("%s(%i) : Not valid replace index.\n");
		return false;
	}
	if (uReplaceIndex == GetCount() - 1) // replacing last file in the archive
	{
		RemoveLast(true);
		uReplaceIndex = ZIP_FILE_INDEX_UNSPECIFIED;
	}
	return true;	
}

void CZipArchive::MakeSpaceForReplace(ZIP_U16_U64 uReplaceIndex, ZIP_U32_U64 uTotal, LPCTSTR lpszFileName)
{		
	ASSERT(uReplaceIndex < GetCount() - 1);
	// we can't use the offset from the central directory here, because the header is already replaced
	ZIP_U32_U64 uReplaceStart = (ZIP_U32_U64)m_storage.m_pFile->GetPosition() - m_storage.m_uBytesBeforeZip;
	
	// find the next offset (files in the central directory may not necessarily be ordered by offset)
	ZIP_U32_U64 uReplaceEnd = ZIP_U32_U64(-1);
	ZIP_U16_U64 i;
	for (i = 0; i < m_centralDir.m_headers.GetSize(); i++)
		if (i != uReplaceIndex)
		{
			ZIP_U32_U64 uOffset = m_centralDir.m_headers[(ZIP_ARRAY_SIZE_TYPE)i]->m_uOffset;
			if (uOffset > uReplaceStart && uOffset < uReplaceEnd)
				uReplaceEnd = uOffset;
		}
		
	ZIP_U32_U64 uReplaceTotal = uReplaceEnd - uReplaceStart;
	if (uTotal == uReplaceTotal)
		return;

	bool bForward = uTotal > uReplaceTotal;
	ZIP_U32_U64 uDelta;
	if (bForward)
		uDelta = uTotal - uReplaceTotal;
	else
		uDelta = uReplaceTotal - uTotal;
	
	
	//	m_info.Init(); don't - the calling functions will
	CZipActionCallback* pCallback = GetCallback(CZipArchive::cbReplace);
	ZIP_U32_U64 uFileLen = (ZIP_U32_U64)m_storage.m_pFile->GetLength();
	ZIP_U32_U64 uUpperLimit = uFileLen - m_storage.m_uBytesBeforeZip; // will be added in m_storage.Seek
	if (pCallback)
	{
		pCallback->Init(lpszFileName, GetArchivePath());
		pCallback->SetTotal(uUpperLimit - uReplaceEnd);
	}			
		
	if (bForward)
		m_storage.m_pFile->SetLength((ZIP_FILE_USIZE)(uFileLen + uDelta)); // ensure the seek is correct

	MovePackedFiles(uReplaceEnd, uUpperLimit, uDelta, pCallback, bForward);

	if (!bForward)
		m_storage.m_pFile->SetLength((ZIP_FILE_USIZE)(uFileLen - uDelta));

	m_storage.Seek(uReplaceStart);
	ZIP_U16_U64 uSize = (ZIP_U16_U64)m_centralDir.m_headers.GetSize();
	for (i = uReplaceIndex + 1; i < uSize; i++)
	{
		ZIP_U32_U64 uOffset = m_centralDir.m_headers[(ZIP_ARRAY_SIZE_TYPE)i]->m_uOffset;
		m_centralDir.m_headers[(ZIP_ARRAY_SIZE_TYPE)i]->m_uOffset = bForward ? uOffset + uDelta : uOffset - uDelta;
	}
	if (pCallback)
		pCallback->CallbackEnd();			
}

void CZipArchive::MovePackedFiles(ZIP_U32_U64 uStartOffset, ZIP_U32_U64 uEndOffset, ZIP_U32_U64 uMoveBy, CZipActionCallback* pCallback, bool bForward)
{
	ASSERT(m_info.m_pBuffer.GetSize() > 0);
	
	ZIP_U32_U64 uTotalToMove = uEndOffset - uStartOffset;
	ZIP_U32_U64 uPack = uTotalToMove > m_info.m_pBuffer.GetSize() ? m_info.m_pBuffer.GetSize() : uTotalToMove;
	char* buf = (char*)m_info.m_pBuffer;
	
	
	UINT size_read;
	bool bBreak = false;
	do
	{
		
		if (uEndOffset - uStartOffset < uPack)
		{
			uPack = uEndOffset - uStartOffset;
			if (!uPack)
				break;
			bBreak = true;
			
		}
		ZIP_U32_U64 uPosition = bForward ? uEndOffset - uPack : uStartOffset;
		
		m_storage.Seek(uPosition);
		size_read = m_storage.m_pFile->Read(buf, (UINT)uPack);
		if (!size_read)
			break;
		
		if (bForward)
			uPosition += uMoveBy;
		else
			uPosition -= uMoveBy;
		m_storage.Seek(uPosition);
		m_storage.m_pFile->Write(buf, size_read);
		if (bForward)
			uEndOffset -= size_read;
		else
			uStartOffset += size_read;
		if (pCallback)
			if (!(*pCallback)(size_read))
				ThrowError(CZipException::abortedAction);
	}
	while (!bBreak);
	
	if (uEndOffset != uStartOffset)
		ThrowError(CZipException::internal);
	
}
