#if !defined(__sqx_h)
#define __sqx_h

/*	Projekt SQX-SDK
	SpeedProject
	Copyright © 1995 - 2005. Alle Rechte vorbehalten.

	Datei:		sqx.h
	Autor:		Sven Ritter
	Datum:		21.05.2005 - 13:39:05


	UEBERBLICK
	==========
	Definition fuer SQX-SDK.
*/


// ****************************************************************************
// ******                Header nur einmal pro Build includen            ******
// ****************************************************************************
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// ****************************************************************************
// ******                          Funktionsaufruf                       ******
// ****************************************************************************
#define SQX_API _stdcall


// ****************************************************************************
// ******                Byteweise Ausrichtung der Strukturen            ******
// ****************************************************************************
#pragma pack(push, 1)


// ****************************************************************************
// ******                 Aktionen fuer Fortschrittsanzeige              ******
// ****************************************************************************
#define SQX_PROGRESS_CANCEL						0
#define SQX_PROGRESS_OK							1


// ****************************************************************************
// ******                Aktionen fuer 'Datei ueberschreiben'            ******
// ****************************************************************************
#define SQX_REPLACE_OVERWRITE					0
#define SQX_REPLACE_SKIP						1
#define SQX_REPLACE_CANCEL						2


// ****************************************************************************
// ******                  Aktionen fuer 'Kennwortabfrage'               ******
// ****************************************************************************
#define SQX_PASSWORD_CANCEL						0
#define SQX_PASSWORD_OK							1


// ****************************************************************************
// ******                      Aktionen fuer 'SkipFile'                  ******
// ****************************************************************************
#define SQX_SKIPFILE_SKIP						1
#define SQX_SKIPFILE_CANCEL						2


// ****************************************************************************
// ******                  Aktionen fuer naechste Diskette               ******
// ****************************************************************************
#define SQX_NEXTDISK_CANCEL						0
#define SQX_NEXTDISK_OK							1


// ****************************************************************************
// ******                            Dateiformat                         ******
// ****************************************************************************
#define SQX_FILEFORMAT_10						0
#define SQX_FILEFORMAT_20						1


// ****************************************************************************
// ******                          Speichergroessen                      ******
// ****************************************************************************
#define SQX_DICTIONARY_0K						0
#define SQX_DICTIONARY_8K						1
#define SQX_DICTIONARY_16K						2
#define SQX_DICTIONARY_32K						3
#define SQX_DICTIONARY_64K						4
#define SQX_DICTIONARY_128K						5
#define SQX_DICTIONARY_256K						6
#define SQX_DICTIONARY_512K						7
#define SQX_DICTIONARY_1M						8
#define SQX_DICTIONARY_2M						9
#define SQX_DICTIONARY_4M						10
#define SQX_DICTIONARY_8M						11
#define SQX_DICTIONARY_16M						12
#define SQX_DICTIONARY_32M						13
#define SQX_DICTIONARY_64M						14
#define SQX_DICTIONARY_128M						15
#define SQX_DICTIONARY_256M						16


// ****************************************************************************
// ******                             Dateinamen                         ******
// ****************************************************************************
#define SQX_FILENAME_ANSI						0
#define SQX_FILENAME_ASCII						1
#define SQX_FILENAME_UNICODE					2


// ****************************************************************************
// ******                          Verschluesselung                      ******
// ****************************************************************************
#define SQX_ENCRYPTION_NONE						0
#define SQX_ENCRYPTION_AES_128BIT				2
#define SQX_ENCRYPTION_AES_256BIT				4


// ****************************************************************************
// ******                              Signatur                          ******
// ****************************************************************************
#define SQX_AVENVELOPE_128BIT					0
#define SQX_AVENVELOPE_256BIT					1
#define SQX_AVENVELOPE_512BIT					2


// ****************************************************************************
// ******                          Kompressionsrate                      ******
// ****************************************************************************
#define SQX_COMPRATE_STORED						0		// Keine Kompression
#define SQX_COMPRATE_FAST						1		// Schnelle Kompression
#define SQX_COMPRATE_NORMAL						2		// Gute Kompression
#define SQX_COMPRATE_HIGH						3		// Hohe Kompression
#define SQX_COMPRATE_MAXIMIUM					4		// Maximale Kompression
#define SQX_COMPRATE_ULTRA						5		// Ultra-Kompression


// ****************************************************************************
// ******                        Kompressionsmethoden                    ******
// ****************************************************************************
#define SQX_METHOD_STORED						0		//- stored (no compression), compression type 1 (lzh)
#define SQX_METHOD_NORMAL						1		//- compressed, normal compression, compression type 1 (lzh)
#define SQX_METHOD_GOOD							2		//- compressed, good compression, compression type 1 (lzh)
#define SQX_METHOD_HIGH							3		//- compressed, high compression, compression type 1 (lzh)
#define SQX_METHOD_BEST							4		//- compressed, best compression, compression type 1 (lzh)
#define SQX_METHOD_AUDIO						5		//- true audio (wav, pcm) compression
#define SQX_METHOD_TEXT							6		//- text compression (ppmd)
#define SQX_METHOD_ULTRA						7		//- ultra compression (lz77 + rc)


// ****************************************************************************
// ******                           Reparaturdaten                       ******
// ****************************************************************************
#define	SQX_RECOVERYDATA_NONE					0
#define	SQX_RECOVERYDATA_1						1
#define	SQX_RECOVERYDATA_2						2
#define	SQX_RECOVERYDATA_3						3
#define	SQX_RECOVERYDATA_4						4
#define	SQX_RECOVERYDATA_5						5


// ****************************************************************************
// ******                            Kompressoren                        ******
// ****************************************************************************
#define SQX_SPECIALCOMP_DISABLED				0
#define SQX_SPECIALCOMP_AUTOMATIC				1
#define SQX_SPECIALCOMP_FORCE					2


// ****************************************************************************
// ******                          Betriebssysteme                       ******
// ****************************************************************************
#define SQX_HOSTOS_DOSWIN						0
#define SQX_HOSTOS_DOS							1
#define SQX_HOSTOS_OS2							2
#define SQX_HOSTOS_WIN32						3
#define SQX_HOSTOS_WINNT						4
#define SQX_HOSTOS_UNIX							5
#define SQX_HOSTOS_MAC							6
#define SQX_HOSTOS_WINNT_ALPHA					7
#define SQX_HOSTOS_ATARI						8
#define SQX_HOSTOS_VAX_VMS						9
#define SQX_HOSTOS_AMIGA						10
#define SQX_HOSTOS_NEXT							11
#define SQX_HOSTOS_LINUX						12
#define SQX_HOSTOS_CPM							13
#define SQX_HOSTOS_ZSYS							14
#define SQX_HOSTOS_VMCMS						15
#define SQX_HOSTOS_BEOS							16
#define SQX_HOSTOS_UNKNOWN						17
#define SQX_HOSTOS_OS2_HPFS						18
#define SQX_HOSTOS_WIN32_FAT32					19
#define SQX_HOSTOS_WINNT_NTFS					20
#define SQX_HOSTOS_WINNTAXP_NTFS				21
#define SQX_HOSTOS_MVS							22
#define SQX_HOSTOS_VSE							23
#define SQX_HOSTOS_ARCORN_RISC					24
#define SQX_HOSTOS_MVS_ALT						25


// ****************************************************************************
// ******                   Definition zu tagSQX_FILETIME                ******
// ****************************************************************************
typedef struct tagSQX_FILETIME
{
	BOOL			fBlockPresent;						// Dateizeit ist gueltig
	FILETIME		ftCreationTime;						// Erstellung
	FILETIME		ftLastAccessTime;					// Letzter Zugriff
	FILETIME		ftLastWriteTime;					// Letzter Schreibzugriff

} SQX_FILETIME, *LPSQX_FILETIME;


// ****************************************************************************
// ******                    Definition zu tagSQX_ARCNODE                ******
// ****************************************************************************
typedef struct tagSQX_ARCNODE
{
	LPCTSTR				pszFileName;					// Dateiname
	DWORD				dwFileNameLen;					// Laenge des Dateinamens
	void*				pExtraName;						//
	DWORD				dwExtraNameLen;					//
	DWORD				dwFileNameType;					// Filename base (0 ASCII, 1 ANSI, 2 UNICODE)
	LONGLONG			dwlSize;						// Komprimierte Groesse
	LONGLONG			dwlSizeOrig;					// Entpackte Groesse
	DWORD				dwDosFileTime;					// DOS-Zeit
	DWORD				dwAttributes;					// Dateiattribute
	DWORD				dwArcerVersion;					// Versionsnummer des Archivers
	DWORD				dwFileCRC;						// CRC der Datei
	DWORD				dwHostOS;						// OS, unter welchem die Datei komprimiert wurde
	DWORD				dwMethod;						// Kompressionsmethode
	DWORD				dwCommentLen;					// Laenge des Dateikommentars
	LPCTSTR				pszComment;						// Dateikommentar
	BOOL				fEncrypted;						// Verschluesselung
	BOOL				fTagged;						// Eintrag ist gueltig, wenn ungleich 0
	DWORD				dwMappedMethod;					// Kompressionsmethode (SQ4)
	DWORD				dwExtendedError;				// used when repairing archives
	SQX_FILETIME		win32FileTime;					// Dateizeit

} SQX_ARCNODE, *LPSQX_ARCNODE;


// ****************************************************************************
// ******                  Definition zu tagSQX_ARCLISTNODE              ******
// ****************************************************************************
typedef struct tagSQX_ARCLISTNODE
{
	tagSQX_ARCLISTNODE*	pNext;						// Naechster Eintrag
	LPSQX_ARCNODE		pArcNode;					// Daten

} SQX_ARCLISTNODE, *LPSQX_ARCLISTNODE;


// ****************************************************************************
// ******                    Definition zu tagSQX_ARCLIST                ******
// ****************************************************************************
typedef struct tagSQX_ARCLIST
{
	DWORD				dwItemCount;				// Anzahl der Eintraege
	SQX_ARCLISTNODE*	pHead;						//
	SQX_ARCLISTNODE*	pTail;						//

} SQX_ARCLIST, *LPSQX_ARCLIST;


// ****************************************************************************
// ******                    Definition zu tagSQX_AVINFO                 ******
// ****************************************************************************
typedef struct tagSQX_AVINFO
{
	BOOL			fAVInfoPresent;						// AV-Info ist da
	TCHAR			szAV_ID[MAX_PATH];					// AV-String
	FILETIME		ftCreationTime;						// Erstellzeit

} SQX_AVINFO, *LPSQX_AVINFO;


// ****************************************************************************
// ******                    Definition zu tagSQX_ARCINFO                ******
// ****************************************************************************
typedef struct tagSQX_ARCINFO
{
	DWORD			cbSize;								// Groesse der Struktur
	DWORD			dwFileFormat;						// Archivformat
	DWORD			dwArcerMajorVersion;				// Hauptversion des Archivers
	DWORD			dwArcerMinorVersion;				// Nebenversion des Archivers
	DWORD			dwDictionarySize;					// Speichergroesse
	BOOL			fRecoveryData;						// Reparaturdaten
	DWORD			dwEncryption;						// Verschluesslung
	BOOL			fSolid;								// Solid-Archiv
	DWORD			dwHostOS;							// Betriebssystem
	DWORD			dwTotalFiles;						// Anzahl der Dateien
	LONGLONG		dwlCompressedSize;					// Komprimierte Groesse
	LONGLONG		dwlUncompressedSize;				// Unkomprimierte Groesse
	int				iRatio;								// Einsparung
	BOOL			fHeaderEncrypted;					// Archivverzeichnis verschluesselt
	BOOL			fIsMultiVolume;						// Multivolume?
	BOOL			fArchiveComment;					// Archivkommentar
	BOOL			fFileComments;						// Dateikommentare
	SQX_AVINFO		avInfo;								// AV-Info

} SQX_ARCINFO, *LPSQX_ARCINFO;


// ****************************************************************************
// ******                Definition zu tagSQX_EXTRACTOPTIONS             ******
// ****************************************************************************
typedef struct tagSQX_EXTRACTOPTIONS 
{
	//
	DWORD			cbSize;								// Groesse der Struktur

	//
	TCHAR			szPassword[MAX_PATH];				// Kennwort
	TCHAR			szPasswordHeader[MAX_PATH];			// Kennwort fuer Archivverzeichnis
	TCHAR			szTempDir[MAX_PATH];				// Temporaeres Verzeichnis
	TCHAR			szRelativePath[MAX_PATH];			// Relativer Ordner im Archiv
	TCHAR			szOutputPath[MAX_PATH];				// Ausgabeverzeichnis

	//
	BOOL			fResetArchiveAttribute;				// Archivattribut zuruecksetzen
	BOOL			fOverwriteAlways;					// Immer ueberschreiben
	BOOL			fKeepBrokenFiles;					// Beschaedigte Dateien erhalten
	BOOL			fCreateFolders;						// Verzeichnisse erstellen

	//
	BOOL			fRestoreACLs;						// ACL wiederherstellen
	BOOL			fRestoreStreams;					// Streams wiederherstellen
	BOOL			fRestoreUnicodeNames;				// Unicode-Dateien wiederherstellen
	BOOL			fRestoreWin32FileTimes;				// Erweiterte Zeitstempel wiederherstellen
	BOOL			fRestoreDirectoryTimeStamps;		// Zeitstempel fuer Verzeichnisse wiederherstellen
	BOOL			fAutoRestoreComments;				// Kommentare wiederherstellen
	BOOL			fSetZoneID;							// ZoneID wiederherstellen (0, Auto, Force)

} SQX_EXTRACTOPTIONS, *LPSQX_EXTRACTOPTIONS;


// ****************************************************************************
// ******                   Definition zu tagSQX_FORCECOMP               ******
// ****************************************************************************
typedef struct tagSQX_FORCECOMP
{
	DWORD			dwCompRate;							// Kompressionsrate
	BOOL			fSolidFlag;							// Solid-Archiv
	DWORD			dwDictionarySize;					// Speichergroesse
	DWORD			dwExeCompression;					// EXE-Komprimierung
	DWORD			dwTextCompression;					// PPM
	DWORD			dwTextMemory;						// Groesse in MB (4 - 128) 
	DWORD			dwTextOrder;						// Ordnung vom PPM (1 - 16)
	DWORD			dwMultimediaCompression;			// Multimedia-Kompression
	DWORD			dwMultimediaChannels;				// 0 - Automatisch (1-256)
	DWORD			dwAudioCompression;					// Audio-Kompression
	DWORD			dwAudioChannels;					// 0 - Automatisch (1-32)
	DWORD			dwAudioChannelsWidth;				// 8, 16
	DWORD			dwRgbCompression;					// RGB-Kompression
	DWORD			dwUltraMatchLink;					// Schnellere gegen bessere Kompression (1 - 10)

} SQX_FORCECOMP, *LPSQX_FORCECOMP;


// ****************************************************************************
// ******                  Definition zu tagSQX_VERSIONINFO              ******
// ****************************************************************************
typedef struct tagSQX_VERSIONINFO
{
	TCHAR		szLegalCopyright[40];
	TCHAR		szCompanyName[40];
	TCHAR		szFileDescription[40];
	TCHAR		szFileVersion[40];
	TCHAR		szProductName[40];
	TCHAR		szProductVersion[40];

} SQX_VERSIONINFO, *LPSQX_VERSIONINFO;


// ****************************************************************************
// ******                  Definition zu tagSQX_SFXCOMMAND               ******
// ****************************************************************************
typedef struct tagSQX_SFXCOMMAND
{
	//
	BOOL				fCreateSfx;							// SFX erstellen

	// Allgemein
	DWORD				dwSfxType;							// Typ (Normal - SWInstall)
	DWORD				dwSfxSystem;						// System (DOS - WIN16 - WIN32 - WIN32 Alpha)

	// HintDlg
	TCHAR				szSfxHintTitle[256];				// Titel
	TCHAR				szSfxHintMsg[4096];					// Text
	DWORD				dwSfxHintBtn;						// Button (OK - YES/NO - OK/CANCEL)

	// InfoAbout
	TCHAR				szSfxInfoAboutMsg[256];				// Text

	// Normale SFX'e
	TCHAR				szSfxNormalFolder[MAX_PATH];		// Zielordner
	TCHAR				szSfxNormalCmd[MAX_PATH];			// Befehl
	BOOL				fSfxNormalOverwriteWOPrompt;		// Immer ueberschreiben (Nur Normal)
	BOOL				fSfxNormalAutoExtract;				// Sofort entpacken (Nur Normal)

	// SW-Install
	TCHAR				szSfxInstallMsg[256];				// Installationshinweis
	TCHAR				szSfxInstallCmd[MAX_PATH];			// Befehl
	TCHAR				szSfxInstallWaitFor[MAX_PATH];		// WaitFor

	// Sonstiges
	TCHAR				szIconFile[MAX_PATH];				// Iconpfad
	TCHAR				szLanguageFile[MAX_PATH];			// Sprachdatei

	// Versionsinfos
	SQX_VERSIONINFO		versionInfo;						// Versionsinformation

} SQX_SFXCOMMAND, *LPSQX_SFXCOMMAND;


// ****************************************************************************
// ******               Definition zu tagSQX_COMPRESSOPTIONS             ******
// ****************************************************************************
typedef struct tagSQX_COMPRESSOPTIONS 
{
	//
	DWORD				cbSize;							// Groesse der Struktur
	DWORD				dwFileFormat;					// Archivformat

	//
	SQX_FORCECOMP		compOptions;					// Komprimierung
	SQX_SFXCOMMAND		sfxCommand;						// SFX-Struktur

	//
	DWORD				dwRecoveryData;					// Reparaturdaten
	BOOL				fExternalRecoveryData;			// Externe Reparaturdaten
	LPCTSTR				pszMainComment;					// Hauptkommentar

	// Verschluesselung und Signatur
	DWORD				dwEncryption;					// Verschluesselung (Keine / Symmetrisch / Asymmetrisch)
	BOOL				fEncryptHeaders;				// Archivverzeichnis verschluesseln
	BOOL				fAVSignature;					// Signatur erstellen
	BOOL				fAVSignatureExternal;			// Externe Signatur erstellen
	DWORD				dwAVEnvelope;					// Verschluesslungsverfahren fuer die Signatur
	TCHAR				szSignatureFile[MAX_PATH];		// Alternative Signatur
	TCHAR				szPassword[MAX_PATH];			// Kennwort
	TCHAR				szPasswordHeader[MAX_PATH];		// Kennwort fuer Archivverzeichnis

	// Diverses
	BOOL				fRetainFolderStructure;			// Ordnerstruktur beibehalten
	BOOL				fRetainDrivePart;				// Laufwerksbezeichner speichern
	TCHAR				szRelativePath[MAX_PATH];		// Relativer Pfad im Archiv
	TCHAR				szInputPath[MAX_PATH];			// Eingabeverzeichnis
	DWORD				dwFileNames;					// Speicherung der Dateinamen
	BOOL				fStoreWin32FileTime;			// Win32-Zeitangaben speichern
	BOOL				fStoreACL;						// ACL speichern
	BOOL				fStoreStreams;					// Streams speichern
	BOOL				fAutoSaveComments;				// Kommentare automatisch speichern
	BOOL				fExtendedTest;					// Testen nach Archivieren

	//
	TCHAR				szTempDir[MAX_PATH];			// Temporaerer Ordner
	DWORDLONG			dwlVolumeSize;					// Groesse der einzelnen Volumes
	BOOL				fAutoVolumeSize;				// Volumegroesse selbstaendig berechnen
	BOOL				fCreateAlwaysNewArchive;		// Immer neues Archiv erstellen
	BOOL				fResetArchiveAttribute;			// Archivattribut zuruecksetzen
	DWORD				dwLogType;						// Logging

} SQX_COMPRESSOPTIONS, *LPSQX_COMPRESSOPTIONS;


// ****************************************************************************
// ******                        Callback-Funktionen                     ******
// ****************************************************************************
#define SQX_CALLBACK_FILENAME			0
#define SQX_CALLBACK_PROGRESS			1
#define SQX_CALLBACK_REPLACE			2
#define SQX_CALLBACK_PASSWORD			3
#define SQX_CALLBACK_PASSWORD_HEADER	4
#define SQX_CALLBACK_SKIP				5
#define SQX_CALLBACK_NEXTDISK			6
#define SQX_CALLBACK_SIGNAL				7


// ****************************************************************************
// ******                          Signalfunktionen                      ******
// ****************************************************************************
#define SQX_SIGNAL_COMPRESS						1
#define SQX_SIGNAL_UNCOMPRESS					2
#define SQX_SIGNAL_DELETE						3
#define SQX_SIGNAL_TEMP_ARC_COPY				4
#define SQX_SIGNAL_CREATE_AV_SIG				5
#define SQX_SIGNAL_TEST_AV_SIG					6
#define SQX_SIGNAL_ADD_MV_UNCOMPRESS			7
#define SQX_SIGNAL_CREATE_RDATA					8
#define SQX_SIGNAL_REPAIR_ARCHIVE				9
#define SQX_SIGNAL_STRIP_RDATA					10
#define SQX_SIGNAL_TEST_ARCHIVE					11
#define SQX_SIGNAL_CREATE_SFX_ARCHIVE			12
#define SQX_SIGNAL_ADD_MV_TEST					13
#define SQX_SIGNAL_EX_TEST_ARCHIVE				14
#define SQX_SIGNAL_ADD_MV_EX_TEST				15


// ****************************************************************************
// ******                 Definition zu tagSQX_CALLBACKINFO              ******
// ****************************************************************************
typedef struct tagSQX_CALLBACKINFO
{
	//
	DWORD				dwCallbackType;

	// Dateiname
	LPCTSTR				pszSourceFileName;
	LPCTSTR				pszTargetFileName;

	// Fortschritt
	int					iProgress;

	// Ersetzen
	LPWIN32_FIND_DATA	pFindDataExist;
	LPWIN32_FIND_DATA	pFindDataReplace;

	// Kennwortabfrage
	TCHAR				szCryptKey[MAX_PATH];
	TCHAR				szOldCryptKey[MAX_PATH];
	TCHAR				szCryptFileName[MAX_PATH];

	// Naechste Disk
	DWORD				dwTotalSize;
	DWORD				dwDiskNum;
	TCHAR				szNextDiskPath[MAX_PATH];

	// Signalfunktion und Fortschritt
	DWORD				dwSignal;
	LONGLONG			dwlBlockSize;

} SQX_CALLBACKINFO, *LPSQX_CALLBACKINFO;


// ****************************************************************************
// ******                              Callback                          ******
// ****************************************************************************
typedef int	(CALLBACK* SQXCALLBACK)	(LPVOID pParam, LPSQX_CALLBACKINFO pCallbackInfo);


// ****************************************************************************
// ******                           SQX-Funktionen                       ******
// ****************************************************************************
typedef int				(SQX_API* SqxInitArchive)		(LPCTSTR pszArchiveName, SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE* phArchiver);
typedef void			(SQX_API* SqxDoneArchive)		(HANDLE hArchive);
typedef HANDLE			(SQX_API* SqxInitFileList)		(HANDLE hArchive);
typedef void			(SQX_API* SqxAppendFileList)	(HANDLE hArchive, HANDLE hFileMaskList, LPCTSTR pszFileName);
typedef void			(SQX_API* SqxDoneFileList)		(HANDLE hArchive, HANDLE hFileMaskList);
typedef LPSQX_ARCLIST	(SQX_API* SqxInitArcFileList)	(HANDLE hArchive);
typedef void			(SQX_API* SqxDoneArcFileList)	(HANDLE hArchive, LPSQX_ARCLIST pList);
typedef int				(SQX_API* SqxListFiles)			(HANDLE hArchive, HANDLE hFileMaskList, LPSQX_ARCLIST pArcFileList, LPSQX_ARCINFO pArchiveInfo);
typedef int				(SQX_API* SqxCompressFiles)		(HANDLE hArchive, SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE hFileMaskList, LPSQX_COMPRESSOPTIONS pCompressOptions);
typedef int				(SQX_API* SqxExtractFiles)		(HANDLE hArchive, SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE hFileMaskList, LPSQX_EXTRACTOPTIONS pExtractOptions);
typedef int				(SQX_API* SqxDeleteFiles)		(HANDLE hArchive, SQXCALLBACK pfnCallback, LPVOID pParam, HANDLE hFileMaskList);
typedef int				(SQX_API* SqxTestArchive)		(HANDLE hArchive, SQXCALLBACK pfnCallback, LPVOID pParam);
typedef int				(SQX_API* SqxRepairArchive)		(HANDLE hArchive, LPCTSTR pszRepairedFile, LPSQX_ARCLIST pGoodFileList, LPSQX_ARCLIST pBadFileList);
typedef int				(SQX_API* SqxAddArchiveComment)	(HANDLE hArchive, LPCTSTR pszMainComment);
typedef int				(SQX_API* SqxGetArchiveComment)	(HANDLE hArchive, LPTSTR pszMainComment, DWORD cchMainComment);
typedef int				(SQX_API* SqxAddFileComments)	(HANDLE hArchive, HANDLE hFileMaskList, LPCTSTR pszComment);
typedef int				(SQX_API* SqxCreateFolder)		(HANDLE hArchive, LPCTSTR pszFolder);


// ****************************************************************************
// ******                          Fehlermeldungen                       ******
// ****************************************************************************
#define SQX_ERR_SUCCESS								0	//- No error.
#define SQX_ERR_ERROR								1	//- Unknown error in the last archive operation.
#define SQX_ERR_FILE_NOT_FOUND						2	//- The system cannot find the file specified.
#define SQX_ERR_PATH_NOT_FOUND						3	//- The system cannot find the path specified.
#define SQX_ERR_TOO_MANY_FILES						4	//- The system cannot open the file.
#define SQX_ERR_ACCESS_DENIED						5	//- Access is denied.
#define SQX_ERR_INVALID_HANDLE						6	//- The file handle is invalid.
#define SQX_ERR_DISK_FULL							7	//- The disk is full.
#define SQX_ERR_OUT_OF_MEMORY						8	//- Not enough memory is available to complete this operation.
#define SQX_ERR_CANT_ACCESS_TEMP_DIR				9	//- Cannot access temp folder during a delete or update operation on a solid archive.
#define SQX_ERR_TEMP_DIR_FULL						10	//- Not enough space on drive for the temporary folder.
#define SQX_ERR_USER_ABORT							11	//- Cancelled by user.
#define SQX_ERR_INVALID_ARC_HANDLE					12	//- Invalid archive handle.
#define SQX_ERR_CANT_FIND_LANG_DATA					13	//- Could not find the selected language file for self-extracting archives.
#define SQX_ERR_UNKNOWN_SUBSTREAM					14	//- The archive contains at least one unknown alternate data stream.
#define SQX_ERR_BAD_SUBSTREAM_CRC					15	//- Bad alternate stream CRC. Archive is probably corrupted.
#define SQX_ERR_UNKNOWN_METHOD						16	//- This method of compression is not supported by the archiver.
#define SQX_ERR_FILE_ENCRYPTED						17	//- Archive is encrypted with a password.
#define SQX_ERR_BAD_CRC								18	//- Bad CRC. Archive is probably corrupted.
#define SQX_ERR_CANT_CREATE_FILE					19	//- Could not create file or folder.
#define SQX_ERR_BAD_FILE_FORMAT						20	//- Bad archive file format.
#define SQX_ERR_FUNCTION_NOT_SUPPORTED				21	//- Function is not supported.
#define SQX_ERR_FUNC_NOT_SUPPORTED_BY_ARCHIVE		22	//- Function is not supported for this archive type.
#define SQX_ERR_CANT_CREATE_ARC_DIR					23	//- Could not create folder in archive. It already exists a folder with this name.
#define SQX_ERR_INVALID_DIR_NAME					24	//- Could not create folder in archive. The folder name contains invalid characters.
#define SQX_ERR_INVALID_FILE_NAME					25	//- Could not create file in archive. The file name contains invalid characters.
#define SQX_ERR_TOO_MANY_BROKEN_FBLOCKS				26	//- The archive file contains too many damaged file blocks and cannot be repaired.
#define SQX_ERR_ARCHIVE_OK_RDATA_NOT				27	//- It seems that the archive has no errors, but the recovery data does not match the archive. Maybe this archive was edited by a program that could not identify the recovery data.
#define SQX_ERR_RDATA_DAMAGED						28	//- The recovery data of the archive file is damaged, too. This archive file cannot be repaired.
#define SQX_ERR_RDATA_NEW_VERSION					29	//- The archive contains recovery data that cannot be used with this version of our software. The newest versions of our software are always available at http://www.speedproject.de/.
#define SQX_ERR_RDATA_DOES_NOT_MATCH				30	//- The recovery data does not match the archive. It seems that this archive was edited by a program that could not identify the recovery data.
#define SQX_ERR_CANT_FIND_RDATA						31	//- It seems that the archive does not contain any recovery data.
#define SQX_ERR_ARCHIVE_IS_LOCKED					32	//- Cannot modify locked archive.
#define SQX_ERR_CANT_ADD_TO_MV						33	//- It is not possible to add files to a multi-volume archive.
#define SQX_ERR_CANT_DELETE_FROM_MV					34	//- It is not possible to delete files from a multi-volume archive.
#define SQX_ERR_NEED_1ST_VOLUME						35	//- This file is part of a multi-volume archive. The first volume of the archive is needed to open it. Please select the first volume to open the archive.
#define SQX_ERR_MISSING_VOLUME						36	//- The last volume of the archive could not be found.
#define SQX_ERR_VOLUME_LIMIT_REACHED				37	//- Cannot create more than 999 volumes.
#define SQX_ERR_SFXSTUB_NOT_INSTALLED				38	//- This system lacks the support for the SFX type you have selected. Please visit the download section of http://ww.speedproject.de/ to obtain complete support for SFX archives.
#define	SQX_ERR_BACKUP_READ_ACCESS_DENIED			39	//- Read access to alternate data stream was denied. Only the file itself is being compressed.
#define	SQX_ERR_BACKUP_WRITE_ACCESS_DENIED			40	//- Could not write alternate data stream (e.g. file comments).
#define	SQX_ERR_ACL_READ_ACCESS_DENIED				41	//- Could not read security attributes. Only the file itself is being compressed.
#define	SQX_ERR_ACL_WRITE_ACCESS_DENIED				42	//- Could not write security attributes.
#define SQX_ERR_WRONG_ARCHIVER_VERSION				43	//- This archive contains data created with a higher version of this software. Please download the lastest update of the software from http://www.speedproject.de/.
#define SQX_ERR_CANT_COPY_SOURCE_TO_SOURCE			44	//- Cannot copy an archive to itself. Please choose a different target name.
#define SQX_ERR_VOLUMES_TOO_SMALL					45	//- The volume size cannot be smaller than 130 kByte.
#define SQX_ERR_ARCHIVE_VERSION_TOO_HIGH			46	//- This archive can only be extracted. It is not possible to add files.
#define SQX_ERR_EXT_RDATA_DOES_NOT_MATCH			47	//- The external recovery data don't seem to belong to the selected archive.
#define SQX_ERR_BAD_PARAMETER						48	//- 
#define SQX_ERR_EQUAL_PASSWORDS						49	//- The passwords for the archive files and archive directory are identical. Please enter different passwords.
#define SQX_ERR_REQUIRES_ENCRYPTION					50	//- You cannot encrypt the archive directory without encrypting the files. Please enter one password for the archive directory and another password for the files.
#define SQX_ERR_MISSING_HEADER_PASSWORD				51	//- Please enter a password to encrypt the archive directory.
#define SQX_ERR_MISSING_SQX_PRIVATE_KEY				52	//- Could not find/access the private key required to encrypt the archive. Please make sure that the software can access all the keys in question.
#define SQX_ERR_MISSING_SQX_AVKEY					53	//- Could not find the key required to create a digital signature. Please make sure that the software can access the authentication key.
#define SQX_ERR_MISSING_EXTERNAL_AVKEY				54	//- Could not find/access the external key necessary to sign the archive. Please make sure that the software can access all keys in question.
#define SQX_ERR_INVALID_SQX_AVKEY					55	//- The key you have selected to create digital signatures is invalid.
#define SQX_ERR_SQX_AVKEY_VERSION					56	//- This version of the software cannot use the existing key to create digital signatures. Please download the lastest update of the software from http://www.speedproject.de/.
#define SQX_ERR_SQX_AVDATA_VERSION					57	//- This version of the software cannot process the digital signature embedded within the archive. Please download the lastest update of the software from http://www.speedproject.de/.
#define SQX_ERR_SQX_BROKEN_AVRECORD					58	//- The archive contains an invalid digital signature. The archive is either damaged or it has been manipulated.
#define SQX_ERR_RIJNDAEL_RSA						59	//- Unexpected error in an encryption function. Please contact our product support.
#define SQX_ERR_REQUIRES_NTFS						60	//- An option you selected requires NTFS.
#define SQX_ERR_REQUIRES_WINNT						61	//- An option you selected requires Windows NT.
#define SQX_ERR_REQUIRES_W2K						62	//- An option you selected requires Windows 2000.
#define SQX_ERR_REQUIRES_WINXP						63	//- An option you selected requires Windows XP.
#define SQX_ERR_REQUIRES_WINXP_SP1					64	//- An option you selected requires Windows XP SP1.
#define SQX_ERR_REQUIRES_WINXP_SP2					65	//- An option you selected requires Windows XP SP2.
#define SQX_ERR_REQUIRES_LONGHORN					66	//- An option you selected requires Longhorn.
#define SQX_ERR_NO_RESOURCES_FOUND					67	//- The selected SFX stub does not contain resources. Can not modify SFX stub.
#define SQX_ERR_UNKNOWN_ICON_FORMAT					68	//- Could not determine the format of the selected icon file.
#define SQX_ERR_NO_MATCHING_ICON_SIZE				69	//- The selected SFX stub does not contain icons with valid sizes. Can not modify SFX stub.
#define SQX_ERR_UNKNOWN_EXE_FORMAT					70	//- The format of the selected SFX stub is unknown.
#define SQX_ERR_REQUIRES_SOURCE_PATH				71	//- The extended archive test requires the source path.
#define SQX_ERR_FILE_DATA_NOT_EQUAL					72	//- Extended archive test: The source on hard disk is different from the file in the archive.
#define SQX_ERR_COMMENT_BIGGER_4K					73	//- You cannot add to the archive comments longer than 4096 chars. Please shorten your comments and try again.
#define	SQX_ERR_CANT_CREATE_SFX_FROM_MV				74	//- Cannot create a self-extracting archive from a multi-volume archive.


// ****************************************************************************
// ******                       Vorherige Ausrichtung                    ******
// ****************************************************************************
#pragma pack(pop)


// ****************************************************************************
// ******                        INLINE-Definitionen                     ******
// ****************************************************************************


#endif										// __sqx_h


// ****************************************************************************
// ******                            END OF FILE                         ******
// ****************************************************************************