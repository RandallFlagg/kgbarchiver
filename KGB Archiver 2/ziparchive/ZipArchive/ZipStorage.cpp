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
#include "ZipStorage.h"
#include "ZipArchive.h"
#include "ZipPlatform.h"

//////////////////////////////////////////////////////////////////////
// archive segmentation objectives:
// - sinature at the first disk at the beginning
// - headers and central dir records not divided between disks
// - each file has a data descriptor preceded by the signature
//	(bit 3 set in flag);

int CZipActionCallback::m_iStep = 256;
char CZipStorage::m_gszExtHeaderSignat[] = {0x50, 0x4b, 0x07, 0x08};
const ZIP_FILE_USIZE CZipStorage::SignatureNotFound = ZIP_FILE_USIZE(-1);

CZipStorage::CZipStorage()
{
	m_pChangeDiskFunc = NULL;
	m_iWriteBufferSize = 65536;	
	m_pFile = NULL;
	m_szSplitExtension = _T("zip");
	m_iLocateBufferSize = 32768;
	m_uBytesBeforeZip = 0;
	m_uCurrentDisk = ZIP_DISK_NUMBER_UNSPECIFIED;
}

CZipStorage::~CZipStorage()
{

}

DWORD CZipStorage::Read(void *pBuf, DWORD iSize, bool bAtOnce)
{
	if (iSize == 0)
		return 0;
	DWORD iRead = 0;
	while (!iRead)
	{
		iRead = m_pFile->Read(pBuf, iSize);
		if (!iRead)
			if (IsSegmented())
				ChangeDisk(m_uCurrentDisk + 1);
			else
				ThrowError(CZipException::badZipFile);
	}

	if (iRead == iSize)
		return iRead;
	else if (bAtOnce || !IsSegmented())
		ThrowError(CZipException::badZipFile);

	while (iRead < iSize)
	{
		ChangeDisk(m_uCurrentDisk + 1);
		UINT iNewRead = m_pFile->Read((char*)pBuf + iRead, iSize - iRead);
		if (!iNewRead && iRead < iSize)
			ThrowError(CZipException::badZipFile);
		iRead += iNewRead;
	}

	return iRead;
}

void CZipStorage::Open(LPCTSTR szPathName, int iMode, ZIP_U32_U64 uVolumeSize)
{
	m_uCurrentDisk = ZIP_DISK_NUMBER_UNSPECIFIED;
	m_pWriteBuffer.Allocate(m_iWriteBufferSize); 
	m_uBytesInWriteBuffer = 0;
	m_bNewSegm = false;
	m_pFile = &m_internalfile;
	m_bInMemory = false;

	if ((iMode == CZipArchive::zipCreate) ||(iMode == CZipArchive::zipCreateSegm)) // create new archive
	{
		m_bReadOnly = false;
		m_uCurrentDisk = 0;
		if (iMode == CZipArchive::zipCreate)
		{
			m_iSegmMode = noSegments;
			OpenFile(szPathName, CZipFile::modeCreate | CZipFile::modeReadWrite);
		}
		else // create a segmented archive
		{
			m_bNewSegm = true;
			m_uBytesWritten = 0;
			if (uVolumeSize == ZIP_AUTODETECT_VOLUME_SIZE) // spanned archive
			{
				if (!m_pChangeDiskFunc)
					ThrowError(CZipException::noCallback);
				if (!ZipPlatform::IsDriveRemovable(szPathName))
					ThrowError(CZipException::nonRemovable);
				m_iSegmMode = spannedArchive;
			}
			else
			{
				m_uSplitData = uVolumeSize;
				m_iSegmMode = splitArchive;
			}

			NextDisk(4, szPathName);
			Write(m_gszExtHeaderSignat, 4, true);
		}
	}
	else // open existing
	{
		m_bReadOnly = iMode == CZipArchive::zipOpenReadOnly;
		OpenFile(szPathName, CZipFile::modeNoTruncate |
			(m_bReadOnly ? CZipFile::modeRead : CZipFile::modeReadWrite));
		// m_uData and m_iSegmMode are automatically set during reading the central dir
		m_iSegmMode = uVolumeSize == 0 ? suggestedAuto : suggestedSplit;
	}
		
}


void CZipStorage::Open(CZipAbstractFile& af, int iMode)
{
	m_pWriteBuffer.Allocate(m_iWriteBufferSize); 
	m_uBytesInWriteBuffer = 0;
	m_bNewSegm = false;
	m_pFile = &af;
	m_bInMemory = true;

	if (iMode == CZipArchive::zipCreate)
	{
		m_uCurrentDisk = 0;
		m_iSegmMode = noSegments;
		af.SetLength(0);
	}
	else // open existing
	{
		af.SeekToBegin();
		m_iSegmMode = suggestedAuto;
	}
}


void CZipStorage::ChangeDisk(ZIP_U16_U32 uNumber)
{
	if (uNumber == m_uCurrentDisk || m_iSegmMode == noSegments) // the second condition may happen in some bad archives
		return;

	m_uCurrentDisk = uNumber;
	OpenFile(m_iSegmMode == spannedArchive ? ChangeSpannedRead() : ChangeSplitRead(),
		CZipFile::modeNoTruncate | CZipFile::modeRead);
}

void CZipStorage::ThrowError(int err)
{
	CZipException::Throw(err, m_pFile->GetFilePath());
}

bool CZipStorage::OpenFile(LPCTSTR lpszName, UINT uFlags, bool bThrow)
{
	return m_pFile->Open(lpszName, uFlags | CZipFile::shareDenyWrite, bThrow);
}


CZipString CZipStorage::ChangeSpannedRead()
{
	CZipString szTemp = m_pFile->GetFilePath();
	m_pFile->Close();
	CallCallback(0, CZipSpanCallback::scDiskNeededForRead, szTemp);
	return szTemp;
}

CZipString CZipStorage::ChangeSplitRead()
{
	CZipString szTemp = GetSplitVolumeName(m_uCurrentDisk == m_uSplitData);
	m_pFile->Close();
	return szTemp;
}

CZipString CZipStorage::RenameLastFileInSplitArchive()
{
	ASSERT(m_iSegmMode == splitArchive);
		// give to the last volume the zip extension
	CZipString szFileName = m_pFile->GetFilePath();
	CZipString szNewFileName = GetSplitVolumeName(true);
	if (!m_bInMemory)
	{
		m_pFile->Flush();
		m_pFile->Close();
	}
	if (ZipPlatform::FileExists(szNewFileName))
		ZipPlatform::RemoveFile(szNewFileName);
	ZipPlatform::RenameFile(szFileName, szNewFileName);
	return szNewFileName;
}

CZipString CZipStorage::Close(bool bAfterException)
{
	bool bClose = true;
	CZipString sz;
	if (!bAfterException)
	{
		Flush();
		if ((m_iSegmMode == splitArchive) && (m_bNewSegm))
		{
			sz = RenameLastFileInSplitArchive();
			bClose = false;// already closed in RenameLastFileInSplitArchive
		}
	}
	if (sz.IsEmpty())
		sz = m_pFile->GetFilePath();
	if (bClose && !m_bInMemory)
	{
		if (!bAfterException)
			FlushFile();
		m_pFile->Close();
	}

	m_pWriteBuffer.Release();
	m_uCurrentDisk = ZIP_DISK_NUMBER_UNSPECIFIED;
	m_iSegmMode = noSegments;
	m_pFile = NULL;
	m_uBytesBeforeZip = 0;
	return sz;
}

CZipString CZipStorage::GetSplitVolumeName(bool bLast, LPCTSTR lpszZipName) const
{
	CZipString szFilePath = lpszZipName ? lpszZipName : (LPCTSTR)m_pFile->GetFilePath();
	CZipPathComponent zpc(szFilePath);
	CZipString szExt;
	if (bLast)
		szExt = m_szSplitExtension;
	else
	{
		int vol = m_uCurrentDisk + 1;
		if (vol < 100)
			szExt.Format(_T("z%.2d"), vol);
		else
			szExt.Format(_T("z%d"), vol);
	}
	zpc.SetExtension(szExt);
	return zpc.GetFullPath();
}

void CZipStorage::NextDisk(ZIP_U32_U64 uNeeded, LPCTSTR lpszFileName)
{
	Flush();
	ASSERT(m_iSegmMode != noSegments);
	bool bSpan = (m_iSegmMode == spannedArchive);
	if (m_uBytesWritten)
	{
		m_uBytesWritten = 0;
		m_uCurrentDisk++;
		ZIP_U16_U32 uMaxVolumes = bSpan ? 999 : 0xFFFF;
		if (m_uCurrentDisk >= uMaxVolumes) // m_uCurrentDisk is a zero-based index
			ThrowError(CZipException::tooManyVolumes);
	} 
	CZipString szFileName;
	
	if (bSpan)
		szFileName  = lpszFileName ? lpszFileName : (LPCTSTR)m_pFile->GetFilePath();
	else
		szFileName =  GetSplitVolumeName(false, lpszFileName);

	if (!m_pFile->IsClosed())
	{
		m_pFile->Flush();
		m_pFile->Close();
	}

	if (bSpan)
	{
		int iCode = CZipSpanCallback::scDiskNeededForWrite;
		while (true)
		{
			CallCallback(uNeeded, iCode, szFileName);
			if (ZipPlatform::FileExists(szFileName))
				iCode = CZipSpanCallback::scFileNameDuplicated;
			else
			{
				CZipString label;
				label.Format(_T("pkback# %.3d"), m_uCurrentDisk + 1);
				if (!ZipPlatform::SetVolLabel(szFileName, label))
					iCode = CZipSpanCallback::scCannotSetVolLabel;
				else if (!OpenFile(szFileName, CZipFile::modeCreate | CZipFile::modeReadWrite, false))
					iCode = CZipSpanCallback::scFileCreationFailure;
				else
					break;
			}

		}
		m_uCurrentVolSize = GetFreeVolumeSpace();
	}
	else
	{
		m_uCurrentVolSize = m_uSplitData;
		OpenFile(szFileName, CZipFile::modeCreate | CZipFile::modeReadWrite);
	}
}

void CZipStorage::CallCallback(ZIP_U32_U64 uNeeded, int iCode, CZipString szTemp)
{
	if (!m_pChangeDiskFunc)
		ThrowError(CZipException::internal);
	m_pChangeDiskFunc->m_szExternalFile = szTemp;
	m_pChangeDiskFunc->m_uDiskNeeded = m_uCurrentDisk + 1;
	m_pChangeDiskFunc->m_iCode = iCode; 
	if (!m_pChangeDiskFunc->Callback(uNeeded))
		CZipException::Throw(CZipException::aborted, szTemp);
}

ZIP_U32_U64 CZipStorage::GetFreeVolumeSpace() const
{
	ASSERT (m_iSegmMode == spannedArchive);
	CZipString szTemp = m_pFile->GetFilePath();
	if (szTemp.IsEmpty()) // called once when creating a segmented archive
		return 0;
	else
	{
		CZipPathComponent zpc(szTemp);
                ULONGLONG ret = ZipPlatform::GetDeviceFreeSpace(zpc.GetFilePath());
                if (ret > (ZIP_U32_U64)(-1))
                        return (ZIP_U32_U64)(-1);
                else
                        return (ZIP_U32_U64)ret;
	}
}


void CZipStorage::UpdateSegmMode(ZIP_U16_U32 uLastDisk)
{
	m_uCurrentDisk = uLastDisk;
	if (uLastDisk)
	{
		// segmentation detected
		CZipString szFilePath = m_pFile->GetFilePath();
		if (m_iSegmMode == suggestedAuto)
			m_iSegmMode = ZipPlatform::IsDriveRemovable(szFilePath) ? 
				spannedArchive : splitArchive;
		else
		{
			ASSERT(m_iSegmMode == suggestedSplit);
			m_iSegmMode = splitArchive;
		}

		if (m_iSegmMode == spannedArchive)
		{
			if (!m_pChangeDiskFunc)
					ThrowError(CZipException::noCallback);
		}
		else /*if (m_iSegmMode == splitArchive)*/
			m_uSplitData = uLastDisk; // disk with .zip extension ( the last one)
		CZipPathComponent zpc(szFilePath);
		m_szSplitExtension = zpc.GetFileExt();
		m_pWriteBuffer.Release(); // no need for this in this case
	}
	else 
		m_iSegmMode = noSegments;

}

ZIP_U32_U64 CZipStorage::AssureFree(ZIP_U32_U64 uNeeded)
{
	ZIP_U32_U64 uFree;
	while ((uFree = VolumeLeft()) < uNeeded)
	{
		if ((m_iSegmMode == splitArchive) && !m_uBytesWritten && !m_uBytesInWriteBuffer)
			// in the splitArchive mode, if the size of the archive is less 
			// than the size of the packet to be written at once,
			// increase once the size of the volume
			m_uCurrentVolSize = uNeeded;
		else
			NextDisk(uNeeded);
	}
	return uFree;
}

void CZipStorage::Write(const void *pBuf, DWORD iSize, bool bAtOnce)
{
	if (!IsSegmented())
		WriteInternalBuffer((char*)pBuf, iSize);
	else
	{
		// if not at once, one byte is enough free space
		DWORD iNeeded = bAtOnce ? iSize : 1; 
		DWORD uTotal = 0;

		while (uTotal < iSize)
		{			
			ZIP_U32_U64 uFree = AssureFree(iNeeded);
			DWORD uLeftToWrite = iSize - uTotal;
			DWORD uToWrite = uFree < uLeftToWrite ? (DWORD)uFree : uLeftToWrite;
			WriteInternalBuffer((char*)pBuf + uTotal, uToWrite);
			if (bAtOnce)
				return;
			else
				uTotal += uToWrite;
		}

	}
}


void CZipStorage::WriteInternalBuffer(const char *pBuf, DWORD uSize)
{
	DWORD uWritten = 0;
	while (uWritten < uSize)
	{
		DWORD uFreeInBuffer = GetFreeInBuffer();
		if (uFreeInBuffer == 0)
		{
			Flush();
			uFreeInBuffer = m_pWriteBuffer.GetSize();
		}
		DWORD uLeftToWrite = uSize - uWritten;
		DWORD uToCopy = uLeftToWrite < uFreeInBuffer ? uLeftToWrite : uFreeInBuffer;
		memcpy(m_pWriteBuffer + m_uBytesInWriteBuffer, pBuf + uWritten, uToCopy);
		uWritten += uToCopy;
		m_uBytesInWriteBuffer += uToCopy;
	}
}

ZIP_U32_U64 CZipStorage::VolumeLeft() const
{
	// for spanned archives m_uCurrentVolSize is updated after each flush()
	return m_uCurrentVolSize  - m_uBytesInWriteBuffer - ((m_iSegmMode == spannedArchive) ? 0 : m_uBytesWritten);
}

void CZipStorage::Flush()
{
	if (m_iSegmMode != noSegments)
		m_uBytesWritten += m_uBytesInWriteBuffer;
	if (m_uBytesInWriteBuffer)
	{
		m_pFile->Write(m_pWriteBuffer, m_uBytesInWriteBuffer);
		m_uBytesInWriteBuffer = 0;
	}
	if (m_iSegmMode == spannedArchive) 
		// after writing it is difficult to predict the free space due to 
		// not completly written clusters, write operation may start from a new cluster
		m_uCurrentVolSize = GetFreeVolumeSpace();
	
}

ZIP_FILE_USIZE CZipStorage::LocateSignature(char* szSignature, ZIP_U32_U64 uMaxDepth)
{
	const int recordSize = 4;
	CZipAutoBuffer buffer(m_iLocateBufferSize);
	ZIP_FILE_USIZE uFileLength = m_pFile->GetLength();
	ZIP_U32_U64 max = (ZIP_U32_U64)(uFileLength < uMaxDepth ? uFileLength : uMaxDepth);
	ZIP_U32_U64 position = (ZIP_U32_U64)(uFileLength - m_pFile->GetPosition());
	int offset = 0;
	int leftToFind = recordSize - 1;
	int toRead = m_iLocateBufferSize;
	bool found = false; // for fast checking if leftToFind needs resetting
	while ( position < max )
	{
		position += toRead;
		if ( position > max )
		{
			int diff = (int) ( position - max );
			toRead -= diff;
			offset = diff;
			position = max;
		}
		Seek(position, false);		
		int actuallyRead = m_pFile->Read(buffer + offset, toRead);
		if (actuallyRead != toRead)
			ThrowError(CZipException::badZipFile);
		int pos = m_iLocateBufferSize - 1;
		while ( pos >= offset )
		{
			if ( buffer[pos] == szSignature[leftToFind] )
			{
				if ( leftToFind == 0 )
					return (ZIP_FILE_USIZE)(uFileLength - ( position - ( pos - offset ) ));
				if ( !found )
					found = true;
				leftToFind--;
				pos--;
			}
			else if ( found )
			{
				leftToFind = recordSize - 1;
				found = false;
				// do not decrease position, the current pos may be the first to find
			}
			else
				pos--;
		}
	}
	return SignatureNotFound;
}

ULONGLONG CZipStorage::Seek(ULONGLONG lOff, bool fromBeginning)
{

	if (m_uCurrentDisk == 0 && m_uBytesBeforeZip > 0)
		lOff += m_uBytesBeforeZip;
	return m_pFile->Seek((ZIP_FILE_USIZE)lOff, fromBeginning);
}

void CZipStorage::FinalizeSegm()
{
	ASSERT(IsSegmented() == 1); // spanned archive in creation
	ASSERT(!m_bInMemory);

	CZipString szFileName;
	if ((m_iSegmMode == splitArchive) && (m_bNewSegm))
		szFileName = RenameLastFileInSplitArchive();
	else
	{
		szFileName = m_pFile->GetFilePath();
		// the file is already closed
		m_pFile->Close();
	}
	m_bNewSegm = false;
	if (m_uCurrentDisk == 0) // one-disk segmented archive was converted to normal archive
		m_iSegmMode = noSegments;
	else
		m_uSplitData = m_uCurrentDisk;
	
	OpenFile(szFileName, CZipFile::modeNoTruncate | (m_iSegmMode == noSegments ? CZipFile::modeReadWrite : CZipFile::modeRead));	
}

