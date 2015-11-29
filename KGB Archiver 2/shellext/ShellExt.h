#include <Dsclient.h>
#include <shellapi.h>
#include <string>
#include <vector>

using namespace std;

#define ODS(sz) OutputDebugString(sz)

#ifndef _SHELLEXT_H
#define _SHELLEXT_H

#ifndef X64
	// {0E3EB241-8420-4ab3-97DE-9B8EC429CCC0}
	DEFINE_GUID(CLSID_ShellExtension, 0xe3eb241, 0x8420, 0x4ab3, 0x97, 0xde, 0x9b, 0x8e, 0xc4, 0x29, 0xcc, 0xc0);
	#define GUIDName L"{0E3EB241-8420-4ab3-97DE-9B8EC429CCC0}"
#else
	// {3E1433DE-7E98-441b-B4D1-4D33F40408FF}
	DEFINE_GUID(CLSID_ShellExtension, 0x3e1433de, 0x7e98, 0x441b, 0xb4, 0xd1, 0x4d, 0x33, 0xf4, 0x4, 0x8, 0xff);
	#define GUIDName L"{3E1433DE-7E98-441b-B4D1-4D33F40408FF}"
#endif

// this class factory object creates context menu handlers for Windows 95 shell
class CShellExtClassFactory : public IClassFactory
{
protected:
    ULONG   m_cRef;

public:
    CShellExtClassFactory();
    ~CShellExtClassFactory();

    //IUnknown members
    STDMETHODIMP            QueryInterface(REFIID, LPVOID FAR *);
    STDMETHODIMP_(ULONG)    AddRef();
    STDMETHODIMP_(ULONG)    Release();

    //IClassFactory members
    STDMETHODIMP        CreateInstance(LPUNKNOWN, REFIID, LPVOID FAR *);
    STDMETHODIMP        LockServer(BOOL);

};
typedef CShellExtClassFactory *LPCSHELLEXTCLASSFACTORY;

// this is the actual OLE Shell context menu handler
class CShellExt : public IContextMenu,
                         IShellExtInit/*,
                         IExtractIcon,
                         IPersistFile,
                         IShellPropSheetExt,
                         ICopyHook*/
{
protected:
    ULONG        m_cRef;
    LPDATAOBJECT m_pDataObj;
    char         m_szFileUserClickedOn[MAX_PATH];
	vector<wstring> files;
	int			 m_mode;

    STDMETHODIMP DoCompress(LPCMINVOKECOMMANDINFO lpcmi);

    STDMETHODIMP DoDecompress(LPCMINVOKECOMMANDINFO lpcmi);
public:
    CShellExt();
    ~CShellExt();

    //IUnknown members
    STDMETHODIMP            QueryInterface(REFIID, LPVOID FAR *);
    STDMETHODIMP_(ULONG)    AddRef();
    STDMETHODIMP_(ULONG)    Release();

    //IShell members
    STDMETHODIMP            QueryContextMenu(HMENU hMenu,
                                             UINT indexMenu,
                                             UINT idCmdFirst,
                                             UINT idCmdLast,
                                             UINT uFlags);

    STDMETHODIMP            InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi);

    STDMETHODIMP            GetCommandString(UINT_PTR idCmd,
                                             UINT uFlags,
                                             UINT FAR *reserved,
                                             LPSTR pszName,
                                             UINT cchMax);

    //IShellExtInit methods
    STDMETHODIMP            Initialize(LPCITEMIDLIST pIDFolder,
                                       LPDATAOBJECT pDataObj,
                                       HKEY hKeyID);

    //IExtractIcon methods
    /*STDMETHODIMP GetIconLocation(UINT   uFlags,
                                 LPSTR  szIconFile,
                                 UINT   cchMax,
                                 int   *piIndex,
                                 UINT  *pwFlags);

    STDMETHODIMP Extract(LPCSTR pszFile,
                         UINT   nIconIndex,
                         HICON  *phiconLarge,
                         HICON  *phiconSmall,
                         UINT   nIconSize);

    //IPersistFile methods
    STDMETHODIMP GetClassID(LPCLSID lpClassID);

    STDMETHODIMP IsDirty();

    STDMETHODIMP Load(LPCOLESTR lpszFileName, DWORD grfMode);

    STDMETHODIMP Save(LPCOLESTR lpszFileName, BOOL fRemember);

    STDMETHODIMP SaveCompleted(LPCOLESTR lpszFileName);

    STDMETHODIMP GetCurFile(LPOLESTR FAR* lplpszFileName);

    //IShellPropSheetExt methods
    STDMETHODIMP AddPages(LPFNADDPROPSHEETPAGE lpfnAddPage, LPARAM lParam);

    STDMETHODIMP ReplacePage(UINT uPageID,
                             LPFNADDPROPSHEETPAGE lpfnReplaceWith,
                             LPARAM lParam);

    //ICopyHook method
    STDMETHODIMP_(UINT) CopyCallback(HWND hwnd,
                                     UINT wFunc,
                                     UINT wFlags,
                                     LPCSTR pszSrcFile,
                                     DWORD dwSrcAttribs,
                                     LPCSTR pszDestFile,
                                     DWORD dwDestAttribs);*/

};
typedef CShellExt *LPCSHELLEXT;

#endif // _SHELLEXT_H
