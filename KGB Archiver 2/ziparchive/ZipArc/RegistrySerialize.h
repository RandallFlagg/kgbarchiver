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

#if !defined(AFX_REGISTRYSERIALIZE_H__50F70D46_D431_4414_9777_1360E38400E6__INCLUDED_)
#define AFX_REGISTRYSERIALIZE_H__50F70D46_D431_4414_9777_1360E38400E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <atlbase.h>
class CRegistrySerialize  
{
public:
	void SetInfo(CString szSection, LPCTSTR lpszValue, CObject& oObject);
	bool Save();
	bool Load();
	void SetInfo(CString szSection, LPCTSTR lpszValue, CObject* pObject);
	CRegistrySerialize();
	virtual ~CRegistrySerialize();

protected:
	bool Check();
	CObject* m_pObject;
	CString m_szValue;
	CRegKey m_Key;
};

#endif // !defined(AFX_REGISTRYSERIALIZE_H__50F70D46_D431_4414_9777_1360E38400E6__INCLUDED_)
