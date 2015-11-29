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
#include "RegistrySerialize.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistrySerialize::CRegistrySerialize()
{
	m_pObject = NULL;
}

CRegistrySerialize::~CRegistrySerialize()
{

}

void CRegistrySerialize::SetInfo(CString szSection, LPCTSTR lpszValue, CObject* pObject)
{
	m_Key.Close();
	m_Key.Attach(AfxGetApp()->GetSectionKey(szSection));
	m_pObject = pObject;
	m_szValue = lpszValue;
}



// serializing structures must follow serializing a common data type,
// because structures are serialized using CArchive::Read function
// which doesn't throw CArchiveException on end-of-file
bool CRegistrySerialize::Load()
{
	if (!Check())
		return false;
	LPBYTE pData = NULL;
	bool ret = false;
	try
	{
	
		DWORD dwType, dwCount;
		LONG lResult = RegQueryValueEx(m_Key, m_szValue, NULL, &dwType, NULL, &dwCount);
		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			pData = new BYTE[dwCount];
			lResult = RegQueryValueEx(m_Key, m_szValue, NULL, &dwType, pData, &dwCount);
		}


		if (lResult == ERROR_SUCCESS)
		{
			ASSERT(dwType == REG_BINARY);
			CMemFile mf(pData, dwCount);
			CArchive ar(&mf, CArchive::load);
			m_pObject->Serialize(ar);
			ret = true;
		}

	}
	catch(CException*e)
	{
		e->Delete();
// 		if (pData)
			delete[]pData;
		return false;
	}
// 	if (pData)
		delete[]pData;
	return ret;

}

// serializing structures must follow serializing a common data type,
// because structures are serialized using CArchive::Read function
// which doesn't throw CArchiveException on end-of-file
bool CRegistrySerialize::Save()
{
	if (!Check())
		return false;

	CMemFile mf;
	try
	{
	
		CArchive ar(&mf, CArchive::store);
		m_pObject->Serialize(ar);
		ar.Close();
	}
	catch(CException* e)
	{
		e->Delete();
		return false;
	}

	DWORD iSize = (DWORD)mf.GetLength();
	BYTE* pData = mf.Detach();
	bool ret = RegSetValueEx(m_Key, m_szValue, 0, REG_BINARY, pData, iSize) == ERROR_SUCCESS;
	free(pData);
	return ret;
}

bool CRegistrySerialize::Check()
{
	return m_Key.m_hKey && m_pObject;
}


void CRegistrySerialize::SetInfo(CString szSection, LPCTSTR lpszValue, CObject &oObject)
{
	SetInfo(szSection, lpszValue, &oObject);
}
