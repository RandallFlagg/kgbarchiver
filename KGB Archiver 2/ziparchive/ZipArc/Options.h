/*
* This source file is part of the ZipArc project source distribution and
* is Copyrighted 2000 - 2006 by Tadeusz Dracz (http://www.artpol-software.com/)
*
* This code may be used in compiled form in any way you desire PROVIDING 
* it is not sold for profit as a stand-alone application.
*
* This file may be redistributed unmodified by any means providing it is
* not sold for profit without the authors written consent, and
* providing that this notice and the authors name and all copyright 
* notices remains intact. 
*
* This file is provided 'as is' with no expressed or implied warranty.
* The author accepts no liability if it causes any damage to your computer.
*
*****************************************************/

// TODO: [changelog] Set Password -> Set Encryption Options

#if !defined(AFX_OPTIONS_H__78001CB2_94A7_4E1A_91E7_53581A687021__INCLUDED_)
#define AFX_OPTIONS_H__78001CB2_94A7_4E1A_91E7_53581A687021__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RegistrySerialize.h"
#include "..\ZipArchive\ZipCompatibility.h"

struct COptionsSmart
{
	BOOL	m_bClrPass; // clear password for files of 0 size
	BOOL	m_bSmart;   // smart compression
	BOOL	m_bDefault; // use system default temp dir when smart	
	BOOL	m_bStoreSmall; // do not compress small files (they will be larger after compression)
	CString	m_szTemp;	// temp dir to use if not default
	void Serialize(CArchive &ar)		
	{
		if (ar.IsStoring())	
		{
			ar << m_bClrPass << m_szTemp << m_bSmart << m_bDefault << m_bStoreSmall;
		}
		else
		{
			ar >> m_bClrPass >> m_szTemp >> m_bSmart >> m_bDefault >> m_bStoreSmall;
		}
	}
	void SetDefaults()
	{
		m_bStoreSmall = m_bDefault = m_bClrPass = TRUE;
		m_bSmart = FALSE;
		m_szTemp.Empty();
	}
	void MakeGlobal();
};

struct COptionsCompress
{
	int		m_iLevel;		// the level of compression
	int		m_iSystem;		// system compatibility of the files to add
	int		m_iFileNames;	// the way to store filenames
	BOOL	m_bRecursive;	// adding folders is recursive
	BOOL	m_bReplace;		// replace files when the filename of the new file would be duplicated
	BOOL	m_iReplaceAction;// == 0: replace the existing file, == 1 : ask before replacing the file, ==2 replace if existing is older, == 3 : skip the file
	void Serialize(CArchive &ar)		
	{
		if (ar.IsStoring())	
		{
			ar << m_iLevel << m_iSystem << m_bRecursive << m_bReplace << m_iReplaceAction << m_iFileNames;
		}
		else
		{
			ar >> m_iLevel >> m_iSystem >> m_bRecursive >> m_bReplace >> m_iReplaceAction >> m_iFileNames;
		}
	}
	void SetDefaults()
	{
		m_iLevel = 6;
		m_iSystem = ZipCompatibility::zcDosFat;
		m_bReplace = m_bRecursive = TRUE;
		m_iReplaceAction = 1; // ask
		m_iFileNames = 0; // normall add
	}
	void MakeGlobal();
};

//	Creating the archive
struct COptionsNew
{
	UINT	m_iVolume;			// the size of the volume in a split archive
	BOOL	m_bUseSegm;			// use segmentation or not
	BOOL	m_iSegm;			// type of segmentation (Split or Span)
	void Serialize(CArchive &ar)		
	{
		if (ar.IsStoring())	
		{
			ar << m_iVolume << m_bUseSegm << m_iSegm;
		}
		else
		{
			ar >> m_iVolume >> m_bUseSegm >> m_iSegm;
		}
	}
	void SetDefaults()
	{
		m_iVolume = 1400000;
		m_iSegm = 0;
		m_bUseSegm = FALSE;
	}
	void MakeGlobal();
	void GetFromGlobal();
};


struct COptionsDoc: public COptionsSmart, public COptionsCompress
{
	COptionsSmart* GetSmart()
	{
		return static_cast<COptionsSmart*>(this);
	}
	COptionsCompress* GetCompress()
	{
		return static_cast<COptionsCompress*>(this);
	}
	void Serialize(CArchive &ar)		
	{
		COptionsSmart::Serialize(ar);	
		COptionsCompress::Serialize(ar);
	}
	void SetDefaults()
	{
		COptionsSmart::SetDefaults();
		COptionsCompress::SetDefaults();
	}
	enum
	{
		glSmart		= 0x01,
		glCompress	= 0x02
	};
	void MakeGlobal(int iWhich)
	{
		if (iWhich & glSmart)
			COptionsSmart::MakeGlobal();
		if (iWhich & glCompress)
			COptionsCompress::MakeGlobal();
	}
	void GetFromGlobal();
};


class COptions:public CObject, public COptionsDoc, public COptionsNew
{
public:
	COptionsDoc* GetDoc()
	{
		return static_cast<COptionsDoc*>(this);
	}
	void SetInfo(CString szSection, LPCTSTR lpszValue);
	bool Save();
	void Load();
	void SetDefaults();
	void Serialize( CArchive& ar );
	COptions();
	virtual ~COptions();


//	GENERAL
	BOOL	m_bAssociate;		// associate .zip files with ZipArc
	BOOL	m_bTDOnRemovable;		// assume TD on removable
	BOOL	m_bAlwaysSave;		// do not prompt to save

//	APPEARANCE
	BOOL	m_bListSorted;		// sort the list
	BOOL	m_bMaximized;		// start child maximized

//	Other
	CString	m_szLastExtractFolder;	// last folder used to extract files to
	int		m_iLastOptionsPage;
	BOOL	m_bFullPath;			// extract with full path
	BOOL	m_bOverwrite;			// overwrite existing files when extracting
	int		m_iExtract;				// extract all or only selected
// Searching
	BOOL	m_bSearchCaseSensitive; // search is case-sens.
	BOOL	m_bSearchFileName;		// search in filenames only (omit the path)
	CString	m_szSearchPattern;		// the wildcard pattern to match against the filenames
protected:
	CRegistrySerialize m_reg;

};

#endif // !defined(AFX_OPTIONS_H__78001CB2_94A7_4E1A_91E7_53581A687021__INCLUDED_)
