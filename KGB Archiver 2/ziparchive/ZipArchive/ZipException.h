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
* \file ZipException.h
* Interface for the CZipException class.
*
*/

#define generic _generic

#if !defined(ZIPARCHIVE_ZIPEXCEPTION_DOT_H)
#define ZIPARCHIVE_ZIPEXCEPTION_DOT_H

#if _MSC_VER > 1000
#pragma once
#pragma warning( push )
#pragma warning (disable:4702) // disable "Unreachable code" warning in Throw function in the Release mode
#endif // _MSC_VER > 1000


#include "ZipString.h"
#include "ZipBaseException.h"
#include "ZipExport.h"
 
/**
	Represents exceptions specific to the ZipArchive Library.

	\see
		<a href="kb">0610222049</a>
*/
class ZIP_API CZipException : public CZipBaseException
{
public:

	/**
		Throws an exception.
		Whether it throws an object or a pointer to it, depends 
		on the current version (STL or MFC correspondingly).

		\param	iCause
			The error cause. Takes one of the #ZipErrors values.

		\param	lpszZipName
			The name of the file where the error occurred (if applicable).
			May be \c NULL.

		\see
			<a href="kb">0610222049</a>
	*/
	static void Throw(int iCause = CZipException::generic, LPCTSTR lpszZipName = NULL)
	{
		#ifdef ZIP_ARCHIVE_MFC
			throw new CZipException(iCause, lpszZipName);
		#else
			CZipException e(iCause, lpszZipName);
			throw e;
		#endif
	}

	 
	/**
		Converts a Zlib library error code to one of the #ZipErrors values.

		\param	iZlibError
			The Zlib library error code.

		\return
			One of the #ZipErrors values corresponding to \a iZlibError.
	*/
	static int ZlibErrToZip(int iZlibError);

	/**
		Initializes a new instance of the CZipException class.

		\param	iCause
			The error cause. Takes one of the #ZipErrors values.

		\param	lpszZipName
			The name of the file where the error occurred (if applicable).
			May be \c NULL.
	*/
	CZipException(int iCause = generic, LPCTSTR lpszZipName = NULL);

	CZipException(CZipException& e)
	{
		m_szFileName = e.m_szFileName;
		m_iCause = e.m_iCause;
	}

#ifdef ZIP_ENABLE_ERROR_DESCRIPTION

    /**
		Gets the error description.

		\return 
			The error description.
     */
	CZipString GetErrorDescription();

	
    /**
		Gets the error description. This method is provided for compatibility with the MFC version (\c CException::GetErrorMessage).

		\param lpszError 
			The buffer to receive the error message.

		\param nMaxError
			The maximum number of characters \a lpszError can hold, 
			including the ending \c NULL character.

		\return 
			\c TRUE if the error string was successfully copied to \a lpszError; \c FALSE otherwise.

		\note 
			The method will not copy more than \c nMaxError  1 characters 
			to the buffer, and it always appends a \c NULL character.
			If \a lpszError is too small, the error message will be truncated.
     */
	BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, UINT* = NULL);

#endif //ZIP_ENABLE_ERROR_DESCRIPTION

	/**
		The name of the zip file where the error occurred.
	*/
	CZipString m_szFileName;

	/**
		The codes of errors thrown by the ZipArchive Library.
	*/
	enum ZipErrors
	{
		noError,			///< No error.
// 			 1 - 42 reserved for errno (from STL) values - used only in non-MFC versions
// 			 43 - 99 reserved
		generic		= 100,	///< An unknown error.
		badZipFile,			///< Damaged or not a zip file.
		badCrc,				///< Crc is mismatched.
		noCallback,			///< There is no spanned archive callback object set.
		aborted,			///< The disk change callback method returned \c false.
		abortedAction,		///< The action callback method returned \c false.
		abortedSafely,		///< The action callback method returned \c false, but the data is not corrupted.
		nonRemovable,		///< The device selected for the spanned archive is not removable.
		tooManyVolumes,		///< The limit of the maximum volumes reached.
		tooManyFiles,		///< The limit of the maximum files in an archive reached.
		tooLongData,		///< The filename, the comment or the local or central extra field of the file added to the archive is too long.
		tooBigSize,			///< The file size is too large to be supported.
		badPassword,		///< An incorrect password set for the file being decrypted.
		dirWithSize,		///< The directory with a non-zero size found while testing.
		internal,			///< An internal error.
		notRemoved,			///< Error while removing a file (under Windows call \c GetLastError() to find out more).
		notRenamed,			///< Error while renaming a file (under Windows call \c GetLastError() to find out more).
		platfNotSupp,		///< Cannot create a file for the specified platform.
		cdirNotFound,		///< The central directory was not found in the archive (or you were trying to open not the last disk of a segmented archive).
		noZip64,			///< The Zip64 format has not been enabled for the library, but is required to open the archive.
		noAES,				///< WinZip AES encryption has not been enabled for the library, but is required to decompress the archive.
		streamEnd	= 500,	///< Zlib library error.
		needDict,			///< Zlib library error.
		errNo,				///< Zlib library error.
		streamError,		///< Zlib library error.
		dataError,			///< Zlib library error.
		memError,			///< Zlib library or CZipMemFile error.
		bufError,			///< Zlib library error.
		versionError,		///< Zlib library error.
	};

	/**
		The error code - takes one of the #ZipErrors values.
	*/
	int m_iCause;

	virtual ~CZipException() throw();

protected:

#ifdef ZIP_ENABLE_ERROR_DESCRIPTION

	/**
		Gets the error description.

		\param	iCause
			The error cause. Takes one of the #ZipErrors values.

		\param bNoLoop
			If \c true, does not search for en error description, it the error code is #generic.

		\return 
			The error description.
	 */
	CZipString GetInternalErrorDescription(int iCause, bool bNoLoop = false);


	/**
	   Gets the error description based on system variables.
	   
	  \return 
			The error description.
	 */
	CZipString GetSystemErrorDescription();


#endif //ZIP_ENABLE_ERROR_DESCRIPTION

#ifdef _MFC_VER
	DECLARE_DYNAMIC(CZipException)
	#if _MSC_VER > 1000
		#pragma warning( pop )
	#endif
#endif
};


#endif // !defined(ZIPARCHIVE_ZIPEXCEPTION_DOT_H)


