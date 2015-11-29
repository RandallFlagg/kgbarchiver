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
// Check the site http://www.artpol-software.com for the updated version of the library.
//   
//	The following information files are distributed along with this library:
//		License.txt		- licensing information
//		Appnote.txt		- details on the zip format
//							( also available at ftp://ftp.pkware.com/appnote.zip)
//
 


/**
* \file ZipArchive.h
*	Interface for the CZipArchive class.
*
*/

#if !defined(ZIPARCHIVE_ZIPARCHIVE_DOT_H)
#define ZIPARCHIVE_ZIPARCHIVE_DOT_H

#if (_MSC_VER > 1000) && (defined ZIP_HAS_DLL)
	#pragma warning (push)
	#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class
	#pragma warning( disable : 4275 ) // non dll-interface class used as base for dll-interface
#endif

#include "zlib/zlib.h"

#include "ZipException.h"
#include "ZipAutoBuffer.h"
#include "ZipCentralDir.h"	
#include "ZipStorage.h"
#include "ZipPathComponent.h"
#include "ZipString.h"
#include "ZipExport.h"
#include "ZipCryptograph.h"

#define ZIP_AUTODETECT_VOLUME_SIZE ZIP_U32_U64(-1)
#define ZIP_WINZIP_AES_COMPRESSION_METHOD 99

/**
	The structure used as a parameter in CZipArchive::AddNewFile(CZipAddNewFileInfo& ).
	Use one of the provided constructors and then adjust the member variables as needed.

	\see
		<a href="kb">0610231446|simple</a>
*/
struct ZIP_API CZipAddNewFileInfo
{
	/**
		Initializes a new instance of the CZipAddNewFileInfo class.
		
		\param lpszFilePath
			Sets #m_szFilePath.
		
		\param bFullPath
			Sets #m_bFullPath.
	 */
	CZipAddNewFileInfo(LPCTSTR lpszFilePath, bool bFullPath = true)
		: m_szFilePath(lpszFilePath),m_bFullPath(bFullPath)
	{
		m_pFile = NULL;
		Defaults();
	}

	/**
		Initializes a new instance of the CZipAddNewFileInfo class.
		
		\param lpszFilePath
			Sets #m_szFilePath.
		
		\param lpszFileNameInZip
			Sets #m_szFileNameInZip.
	 */
	CZipAddNewFileInfo(LPCTSTR lpszFilePath, LPCTSTR lpszFileNameInZip)
		: m_szFilePath(lpszFilePath), m_szFileNameInZip(lpszFileNameInZip)
	{
		m_pFile = NULL;
		Defaults();
	}

	/**
		Initializes a new instance of the CZipAddNewFileInfo class.
		
		\param pFile
			Sets #m_pFile.
		
		\param lpszFileNameInZip
			Sets #m_szFileNameInZip.
	 */
	CZipAddNewFileInfo(CZipAbstractFile* pFile, LPCTSTR lpszFileNameInZip)
		: m_pFile(pFile), m_szFileNameInZip(lpszFileNameInZip)
	{
		Defaults();
	}

	/**
		The full path to the file to be added. If it is empty, you need to initialize #m_pFile.
		If #m_bFullPath is \c true and the path contains a drive letter, the drive letter is removed unless
		CZipArchive::m_bRemoveDriveLetter is set to \c false.
	*/
	CZipString m_szFilePath;		

	/**
		The file name that will be stored in the archive. If the file is a directory,
		there will be a path separator automatically appended. The CZipArchive::SetRootPath
		method has no effect on this parameter.
	*/
	CZipString m_szFileNameInZip;
	
	/**
		It has only the meaning when #m_szFileNameInZip is not specified and 
		#m_szFilePath is not empty. 

		- If set to \c true, instructs to store the full path of the file inside the archive,
		even if CZipArchive::m_szRootPath is set.
		- If set to \c false only a filename without a path is stored in the archive. 
		In this case, if CZipArchive::m_szRootPath is set previously with 
		CZipArchive::SetRootPath, and if the beginning of #m_szFilePath equals 
		CZipArchive::m_szRootPath, then the filename is set to the not matched part 
		of #m_szFilePath (you could say to #m_szFilePath minus CZipArchive::m_szRootPath).
	*/
	bool m_bFullPath;

	/**
		The compression level, see CZipArchive::OpenNewFile(CZipFileHeader&, int, LPCTSTR)
		for the description.
	*/
	int m_iComprLevel;				

	/**
		The smartness level of  of the library. Can be one or combined CZipArchive::Smartness
		values (you can use the logical \c OR).
	*/
	int m_iSmartLevel;

	/**
		The index of an existing file in the archive to be replaced by the file being added.
		See <a href="kb">0610231944|replace</a> for more information.
		The meaning of its values is as follows:

		- >= 0 : the index of the file to be replaced.
		- \c ZIP_FILE_INDEX_UNSPECIFIED : do not replace any file and add the new file at 
		the end of the archive (default). Use this value in segmented archives.

		\note
			- If you use an invalid index, the action will fail. 
			- If you specify the last file in the archive to be replaced, it'll be removed 
			and the usual action will be taken.

		\see CZipArchive::SetAdvanced
		\see CZipArchive::WillBeDuplicated
	*/
	ZIP_U16_U64 m_uReplaceIndex;		

	/**
		The size of the buffer used while file operations.
	*/
	unsigned long m_nBufSize;		

	/**			
		Initialize this field to set the source data for compression to be taken from 
		the CZipAbstractFile object (such as CZipMemFile) instead of from a physical file.
		
		 \note 
		  - You have to leave #m_szFilePath empty if you set #m_pFile to not \c NULL.
		  - The time of a file in zip will be set to the current time, and the attributes to the default
		  file attributes (depending on the system).
		  - You cannot add directories this way.
	*/
	CZipAbstractFile* m_pFile;

	/**
		Sets default values for #m_iSmartLevel, #m_uReplaceIndex, #m_nBufSize and #m_iComprLevel.
		Examine the source code for the current values.
	*/
	void Defaults();
};



/// <summary>
/// Represents a zip archive file.	
/// </summary>
class ZIP_API CZipArchive  
{	
public:

	/**
		The purpose of this structure is to hold the data that allow
		communication with the Zlib library.
	*/
	struct ZIP_API CZipInternalInfo  
	{	
		CZipInternalInfo()
		{
			m_iBufferSize = 65536;
			m_uUncomprLeft = 0;
			m_uComprLeft  = 0;
			m_uCrc32 = 0;
		}
		virtual ~CZipInternalInfo(){}

		/**
			Allocates an internal buffer of #m_iBufferSize size.
		*/
		void Init()
		{
			m_pBuffer.Allocate(m_iBufferSize);
		}
		void ReleaseBuf()
		{
			m_pBuffer.Release();
		}
		z_stream m_stream;			///< The Zlib library data stream.
		ZIP_U32_U64 m_uUncomprLeft;	///< The number of bytes left to decompress.
		ZIP_U32_U64 m_uComprLeft;	///< The number of bytes left to compress.
		DWORD m_uCrc32;				///< The CRC32 file checksum.

		/**
			The size of the #m_pBuffer buffer. Set it before opening the archive.
			It is usually set with the CZipArchive::SetAdvanced method
			(specify this value as the second argument).

			\see
				CZipArchive::SetAdvanced
		*/
		DWORD m_iBufferSize;

		/**
			This buffer caches data during compression and decompression.
		*/
		CZipAutoBuffer m_pBuffer;
	};

	CZipArchive();
	virtual ~CZipArchive();
	
	/** 
		Sets a password for the file to be opened or created.
		Use this method before opening or adding a file, but after opening the archive.
		The password is cleared during opening the archive.

		\param	lpszPassword
			The password. Set it to \c NULL or an empty string to clear the password.

		\returns
			\c false if the password cannot be changed at this time; \c true otherwise.

		\see
			<a href="kb">0610201627</a>
		\see
			SetEncryptionMethod
	*/
	bool SetPassword(LPCTSTR lpszPassword = NULL);	

	/**
		Gets the current archive password or an empty string if there is no password set. 
		\returns
			The current password.
		\see
			SetPassword
	*/
	CZipString GetPassword()const ;

	/**
		Sets the encryption method when encrypting files.
		You can encrypt different files using different methods.
		You need to set the password with the #SetPassword method for the encryption to work.
		It is important to set the encryption method only when compressing. When extracting, 
		the encryption method will be detected automatically.
		
		\param iEncryptionMethod
			One of the CZipCryptograph::EncryptionMethod values.
		
		\returns
			\c false if selected encryption method is not supported or a 
			file is opened for compression; \c true otherwise.

		\see
			<a href="kb">0610201627</a>
		\see
			SetPassword
		\see
			CZipCryptograph::EncryptionMethod
		\see
			GetEncryptionMethod		
	 */
	bool SetEncryptionMethod(int iEncryptionMethod = CZipCryptograph::encStandard);


	/**	
		Gets the current encryption method.
		\returns
			One of CZipCryptograph::EncryptionMethod values.

		\see
			SetEncryptionMethod		
	 */
	int GetEncryptionMethod() const
	{
		return m_iEncryptionMethod;
	}

	/**
		Sets the internal buffer sizes. No buffer size can be set smaller than 1024.
		Use this method before opening the archive. The optimal size for 
		the write buffer for a segmented archive is the size of the volume.

		\param iWriteBuffer
			The write buffer size. See also CZipStorage::m_iWriteBufferSize.

		\param iGeneralBuffer
			A buffer used in extracting, compressing, deleting, getting (#GetFromArchive)
			files, renaming and replacing. See also CZipInternalInfo::m_iBufferSize.

		\param iSearchBuffer
			A buffer used in searching for the central directory. See also CZipStorage::m_iLocateBufferSize.
			
		\see
			GetAdvanced
	*/
	void SetAdvanced(int iWriteBuffer = 65536, int iGeneralBuffer = 65536, int iSearchBuffer = 32768);

	
	/**
		Retrieves buffer sizes as set with the #SetAdvanced method.
	
		\param piWriteBuffer
				
		\param piGeneralBuffer
	
		\param piSearchBuffer

		\see
			SetAdvanced
	 */
	void GetAdvanced(int* piWriteBuffer = NULL, int* piGeneralBuffer = NULL, int* piSearchBuffer= NULL)
	{
		if (piWriteBuffer)
			*piWriteBuffer = 	m_storage.m_iWriteBufferSize;
		if (piGeneralBuffer)
			*piGeneralBuffer = m_info.m_iBufferSize;
		if (piSearchBuffer)
			*piSearchBuffer = m_storage.m_iLocateBufferSize;
	}

	/**
		Values used for specifying a callback type in the #SetCallback method.
		You can assign several values to the same callback object (use the logical \c OR).
		\see
			<a href="kb">0610231200</a>
		\see
			SetCallback
		\see
			CZipActionCallback
	*/
	enum CallbackType
	{
		/**
			\brief .
			Not used.
			You can use it for your own purposes.			
		*/
		cbNothing	= 0x0000,

		/**
			\brief .
			Compressing a file.
			The callback called when adding a file with one of the #AddNewFile methods.
		*/
		cbAdd		= 0x0001,

		/**
			\brief .
			Moving a file from a temporary archive.
			The callback called while adding a file (only on a segmented archive) when the smartness level contains
			CZipArchive::zipsmCheckForEff or CZipArchive::zipsmCheckForEffInMem
			and if the just compressed file is being moved from a temporary place (file or memory) to the archive.
		*/
		cbAddTmp	= 0x0002,

		/**
			\brief .
			Storing a file.
			The callback called while adding a file and if its compressing has proven to be inefficient
			and it is now being stored (instead of compressed) in the archive. The smartness level must 
			contain CZipArchive::zipsmCheckForEff or CZipArchive::zipsmCheckForEffInMem. The archive can be
			segmented or not.
		*/
		cbAddStore	= 0x0004,	

		/**
			\brief .
			Extracting a file.
			The callback called when extracting a file with one of the #ExtractFile methods.
		*/
		cbExtract	= 0x0008,

		/**
			\brief .
			Counting data to process before deleting.
			The callback called before the actual deletion takes place and the map of holes and continuous
			areas is being created. It is safe to abort the operation (by returning \c false from
			the callback method).
		*/
		cbDeleteCnt	= 0x0010,

		/**
			\brief .
			Deleting files.
			The callback called when moving data while deleting file(s) with the #DeleteFile
			method or one of the #DeleteFiles methods.
		*/
		cbDelete	= 0x0020,

		/**
			\brief .
			Testing a file.
			The callback called when testing a file with the #TestFile method.
		*/
		cbTest		= 0x0040,

		/**
			\brief .
			Saving the central directory.
			The callback called when saving the central directory with the CZipCentralDir::Write method
			(usually on close or flush). It is safe to abort the operation on a segmented archive - 
			the saved part of the central directory will be removed from disk.
		*/
		cbSave		= 0x0080,

		/**
			\brief .
			Getting a file from another archive.
			The callback called when using one of the #GetFromArchive methods.
		*/
		cbGet		= 0x0100,

		/**
			\brief .
			Renaming a file.
			The callback called when during renaming a file there is a need to make less or more space 
			for the new filename.
		*/
		cbRename	= 0x0200,

		/**
			\brief .
			Replacing a file.
			The callback called when moving data while replacing files to make less or more space 
			for the new file.
		*/
		cbReplace	= 0x0400,

		/**
			\brief .
			Reserved.
			You can declare your own callback types above this value.
		*/
		cbNextValue = 0x0800,
		
		/**
			\brief .
			All sub-actions.
			Represents the sub-actions callbacks - they are called as a part of bigger actions
			(#cbAddTmp | #cbAddStore | #cbDeleteCnt | #cbReplace).
		*/
		cbSubActions= cbAddTmp | cbAddStore | cbDeleteCnt | cbReplace,

		/**
			\brief .
			Main callbacks.
			Represents the main action callbacks (#cbAdd | #cbExtract | #cbDelete | #cbTest | #cbSave | #cbGet | #cbRename).
		*/
		cbActions	= cbAdd | cbExtract | cbDelete | cbTest | cbSave | cbGet | cbRename,

		/**
			\brief .
			All values.
			Use this value to call one callback object for all callback types.
		*/
		cbAll		= cbActions | cbSubActions
	};


    /**
		Registers the callback object to receive specified notifications.		

		\param pCallback
			The callback object to receive notifications.
			Set it to \c NULL to stop receiving the selected notifications.

		\param iWhich
			The callback type to register for (or unregister from). Can be one or more of the #CallbackType values.

		\see
			<a href="kb">0610231200</a>
		\see
			CZipActionCallback
		\see
			#CallbackType
		\see
			GetCallback
		
     */
	void SetCallback(CZipActionCallback* pCallback = NULL, int iWhich = cbAll);

	/**
		Gets the callback object registered for a given notification.
		\param iWhich
			The callback type.
		\return
			The callback object set previously with #SetCallback.
		\see
			<a href="kb">0610231200</a>
		\see
			SetCallback
	*/
	CZipActionCallback* GetCallback(CallbackType iWhich)
	{
		return m_callbacks.Get(iWhich);
	}

	/**
		Sets the callback object used during operations on a segmented archive to change disks.
		Set it before opening the archive. If you open a spanned archive and don't set the callback object, 
		the exception CZipException::noCallback will be thrown.

		Callback object's method CZipSpanCallback::Callback is called when there is a need for a disk change.
		Calling CZipArchive methods from inside this method may result in an unexpected behavior.
		\param pCallback
			The address of the callback object. Set it to \c NULL to clear the callback.

		\see
			<a href="kb">0610051553</a>
		\see CZipSpanCallback
	*/
	void SetSpanCallback(CZipSpanCallback* pCallback = NULL){m_storage.m_pChangeDiskFunc = pCallback;}


	
	/**
		Archive open modes used in the #Open(LPCTSTR , int , ZIP_U32_U64 )
		and #Open(CZipAbstractFile&, int ) methods.
	*/
	enum OpenMode
	{
		zipOpen,			///< Opens an existing archive.
		zipOpenReadOnly,	///< Opens an existing archive as a read only file. This mode is intended to use in a self extract code or opening archive on a storage without the write access (e.g. CD-ROMS). If you try to modify the archive in this mode, an exception will be thrown.
		zipCreate,			///< Creates a new archive.
		zipCreateSegm		///< Creates a segmented archive.
	};



	/**
		Opens or creates a zip archive.

		The archive creation mode depends on the \a iMode and \a uVolumesSize values:
		- If \a iMode == #zipCreateSegm and \a uVolumeSize is \c ZIP_AUTODETECT_VOLUME_SIZE then creates a spanned archive.
		- If \a iMode == #zipCreateSegm and \a uVolumeSize > 0 then creates a split archive.
		- If \a iMode == #zipOpen and the existing archive is a segmented archive
			then CZipStorage::spannedArchive mode is assumed if the archive is on a removable device
			or CZipStorage::splitArchive otherwise. If you want to open a split archive on a removable device, 
			set \a uVolumeSize to a value different from 0 (under Linux, the device is always assumed to be removable due to lack of 
			implementation of the ZipPlatform::IsDriveRemovable method).
		- If \a iMode == #zipCreate then \a uVolumeSize doesn't matter.

		\param	szPathName
			The path to the archive.

		\param	iMode
			Can be one of the #OpenMode values.

		\param	uVolumeSize
			The volume size in a split archive. The size of the volume may be from 1 to 4,294,967,295.
		The bigger this value is - the faster is creation and extraction of segmented archives,
		because there is no volume changes.
		
		\note
			Throws exceptions.

		\see
			<a href="kb">0610231446</a>
		\see
			<a href="kb">0610241003</a>
		\see
			<a href="kb">0610051553</a>
		\see
			Open(CZipAbstractFile&, int);
		\see
			GetSegmMode
	*/
	void Open(LPCTSTR szPathName, int iMode = zipOpen, ZIP_U32_U64 uVolumeSize = 0);


	/**
		Opens or creates an archive in memory. The CZipAbstractFile object is not closed
		after closing the archive, so that it is possible to work with it afterwards.

		\param	af
			\c CZipAbstractFile object to store the archive data.

		\param	iMode
			The open mode. The following values are valid:  #zipOpen, #zipOpenReadOnly, #zipCreate.

		\note
			Throws exceptions.

		\see
			<a href="kb">0610231924</a>
		\see
			Open(LPCTSTR, int, ZIP_U32_U64);		
	*/
	void Open(CZipAbstractFile& af, int iMode = zipOpen);

	/**
		Sets the path fragment to be removed from the beginning of the full path when adding or extracting a file.

		Use it if you don't want to use the full path (by setting the \a bFullPath parameter in #AddNewFile or #ExtractFile to \c true)
		and you don't want to remove the whole path neither (be setting the same parameter to \c false), but only a specific beginning.
		Use it after opening the archive and before calling the #AddNewFile or #ExtractFile methods.

		\param	szPath
			The string that you want to be removed from the beginning of the path of the file
			in the archive. Set it to \c NULL stop removing the path beginning.

		\note Set the case-sensitivity with the #SetCaseSensitivity method.

		\see
			AddNewFile 
		\see
			ExtractFile
		\see
			GetRootPath

	*/
	void SetRootPath(LPCTSTR szPath = NULL);


    /**
		Gets the value set previously with the #SetRootPath method.

		\return
			The current value of the #m_szRootPath field.

		\see
			SetRootPath
     */
	CZipString GetRootPath()const 
	{
		return m_szRootPath;
	}

	/**
		The levels of smartness of the adding files action (#AddNewFile).
		\note If you use #zipsmCheckForEff, you should use 
		#zipsmNotCompSmall as well, because the small file will be surely larger 
		after compression, so that you can add it not compressed straight away. The compression level 
		is always ignored for a directory and set to 0.
	*/
	enum Smartness
	{
		zipsmLazy			= 0x0000,		///< All the smartness options turned off.
		zipsmCPassDir		= 0x0001,		///< Clears the password for directories.
		zipsmCPFile0		= 0x0002,		///< Clears the password for files of 0 size.

		/**
			Does not compress files smaller than 5 bytes - they are always stored larger than uncompressed.
		*/
		zipsmNotCompSmall	= 0x0004,

		/**
			\brief . 
			Checks whether the compressed file is larger than uncompressed and if so, remove it and store 
			without the compression. In a segmented archive, a temporary file is used for that: 
			if the file compression is efficient, the data is not compressed again, but moved from 
			the temporary file to the archive. You can use the #SetTempPath method to set the path where the 
			file will be created or you can let the library figure it out (see #SetTempPath).
			If the the library could not find enough free space for the temporary file, 
			the compression goes the usual way.
		*/
		zipsmCheckForEff	= 0x0008,

		/**
			Combine it with #zipsmCheckForEff or use #zipsmCheckForEffInMem, the temporary file 
			will be created in memory then. This flag is effective also when replacing files 
			(see the note at CZipAddNewFileInfo::m_uReplaceIndex).
		*/
		zipsmMemoryFlag		= 0x0010,

		/**
			\brief .
			The same as #zipsmCheckForEff, but the temporary file is created created in memory instead.
			It has the meaning only in a segmented archive, not segmented archives don't need a temporary file.
		*/
		zipsmCheckForEffInMem = zipsmMemoryFlag | zipsmCheckForEff,
		zipsmSmartPass	= zipsmCPassDir | zipsmCPFile0,			///< The password policy (a combination of #zipsmCPassDir and #zipsmCPFile0).
		zipsmSmartAdd = zipsmNotCompSmall | zipsmCheckForEff,	///< Smart adding (a combination of #zipsmNotCompSmall and #zipsmCheckForEff).
		zipsmSafeSmart = zipsmSmartPass | zipsmNotCompSmall,    ///< Safe smart (smartest without checking for efficiency).
		zipsmSmartest = zipsmSmartPass | zipsmSmartAdd,			///< Smartness at its best.
		zipsmInternal01		= 0xf000   ///< Intended for the internal use only.
	};

	
	/**
		Adds a new file to the opened archive.

		\param info
			See CZipAddNewFileInfo for more information.

		\return	
			If \c false then the file was not added (in this case, you can still try to add other files).
		
		\note 
			- If you abort the operation from a callback object, while adding a file in a not segmented archive, the added data will be removed from the archive.
			- Throws exceptions. When an exception is thrown, you may need to call #CloseNewFile with \a bAfterException set to \c true, to make the archive reusable.
		
		\see
			<a href="kb">0610231446</a>
		\see
			<a href="kb">0610231200</a>
		\see
			<a href="kb">0610231924</a>
		\see
			AddNewFile(LPCTSTR, LPCTSTR, int, int, unsigned long)
		\see 
			AddNewFile(LPCTSTR, int, bool, int, unsigned long)
		\see 
			AddNewFile(CZipMemFile&, LPCTSTR, int, int, unsigned long)
		\see
			SetCallback
	*/
	bool AddNewFile(CZipAddNewFileInfo& info);


	/**
		Adds a new file to the opened archive.
		See the #AddNewFile(CZipAddNewFileInfo& ) method.
		The parameters are equivalent to CZipAddNewFileInfo member variables.
	
	*/
	bool AddNewFile(LPCTSTR lpszFilePath, int iComprLevel = -1, bool bFullPath = true,
		int iSmartLevel = zipsmSafeSmart, unsigned long nBufSize = 65536);


	/**
		Adds a new file to the opened archive.
		See the #AddNewFile(CZipAddNewFileInfo& ) method.
		The parameters are equivalent to CZipAddNewFileInfo member variables.
	*/
	bool AddNewFile(LPCTSTR lpszFilePath,
							 LPCTSTR lpszFileNameInZip,
                             int iComprLevel = -1,                             
							 int iSmartLevel = zipsmSafeSmart,
                             unsigned long nBufSize = 65536);

	/**
		Adds a new file to the opened archive.
		See the #AddNewFile(CZipAddNewFileInfo& ) method.
		The parameters are equivalent to CZipAddNewFileInfo member variables.
	*/
	bool AddNewFile(CZipMemFile& mf,
							 LPCTSTR lpszFileNameInZip,
                             int iComprLevel = -1,                             
							 int iSmartLevel = zipsmSafeSmart,
                             unsigned long nBufSize = 65536);



	/**
		Adds a new file to the opened archive. The archive cannot be 
		an existing (at the moment of opening the archive) segmented archive,
		because modifying such an archive is not possible with this version.

		\param	header
			The structure that provides additional information about the added file and serves as a template for
			the properties of the new file. The following fields are used:
			\li CZipFileHeader::m_uMethod - the compression method. Can be 0 (storing) or \c Z_DEFLATE (deflating);
			otherwise \c Z_DEFLATE is assumed.
			\li CZipFileHeader::m_uModDate and CZipFileHeader::m_uModTime - the modification time. Use CZipFileHeader::SetTime to set them.
				If \a lpszFilePath is not \c NULL these fields are overwritten and updated automatically.
			\li CZipFileHeader::m_uExternalAttr - the attributes of the file. Use #SetFileHeaderAttr to set them.
				If \a lpszFilePath is not \c NULL this field is overwritten and updated automatically.
			\li \c CZipFileHeader::m_pszFileName - the filename (may be with a path) to be stored inside the archive
				to represent this file. Use CZipFileHeader::SetFileName to set it.
			\li CZipFileHeader::m_uLocalComprSize - predicted compressed size. Set it when using the Zip64 functionality and not using 
				encryption nor segmentation - you may set it to the size of the file that you want to compress.
				This is for prediction if the Zip64 extensions are needed, but you should not worry too much about setting 
				this value, The ZipArchive Library will fix the headers if necessary - it just may save some processing.
			\li \c CZipFileHeader::m_pszComment - the file comment. Use CZipFileHeader::SetComment to set it.
			\li CZipFileHeader::m_aLocalExtraData - the local extra fields.
			\li CZipFileHeader::m_aCentralExtraData - the central extra fields.
		
		Other fields are ignored - they are updated automatically.
		If the method returns \c true, \link #SetSystemCompatibility 
		system compatibility \endlink for this object is set to the correct value.
		Additionally if \a lpszFilePath was not \c NULL, the attributes and 
		the time fields are filled with information retrieved from 
		the file pointed by \a lpszFilePath.

		\param	iLevel
			The level of compression (-1 value and the values from 0 to 9).
			The are several preset values for the compression level:
			- \c Z_DEFAULT_COMPRESSION	: -1 (equals 6)
			- \c Z_NO_COMPRESSION		: 0
			- \c Z_BEST_SPEED			: 1
			- \c Z_BEST_COMPRESSION		: 9			

		\param	lpszFilePath
			The path to the file to retrieve the date stamp and attributes from.
			These values are stored inside the archive.

		\return
			\c false in the following cases:
		\li The \a lpszFilePath is not \c NULL and the file attributes and data was not correctly retrieved.
		\li A file is already opened for extraction or compression.
		\li The archive is an existing segmented archive.
		\li The maximum file count inside the archive has already been reached
		(65,535 for a standard archive and \c 0xFFFFFFFFFFFFFFFF for an archive in the Zip64 format).

		\c true otherwise.

		\note
			Throws exceptions.

		\see
			<a href="kb">0610231446|advanced</a>
		\see
			<a href="kb">0610242300</a>
		\see 
			WriteNewFile
		\see 
			CloseNewFile
	*/
	bool OpenNewFile(CZipFileHeader& header, int iLevel = Z_DEFAULT_COMPRESSION, LPCTSTR lpszFilePath = NULL)
	{
		return OpenNewFile(header, iLevel, lpszFilePath, ZIP_FILE_INDEX_UNSPECIFIED);
	}

	/**
		Compresses the contents of the buffer and write it to a new file.

		\param	pBuf
			The buffer containing the data to be compressed and written.

		\param	iSize
			The number of bytes to be written from \a pBuf.

		\return
			\c false, if the new file hasn't been opened yet; \c true otherwise.

		\note
			Throws exceptions.

		\see
			<a href="kb">0610231446|advanced</a>
		\see
			OpenNewFile
		\see
			CloseNewFile
	*/
	bool WriteNewFile(const void *pBuf, DWORD iSize);

	/**
		Closes the new file in the archive.
		\return
			\c false, if there is no new file opened; \c true otherwise.

		\param bAfterException 
			If \c true, the new file will be closed without writing anything. Set it to \c true
			after an exception was thrown. Use it also this way if an exception other than 
			CZipException::abortedSafely was thrown from one of the #AddNewFile methods.

		\note
			Throws exceptions.

		\see
			<a href="kb">0610231446|advanced</a>
		\see
			OpenNewFile
		\see 
			WriteNewFile
	*/
	bool CloseNewFile(bool bAfterException = false);

	/**
		Acquires a file with the given index from another archive. 
		The compressed data of the file from another archive is copied 
		without decompressing to the current archive.

		\param zip 
			The opened archive to get the file from (must not be a segmented archive).

		\param	uIndex
			A zero-based index of the file to get from the \a zip archive.

		\param lpszNewFileName
			The new filename to replace the old one from the \a zip archive. Can be \c NULL to leave the filename the same.

		\param uReplaceIndex
			The same as CZipAddNewFileInfo::m_uReplaceIndex. Can be \c ZIP_FILE_INDEX_UNSPECIFIED.
			
		\param bKeepSystComp
			If \c false, then the system compatibility of the file from the \a zip archive
			is converted to the current archive system compatibility, if they differ. Otherwise
			the source system compatibility is copied.

		\return
			\c false, if the operation could not be performed (either of archives is closed, 
		a file inside either of archives is opened, \a zip archive is segmented or the current
		archive is an existing segmented archive.

		\note
			Throws exceptions. When an exception is thrown, you may need to call #CloseNewFile with 
			\a bAfterException set to \c true, to make the archive reusable.

		\note
			It is safe to abort the action (by returning \c false from the CZipCallback::Callback) 
			in a not segmented archive and when no replacing is taking place. The file that is not 
			entirely added is removed from the archive then.
		
		\see
			<a href="kb">0610231446|get</a>
		\see
			<a href="kb">0610231200</a>
		\see
			GetFromArchive(CZipArchive&, CZipIndexesArray&, bool)
		\see
			GetFromArchive(CZipArchive&, CZipStringArray&, bool)		
		\see
			SetCallback
		\see
			SetAdvanced 
	*/
	bool GetFromArchive(CZipArchive& zip, ZIP_U16_U64 uIndex, LPCTSTR lpszNewFileName = NULL, ZIP_U16_U64 uReplaceIndex = ZIP_FILE_INDEX_UNSPECIFIED, bool bKeepSystComp = false)
	{
				
		m_info.Init();
		bool bRet;
		try
		{
			bRet = GetFromArchive(zip, uIndex, lpszNewFileName, uReplaceIndex, bKeepSystComp, GetCallback(cbGet));
		}
		catch(...)
		{
			m_info.ReleaseBuf();
			throw;
		}
		m_info.ReleaseBuf();
		if (bRet && m_bAutoFlush)
			Flush();

		return bRet;
	}

	/**
		Acquires files with the given indexes from another archive. 
		The compressed data of the files from another archive is copied 
		without decompressing to the current archive.

		\param zip 
			The opened archive to get the file from (must not be a segmented archive).

		\param	aIndexes
			An array of zero-based indexes of the files to acquire from the \a zip archive.

		\param bKeepSystComp
			If \c false, then the system compatibility of the file from the \a zip archive
			is converted to the current archive system compatibility, if they differ. Otherwise
			the source system compatibility is copied.

		\return
			\c false, if the operation could not be performed (either of archives is closed, 
		a file inside either of archives is opened, \a zip archive is segmented or the current
		archive is an existing segmented archive.

		\note
			Throws exceptions. When an exception is thrown, you may need to call #CloseNewFile with 
			\a bAfterException set to \c true, to make the archive reusable.

		\note
			It is safe to abort the action (by returning \c false from the CZipCallback::Callback) 
			in a not segmented archive and when no replacing is taking place. The file that is not 
			entirely added is removed from the archive then.
		
		\see
			<a href="kb">0610231446|get</a>
		\see
			<a href="kb">0610231200</a>
		\see
			GetFromArchive(CZipArchive&, ZIP_U16_U64, LPCTSTR, ZIP_U16_U64, bool)
		\see
			GetFromArchive(CZipArchive&, CZipStringArray&, bool)		
		\see
			SetCallback
		\see
			SetAdvanced 
	*/
	bool GetFromArchive(CZipArchive& zip, CZipIndexesArray &aIndexes, bool bKeepSystComp = false);
	
	/**
		Acquires files with the given names from another archive. 
		The compressed data of the files from another archive is copied 
		without decompressing to the current archive.

		\param zip 
			The opened archive to get the file from (must not be a segmented archive).

		\param	aNames
			An array of filenames to acquire from the \a zip archive.

	\param bKeepSystComp
			If \c false, then the system compatibility of the file from the \a zip archive
			is converted to the current archive system compatibility, if they differ. Otherwise
			the source system compatibility is copied.

		\return
			\c false, if the operation could not be performed (either of archives is closed, 
		a file inside either of archives is opened, \a zip archive is segmented or the current
		archive is an existing segmented archive.

		\note
			Throws exceptions. When an exception is thrown, you may need to call #CloseNewFile with 
			\a bAfterException set to \c true, to make the archive reusable.

		\note
			It is safe to abort the action (by returning \c false from the CZipCallback::Callback) 
			in a not segmented archive and when no replacing is taking place. The file that is not 
			entirely added is removed from the archive then.

		\note
			This method calls #GetIndexes on the \a zip archive.
		
		\see
			<a href="kb">0610231446|get</a>
		\see
			<a href="kb">0610231200</a>
		\see
			GetFromArchive(CZipArchive&, ZIP_U16_U64, LPCTSTR, ZIP_U16_U64, bool)
		\see
			GetFromArchive(CZipArchive&, CZipIndexesArray&, bool)		
		\see
			SetCallback
		\see
			SetAdvanced 
	*/
	
	bool GetFromArchive(CZipArchive& zip, CZipStringArray &aNames, bool bKeepSystComp = false)
	{
		CZipIndexesArray indexes;
		zip.GetIndexes(aNames, indexes);
		return GetFromArchive(zip, indexes, bKeepSystComp);
		
	}

	/**
		Gets indexes of the files with names stored in \a aNames and puts them into \a aIndexes.
		If a filename was not found, \c ZIP_FILE_INDEX_NOT_FOUND is inserted at the appropriate 
		position in \a aIndexes.

		\param	aNames
			An array of filenames inside the archive.
		\param aIndexes
			An array of indexes to be found.

		\note 
			Use the #SetCaseSensitivity method to set case-sensitivity.

		\see
			<a href="kb">0610242025|findfast</a>
		\see 
			EnableFindFast
	*/
	void GetIndexes(const CZipStringArray &aNames, CZipIndexesArray& aIndexes);

	/**
		Extracts a file from the archive.
		The argument \a lpszNewName may point to a full path and is influenced by \a bFullPath.
		If \a lpszNewName contains a drive specification then the drive part is removed
		unless CZipArchive::m_bRemoveDriveLetter is set to \c false.

		\param	uIndex
			The index of the file to extract.

		\param	lpszPath
			The directory to extract the file to. May not be \c NULL.

		\param	bFullPath
			In this paragraph the source path means:
			\li The full filename stored in the archive, if \a lpszNewName is \c NULL
			\li \a lpszNewName, if it is not \c NULL

			The meaning of \a bFullPath is following:
			\li If it is \c true, then the method extracts using the full source path.
			In this case the resulting file path is \a lpszPath plus the source path. 
			If #m_szRootPath is set (with #SetRootPath), then before adding the source path
			to \a lpszPath, from the source path is removed the part that matches #m_szRootPath
			(matching is performed starting from the beginning of both strings).
			\li If it is \c false, then the destination file path is \a lpszPath plus 
			the filename only extracted from the source path.

		\param	lpszNewName 
				The new name of the file after extraction. 
				If \c NULL, the original filename stored in the archive is used.
				May include a path information, but if \a bFullPath is \c false, 
				only the filename is extracted from this value.

		\param	nBufSize 
			The size of the buffer used while file operations.

		\return
			\c true if successful; \c false otherwise.

		\note 
		- To extract files which filenames that match a specified pattern, use the #FindMatches method.
		- Throws exceptions.
		
		\see
			<a href="kb">0610241003</a>
		\see
			<a href="kb">0610231200</a>
		\see 
			ExtractFile(ZIP_U16_U64, CZipMemFile&, bool, DWORD)
		\see
			FindMatches
		\see
			SetCallback
	*/
	bool ExtractFile(ZIP_U16_U64 uIndex, LPCTSTR lpszPath, bool bFullPath = true,
		LPCTSTR lpszNewName = NULL, DWORD nBufSize = 65536);

	
	/**
		The same as #ExtractFile(ZIP_U16_U64, LPCTSTR, bool, LPCTSTR, DWORD )
		but instead to a physical file, this method decompresses data into a CZipMemFile object.

		\param	uIndex
			The index of the file to extract.

		\param mf
			The memory file to receive the decompressed data.

		\param bRewind
			If \c true, the memory file pointer is positioned at the beginning of the compressed data after compression.
			The rewind operation is performed even if extraction was aborted, but rewinding will not take 
			place, if other exception than CZipException::abortedAction or CZipException::abortedSafely was thrown in the meantime.

		\param	nBufSize 
			The size of the buffer used while file operations.

		\note 
		- Writing of the decompressed data starts at the current position of the memory file. 
		Keep this in mind, when you pass a CZipMemFile object that already contains data 
		(or has a buffer attached) - its contents may be overwritten.
		- If you try to extract a directory, the method will return \c false.

		\see
			<a href="kb">0610231924</a>
		\see
			ExtractFile(ZIP_U16_U64, LPCTSTR, bool, LPCTSTR, DWORD )
	*/
	bool ExtractFile(ZIP_U16_U64 uIndex, CZipMemFile& mf, bool bRewind = true, DWORD nBufSize = 65536);

	/**
		Opens the file with the given index in the archive for extracting.
		Not successful opening of the file doesn't lock the whole archive, so 
		you can try to open another one (after catching an exception, if it was 
		thrown)
		\param	uIndex
			The index of the file to open.

		\return
			\c true, if successful; \c false otherwise.

		\throws CZipException
			with the CZipException::badPassword code, if the file is encrypted 
			and the password was not set.

		\see
			<a href="kb">0610241003|advanced</a>
		\see
			ReadFile
		\see
			CloseFile
	*/
	bool OpenFile(ZIP_U16_U64 uIndex);

	/**
		Reads data from the currently opened file and decompresses it to \a pBuf.
		\param	pBuf
			The buffer to receive the decompressed data.

		\param	uSize
			The size of \a pBuf.

		\return
			The number of bytes read.

		\see
			<a href="kb">0610241003|advanced</a>
		\see
			OpenFile
		\see
			CloseFile

		\note Throws exceptions.
	*/
	DWORD ReadFile(void *pBuf, DWORD uSize);

	/**
		Closes the file opened for extraction in the archive and copy its date and 
		attributes to the file pointed by \a lpszFilePath.

		\param	lpszFilePath 
			The path of the file to have the date and attributes information updated.
			Make sure you have the read access to this file.

		\param bAfterException
			Set it to \c true, when you call this method after an exception was
			thrown, to allow further operations on the archive.

		\return
			One of the following values:
			-  \c 1 : The operation was successful.
			-  \c 0 : There is no file opened.
			-  \c -1 : Some bytes were left to uncompress - probably due to a bad password or a corrupted archive.
			-  \c -2 : Setting extracted file date and attributes was not successful.

		\see
			<a href="kb">0610241003|advanced</a>
		\see
			OpenFile
		\see
			ReadFile
		\see
			CloseFile(CZipFile&)

		\note Throws exceptions.
	*/
	int CloseFile(LPCTSTR lpszFilePath = NULL, bool bAfterException = false);

	/**
		Closes \a file and then calls the #CloseFile(LPCTSTR, bool) method using the path of \a file as an argument.		
		Don't call this method, if an exception was thrown before, call the #CloseFile(LPCTSTR, bool) method instead.

		\param	file
			A \c CZipFile object of the extracted file. It must be opened.

		\return	
			The same values as the #CloseFile(LPCTSTR, bool) method.

		\note Throws exceptions.

		\see
			<a href="kb">0610241003|advanced</a>
		\see
			OpenFile
		\see
			ReadFile
		\see
			CloseFile(LPCTSTR, bool)
	*/
	int CloseFile(CZipFile &file);

	/**
		Tests the file with the given index for the integrity.
 		The method throws exceptions but performs all the necessary cleanup
		before, so that the next file can be tested after catching the exception.

		\param	uIndex
			The index of the file to test.

		\param	uBufSize
			The size of the buffer used during extraction.

		\return
			\c false, if the incorrect action has been taken (when #OpenFile or #GetFileInfo returned \c false or \a uBufSize is 0); \c true otherwise.
		If the file didn't passed the test or there was a disk I/O error or the supplied password was incorrect, an exception is thrown.

		\note
			Throws exceptions.

		\see
			<a href="kb">0610241003</a>
		\see
			SetCallback		
	*/
	bool TestFile(ZIP_U16_U64 uIndex, DWORD uBufSize = 65536);

	/**
		Deletes the file with the given index from the archive.		
		If you plan to delete more than one file, use the #DeleteFiles(CZipIndexesArray&) or 
		#DeleteFiles(const CZipStringArray&) method. These methods are optimized for deleting multiple files.

		\param	uIndex
			A zero-based index of the file to delete.

		\note
			Throws exceptions.

		\see
			<a href="kb">0610231944|delete</a>
		\see	
			DeleteFiles(CZipIndexesArray&)
		\see
			DeleteFiles(const CZipStringArray&)
		\see
			SetCallback
		\see
			FindMatches
	*/
	void DeleteFile(ZIP_U16_U64 uIndex);

	/**
		Deletes files from the archive.	
		Sorts \a aIndexes in the ascending order.

		\param	aIndexes
			An array of zero-based indexes of the files to delete.

		\note 
		- To remove files which filenames match a specified pattern, use the #FindMatches method before to find the indexes.
		- Throws exceptions.


		\see
			<a href="kb">0610231944|delete</a>
		\see
			DeleteFile
		\see
			DeleteFiles(const CZipStringArray& )
		\see
			SetCallback
		\see
			FindMatches
	*/
	void DeleteFiles(CZipIndexesArray& aIndexes);


	/**
		Delete files from the archive.

		\param	aNames
			An array of filenames to delete; 	

		\note 
		- Use the #SetCaseSensitivity method to set case-sensitivity.
		- This method uses Find Fast (see <a href="kb">0610242025|findfast</a> for more information).
		- Throws exceptions.

		\see
			<a href="kb">0610231944|delete</a>
		\see	
			DeleteFile
		\see
			DeleteFiles(CZipIndexesArray&)
		\see
			SetCallback
		\see
			EnableFindFast
	*/
	void DeleteFiles(const CZipStringArray& aNames);

	/**
		Sets the global comment in the archive.

		\param	lpszComment
			The comment to set.

		\return
			\c false, if the archive is closed or it is an existing segmented archive;
			\c true otherwise.

		\note
			Throws exceptions.
		\see
			<a href="kb">0610231944|comment</a>
		\see
			GetGlobalComment
	*/
	bool SetGlobalComment(LPCTSTR lpszComment);


	/**
		Gets the global comment for the archive.

		\return
			The global comment or an empty string if the archive is closed.

		\see
			<a href="kb">0610231944|comment</a>
		\see
			SetGlobalComment
	*/
	CZipString GetGlobalComment()const ;


	/**
		Sets the comment for the file with the given index.

		\param	uIndex
			A zero-based index of the file.

		\param	lpszComment
			The comment to set.

		\return
			\c false, if the comment change is impossible; \c true otherwise.

		\note
			Throws exceptions.

		\see
			<a href="kb">0610231944|comment</a>
		\see
			GetFileComment
	*/
	bool SetFileComment(ZIP_U16_U64 uIndex, LPCTSTR lpszComment);

	/**
		Gets the comment for the file with the given index.

		\param uIndex
			A zero-based index of the file.

		\return
			The file comment or an empty string, if the archive is closed.

		\see
			<a href="kb">0610231944|comment</a>
		\see
			SetFileComment
	*/
	CZipString GetFileComment(ZIP_U16_U64 uIndex) const
	{
		const CZipFileHeader* info = GetFileInfo(uIndex);
		return info == NULL ? _T("") : info->GetComment();
	}

	/**
		Gets the path of the currently opened archive segment.
		\return
			The current segments' path or an empty string, if the archive is closed.

		\see
			<a href="kb">0610051553</a>
	*/
	CZipString GetArchivePath()const;

	/**
		Gets the current disk.

		This method is useful while working with a segmented archive in creation to find out
		how many disks were already created. To find out how many disks are in an existing segmented archive,
		use the #GetCentralDirInfo method.

		\return
			A one-based number of the current disk or 0, if there is no current disk (the archive is closed).

		\see
			<a href="kb">0610051553</a>
	*/
	ZIP_U16_U32 GetCurrentDisk()const ;

	/**
		Gets the segmentation mode of the current archive.

		\return
			One of the following values:
			- \c -2 : An existing split archive.
			- \c -1 : An existing spanned archive.
			- \c 0 : No archive segmentation or one part only.
			- \c 1 : A spanned archive in creation.
			- \c 2 : A split archive in creation.

		\see
			<a href="kb">0610051553</a>
	*/
	int GetSegmMode()const 
	{
		return m_storage.m_iSegmMode * m_storage.IsSegmented();
	}

	/**
		Case-sensitivity values used as the \a iCaseSensitive argument in the #FindFile method.
	*/
	enum FFCaseSens
	{
		/**
			\brief .
			Uses the default case-sensitivity as set with the #SetCaseSensitivity method.
			If the Find Fast array was built before with a different case-sensitivity,
			it is rebuilt again, if it hasn't been built so far, it is built now with the
			default case-sensitivity.
		*/
		ffDefault,

		/**
			\brief .
			Performs a case-sensitive search. If the CZipArchive is non-case-sensitive,
			a less effective search is performed. It does not rebuild the Find Fast array,
			but if the array hasn't been built yet, it is built now as \b non-case-sensitive
			(you can use \c SetCaseSensitivity(true) and then #ffDefault to build it as case-sensitive).
		*/
		ffCaseSens,

		/**
			\brief .
			Performs a non-case-sensitive search. If the CZipArchive is case-sensitive,
			a less effective search is performed. It does not rebuild the Find Fast array,
 			but if the array hasn't been built yet, it is build now as \b case-sensitive
			(you can use \c SetCaseSensitivity(false) and then #ffDefault to build it as non-case-sensitive).
		*/
		ffNoCaseSens
	};

	/**
		Finds a file in the archive.	
		This method enables the Find Fast feature, if it is not enabled already.

		\param	lpszFileName
			The name of the file to be found in the archive. If the file in the archive is stored with the 
			path information, you must specify it here or set \a bFileNameOnly to \c true.
			Use the same path separators as they are defined for your system as default
			(\e "\" for Windows and \e "/" for Unix/Linux).

		\param iCaseSensitive
			It can be one of the #FFCaseSens values.

		\param bFileNameOnly
			If \c true, the method tries to find a filename without a path (a less effective search is performed).
			If you wish to find a directory name, do not end it with a path separator.
			The path separator is required, if you set \a bFileNameOnly to \c false.

		\note 
			Use the #SetCaseSensitivity method to set the global case-sensitivity.

		\return
			The index of the file, if the file was found; \c ZIP_FILE_INDEX_NOT_FOUND if the file was not found.

		\see
			<a href="kb">0610242025|findfast</a>		
		\see
			EnableFindFast
	*/
	ZIP_U16_U64 FindFile(LPCTSTR lpszFileName, int iCaseSensitive = ffDefault, bool bFileNameOnly = false);

	/**
		Gets the information about the file with the given index.
		The data is copied to \a fhInfo. This may not be 
		optimal for querying large number of file. To avoid copying data, see the 
		#GetFileInfo(ZIP_U16_U64) method.

		\param	fhInfo
			The object to receive data.

		\param	uIndex
			A zero-based index of the file to get the information about.

		\return
			\c true if successful; \c false otherwise.
		\see
			<a href="kb">0610242128|file</a>
		\see
			GetFileInfo(ZIP_U16_U64)
		\see
			GetFileInfo(ZIP_U16_U64) const
		\see
			operator[](ZIP_U16_U64)			
		\see
			operator[](ZIP_U16_U64) const
	*/
	bool GetFileInfo(CZipFileHeader& fhInfo, ZIP_U16_U64 uIndex) const;

	/**
		Gets the information about the file with the given index.
		This method provides a direct access to the file data. You should not modify 
		the returned object apart from the extra fields in the central directory (CZipFileHeader::m_aCentralExtraData).

		\param	uIndex
			A zero-based index of the file to get the information about.

		\return
			A CZipFileHeader object that directly points to a central directory entry in memory;
			\c NULL if the archive is closed or there is no such index in the archive.

		\see
			<a href="kb">0610242128|file</a>
		\see
			GetFileInfo(CZipFileHeader&, ZIP_U16_U64) const
		\see
			GetFileInfo(ZIP_U16_U64) const
		\see
			operator[](ZIP_U16_U64)	
		\see
			operator[](ZIP_U16_U64) const

	*/
	CZipFileHeader* GetFileInfo(ZIP_U16_U64 uIndex);

	/**
		Gets the information about the file with the given index.
		This method provides a direct access to the file data. This method does not allow
		modification of the returned object.

		\param	uIndex
			A zero-based index of the file to get the information about.

		\return
			A CZipFileHeader object that directly points to a central directory entry in memory;
			\c NULL if the archive is closed or there is no such index in the archive.

		\see
			<a href="kb">0610242128|file</a>
		\see
			GetFileInfo(CZipFileHeader&, ZIP_U16_U64) const
		\see
			GetFileInfo(ZIP_U16_U64)
		\see
			operator[](ZIP_U16_U64)	
		\see
			operator[](ZIP_U16_U64) const

	*/
	const CZipFileHeader* GetFileInfo(ZIP_U16_U64 uIndex) const;

	/**
		Gets the information about the file with the given index.
		This method provides a direct access to the file data. You should not modify 
		the returned object apart from the extra fields in the central directory (CZipFileHeader::m_aCentralExtraData).

		\param	uIndex
			A zero-based index of the file to get the information about.

		\return
			A CZipFileHeader object that directly points to a central directory entry in memory;
			\c NULL if the archive is closed or there is no such index in the archive.

		\see
			<a href="kb">0610242128|file</a>
		\see
			GetFileInfo(CZipFileHeader&, ZIP_U16_U64) const
		\see
			GetFileInfo(ZIP_U16_U64)
		\see
			GetFileInfo(ZIP_U16_U64) const
		\see
			operator[](ZIP_U16_U64) const		

	*/
	CZipFileHeader* operator[](ZIP_U16_U64 uIndex)
	{
		return GetFileInfo(uIndex);
	}

	/**
		Gets the information about the file with the given index.
		This method provides a direct access to the file data. This method does not allow
		modification of the returned object.

		\param	uIndex
			A zero-based index of the file to get the information about.

		\return
			A CZipFileHeader object that directly points to a central directory entry in memory;
			\c NULL if the archive is closed or there is no such index in the archive.

		\see
			<a href="kb">0610242128|file</a>
		\see
			GetFileInfo(CZipFileHeader&, ZIP_U16_U64) const
		\see
			GetFileInfo(ZIP_U16_U64)
		\see
			GetFileInfo(ZIP_U16_U64) const
		\see
			operator[](ZIP_U16_U64)
	*/
	const CZipFileHeader* operator[](ZIP_U16_U64 uIndex) const
	{
		return GetFileInfo(uIndex);
	}


	/**
		Gets the number of files in the archive.

		\param	bOnlyFiles
			If \c true, directories are not included in the total count; otherwise all entries are included.

		\return
			The number of files in the archive.
			
		\see
			<a href="kb">0610242128|file</a>
	*/
	ZIP_U16_U64 GetCount(bool bOnlyFiles = false)const 
	{
		ZIP_U16_U64 iTotalCount = (ZIP_U16_U64) m_centralDir.m_headers.GetSize();
		if (bOnlyFiles)
		{
			ZIP_U16_U64 iCount = 0;
			for (ZIP_U16_U64 i = 0; i < iTotalCount; i++)
				if (!m_centralDir.m_headers[i]->IsDirectory())
					iCount++;
			return iCount;
		}
		else
			return iTotalCount;
	}
	/** 
		Calculates the actual size (in bytes) currently occupied by the archive.

		\return
			The sum of the sizes: the number of bytes already written to the file, the number of bytes in the write buffer and the whole size of the central directory.
			If the archive or a volume is closed, the return value is \c 0.

		\see
			<a href="kb">0610242128|archive</a>			
	*/
	ZIP_U32_U64 GetOccupiedSpace() const
	{
		if (IsClosed(true) || IsClosed(false))
		{
			ZIPTRACE("%s(%i) : ZipArchive or the current volume file is closed.\n");
			return 0;
		}
		return m_storage.GetOccupiedSpace() + m_centralDir.GetSize(true);
	}

	/**
		The values used in the #Close method.
	*/
	enum CloseAfterException
	{
		/**
			\brief .
			Normal closing. Use it, when no exception was thrown while processing the archive.
		*/
		afNoException,

		/**
			\brief .
			Use when an exception was thrown.
			The #Close method doesn't write any data but performs necessary cleaning to reuse the 
			CZipArchive object for another archive processing.
		*/
		afAfterException,

		/**	
			\brief .
			Use when an exception was thrown.
			The #Close method writes the central directory structure to the archive, 
			so that the archive should be usable.
		*/
		afWriteDir
	};

	/**
		Closes the archive.

		\param	iAfterException
			One of the #CloseAfterException values.

		\param bUpdateTimeStamp
			If \c true, the method sets the modification date of the zip file to the date of the newest file in the archive.
			In a segmented archive, only the last segment file will have the time stamp updated.
			You can use this option even without performing any additional processing on the archive, just open and close the archive.

		\note
			Throws exceptions. Does not throw any exceptions, if \a iAfterException is set to the #afAfterException value.

		\see
			<a href="kb">0610222049</a>
		\see
			IsClosed
	*/
	void Close(int iAfterException = afNoException, bool bUpdateTimeStamp = false);


	/**
		Tests if the whole archive or the current volume is closed.

		\param	bArchive <BR>
			If \c true, test for the whole archive. If \c false, test for the current volume only.

		\return
			\c true if a file closed; \c false otherwise.

		\see
			Close
	*/
	bool IsClosed(bool bArchive = true)const
	{
		return m_storage.IsClosed(bArchive);
	}

	/**
		Writes the central directory to the archive and flushes the internal buffers to the disk.
		After that the archive is finalized on the disk, but you can still modify it, if it is not a segmented archive.		

		\note 
		- This method cannot be used on existing segmented archives - they are not modifiable.
		- If you have an archive with a huge central directory, it'll influence the performance calling this function without a reason.
		- Throws exceptions.

		\see
			<a href="kb">0610231446|flush</a>		
		\see
			SetAutoFlush
		\see
			GetAutoFlush
		\see
			GetSegmMode
	*/
	void Flush();


	/**
		Sets the CZipArchive object to call the #Flush method after each operation that modifies the archive (apart from changing central extra fields).
		It is useful when you want to prevent the loss of data in case of the program crash - the zip file will then be finalized on the disk.
		Use it after opening the archive.

		\note 
		- You can set auto-flush only for non-segmented archives.
		- If you have an archive with a huge central directory, enabling auto-flush will influence the performance.

		\see
			<a href="kb">0610231446|flush</a>
		\see
			Flush	
		\see
			GetAutoFlush
	*/
	void SetAutoFlush(bool bAutoFlush = true);
	

	/**
		Gets the current auto-flush value.

		\return 
			The current auto-flush value.

		\see
			<a href="kb">0610231446|flush</a>
		\see
			SetAutoFlush
		\see
			Flush	
		
	*/
	bool GetAutoFlush()const {return m_bAutoFlush;}

	/**
		Sets the system compatibility of the archive. Use it after opening the archive,
		but before adding a new file or using the #SetFileHeaderAttr method.
		If the filename code page is standard for the current system compatibility, 
		the ZipArchive Library will change the filename code page to be default for the 
		new system compatibility (see <a href="kb">0610051525</a> for more information).
		
		\param iSystemComp
			The new system compatibility to use. It can be one of the ZipCompatibility::ZipPlatforms values.
		\return
			\c false, if the value \a iSystemComp is not supported 
			or it is not possible to set the value at this moment; \c true otherwise.

		\see
			<a href="kb">0610231446|compatibility</a>
		\see
			GetSystemCompatibility
		\see
			CZipFileHeader::GetSystemCompatibility
		\see
			ZipCompatibility::ZipPlatforms
		\see
			ZipPlatform::GetSystemID
	*/
	bool SetSystemCompatibility(int iSystemComp);

	/**
		Gets the system compatibility of the current archive.			
		\return	
			One of the ZipCompatibility::ZipPlatforms values.

		\see
			<a href="kb">0610231446|compatibility</a>
		\see
			SetSystemCompatibility
		\see
			CZipFileHeader::SetSystemCompatibility
		\see
			ZipCompatibility::ZipPlatforms
		\see
			ZipPlatform::GetSystemID		
	*/
	int GetSystemCompatibility() const {return m_iArchiveSystCompatib;}



	/**
		Sets the attributes for the CZipFileHeader object to be used
		in the #OpenNewFile(CZipFileHeader&, int, LPCTSTR) method.

		This special procedure is required, because the attributes value depends 
		on the system compatibility of the archive.

		\param	header
			The object to have the attributes set.

		\param	uAttr
			The attributes to set.

		\throws CZipException
			with the CZipException::platfNotSupp code, if the system compatibility
			is not supported by the ZipArchive Library.

		\see
			<a href="kb">0610231446|advanced</a>
		\see
			SetSystemCompatibility
	*/
	void SetFileHeaderAttr(CZipFileHeader& header, DWORD uAttr)const;

	/**
		Gets the underlying archive storage object. It's for an advanced use - you will normally never need it.

		\return
			The pointer to CZipStorage.

		\see
			CZipStorage
	*/
	CZipStorage* GetStorage(){return &m_storage;}


	/**
		Sets whether to control the allocation and freeing of memory by the Zlib library.
		Use is before opening a file in the archive.

		\param bDetect
			\c true, if you want to control the memory allocation and freeing; \c false otherwise.
			
	*/
	void SetDetectZlibMemoryLeaks(bool bDetect)
	{
		if (m_iFileOpened != nothing)
		{
			ZIPTRACE("CZipArchive::SetDetectZlibMemoryLeaks: Set it before opening a file in the archive");
			return;
		}
		m_bDetectZlibMemoryLeaks = bDetect;
		
	}

	/**
		Sets the current settings that control storing of filenames and comments in the archive.

		\param settings
			The settings to set.

		\see
			<a href="kb">0610051525</a>
		\see
			SetStringStoreSettings(UINT, bool, UINT)
		\see
			SetStringStoreSettings(UINT, bool)
		\see
			ResetStringStoreSettings
		\see
			GetStringStoreSettings		
	*/
	void SetStringStoreSettings(const CZipStringStoreSettings& settings)
	{
		m_centralDir.m_stringSettings = settings;
	}

	/**
		Sets the current settings that control storing of filenames and comments in the archive.

		\param uFileNameCodePage
			The code page for filenames.
	
		\param bStoreNameInExtraData
			If \c true, the encoded filenames are stored in central extra fields.
	
		\param uCommentCodePage
			The code page for comments.

		\see
			<a href="kb">0610051525</a>
		\see
			SetStringStoreSettings(const CZipStringStoreSettings&)
		\see
			SetStringStoreSettings(UINT, bool)
		\see
			ResetStringStoreSettings
		\see
			GetStringStoreSettings		
	*/
	void SetStringStoreSettings(UINT uFileNameCodePage, bool bStoreNameInExtraData, UINT uCommentCodePage)
	{
		m_centralDir.SetStringStoreSettings(uFileNameCodePage, bStoreNameInExtraData, uCommentCodePage);
	}

	/**
		Sets the current settings that control storing of filenames and comments in the archive.
		The code page for comments stays the same when calling this method.

		\param uFileNameCodePage
			The code page for filenames.
	
		\param bStoreNameInExtraData
			If \c true, the encoded filenames are stored in central extra fields.
	
		\see
			<a href="kb">0610051525</a>
		\see
			SetStringStoreSettings(const CZipStringStoreSettings&)
		\see
			SetStringStoreSettings(UINT, bool, UINT)
		\see
			ResetStringStoreSettings
		\see
			GetStringStoreSettings		
	*/
	void SetStringStoreSettings(UINT uFileNameCodePage, bool bStoreNameInExtraData = false)
	{
		SetStringStoreSettings(uFileNameCodePage, bStoreNameInExtraData,  m_centralDir.m_stringSettings.m_uCommentCodePage);
	}
	
	/**
		Sets the current settings that control storing of filenames and comments in the archive to their default values.

		\see
			<a href="kb">0610051525</a>
		\see
			SetStringStoreSettings(const CZipStringStoreSettings&)
		\see
			SetStringStoreSettings(UINT, bool, UINT)
		\see
			SetStringStoreSettings(UINT, bool)
		\see
			GetStringStoreSettings		
	*/
	void ResetStringStoreSettings()
	{
		SetStringStoreSettings(CZipStringStoreSettings::GetDefaultNameCodePage(m_iArchiveSystCompatib), false, CZipStringStoreSettings::GetDefaultCommentCodePage());
	}

	/**
		Gets the current settings that control storing of filenames and comments in the archive.
	
		\returns
			The current string store settings.

		\see
			<a href="kb">0610051525</a>
		\see
			SetStringStoreSettings(const CZipStringStoreSettings&)
		\see
			SetStringStoreSettings(UINT, bool, UINT)
		\see
			SetStringStoreSettings(UINT, bool)
		\see
			ResetStringStoreSettings		
		\see
			CZipFileHeader::GetStringStoreSettings
	 */
	CZipStringStoreSettings& GetStringStoreSettings()
	{
		return m_centralDir.m_stringSettings;
	}

	/**
		Enables or disables fast searching for files inside the archive using filenames.

		\see
			<a href="kb">0610242025|findfast</a>
		\see
			FindFile
		\see
			GetIndexes
		\see
			GetFindFastIndex
	*/
	void EnableFindFast(bool bEnable = true);


    /**
		Allows to retrieve the order of sorted files after you enabled the Find Fast feature with #EnableFindFast.
       
       \param iFindFastIndex
			The index of the file in the sorted array.

       \return 
	        The index of the file in the central directory. You can use the return value
			in other methods that require the file index (such as #GetFileInfo).
			This method returns \c -1, if you have not called 
			#EnableFindFast before or the archive is closed or the \a iFindFastIndex
			is out of range.

		\see
			<a href="kb">0610242025|findfast</a>		
		\see
			EnableFindFast
     */
	ZIP_U16_U64 GetFindFastIndex(ZIP_U16_U64 iFindFastIndex) const
	{
		if (IsClosed())
		{
			ZIPTRACE("CZipArchive::GetFindFastIndex: ZipArchive not yet opened.\n");
			return ZIP_FILE_INDEX_UNSPECIFIED;
		}
		
		return m_centralDir.GetFindFastIndex(iFindFastIndex);
	}


	/**
		Sets the temporary path used when compressing files and there is a need for a temporary archive.
		Temporary files are used when replacing or when compressing a segmented archive with the #zipsmCheckForEff flag.
		If the path is not set or it does not exists or there is not enough free space in it, 
		then the ZipArchive Library first tries the the following folders
		in the given order for sufficient free space:
		\li system default temporary directory,
		\li the current directory.

		If all above fails, no temporary file is created and the compression goes the usual way.

		\param lpszPath
			The path used for storing temporary files. Set it to \c NULL, to clear the temporary path and let the 
			ZipArchive Library figure it out (see above).

		\param bForce
			If \a lpszPath is not \c NULL and this parameter set to \c true
			the directory is created, if it doesn't exist. Otherwise the the ZipArchive Library
			tries to figure out the location for temporary files by itself (see above).

		\see
			GetTempPath
		\see
			AddNewFile
		\see
			Smartness
	*/
	void SetTempPath(LPCTSTR lpszPath = NULL, bool bForce = true);

	/**   
		Gets the current temporary path used when compressing files.
       
		\return
			The current temporary path.

		\see
			SetTempPath
     */
	CZipString GetTempPath()const 
	{
		return m_szTempPath;
	}

	/**
		The values used in the #PredictFileNameInZip method.
	*/
	enum Predict
	{
	
		prDir,  ///< If \a lpszFilePath is a directory, appends a separator.
		prFile, ///< Treats \a lpszFilePath as a common file.
		prAuto  ///< Treats \a lpszFilePath as a directory only if it has a path separator appended.				
	};

    /**
       Predicts the filename as it would be stored in the archive, when given parameters would be used with 
	   one of the #AddNewFile methods. The method takes into account the root path set with the #SetRootPath method. 
	   You can use this method to eliminate duplicates before adding a list of files.
       
       \param lpszFilePath
			The file path, the same as CZipAddNewFileInfo::m_szFilePath.
	   \param bFullPath
			The same as CZipAddNewFileInfo::m_bFullPath.
       \param iWhat
			One of the #Predict values to interpret \a lpszFilePath correctly.
       \return
			The filename as it would be stored in the archive.
     */
	CZipString PredictFileNameInZip(LPCTSTR lpszFilePath, bool bFullPath, int iWhat = prAuto)const ;

	/**
		Calculates the maximum number of bytes that the file represented by #CZipFileHeader would occupy in the current archive.

		You need to set the following members in the structure:
			\li File attributes (use the #SetFileHeaderAttr method). It is for determining whether the file is a directory or not.
			You can use ZipPlatform::GetFileAttr.
			\li The filename as it would appear in the archive (use CZipFileHeader::SetFileName). You can use #PredictFileNameInZip.
			\li The compressed size of the file (use CZipFileHeader::m_uLocalComprSize). You can use an uncompressed file size
			here for the maximum value estimate.

		Additionally you may set:
			- A file comment (use the CZipFileHeader::SetComment method).
			- Extra fields (use CZipFileHeader::m_aLocalExtraData and CZipFileHeader::m_aCentralExtraData).

		\param fh
			A template object pre-filled with data.

		\return
			The maximum number of bytes the file would occupy in the archive.

		\note
		- The method takes into account if the current archive is a segmented archive.
		- If the archive has a password set, the method will assume that the file would be stored encrypted in the archive (extra bytes are added then).
		- The method calls CZipFileHeader::PrepareData
		- Zip64 only: The method takes into account the current file pointer position in the archive to determine the need for the Zip64 extensions 
			and updates CZipFileHeader::m_uDiskStart and CZipFileHeader::m_uOffset.
		- The method does not take into account a situation when a file would be compressed, but mostly stored blocks would be emitted by the Huffman compression engine.
		In this case extra 5 bytes are added per a single stored block. You should remove the file and store it instead when it happens (see #zipsmCheckForEff).

		\see
			PredictMaximumFileSizeInArchive(LPCTSTR, bool) const
	*/
	ZIP_U32_U64 PredictMaximumFileSizeInArchive(CZipFileHeader& fh) const;

	 /**
		Calls the #PredictMaximumFileSizeInArchive(CZipFileHeader&) const method. 
		Before calling, fills the #CZipFileHeader structure with the filename as it 
		would appear in the archive and sets the proper file size (unless \a lpszFilePath is a directory).

		\param lpszFilePath
			A path to the file for which you want to predict the maximum size it would occupy.

		\param bFullPath 
			The same as CZipAddNewFileInfo::m_bFullPath.

		\return
			The maximum number of bytes the file would occupy in the archive.

		\see
			PredictMaximumFileSizeInArchive(CZipFileHeader&) const
	 */
	ZIP_U32_U64 PredictMaximumFileSizeInArchive(LPCTSTR lpszFilePath, bool bFullPath) const;
	

	/**
		Checks if the filename of the given file will be duplicated in the archive, if added to the archive with the given parameters.

		\param lpszFilePath
			The file path. You normally use it in one of the #AddNewFile methods.

	   \param bFullPath
			The same as CZipAddNewFileInfo::m_bFullPath.

		\param bFileNameOnly
			If \c true, the method assumes that the filename is duplicated if only the name part (no path)
			is the same (\a bFullPath is ignored), otherwise the whole filename with a path is taken into account.

		\param iWhat
			One of the #Predict values to interpret \a lpszFilePath correctly.

		\return
			The zero-based index of the file in the archive which filename would be duplicated, 
			or \c ZIP_FILE_INDEX_NOT_FOUND, if the filename would be unique.
	*/
	ZIP_U16_U64 WillBeDuplicated(LPCTSTR lpszFilePath, bool bFullPath, bool bFileNameOnly = false, int iWhat = prAuto);

    /**
       Predicts the full resulting filename with path after extraction. 
	   The parameters (except for the first) are in the form you'd pass
	   to the #ExtractFile(ZIP_U16_U64 , LPCTSTR , bool , LPCTSTR , DWORD ) method.
	   The method takes into account the root path set with the #SetRootPath method.

       \param lpszFileNameInZip
			The filename of the file inside the archive (may be \c NULL if \a lpszNewName is not \c NULL).

       \param lpszPath

       \param bFullPath

       \param lpszNewName       

       \return
			The predicted resulting file path.
     */
	CZipString PredictExtractedFileName(LPCTSTR lpszFileNameInZip, LPCTSTR lpszPath, bool bFullPath, LPCTSTR lpszNewName = NULL)const ;


/**
		Removes the root path from \a zpc.

		\param	zpc
			The path to have the common beginning removed from.

		\see
			SetRootPath
*/
	CZipString TrimRootPath(CZipPathComponent& zpc) const ;

    /**
		Removes \a lpszBeginning from the beginning of \a szPath. Both argument are
		considered to be paths - they must match up to the path separator.
       
		\param lpszBeginning
			The beginning to remove.

		\param szPath
			The path to have the beginning removed.

		\param pCompareFunction 
			The compare function used (see #m_pZipCompare).

		\return
			\c true, if the path beginning was removed; \c false otherwise.
     */
	static bool RemovePathBeginning(LPCTSTR lpszBeginning, CZipString& szPath, ZIPSTRINGCOMPARE pCompareFunction);

	/**
		Sets the default archive case-sensitivity. The default CZipArchive case-sensitivity depends
		on the system and is set as follows:
			\li on Windows:	\c false
			\li on Linux:	\c true
			
		Calling this method affects the following methods:

		- #FindFile
		- #GetIndexes
		- #FindMatches		
		- #EnableFindFast
		- #TrimRootPath
		- #DeleteFiles(const CZipStringArray&)
			
		\param bCaseSensitive
			The case-sensitivity to be used.

		\note
			Set it before using one of the mentioned methods or leave it as it was set by default.
	*/
	void SetCaseSensitivity(bool bCaseSensitive) 
	{
		m_bCaseSensitive = bCaseSensitive;
		m_pZipCompare = GetCZipStrCompFunc(bCaseSensitive);
	}

	/**
		Gets the central directory information.
		\param info
			The object to retrieve information data.

		\see
			GetCentralDirSize
	*/
	void GetCentralDirInfo(CZipCentralDir::Info& info)const;
	

	/**
		Gets the central directory size.

		\param bWhole
			If \c true, the return value includes the size of file headers.

		\return
			The size of the central directory.

		\see
			GetCentralDirInfo
	*/
	ZIP_U32_U64 GetCentralDirSize(bool bWhole = true) const
	{
		return m_centralDir.GetSize(bWhole);
	}

	/**
		Gets a value indicating whether the archive can be modified or not.
		An archive can be read-only when it is an existing segmented archive
		or it was opened with the #zipOpenReadOnly flag.

		\return
			\c true if the archive is read-only; \c false otherwise.
	*/
	bool IsReadOnly(){return m_storage.IsReadOnly();}

	/**
		Sets the number of non-archive bytes that are present before the actual archive in the archive file 
		(such as a self-extract stub).
		The library usually tries to automatically calculate this value, but this may not be possible
		under some conditions.
		
		\param uCount
			The number of bytes before the actual archive.
				
		\see
			GetBytesBeforeZip
	 */
	void SetBytesBeforeZip(ZIP_U32_U64 uCount = 0)
	{
		if (!IsClosed())
		{
			ZIPTRACE("%s(%i) : Set it before opening the archive.\n");
			return;
		}
		m_storage.m_uBytesBeforeZip = uCount;
	}

	/**
		Gets the number of non-archive bytes that are present before the actual archive in the archive file 
		(such as a self-extract stub).

		\returns
			The number of bytes before the actual archive.
		\see
			SetBytesBeforeZip
	 */
	ZIP_U32_U64 GetBytesBeforeZip()
	{
		return m_storage.m_uBytesBeforeZip;
	}

    /**
		Sets a value indicating whether checksum is checked for the files being extracted or not.
		This affects also testing of archives. This method is useful when working with Java <sup><small>TM</small></sup> Archives (jar)
		(set \a bIgnore to \c true).

		\param bIgnore
			If you set it to \c true, a checksum is not checked for the files being tested or extracted.        
     */
	void SetIgnoreCRC(bool bIgnore = true){m_bIgnoreCRC = bIgnore;}

	/**
		A class used in the wildcard pattern matching.
		This class is based on code by J. Kercheval, created 01/05/1991
		and available as a public domain at http://www.snippets.org.

		\see
			<a href="kb">0610242025|match</a>
	*/
	class ZIP_API CWildcard  
	{
	public:
		
		enum Match
		{
			matchNone,			///< For internal use.
			matchValid,			///< Valid match.
			matchEnd,			///< Premature end of the pattern string.
			matchAbort,			///< Premature end of the text string.
			matchRange,			///< Match failure on the \c [..] construct.
			matchLiteral,		///< Match failure on a literal match
			matchPattern		///< Bad pattern.
		};
		
		enum Pattern 
		{
			patternEmpty = -4,	///< The \c [..] construct is empty
			patternClose,		///< There is no end bracket in the \c [..] construct.
			patternRange,		///< Malformed range in the \c [..] construct.
			patternEsc,			///< Literal escape at the end of the pattern.
			patternValid,		///< Valid pattern.
		};
		
		
		/**
			Matches \a lpszText against the pattern.
			A match means the entire \a lpszText is used up in matching.
			Set the pattern with the #SetPattern method or in the constructor.
			
			\param lpszText
				The string to match against the pattern.

			\param iRetCode
				If not \c NULL, receives one of #Match values indicating a return code.

			\return
				\c true, if \a lpszText matches the pattern.

			\see
				SetPattern
		*/
		bool IsMatch(LPCTSTR lpszText, int* iRetCode = NULL);

		/**
			Gets a value indicating if \a lpszPattern has any special wildcard characters.

			\param lpszPattern
				The pattern to test.
       
			\return
				\c true, if the pattern has wildcard characters; \c false otherwise.

		 */
		static bool IsPattern(LPCTSTR lpszPattern);

		/**
			Tests \a lpszPattern for validity.
       
			\param lpszPattern
				The pattern to test.

			\param iErrorType
				If not \c NULL, receives one of the #Pattern values indicating a return code.

			\return
				\c true, if \a lpszPattern is a well formed regular expression according
				to the CWildcard class syntax (see #SetPattern); \c false otherwise.
		 */
		static bool IsPatternValid(LPCTSTR lpszPattern, int* iErrorType = NULL);
		
		/**
			Matches \a lpszText against \a lpszPattern.

			A match means the entire \a lpszText is used in matching.
		  
			\param lpszPattern
				The pattern to match.

			\param lpszText
				The string to match against the pattern.

			\return 
				One of #Match values.

			\see
				SetPattern
		*/
		static int Match(LPCTSTR lpszPattern, LPCTSTR lpszText);
		
		/**	
			Initializes a new instance of the CWildcard class.
		*/
		CWildcard(){}

		/**
			Initializes a new instance of the CWildcard class.

			\param lpszPattern
				The pattern to use in matching.

			\param bCaseSensitive
				The case-sensitivity of matching.

			\see
				<a href="kb">0610242025|match</a>
		*/
		CWildcard(LPCTSTR lpszPattern, bool bCaseSensitive)
		{
			SetPattern(lpszPattern, bCaseSensitive);
		}

		virtual ~CWildcard(){}
		
		/**
			Sets the current pattern

			\param lpszPattern
				The pattern used in matching.

			\param bCaseSensitive
				The case-sensitivity of matching.

			\see
				<a href="kb">0610242025|match</a>
		*/
		void SetPattern(LPCTSTR lpszPattern, bool bCaseSensitive)
		{
			m_szPattern = lpszPattern;
			m_bCaseSensitive=bCaseSensitive;
			if (!bCaseSensitive)
				m_szPattern.MakeLower();
		}
		operator LPCTSTR()
		{
			return (LPCTSTR)m_szPattern;
		}
	protected:
		bool m_bCaseSensitive;		
		static int MatchAfterStar(LPCTSTR p , LPCTSTR t);
		CZipString m_szPattern;
	};


    /**
		Finds indexes of the files, which filenames match the specified pattern. The indexes are stored in the \a ar array.
		The indexes can be used then e.g. in deleting or extracting files.
       
       \param lpszPattern
			The pattern to match. The case-sensitivity of the pattern 
			is set to the global archive case-sensitivity (set with the #SetCaseSensitivity method).

       \param ar
			The array which will contain the resulting indexes. The contents of \a ar are not cleared, but the 
			indexes are appended to it.

	   \param bFullPath
			\li If \c true, the method matches the filename with path (if present) of the file.
			If the file is a directory, end it with a path separator or use a pattern that will recognize it.
			\li If \c false, the method matches only the name of the file. If the file is a directory, 
			do not end it with a path separator.

		\see
			<a href="kb">0610242025|match</a>
		\see
			SetCaseSensitivity
		\see
			CWildcard
       
     */
	void FindMatches(LPCTSTR lpszPattern, CZipIndexesArray& ar, bool bFullPath = true);


    /**
		Renames the file with the given index in the archive.

		\param uIndex
			A zero-based index of the file to rename.

		\param lpszNewName
			The new name for the file.

		\return 
			\c true, if the file was renamed; \c false otherwise.

		\note
			Throws exceptions.
		\see
			<a href="kb">0610231944|rename</a>
     */
	bool RenameFile(ZIP_U16_U64 uIndex, LPCTSTR lpszNewName);
	
	/**
		Removes the central directory from the disk. 
		You may then modify central extra fields and write the central directory back to the disk afterwards (use the #Close method).

		\return 
			\c true, if the central directory was successfully removed; \c false otherwise.

		\see
			<a href="kb">0610242300|central</a>
	*/
	bool RemoveCentralDirectoryFromDisk()
	{
		if (IsClosed())
		{
			ZIPTRACE("%s(%i) : ZipArchive is closed.\n");
			return false;
		}
	
		if (m_storage.IsSegmented())
		{
			ZIPTRACE("%s(%i) : You cannot remove the central directory from a segmented archive.\n");
			return false;
		}
		m_centralDir.RemoveFromDisk();
		return true;
	}

	/**
		If \c true, the drive letter is removed from the filename stored inside the archive when adding
		a new file to the archive or extracting an existing one.
		It affects #AddNewFile, #ExtractFile, #PredictFileNameInZip, #PredictExtractedFileName,
		#WillBeDuplicated methods. The default value is \c true.
	*/
	bool m_bRemoveDriveLetter;

protected:

	/**
		See the description of #OpenNewFile(CZipFileHeader&, int, LPCTSTR)

		\param header
		\param iLevel
		\param lpszFilePath
		\param uReplaceIndex
			For internal use only.
		\see
			OpenNewFile(CZipFileHeader&, int, LPCTSTR)
	*/
	bool OpenNewFile(CZipFileHeader & header, int iLevel, LPCTSTR lpszFilePath, ZIP_U16_U64 uReplaceIndex);

	/**
		A structure for the internal use only. Clears the password if necessary and
		restores it later (also in case of an exception).
	*/
	struct ZIP_API CZipSmClrPass
	{
		void ClearPasswordSmartly(CZipArchive* pZip)
		{
			m_pZip = pZip;
			m_szPass = pZip->GetPassword();
			if (!m_szPass.IsEmpty())
				pZip->SetPassword();
		}
		~CZipSmClrPass()
		{
			if (!m_szPass.IsEmpty())
				m_pZip->SetPassword(m_szPass);
		}
		CZipString m_szPass;
		CZipArchive* m_pZip;
	};
	
	/**
		The storage for callback objects. A structure for the internal use only.
		\see
			SetCallback
	*/
	struct ZIP_API CZipClbckStrg : public CZipMap<CallbackType, CZipActionCallback*>		
	{
		void Set(CZipActionCallback* pCallback, CallbackType iType)
		{
			if (pCallback)
			{
				SetAt(iType, pCallback);
			}
			else
				RemoveKey(iType);
		}
		CZipActionCallback* Get(CallbackType iType)
		{
			CZipActionCallback* pCallback = NULL;
			if (Lookup(iType, pCallback))
			{
				pCallback->m_iType = iType;
				return pCallback;
			}
			else
				return NULL;
		}
	
	};
	
	
	/**
		See CZipClbckStrg.
	*/
	CZipClbckStrg m_callbacks;
	

	/**
		Writes the central directory notifying a callback object if available.
	*/
	void WriteCentralDirectory(bool bFlush = true);

	/**
		The value set with #SetCaseSensitivity.
	*/
	bool m_bCaseSensitive;

	/**
		A pointer to a method used to compare strings. 
		Can point to \c Compare, \c CompareNoCase, \c Collate or \c CollateNoCase method.		
	*/
	ZIPSTRINGCOMPARE m_pZipCompare;

	/**
		Internal data.
		\see
			CZipInternalInfo
	*/
	CZipInternalInfo m_info;

	
	/**
		Physical layer of the archive.
		\see
			CZipStorage
	*/
	CZipStorage m_storage;

	/**
		A central directory object.
		\see
			CZipCentralDir
	*/
	CZipCentralDir m_centralDir;

	/**
		The open mode of the current file inside archive.	
	*/
	enum OpenFileType
	{
		extract = -1,	///< A file is opened for extraction.
		nothing,		///< There is no file inside the archive opened.
		compress		///< A new file is opened for compression.
	};
	
	/**
		Takes one of the #OpenFileType enum values.
	*/
	char m_iFileOpened;

	/**
		The value set with #SetAutoFlush.
	*/
	bool m_bAutoFlush;

	/**
		The value set with #SetIgnoreCRC.
	*/
	bool m_bIgnoreCRC;

	/**
		The value set with #SetRootPath.
	*/
	CZipString m_szRootPath;

	/**
		The value set with #SetTempPath.
	*/
	CZipString m_szTempPath;


	/**
		Opens the archive in the given mode.
		Called by #Open(LPCTSTR, int, ZIP_U32_U64) and #Open(CZipAbstractFile&, int).
		\param	iMode
			The mode.
		\note
			Throws exceptions.
	*/
	void OpenInternal(int iMode);

	/**
		The value set with #SetSystemCompatibility.
	*/
	int m_iArchiveSystCompatib;

	/**
		The value set with #SetDetectZlibMemoryLeaks.
	*/
	bool m_bDetectZlibMemoryLeaks;

	/**
		The value set with #SetPassword.
	*/
	CZipAutoBuffer m_pszPassword;

	/**
		Frees the memory allocated by the Zlib library that hasn't been freed
		due to an error in the Zlib library (usually never happens).
	*/
	void EmptyPtrList();

	/**
		Gets the file currently opened for compression or decompression.
		\return
			The currently opened file or \c NULL, if there is no file opened.
	*/
	CZipFileHeader* CurrentFile();

	/**
		Releases the current cryptograph.
	*/
	void ClearCryptograph()
	{
		if (m_pCryptograph)
		{
			delete m_pCryptograph;
			m_pCryptograph = NULL;
		}
	}

	/**
		Creates a new cryptograph. You can override this method and implement your own cryptograph.
		\param iEncryptionMethod
			The encryption method requested.
	*/
	virtual void CreateCryptograph(int iEncryptionMethod)
	{
		ClearCryptograph();
		m_pCryptograph = CZipCryptograph::CreateCryptograph(iEncryptionMethod);
	}

	/**
		The current cryptograph.
	*/
	CZipCryptograph* m_pCryptograph;

	/**
		The value set with #SetEncryptionMethod.
	*/
	int m_iEncryptionMethod;


private:

	struct ZIP_API CZipDeleteInfo
	{
		CZipDeleteInfo(){m_pHeader = NULL; m_bDelete = false;}
		CZipDeleteInfo(CZipFileHeader* pHeader, bool bDelete)
			:m_pHeader(pHeader), m_bDelete (bDelete){}
		CZipFileHeader* m_pHeader;
		//bool operator < (const CZipDeleteInfo& di) const
		//{
		//	return m_pHeader->m_uOffset < di.m_pHeader->m_uOffset;
		//}
		//bool operator > (const CZipDeleteInfo& di) const
		//{
		//	return m_pHeader->m_uOffset > di.m_pHeader->m_uOffset;
		//}
		//bool operator == (const CZipDeleteInfo& di) const
		//{
		//	return m_pHeader->m_uOffset == di.m_pHeader->m_uOffset;
		//}
		//bool operator <= (const CZipDeleteInfo& di) const
		//{
		//	return m_pHeader->m_uOffset <= di.m_pHeader->m_uOffset;
		//}
		//bool operator >= (const CZipDeleteInfo& di) const
		//{
		//	return m_pHeader->m_uOffset >= di.m_pHeader->m_uOffset;
		//}
		bool m_bDelete;
	};

	void MakeSpaceForReplace(ZIP_U16_U64 iReplaceIndex, ZIP_U32_U64 uTotal, LPCTSTR lpszFileName);

	void MovePackedFiles(ZIP_U32_U64 uStartOffset, ZIP_U32_U64 uEndOffset, ZIP_U32_U64 uMoveBy, CZipActionCallback* pCallback, bool bForward = false);
	
	bool RemoveLast(bool bRemoveAnyway = false);

	bool GetFromArchive(CZipArchive& zip, ZIP_U16_U64 uIndex, LPCTSTR lpszNewFileName, ZIP_U16_U64 iReplaceIndex, bool bKeepSystComp, CZipActionCallback* pCallback);

	bool UpdateReplaceIndex(ZIP_U16_U64& iReplaceIndex);
	
	void CheckForError(int iErr);

	void ThrowError(int err, bool bZlib = false);

	typedef CZipPtrList<void*>::iterator CZipPtrListIter;
	CZipPtrList<void*> m_list; ///< A list holding pointers to the memory areas allocated by the Zlib library.

	static void* _zliballoc(void* opaque, UINT items, UINT size); ///< The memory allocator called by the Zlib library.
	static void _zlibfree(void* opaque, void* address); ///< The memory deallocator called by the Zlib library.

	static const TCHAR m_gszCopyright[];		
};

#if (_MSC_VER > 1000) && (defined ZIP_HAS_DLL)
	#pragma warning (pop)
#endif

#endif // !defined(ZIPARCHIVE_ZIPARCHIVE_DOT_H)
