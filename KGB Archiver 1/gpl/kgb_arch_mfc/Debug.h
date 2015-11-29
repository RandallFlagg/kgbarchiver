////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-2005 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// ---
// General purpose debugging utilities.
//
#ifndef _DEBUG_H
#define _DEBUG_H

#ifndef countof
#define countof(x)	(sizeof(x)/sizeof(x[0]))
#endif

#ifdef _DEBUG

// tell linker to look in ole32 lib (for StringFromClsid)
#pragma comment(linker, "/defaultlib:ole32.lib")

//////////////////
// The following stuff is for getting human-readable names of things so
// you can show them in TRACE statements. For example,
//
// TRACE("Window is: %s\n", _TR(pWnd));
//
// Will generate output showing the name and title of the window, etc.
//

// Macro _TR casts _DbgName(x) to LPCTSTR for use with in printf so you
// can write
//
//   TRACE("Message is %s\n", _TR(uMsg));
//
// instead of
//
//   TRACE("Message is %s\n", (LPCTSTR)_DbgName(uMsg));
//
#define _TR(x)			(LPCTSTR)_DbgName(x)

// overloaded fns to get names of things.
extern CString _DbgName(CWnd* pWnd); // get name of window
extern CString _DbgName(UINT uMsg);	// ... WM_ message
extern CString _DbgName(REFIID iid);	// get name of COM interface
extern CString _DbgName(SCODE sc);	// get name of COM SCODE

// You can use this to debug-print COM interface IDs. Just add the following
// lines to your program:
// TODO
//
//
//
// NULL-terminated array of DEBUGGUIDNAME's in your program, and code
// DBG_ADD_INTERFACE_NAMES with this array. For an example of how this works,
// see dibview.cpp in samples\quikview\dibview. The purpose is so you can write
//
//		TRACE("interface is %s\n", _TR(iid));
//
// to generate a diagnostic like
//
//		interface is IUnknown
//
// PixieLib already knows the names of many common interfaces (see Debug.cpp)
//
struct DBGINTERFACENAME {
	const IID* piid;	// ptr to GUID
	LPCSTR name;		// human-readable name of interface
};

// Used to add interface names to global list -- Use macro
class CInterfaceNames {
protected:
	static CInterfaceNames* s_pFirst;
	CInterfaceNames*			m_pNext;
	DBGINTERFACENAME*			m_pEntries;
	UINT							m_nEntries;
public:
	CInterfaceNames(DBGINTERFACENAME* pdin, UINT n);
	static const DBGINTERFACENAME* FindEntry(REFIID iid);
};

#define DEBUG_BEGIN_INTERFACE_NAMES()								\
static DBGINTERFACENAME _myDBI[] = {								\

#define DEBUG_INTERFACE_NAME(iface)									\
	{ &IID_##iface, _T(#iface) },										\

#define DEBUG_END_INTERFACE_NAMES()									\
};																				\
static CInterfaceNames _initMyDBI(_myDBI, countof(_myDBI));	\

// Macro casts to LPCTSTR for use with TRACE/printf/CString::Format
//
#define DbgName(x)	(LPCTSTR)_DbgName(x)

#else // Below NOT _DEBUG ----------------------------------------------------

#define _TR(x)			((LPCTSTR)NULL)
#define DbgName(x)	((LPCTSTR)NULL)

#define DEBUG_BEGIN_INTERFACE_NAMES()
#define DEBUG_END_INTERFACE_NAMES()
#define DEBUG_INTERFACE_NAME(iface)

#endif // _DEBUG

#endif // _DEBUG_H
