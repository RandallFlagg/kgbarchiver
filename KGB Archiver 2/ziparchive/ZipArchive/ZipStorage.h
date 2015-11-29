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
* \file ZipStorage.h
* Interface for the CZipStorage class.	
*
*/

#if !defined(ZIPARCHIVE_ZIPSTORAGE_DOT_H)
#define ZIPARCHIVE_ZIPSTORAGE_DOT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZipFile.h"	
#include "ZipAutoBuffer.h"
#include "ZipString.h"
#include "ZipMemFile.h"
#include "ZipExport.h"



/**
	A base class for callback objects that are notified when various actions take place.
	You need to derive your own class and overload the #Callback method to use it.
	Do not derive from CZipCallback directly but derive from:
	\li CZipSpanCallback, when you create a spanned archive - you will be notified when there is a need for a disk change
	\li CZipActionCallback, for other notifications.

	\see
		<a href="kb">0610051553|span</a>
	\see
		<a href="kb">0610231200</a>
	\see
		CZipSpanCallback
	\see
		CZipActionCallback
*/
struct ZIP_API CZipCallback
{
	/**
		The method called as a callback.

		Return \c false from inside the method to abort the current operation. If it is a spanned archive callback object,
		a CZipException with CZipException::aborted code will be thrown, otherwise the code will be CZipException::abortedAction or CZipException::abortedSafely.
		The following actions can be safely aborted (without corrupting the archive):
		- counting bytes before deleting files
		- testing
		- saving the central directory in a not segmented archive 
		(saved data is removed in case of break	and you can save it again).

		If the archive is segmented and if saving is aborted, the archive
		will not be damaged, but saved part of the central directory will be not removed
		and the new central directory will have to be saved after it.

		\param uProgress
			The value depends on the type of the operation.

		\return 
			\c false to abort the current operation; \c true to continue it.

		\note
			Override this method in the derived class. If you define this method inside the class declaration, consider inlining
			it to make the action progress faster.
	*/
	virtual bool Callback(ZIP_U32_U64 uProgress) = 0;


	/**
		Stored the filename of an external file, if the action (adding, extracting or segmentation) uses such a file.
	*/
	CZipString m_szExternalFile;
	virtual ~CZipCallback(){}
};

/**
	When you derive from CZipSpanCallback, you can use the derived class as a callback object for the disk change notification.
	You need to override the CZipCallback::Callback method.
	The value of the \a uProgress parameter in the callback method is the minimum number of free bytes required on the disk.

	Return \c false from the callback function to abort the operation: the proper exception will be thrown.

	\see
		<a href="kb">0610051553|span</a>
	\see
		CZipCallback::Callback
	\see
		CZipArchive::SetSpanCallback
*/
struct ZIP_API CZipSpanCallback : public CZipCallback
{
	/**
		Values indicating the reason for calling the callback.
	*/
	enum SpanCodes
	{
		scDiskNeededForRead,	///< A disk is needed when reading a segmented archive. The number of the disk is stored in #m_uDiskNeeded.
		scDiskNeededForWrite,	///< A disk is needed when writing a segmented archive. The number of the disk is stored in #m_uDiskNeeded.
		scFileNameDuplicated,	///< The file with the archive name already exists on the disk.
		scCannotSetVolLabel,	///< The volume label could not be created. The disk may be write-protected.
		scFileCreationFailure	///< The archive file could not be created. The disk may be write-protected.
	};

	ZIP_U16_U32 m_uDiskNeeded;		///< The number of the disk needed when reading or writing a segmented archive. Disk are numbered starting from 1.
	
	int m_iCode; ///< The reason for calling the callback. Can be one of the #SpanCodes values.
};



/**
	When you derive from CZipActionCallback, you can use the new class as a callback object when adding, extracting, deleting, testing files
	or saving the central directory.
	You need to override the CZipCallback::Callback method.
	The value of the \a uProgress parameter in the callback method is the amount of data just processed.

	\see
		<a href="kb">0610231200</a>
	\see
		CZipCallback::Callback
	\see
		CZipArchive::SetCallback
*/
struct ZIP_API CZipActionCallback : public CZipCallback
{
	friend class CZipArchive;
	friend class CZipCentralDir;

	CZipActionCallback()
	{		
		m_uTotalToDo = 0;
		m_uTotalSoFar = 0;
	}
	
	/**
		Called to initialize the callback object.
		Sets the filenames and resets #m_uTotalToDo and #m_uTotalSoFar variables to 0.
		#m_iType variable is already set to the proper value. Called at the beginning of the action.
		\param lpszFileInZip
			The file that is being processed in the archive.

		\param lpszExternalFile
			The external file being processed. This is set to \c NULL, if the operation takes place in memory.
	*/
	virtual void Init(LPCTSTR lpszFileInZip = NULL, LPCTSTR lpszExternalFile = NULL) 
	{
		m_szFileInZip = lpszFileInZip;
		m_szExternalFile = lpszExternalFile;
		m_uTotalToDo = 0;  // not yet known
		m_uTotalSoFar = 0; // nothing yet done
	}

	/**
		Called after calculating the total amount of data to process.

		\param uTotalToDo
			Total amount of data to process. The method initializes #m_uTotalToDo with this value.
	*/
	virtual void SetTotal(ZIP_U32_U64 uTotalToDo)
	{
		m_uTotalToDo = uTotalToDo;
// 		m_uTotalSoFar = 0; // already done in CZipArchive::CZipClbckStrg::Get
	}	

	/**
		Called after the operation finishes.
		It is not called in case of an exception, but
		it is called before throwing CZipException::abortedAction or CZipException::abortedSafely.
	*/
	virtual void CallbackEnd()
	{
		//ASSERT(m_uTotalSoFar == m_uTotalToDo);
	};

	/**
		Gets the amount of data left to process.

		\return
			The amount of data left to process.
	*/
	ZIP_U32_U64 LeftToDo() {return m_uTotalToDo - m_uTotalSoFar;}

	/**
		The total amount of data to process. This value is set when the #SetTotal method is called.

		Depending on the action it is set then to:
		- Adding a file : the size the external file being added (or if the type of the callback is CZipArchive::cbAddTmp, 
			the size of the compressed data: CZipFileHeader::m_uComprSize).
		- Extracting a file : the size of the uncompressed data (CZipFileHeader::m_uUncomprSize).
		- Testing a file : the same as above.
		- Deleting a file : the number of bytes to move - the size of all files to remain above the first 
			file to delete (calculated from offsets in CZipFileHeader::m_uOffset).
		- Saving the central directory : the number of files in the archive.
	*/
	ZIP_U32_U64 m_uTotalToDo;				
	ZIP_U32_U64 m_uTotalSoFar;			///< The total amount of data processed so far.
	CZipString m_szFileInZip;		///< The name of the file that is being processed in the archive.

	/**
		The type of the callback. It is set to one of the CZipArchive::CallbackType values when the action begins.
		It's useful, if you have more than one callback assigned to the same callback object.
	*/
	int m_iType;

	/**
		Specifies how often the callback is called when #m_iType is set to CZipArchive::cbDeleteCnt or CZipArchive::cbSave.
		The callback will be called every \e nth file processed (\e n is the \a m_iStep value).
		The \a uProgress parameter in the callback method will be set to \a m_iStep.
		Do not set the value too low, because it may increase significantly the time needed to process a huge number of files.
	*/
	static int m_iStep;

	/**
		The same as #m_iStep, but does not allow 0 value, which could cause errors.
	*/
	int GetStep(){return m_iStep ? m_iStep : 1;}
private:

	virtual bool operator()(ZIP_U32_U64 uProgress)
	{
		m_uTotalSoFar += uProgress;
		return Callback(uProgress);
	}
};


/**
	Represents the storage layer for an archive.
*/
class ZIP_API CZipStorage  
{
	friend class CZipArchive;
	friend class CZipCentralDir;
public:
	
	/**
		The type of the segmentation of the archive.

		\see
			<a href="kb">0610051553</a>
		\see
			CZipArchive::GetSegmMode
	*/
	enum ZipSegmentationMode
	{
		noSegments,			///< No archive segmentation.
		spannedArchive,		///< A spanned archive.
		splitArchive,		///< A split archive.

		/**
			\brief .
			The archive segmentation type will be auto-detected.			
			If the archive is on the removable device,
			assume a spanned archive, otherwise assume a split archive.
		*/
		suggestedAuto,	

		/**
			\brief .
			If a segmented archive is on a removable device, assume a split archive.
			Normally you create spanned archives on removable devices.
		*/
		suggestedSplit
	};

	CZipStorage();
	virtual ~CZipStorage();

	/**
		Opens a new or existing archive in memory.
		The meaning for the parameters is the same as in the CZipArchive::Open(CZipAbstractFile& , int) method.
	*/
	void Open(CZipAbstractFile& af, int iMode);

	/**
		Opens or creates an archive.

		The meaning for the parameters is the same as in the CZipArchive::Open(LPCTSTR, int, ZIP_U32_U64) method.
	*/
	void Open(LPCTSTR szPathName, int iMode, ZIP_U32_U64 uVolumeSize);


	/**
		Closes a segmented archive in creation and reopens it as an existing segmented archive (no modifications allowed).
		The archive may also turn out to be a not segmented archive.
	*/
	void FinalizeSegm();

	
	/**
		Called only by CZipCentralDir::Read when opening an existing archive.

		\param	uLastDisk
			The number of the disk the central directory is on.

		\note Throws exceptions.

	*/
	void UpdateSegmMode(ZIP_U16_U32 uLastDisk);

	/**
		Ensures than in a segmented archive, there is enough free space on the current volume.

		\param uNeeded
			The size of the required free space in bytes.

		\return
			The number of free bytes on the current volume.

		\note
			Throws exceptions.
	*/
	ZIP_U32_U64 AssureFree(ZIP_U32_U64 uNeeded);

	/**
		Writes a chunk of data to the archive.

		\param	pBuf
			The buffer with data.

		\param	iSize
			The number of bytes to write.

		\param	bAtOnce
			If \c true, the whole chunk must fit in the current volume.
			If there is not enough free space, a disk change is performed.

		\note
			Throws exceptions.
	*/
	void Write(const void *pBuf, DWORD iSize, bool bAtOnce);

	/** 
		Gets the total size currently occupied by the archive.

		\return
			The length of the current archive file increased by the number of bytes in the write buffer.	
	*/
	ZIP_U32_U64 GetOccupiedSpace() const
	{
		return ZIP_U32_U64(m_pFile->GetLength() + m_uBytesInWriteBuffer);
	}

	/**
		The same as the CZipArchive::IsClosed method.
	*/
	bool IsClosed(bool bArchive) const 
	{
		return  bArchive ?(GetCurrentDisk() == ZIP_DISK_NUMBER_UNSPECIFIED) : (!m_pFile ||  m_pFile->IsClosed());
	}

	/**
		Reads a chunk of data from the archive.

		\param	pBuf
			The buffer to receive the data.

		\param	iSize
			The number of bytes to read.

		\param	bAtOnce
			If \c true, the specified number of bytes must be read from the same 
			volume (no disk change is allowed).

		\note
			Throws exceptions.
	*/
	DWORD Read(void* pBuf, DWORD iSize, bool bAtOnce);

	/**
		Gets the position in the file, taking into account the number of bytes in the write buffer 
		and the number of bytes before the archive.

		\return 
			The position in the file.

		\note
			Throws exceptions.
	*/
	ZIP_U32_U64 GetPosition() const
	{
		ZIP_U32_U64 uPos = (ZIP_U32_U64)(m_pFile->GetPosition()) + m_uBytesInWriteBuffer;
		if (m_uCurrentDisk == 0)
			uPos -= m_uBytesBeforeZip;
		return uPos;
	}


	/**
		Flushes the data from the read buffer to the disk.

		\note
			Throws exceptions.
	*/
	void Flush();


	/**
		Forces any data remaining in the file buffer to be written to the disk.
	*/
	void FlushFile()
	{
		if (!m_bInMemory && !IsReadOnly())
			m_pFile->Flush();
	}

	/**
		Changes disks during writing to a segmented archive.

		\param	uNeeded
			The number of bytes needed on the disk.

		\param	lpszFileName
			The archive filename.

		\note
			Throws exceptions.
	*/
	void NextDisk(ZIP_U32_U64 uNeeded, LPCTSTR lpszFileName = NULL);


	/**
		Gets a zero-based number of the current disk.
	*/
	ZIP_U16_U32 GetCurrentDisk() const {return m_uCurrentDisk;}

 
	/**
		Changes the disk during extract operations.

		\param	uNumber
			A zero-based requested disk number.
	*/
	void ChangeDisk(ZIP_U16_U32 uNumber);

	/**
		Detects the segmentation mode.

		\return	
			- \c -1 : An existing segmented archive is opened.
			- \c 0 : The archive is not segmented.
			- \c 1 : A segmented archive in creation.
	*/
	int IsSegmented() const
	{
		return m_iSegmMode == noSegments ? 0 : (m_bNewSegm ? 1 : -1);
	}

	/**
		The same as the CZipArchive::IsReadOnly method.
	*/
	bool IsReadOnly()
	{
		return m_bReadOnly || IsSegmented() < 0;
	}
	
	/**
		Performs the seeking operation on the #m_pFile.

		\param lOff
			The new position in the file.

		\param fromBeginning
			The direction of the seek operation. If \c true, 
			\a lOff is counted from the beginning of the file;
			otherwise the \a lOff is counted from the end of the file.
	*/
	ULONGLONG Seek(ULONGLONG lOff , bool fromBeginning = true);	

	/**
		Gets the number of free bytes on the current volume.	

		\return 
			The number of free bytes on the current volume.
	*/
	ZIP_U32_U64 VolumeLeft() const;

	/**	
		Closes the storage.

		\param	bAfterException
			Set to \c true, if an exception was thrown before.

		\return
			The file path of the archive.

		\note
			Throws exceptions.
	*/
	CZipString Close(bool bAfterException);

	/**
		Represents the physical storage of the current archive segment. 				
	*/
	CZipAbstractFile* m_pFile;

	/**
		The signature of the extended header.
	*/
	static char m_gszExtHeaderSignat[];

protected:
	
	/**
		Reverse-finds the location of the given signature starting from the current position in file.

		\param szSignature
			The signature to locate.

		\param uMaxDepth
			The maximum number of bytes to search for \a szSignature.

		\return
			The location of the signature.

		\note
			Throws exceptions.
	*/
	ZIP_FILE_USIZE LocateSignature(char* szSignature, ZIP_U32_U64 uMaxDepth);
		

	/**
		Flushes without writing. Can be used only on not segmented archives.
	*/
	void EmptyWriteBuffer()
	{
		m_uBytesInWriteBuffer = 0;
	}

	/**
		Opens a physical file.

		\param	lpszName
			The name of the file to open.

		\param	uFlags
			The file open flags.

		\param	bThrow
			If \c true, throw an exception in case of failure.

		\return
			\c true if successful; \c false otherwise.
	*/
	bool OpenFile(LPCTSTR lpszName, UINT uFlags, bool bThrow = true);

	/**
		Renames the last segment file in a split archive when finalizing the whole archive.
	*/
	CZipString RenameLastFileInSplitArchive();

	/**
		Writes data to the internal buffer.

		\param	*pBuf
			The buffer to copy the data from.

		\param	uSize
			The number of bytes to write.

		\note
			Throws exceptions.
	*/
	void WriteInternalBuffer(const char *pBuf, DWORD uSize);

	/**
		Gets the free space size on the current removable disk.

		\return
			The free space in bytes.
	*/
	ZIP_U32_U64 GetFreeVolumeSpace() const;

	/**
		Notifies the callback object.
		Throws an exception if the callback method returns \c false.

		\param uNeeded
			The minimum number of free bytes required on the disk.

		\param	iCode
			The code to be passed to the callback method.

		\param	szTemp
			The string to be used as a filename (as an argument
			in the CZipException::Throw method) when an exception must be thrown.

		\note
			Throws exceptions.
		\see
			CZipArchive::SetSpanCallback
	*/
	void CallCallback(ZIP_U32_U64 uNeeded, int iCode, CZipString szTemp);

	/**
		Constructs the name of a segment in a split archive.

		\param	bLast
			Set it to \c true, if constructing the last volume name.

		\param	lpszZipName
			The name of the archive.

		\return	
			The segment name.
	*/
	CZipString GetSplitVolumeName(bool bLast, LPCTSTR lpszZipName = NULL) const;

	/**
		Changes the disk when processing a split archive.
	*/
	CZipString ChangeSplitRead();

	/**
		Changes the disk when processing a spanned archive.
	*/
	CZipString ChangeSpannedRead();

	/**
		Gets the free space left in the write buffer.

		\return
			The free space left in the write buffer in bytes.
	*/
	DWORD GetFreeInBuffer() const {return m_pWriteBuffer.GetSize() - m_uBytesInWriteBuffer;}	

	/**
		The value it holds, depends on the current mode:		
		- An opened existing split archive - stores the number of the last disk ( the one with "zip" extension).
		- A split archive in creation - the size of the volume.

		This method is used only when processing split archives.
	*/
	ZIP_U32_U64 m_uSplitData;
	
	/**
		The extension of the last segment.
	*/
	CZipString m_szSplitExtension;
	
	/**
		The number of bytes available in the write buffer.		
	*/
	DWORD m_uBytesInWriteBuffer;

	/**
		The value it holds depends on the segmentation mode:
		- A split archive : the total size of the current volume.
		- A spanned archive: the free space on the current volume.
	*/
	ZIP_U32_U64 m_uCurrentVolSize;

	/**
		The write buffer caching data.
	*/
	CZipAutoBuffer m_pWriteBuffer;

	/**
		Stores the number of bytes that have been written physically to the current segment.
		Used only when processing a segmented archive in creation.
	*/
	ZIP_U32_U64 m_uBytesWritten;

	/**
		\c true, if the current archive is a new segmented archive; \c false otherwise.
	*/
	bool m_bNewSegm;

	/**
		The current disk in a segmented archive.
		The value is zero-based.
	*/
	ZIP_U16_U32 m_uCurrentDisk;

	/**
		\c true when the archive is created in memory; \e false otherwise.
	*/
	bool m_bInMemory;

	/**
		\c true if OpenMode::zipOpenReadOnly was specified when opening the archive.
	*/
	bool m_bReadOnly;

	/**
		The number of bytes before the actual zip archive in a file.
		\see
			CZipArchive::GetBytesBeforeZip
	*/
	ZIP_U32_U64 m_uBytesBeforeZip;


	/**
		The size of the write buffer. 		

		\see
			CZipArchive::SetAdvanced
	*/
	int m_iWriteBufferSize;

	/**
		The size of the buffer used in searching for the central directory.

		\see
			CZipArchive::SetAdvanced
	*/
	int m_iLocateBufferSize;	

	/**
		Takes one of the #ZipSegmentationMode values.	
	*/
	int m_iSegmMode;

	/**
		A callback object called when there is a need for a disk change in a spanned archive.

		\see
			CZipArchive::SetSpanCallback
	*/
	CZipSpanCallback* m_pChangeDiskFunc;

private:
	CZipFile m_internalfile;
	static const ZIP_FILE_USIZE SignatureNotFound;
	void ThrowError(int err);
};

#endif // !defined(ZIPARCHIVE_ZIPSTORAGE_DOT_H)
