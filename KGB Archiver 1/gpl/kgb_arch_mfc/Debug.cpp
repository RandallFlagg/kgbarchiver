////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-2005 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// ---
// General purpose debugging utilities.
//
#include "StdAfx.h"
#include "Debug.h"
#include <afxpriv.h>	// for MFC WM_ messages

#ifdef _DEBUG

#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#define _countof(array) (sizeof(array)/sizeof(array[0]))

CString _DbgName(SCODE sc)
{
	extern LPCTSTR AFXAPI AfxGetScodeString(SCODE sc);
	CString s = AfxGetScodeString(sc);
	if (s.IsEmpty())
		s.Format(_T("0x%x"),sc);
	return s;
}

// Used for debugging--Interfaces I want to watch
DEBUG_BEGIN_INTERFACE_NAMES()
	DEBUG_INTERFACE_NAME(IUnknown)
DEBUG_END_INTERFACE_NAMES()

CInterfaceNames* CInterfaceNames::s_pFirst = NULL;

CInterfaceNames::CInterfaceNames(DBGINTERFACENAME* pdin, UINT n)
{
	ASSERT(pdin);
	m_pEntries = pdin;
	m_pNext    = s_pFirst;
	s_pFirst   = this;
	m_nEntries = n;
}

const DBGINTERFACENAME* CInterfaceNames::FindEntry(REFIID iid)
{
	for (CInterfaceNames* pNames = s_pFirst;
		  pNames;
		  pNames=pNames->m_pNext) {

		for (UINT i=0; i<pNames->m_nEntries; i++) {
			const DBGINTERFACENAME* pdin = &pNames->m_pEntries[i];
			if (memcmp(pdin->piid, &iid, sizeof(IID))==0) {
				return pdin;
			}
		}
	}
	return NULL;
}	
		  
//////////////////
// Get human-readable COM interface name.
//
CString _DbgName(REFIID iid)
{
	CString s;

	const DBGINTERFACENAME* pdin = CInterfaceNames::FindEntry(iid);
	if (pdin)
		s = pdin->name;
	else {
		LPOLESTR lp=NULL;
		StringFromCLSID(iid, &lp);
		s = lp;
	}
	return s;
}

#endif // DEBUG

