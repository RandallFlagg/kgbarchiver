#include "priv.h"

//
// Initialize GUIDs (should be done only and at-least once per DLL/EXE)
//
#pragma data_seg(".text")
#define INITGUID
#include <initguid.h>
#include <shlguid.h>
#include "shellext.h"
#pragma data_seg()

#ifndef _delayimp_h
  extern "C" IMAGE_DOS_HEADER __ImageBase;
#endif

//
// Global variables
//
UINT      g_cRefThisDll = 0;    // Reference count of this DLL.
HINSTANCE g_hmodThisDll = NULL; // Handle to this DLL itself.

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        ODS(L"In DLLMain, DLL_PROCESS_ATTACH\r\n");

        // Extension DLL one-time initialization

        g_hmodThisDll = hInstance;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        ODS(L"In DLLMain, DLL_PROCESS_DETACH\r\n");
    }

    return 1;   // ok
}

//---------------------------------------------------------------------------
// DllCanUnloadNow
//---------------------------------------------------------------------------

STDAPI DllCanUnloadNow(void)
{
    ODS(L"In DLLCanUnloadNow\r\n");

    return (g_cRefThisDll == 0 ? S_OK : S_FALSE);
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppvOut)
{
    ODS(L"In DllGetClassObject\r\n");

    *ppvOut = NULL;

    if (IsEqualIID(rclsid, CLSID_ShellExtension))
    {
        CShellExtClassFactory *pcf = new CShellExtClassFactory;

        return pcf->QueryInterface(riid, ppvOut);
    }

    return CLASS_E_CLASSNOTAVAILABLE;
}

STDAPI DllRegisterServer(){
	HRESULT hr = S_OK;
	HKEY key;
	wchar_t module[MAX_PATH];

	GetModuleFileName(reinterpret_cast<HMODULE>(&__ImageBase), module, sizeof(module)/sizeof(*module));

#ifndef X64
	if(ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, L"*\\ShellEx\\ContextMenuHandlers\\KGBShellEx", &key))
		hr = E_UNEXPECTED;
#else
	if(ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, L"*\\ShellEx\\ContextMenuHandlers\\KGBShellEx64", &key))
		hr = E_UNEXPECTED;
#endif
	if(ERROR_SUCCESS != RegSetValue(key, L"", REG_SZ, GUIDName, sizeof(wchar_t)*wcslen(GUIDName)))
		hr = E_UNEXPECTED;
	RegCloseKey(key);

#ifndef X64
	if(ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, L"Directory\\ShellEx\\ContextMenuHandlers\\KGBShellEx", &key))
		hr = E_UNEXPECTED;
#else
	if(ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, L"Directory\\ShellEx\\ContextMenuHandlers\\KGBShellEx64", &key))
		hr = E_UNEXPECTED;
#endif
	if(ERROR_SUCCESS != RegSetValue(key, L"", REG_SZ, GUIDName, sizeof(wchar_t)*wcslen(GUIDName)))
		hr = E_UNEXPECTED;
	RegCloseKey(key);

	if(ERROR_SUCCESS != RegCreateKey(HKEY_CLASSES_ROOT, L"CLSID\\" GUIDName L"\\InProcServer32", &key))
		hr = E_UNEXPECTED;
	if(ERROR_SUCCESS != RegSetValue(key, L"", REG_SZ, module, sizeof(wchar_t)*wcslen(module)))
		hr = E_UNEXPECTED;
	if(ERROR_SUCCESS != RegSetValue(key, L"ThreadingModel", REG_SZ, L"Apartment", sizeof(wchar_t)*wcslen(L"Apartment")))
		hr = E_UNEXPECTED;

	RegCloseKey(key);

	if(ERROR_SUCCESS != RegOpenKey(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved", &key))
		hr = E_UNEXPECTED;
	if(ERROR_SUCCESS != RegSetValue(key, GUIDName, REG_SZ, L"KGB Archiver shell extension", sizeof(wchar_t)*wcslen(L"KGB Archiver shell extension")))
		hr = E_UNEXPECTED;

	RegCloseKey(key);

	return hr;
}

STDAPI DllUnregisterServer(){
	HRESULT hr = S_OK;
	HKEY key;

#ifndef X64
	RegDeleteKey(HKEY_CLASSES_ROOT, L"*\\ShellEx\\ContextMenuHandlers\\KGBShellEx");
	RegDeleteKey(HKEY_CLASSES_ROOT, L"Directory\\ShellEx\\ContextMenuHandlers\\KGBShellEx");
#else
	RegDeleteKey(HKEY_CLASSES_ROOT, L"*\\ShellEx\\ContextMenuHandlers\\KGBShellEx64");
	RegDeleteKey(HKEY_CLASSES_ROOT, L"Directory\\ShellEx\\ContextMenuHandlers\\KGBShellEx64");
#endif
	RegDeleteKey(HKEY_CLASSES_ROOT, L"CLSID\\" GUIDName);
	RegOpenKey(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved", &key);
	RegDeleteValue(key, GUIDName);
	RegCloseKey(key);

	return hr;
}

CShellExtClassFactory::CShellExtClassFactory()
{
    ODS(L"CShellExtClassFactory::CShellExtClassFactory()\r\n");

    m_cRef = 0L;

    g_cRefThisDll++;
}

CShellExtClassFactory::~CShellExtClassFactory()
{
    g_cRefThisDll--;
}

STDMETHODIMP CShellExtClassFactory::QueryInterface(REFIID riid,
                                                   LPVOID FAR *ppv)
{
    ODS(L"CShellExtClassFactory::QueryInterface()\r\n");

    *ppv = NULL;

    // Any interface on this object is the object pointer

    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
    {
        *ppv = (LPCLASSFACTORY)this;

        AddRef();

        return NOERROR;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CShellExtClassFactory::AddRef()
{
    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CShellExtClassFactory::Release()
{
    if (--m_cRef)
        return m_cRef;

    delete this;

    return 0L;
}

STDMETHODIMP CShellExtClassFactory::CreateInstance(LPUNKNOWN pUnkOuter,
                                                      REFIID riid,
                                                      LPVOID *ppvObj)
{
    ODS(L"CShellExtClassFactory::CreateInstance()\r\n");

    *ppvObj = NULL;

    // Shell extensions typically don't support aggregation (inheritance)

    if (pUnkOuter)
        return CLASS_E_NOAGGREGATION;

    // Create the main shell extension object.  The shell will then call
    // QueryInterface with IID_IShellExtInit--this is how shell extensions are
    // initialized.

    LPCSHELLEXT pShellExt = new CShellExt();  //Create the CShellExt object

    if (NULL == pShellExt)
        return E_OUTOFMEMORY;

    return pShellExt->QueryInterface(riid, ppvObj);
}


STDMETHODIMP CShellExtClassFactory::LockServer(BOOL fLock)
{
    return NOERROR;
}

// *********************** CShellExt *************************
CShellExt::CShellExt()
{
    ODS(L"CShellExt::CShellExt()\r\n");

    m_cRef = 0L;
    m_pDataObj = NULL;

    g_cRefThisDll++;
}

CShellExt::~CShellExt()
{
    if (m_pDataObj)
        m_pDataObj->Release();

	files.clear();

    g_cRefThisDll--;
}

STDMETHODIMP CShellExt::QueryInterface(REFIID riid, LPVOID FAR *ppv)
{
    *ppv = NULL;

    if (IsEqualIID(riid, IID_IShellExtInit) || IsEqualIID(riid, IID_IUnknown))
    {
        ODS(L"CShellExt::QueryInterface()==>IID_IShellExtInit\r\n");

        *ppv = (LPSHELLEXTINIT)this;
    }
    else if (IsEqualIID(riid, IID_IContextMenu))
    {
        ODS(L"CShellExt::QueryInterface()==>IID_IContextMenu\r\n");

        *ppv = (LPCONTEXTMENU)this;
    }
    /*else if (IsEqualIID(riid, IID_IExtractIcon))
    {
        ODS(L"CShellExt::QueryInterface()==>IID_IExtractIcon\r\n");

        *ppv = (LPEXTRACTICON)this;
    }
    else if (IsEqualIID(riid, IID_IPersistFile))
    {
        ODS(L"CShellExt::QueryInterface()==>IPersistFile\r\n");

        *ppv = (LPPERSISTFILE)this;
    }
    else if (IsEqualIID(riid, IID_IShellPropSheetExt))
    {
        ODS(L"CShellExt::QueryInterface()==>IShellPropSheetExt\r\n");

        *ppv = (LPSHELLPROPSHEETEXT)this;
    }
    else if (IsEqualIID(riid, IID_IShellCopyHook))
    {
        ODS(L"CShellExt::QueryInterface()==>ICopyHook\r\n");

        *ppv = (LPCOPYHOOK)this;
    }*/

    if (*ppv)
    {
        AddRef();

        return NOERROR;
    }

    ODS(L"CShellExt::QueryInterface()==>Unknown Interface!\r\n");

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CShellExt::AddRef()
{
    ODS(L"CShellExt::AddRef()\r\n");

    return ++m_cRef;
}

STDMETHODIMP_(ULONG) CShellExt::Release()
{
    ODS(L"CShellExt::Release()\r\n");

    if (--m_cRef)
        return m_cRef;

    delete this;

    return 0L;
}
