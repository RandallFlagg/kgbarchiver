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


#include "stdafx.h"
#include "ZipArc.h"
#include "Options.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


COptions::COptions()
{
	m_iLastOptionsPage = 0;
	SetDefaults();
}

COptions::~COptions()
{

}

void COptions::Serialize(CArchive &ar)
{
	COptionsDoc::Serialize(ar);
	COptionsNew::Serialize(ar);
	if (ar.IsStoring())	
	{
		ar << m_bTDOnRemovable << m_szLastExtractFolder
			<< m_bListSorted << m_bAssociate << m_bMaximized
			<< m_bAlwaysSave << m_bFullPath << m_bOverwrite << m_iExtract 
			<< m_szSearchPattern << m_bSearchCaseSensitive << m_bSearchFileName;
	}
	else
	{
		ar >> m_bTDOnRemovable >> m_szLastExtractFolder
			>> m_bListSorted >> m_bAssociate >> m_bMaximized
			>> m_bAlwaysSave >> m_bFullPath >> m_bOverwrite >> m_iExtract
			>> m_szSearchPattern >> m_bSearchCaseSensitive >> m_bSearchFileName;

	}
}

void COptions::SetDefaults()
{
	COptionsDoc::SetDefaults();
	COptionsNew::SetDefaults();
	m_bSearchFileName = m_bSearchCaseSensitive = m_bOverwrite = 
		m_bAlwaysSave = m_bMaximized = m_bAssociate = m_bTDOnRemovable = FALSE;
	m_bFullPath = m_bListSorted = TRUE;
	m_szSearchPattern.Empty();
	m_szLastExtractFolder.Empty();
	m_iExtract = 0;
}

void COptions::Load()
{
	if (!m_reg.Load())
		SetDefaults();
}

bool COptions::Save()
{
	return m_reg.Save();
}

void COptions::SetInfo(CString szSection, LPCTSTR lpszValue)
{
	m_reg.SetInfo(szSection, lpszValue, this);
}

void COptionsCompress::MakeGlobal()
{
	COptions* pOptions = CZipArcApp::GetOptions();
	*static_cast<COptionsCompress*>(pOptions) = *this;
	pOptions->Save();
}

void COptionsSmart::MakeGlobal()
{
	COptions* pOptions = CZipArcApp::GetOptions();
	*static_cast<COptionsSmart*>(pOptions) = *this;
	pOptions->Save();
}

void COptionsNew::MakeGlobal()
{
	COptions* pOptions = CZipArcApp::GetOptions();
	*static_cast<COptionsNew*>(pOptions) = *this;
	pOptions->Save();
}

void COptionsNew::GetFromGlobal()
{
	*this = *static_cast<COptionsNew*>(CZipArcApp::GetOptions());
}

void COptionsDoc::GetFromGlobal()
{
	COptions* pOptions = CZipArcApp::GetOptions();
	*GetSmart() = *static_cast<COptionsSmart*>(pOptions);
	*GetCompress() = *static_cast<COptionsCompress*>(pOptions);
}




