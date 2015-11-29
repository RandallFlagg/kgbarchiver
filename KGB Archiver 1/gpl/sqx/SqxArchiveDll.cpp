/*	Projekt SQX-SDK
	SpeedProject
	Copyright © 1995 - 2005. Alle Rechte vorbehalten.

	Datei:		SqxArchiveDll.cpp
	Autor:		Sven Ritter
	Datum:		21.05.2005 - 14:52:07


	UEBERBLICK
	==========
	Quelltextdatei fuer die Implementation von CSqxArchiveDll.
*/


#include "StdAfx.h"
#include "SqxArchiveDll.h"


// ****************************************************************************
// ******                  DEBUG-Makro (ASSERT und VERIFY)               ******
// ****************************************************************************
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// ****************************************************************************
// ******                   DEBUG-Makro (Speicherfreigabe)               ******
// ****************************************************************************
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ****************************************************************************
// ******                  Konstanten und globale Variablen              ******
// ****************************************************************************


// ****************************************************************************
// ******                  Implementation zu CSqxArchiveDll              ******
// ****************************************************************************
CSqxArchiveDll::CSqxArchiveDll()
{
	m_hInstSqxDll = NULL; m_hArchive = NULL; ResetSqxFunctions();
}

// Aufraeumen
CSqxArchiveDll::~CSqxArchiveDll()
{
	FreeArchiveDll();
}

// Sqx-Dll laden
BOOL CSqxArchiveDll::LoadArchiveDll()
{
	// Dll ist schon geladen
	if (NULL != m_hInstSqxDll)
		return TRUE;

	// Pfad zur Dll
	TCHAR szDllFileName[MAX_PATH]; GetModuleFileName(NULL, szDllFileName, countof(szDllFileName));
	PathRemoveFileSpec(szDllFileName); 
	
	// Name der Dll
#ifdef _UNICODE
	PathAppend(szDllFileName, _T("sqx20u.dll"));
#else
	PathAppend(szDllFileName, _T("sqx20.dll"));
#endif

	// Dll laden
	m_hInstSqxDll = LoadLibrary(szDllFileName);
	if (NULL == m_hInstSqxDll)
		return FALSE;

	// Funktionen importieren
	m_pfnSqxInitArchive = (SqxInitArchive) GetProcAddress(m_hInstSqxDll, "SqxInitArchive");
	m_pfnSqxDoneArchive = (SqxDoneArchive) GetProcAddress(m_hInstSqxDll, "SqxDoneArchive");
	m_pfnSqxInitFileList = (SqxInitFileList) GetProcAddress(m_hInstSqxDll, "SqxInitFileList");
	m_pfnSqxAppendFileList = (SqxAppendFileList) GetProcAddress(m_hInstSqxDll, "SqxAppendFileList");
	m_pfnSqxDoneFileList = (SqxDoneFileList) GetProcAddress(m_hInstSqxDll, "SqxDoneFileList");
	m_pfnSqxInitArcFileList = (SqxInitArcFileList) GetProcAddress(m_hInstSqxDll, "SqxInitArcFileList");
	m_pfnSqxDoneArcFileList = (SqxDoneArcFileList) GetProcAddress(m_hInstSqxDll, "SqxDoneArcFileList");
	m_pfnSqxListFiles = (SqxListFiles) GetProcAddress(m_hInstSqxDll, "SqxListFiles");
	m_pfnSqxCompressFiles = (SqxCompressFiles) GetProcAddress(m_hInstSqxDll, "SqxCompressFiles");
	m_pfnSqxExtractFiles = (SqxExtractFiles) GetProcAddress(m_hInstSqxDll, "SqxExtractFiles");
	m_pfnSqxDeleteFiles = (SqxDeleteFiles) GetProcAddress(m_hInstSqxDll, "SqxDeleteFiles");
	m_pfnSqxTestArchive = (SqxTestArchive) GetProcAddress(m_hInstSqxDll, "SqxTestArchive");
	m_pfnSqxRepairArchive = (SqxRepairArchive) GetProcAddress(m_hInstSqxDll, "SqxRepairArchive");
	m_pfnSqxAddArchiveComment = (SqxAddArchiveComment) GetProcAddress(m_hInstSqxDll, "SqxAddArchiveComment");
	m_pfnSqxGetArchiveComment = (SqxGetArchiveComment) GetProcAddress(m_hInstSqxDll, "SqxGetArchiveComment");
	m_pfnSqxAddFileComments = (SqxAddFileComments) GetProcAddress(m_hInstSqxDll, "SqxAddFileComments");
	m_pfnSqxCreateFolder = (SqxCreateFolder) GetProcAddress(m_hInstSqxDll, "SqxCreateFolder");

	// Fehlerhafter Import
	if (NULL == m_pfnSqxInitArchive || NULL == m_pfnSqxDoneArchive || NULL == m_pfnSqxInitFileList ||
		NULL == m_pfnSqxDoneFileList || NULL == m_pfnSqxInitArcFileList || NULL == m_pfnSqxDoneArcFileList ||
		NULL == m_pfnSqxListFiles || NULL == m_pfnSqxCompressFiles || NULL == m_pfnSqxExtractFiles || 
		NULL == m_pfnSqxDeleteFiles || NULL == m_pfnSqxTestArchive || NULL == m_pfnSqxRepairArchive || 
		NULL == m_pfnSqxAddArchiveComment || NULL == m_pfnSqxGetArchiveComment || NULL == m_pfnSqxAddFileComments || 
		NULL == m_pfnSqxCreateFolder || NULL == m_pfnSqxAppendFileList)
	{
		// Dll freigeben
		FreeArchiveDll();

		// Fehler
		return FALSE;
	}

	// Alles ok
	return TRUE;
}

// Sqx-Dll freigeben
void CSqxArchiveDll::FreeArchiveDll()
{
	// Archiver freigeben
	if (NULL != m_hArchive)
	{
		DoneArchive(); 
	}

	// Dll entladen
	if (NULL != m_hInstSqxDll)
	{
		FreeLibrary(m_hInstSqxDll); m_hInstSqxDll = NULL;
	}

	// Funktionen zuruecksetzen
	ResetSqxFunctions();
}

// Dateiliste initialisieren
HANDLE CSqxArchiveDll::InitFileList()
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxInitFileList(m_hArchive);
}

// Eintrag an Dateiliste anfuegen
void CSqxArchiveDll::AppendFileList(HANDLE hFileMaskList, LPCTSTR pszFileName)
{
	ASSERT(NULL != m_hArchive);
	m_pfnSqxAppendFileList(m_hArchive, hFileMaskList, pszFileName);
}

// Dateiliste freigeben
void CSqxArchiveDll::DoneFileList(HANDLE hFileMaskList)
{
	ASSERT(NULL != m_hArchive);
	m_pfnSqxDoneFileList(m_hArchive, hFileMaskList);
}

// Archivdateiliste initialisieren
LPSQX_ARCLIST CSqxArchiveDll::InitArcFileList()
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxInitArcFileList(m_hArchive);
}

// Archivdateiliste freigeben
void CSqxArchiveDll::DoneArcFileList(LPSQX_ARCLIST pArcFileList)
{
	ASSERT(NULL != m_hArchive);
	m_pfnSqxDoneArcFileList(m_hArchive, pArcFileList);
}

// Archiv initialisieren
int CSqxArchiveDll::InitArchive(LPCTSTR pszArchiveName, SQXCALLBACK pfnCallback, LPVOID pParam)
{
	ASSERT(NULL == m_hArchive);
	return m_pfnSqxInitArchive(pszArchiveName, pfnCallback, pParam, &m_hArchive);
}

// Archiv freigeben
void CSqxArchiveDll::DoneArchive()
{
	ASSERT(NULL != m_hArchive);
	m_pfnSqxDoneArchive(m_hArchive); m_hArchive = NULL;
}

// Archivinhalt auflisten
int CSqxArchiveDll::ListFiles(HANDLE hFileMaskList, LPSQX_ARCLIST pArcFileList, LPSQX_ARCINFO pArchiveInfo)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxListFiles(m_hArchive, hFileMaskList, pArcFileList, pArchiveInfo);
}

// Dateien packen
int CSqxArchiveDll::CompressFiles(SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE hFileMaskList, LPSQX_COMPRESSOPTIONS pCompressOptions)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxCompressFiles(m_hArchive, pfnCallback, pParam, hFileMaskList, pCompressOptions);
}

// Dateien entpacken
int CSqxArchiveDll::ExtractFiles(SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE hFileMaskList, LPSQX_EXTRACTOPTIONS pExtractOptions)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxExtractFiles(m_hArchive, pfnCallback, pParam, hFileMaskList, pExtractOptions);
}

// Dateien loeschen
int CSqxArchiveDll::DeleteFiles(SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE hFileMaskList)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxDeleteFiles(m_hArchive, pfnCallback, pParam, hFileMaskList);
}

// Archiv testen
int CSqxArchiveDll::TestArchive(SQXCALLBACK pfnCallback, LPVOID pParam)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxTestArchive(m_hArchive, pfnCallback, pParam);
}

// Archiv reparieren
int CSqxArchiveDll::RepairArchive(LPCTSTR pszRepairedFile, LPSQX_ARCLIST pGoodFileList, LPSQX_ARCLIST pBadFileList)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxRepairArchive(m_hArchive, pszRepairedFile, pGoodFileList, pBadFileList);
}

// Archivkommentar setzen
int CSqxArchiveDll::AddArchiveComment(LPCTSTR pszMainComment)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxAddArchiveComment(m_hArchive, pszMainComment);
}

// Archivkommentar abfragen
int CSqxArchiveDll::GetArchiveComment(LPTSTR pszMainComment, DWORD cchMainComment)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxGetArchiveComment(m_hArchive, pszMainComment, cchMainComment);
}

// Dateikommentare hinzufuegen
int CSqxArchiveDll::AddFileComments(HANDLE hFileMaskList, LPCTSTR pszComment)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxAddFileComments(m_hArchive, hFileMaskList, pszComment);
}

// Ordner erstellen
int CSqxArchiveDll::CreateFolder(LPCTSTR pszFolder)
{
	ASSERT(NULL != m_hArchive);
	return m_pfnSqxCreateFolder(m_hArchive, pszFolder);
}

// Funktionen zuruecksetzen
void CSqxArchiveDll::ResetSqxFunctions()
{
	m_pfnSqxInitArchive = NULL; m_pfnSqxDoneArchive = NULL; m_pfnSqxInitFileList = NULL;
	m_pfnSqxDoneFileList = NULL; m_pfnSqxInitArcFileList = NULL; m_pfnSqxDoneArcFileList = NULL;
	m_pfnSqxListFiles = NULL; m_pfnSqxCompressFiles = NULL; m_pfnSqxExtractFiles = NULL; 
	m_pfnSqxDeleteFiles = NULL; m_pfnSqxTestArchive = NULL; m_pfnSqxRepairArchive = NULL; 
	m_pfnSqxAddArchiveComment = NULL; m_pfnSqxGetArchiveComment = NULL;	m_pfnSqxAddFileComments = NULL; 
	m_pfnSqxCreateFolder = NULL;
}


// ****************************************************************************
// ******                   Implementation zu SqxCallback                ******
// ****************************************************************************
int SQX_API SqxCallback(LPVOID pParam, LPSQX_CALLBACKINFO pCallbackInfo)
{
	//
	int iResult = TRUE;

	//
	switch (pCallbackInfo->dwCallbackType)
	{
		// Dateiname fuer aktuelle Operation
		case SQX_CALLBACK_FILENAME:

			// Quell- und Zieldateiname
			pCallbackInfo->pszSourceFileName;
			pCallbackInfo->pszTargetFileName;

			iResult = SQX_PROGRESS_OK;
			break;

		// Fortschritt
		case SQX_CALLBACK_PROGRESS:

			// Einzelfortschritt und Blockgroesse
			pCallbackInfo->iProgress;
			pCallbackInfo->dwlBlockSize;

			iResult = SQX_PROGRESS_OK;
			break;

		// Datei ersetzen?
		case SQX_CALLBACK_REPLACE:

			// Infos fuer beide Dateien
			pCallbackInfo->pFindDataExist;
			pCallbackInfo->pFindDataReplace;

			iResult = SQX_REPLACE_OVERWRITE;
			break;

		// Kennwort abfragen
		case SQX_CALLBACK_PASSWORD:

			// Kennwort, vorheriges eingegebenes Kennwort (falls vorhanden) und Name der Datei
			pCallbackInfo->szCryptKey; // Out
			pCallbackInfo->szOldCryptKey;
			pCallbackInfo->szCryptFileName;

			iResult = SQX_PASSWORD_OK;
			break;

		// Kennwort fuer Archivverzeichnis abfragen
		case SQX_CALLBACK_PASSWORD_HEADER:

			// Kennwort, vorheriges eingegebenes Kennwort (falls vorhanden) und Name der Datei
			pCallbackInfo->szCryptKey; // Out
			pCallbackInfo->szOldCryptKey;
			pCallbackInfo->szCryptFileName;

			iResult = SQX_PASSWORD_OK;
			break;

		// Gesperrte Datei ueberspringen
		case SQX_CALLBACK_SKIP:

			// Name der gesperrten Datei
			pCallbackInfo->pszSourceFileName;

			iResult = SQX_SKIPFILE_SKIP;
			break;

		// Naechsten Datentraeger anfordern
		case SQX_CALLBACK_NEXTDISK:

			// Benoetigter Platz, Nummer des Dateitraegers, Pfad zum naechsten Volume
			pCallbackInfo->dwTotalSize;
			pCallbackInfo->dwDiskNum;
			pCallbackInfo->szNextDiskPath; // Out

			iResult = SQX_NEXTDISK_OK;
			break;

		// Aktion
		case SQX_CALLBACK_SIGNAL:

			//
			switch (pCallbackInfo->dwSignal)
			{
				// Komprimieren
				case SQX_SIGNAL_COMPRESS:

					// Gesamtanzahl an zu komprimierenden Bytes
					pCallbackInfo->dwlBlockSize;
					break;

				// Entpacken
				case SQX_SIGNAL_UNCOMPRESS:

					// Gesamtanzahl an zu entpackenden Bytes
					pCallbackInfo->dwlBlockSize;
					break;

				// Loeschen
				case SQX_SIGNAL_DELETE:

					// Anzahl der zu loeschenden Dateien
					pCallbackInfo->dwlBlockSize;
					break;

				// Archiv temporaer kopieren
				case SQX_SIGNAL_TEMP_ARC_COPY:

					// Anzahl der zu kopierenden Bytes
					pCallbackInfo->dwlBlockSize;
					break;

				// AV-Signatur erstellen
				case SQX_SIGNAL_CREATE_AV_SIG:

					// Gesamtgroesse des Blocks
					pCallbackInfo->dwlBlockSize;
					break;

				// AV-Signatur testen
				case SQX_SIGNAL_TEST_AV_SIG:

					// Gesamtgroesse des Blocks
					pCallbackInfo->dwlBlockSize;
					break;

				// Multivolume entpacken
				case SQX_SIGNAL_ADD_MV_UNCOMPRESS:

					// Groesse des aktuellen Volumes, muss zur Gesamtgroesse addiert werden
					pCallbackInfo->dwlBlockSize;
					break;

				// Reparaturdaten erstellen
				case SQX_SIGNAL_CREATE_RDATA:

					// Gesamtgroesse des Blocks
					pCallbackInfo->dwlBlockSize;
					break;

				// Archiv reparieren
				case SQX_SIGNAL_REPAIR_ARCHIVE:

					// Anzahl der Dateien
					pCallbackInfo->dwlBlockSize;
					break;

				// Reparaturdaten entfernen
				case SQX_SIGNAL_STRIP_RDATA:

					// Gesamtgroesse des Blocks
					pCallbackInfo->dwlBlockSize;
					break;

				// SFX erstellen
				case SQX_SIGNAL_CREATE_SFX_ARCHIVE:

					break;

				// Archiv testen
				case SQX_SIGNAL_TEST_ARCHIVE:

					// Gesamtgroesse des Blocks
					pCallbackInfo->dwlBlockSize;
					break;

				// Archiv testen (neues Volume)
				case SQX_SIGNAL_ADD_MV_TEST:

					// Groesse des aktuellen Volumes, muss zur Gesamtgroesse addiert werden
					pCallbackInfo->dwlBlockSize;
					break;

				// Erweitertes Testen nach dem Archivieren
				case SQX_SIGNAL_EX_TEST_ARCHIVE:

					// Gesamtgroesse des Blocks
					pCallbackInfo->dwlBlockSize;
					break;

				// Erweitertes Testen nach dem Archivieren (neues Volume)
				case SQX_SIGNAL_ADD_MV_EX_TEST:

					// Groesse des aktuellen Volumes, muss zur Gesamtgroesse addiert werden
					pCallbackInfo->dwlBlockSize;
					break;
			}

			break;
	}

	// Alles ok
	return iResult;
}
