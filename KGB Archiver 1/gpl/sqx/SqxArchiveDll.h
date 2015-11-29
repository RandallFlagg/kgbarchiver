#if !defined(__SqxArchiveDll_h)
#define __SqxArchiveDll_h

/*	Projekt SQX-SDK
	SpeedProject
	Copyright © 1995 - 2005. Alle Rechte vorbehalten.

	Datei:		SqxArchiveDll.h
	Autor:		Sven Ritter
	Datum:		21.05.2005 - 14:51:41


	UEBERBLICK
	==========
	Klassendefinition fuer CSqxArchiveDll.
*/


// ****************************************************************************
// ******                Header nur einmal pro Build includen            ******
// ****************************************************************************
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// ****************************************************************************
// ******                    Externe Klassendefinitionen                 ******
// ****************************************************************************
#include "sqx.h"


// ****************************************************************************
// ******                  Konstanten und globale Variablen              ******
// ****************************************************************************


// ****************************************************************************
// ******                    Definition zu CSqxArchiveDll                ******
// ****************************************************************************
class CSqxArchiveDll {

// Konstruktion
public:

	CSqxArchiveDll		();
	~CSqxArchiveDll		();

// Operationen
public:

	// Dll
	BOOL			LoadArchiveDll		();
	void			FreeArchiveDll		();

	// Initialisierung
	int				InitArchive			(LPCTSTR pszArchiveName, SQXCALLBACK pfnCallback, LPVOID pParam);
	void			DoneArchive			();

	// Dateilisten
	HANDLE			InitFileList		();
	void			AppendFileList		(HANDLE hFileMaskList, LPCTSTR pszFileName);
	void			DoneFileList		(HANDLE hFileMaskList);
	LPSQX_ARCLIST	InitArcFileList		();
	void			DoneArcFileList		(LPSQX_ARCLIST pArcFileList);

	// Archivfunktionen
	int				ListFiles			(HANDLE hFileMaskList, LPSQX_ARCLIST pArcFileList, LPSQX_ARCINFO pArchiveInfo);
	int				CompressFiles		(SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE hFileMaskList, LPSQX_COMPRESSOPTIONS pCompressOptions);
	int				ExtractFiles		(SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE hFileMaskList, LPSQX_EXTRACTOPTIONS pExtractOptions);
	int				DeleteFiles			(SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE hFileMaskList);
	int				TestArchive			(SQXCALLBACK pfnCallback, LPVOID pParam);
	int				RepairArchive		(LPCTSTR pszRepairedFile, LPSQX_ARCLIST pGoodFileList, LPSQX_ARCLIST pBadFileList);
	int				AddArchiveComment	(LPCTSTR pszMainComment);
	int				GetArchiveComment	(LPTSTR pszMainComment, DWORD cchMainComment);
	int				AddFileComments		(HANDLE hFileMaskList, LPCTSTR pszComment);
	int				CreateFolder		(LPCTSTR pszFolder);

// Implementation
protected:

	//
	void			ResetSqxFunctions	();

// Attribute
protected:
	
	//
	HINSTANCE				m_hInstSqxDll;							// Instanz der Dll
	HANDLE					m_hArchive;								// Archivhandle

	// Funktionen
	SqxInitArchive			m_pfnSqxInitArchive;					// SqxInitArchive
	SqxDoneArchive			m_pfnSqxDoneArchive;					// SqxDoneArchive
	SqxInitFileList			m_pfnSqxInitFileList;					// SqxInitFileList
	SqxAppendFileList		m_pfnSqxAppendFileList;					// SqxAppendFileList
	SqxDoneFileList			m_pfnSqxDoneFileList;					// SqxDoneFileList
	SqxInitArcFileList		m_pfnSqxInitArcFileList;				// SqxInitArcFileList
	SqxDoneArcFileList		m_pfnSqxDoneArcFileList;				// SqxDoneArcFileList
	SqxListFiles			m_pfnSqxListFiles;						// SqxListFiles
	SqxCompressFiles		m_pfnSqxCompressFiles;					// SqxCompressFiles
	SqxExtractFiles			m_pfnSqxExtractFiles;					// SqxExtractFiles
	SqxDeleteFiles			m_pfnSqxDeleteFiles;					// SqxDeleteFiles
	SqxTestArchive			m_pfnSqxTestArchive;					// SqxTestArchive
	SqxRepairArchive		m_pfnSqxRepairArchive;					// SqxRepairArchive
	SqxAddArchiveComment	m_pfnSqxAddArchiveComment;				// SqxAddArchiveComment
	SqxGetArchiveComment	m_pfnSqxGetArchiveComment;				// SqxGetArchiveComment
	SqxAddFileComments		m_pfnSqxAddFileComments;				// SqxAddFileComments
	SqxCreateFolder			m_pfnSqxCreateFolder;					// SqxCreateFolder
};


// ****************************************************************************
// ******                     Definition zu SqxCallback                  ******
// ****************************************************************************
int SQX_API SqxCallback(LPVOID pParam, LPSQX_CALLBACKINFO pCallbackInfo);


// ****************************************************************************
// ******                        INLINE-Definitionen                     ******
// ****************************************************************************


#endif										// __SqxArchiveDll_h


// ****************************************************************************
// ******                            END OF FILE                         ******
// ****************************************************************************