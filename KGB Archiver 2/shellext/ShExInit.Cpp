#include "priv.h"
#include "shellext.h"


//
//  FUNCTION: CShellExt::Initialize(LPCITEMIDLIST, LPDATAOBJECT, HKEY)
//
//  PURPOSE: Called by the shell when initializing a context menu or property
//           sheet extension.
//
//  PARAMETERS:
//    pIDFolder - Specifies the parent folder
//    pDataObj  - Spefifies the set of items selected in that folder.
//    hRegKey   - Specifies the type of the focused item in the selection.
//
//  RETURN VALUE:
//
//    NOERROR in all cases.
//
//  COMMENTS:   Note that at the time this function is called, we don't know
//              (or care) what type of shell extension is being initialized.
//              It could be a context menu or a property sheet.
//

STDMETHODIMP CShellExt::Initialize(LPCITEMIDLIST pIDFolder,
                                   LPDATAOBJECT pDataObj,
                                   HKEY hRegKey)
{
    ODS(L"CShellExt::Initialize()\r\n");

    // Initialize can be called more than once

    if (m_pDataObj)
        m_pDataObj->Release();

    // duplicate the object pointer and registry handle

    if (pDataObj){
        m_pDataObj = pDataObj;
        pDataObj->AddRef();
    }

	FORMATETC fmte = {CF_HDROP, (DVTARGETDEVICE FAR *)NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
    STGMEDIUM medium;
	HRESULT hr = NOERROR;

	if(m_pDataObj)  //Paranoid check, m_pDataObj should have something by now...
       hr = m_pDataObj->GetData(&fmte, &medium);

    if(SUCCEEDED(hr)){
		unsigned int cbFiles = 0;
		wchar_t temp[MAX_PATH], temp2[MAX_PATH];

		if(medium.hGlobal)
            cbFiles = DragQueryFile((HDROP)medium.hGlobal, (UINT)-1, 0, 0);

		for(unsigned int i=0;i<cbFiles;++i){
               DragQueryFile((HDROP)medium.hGlobal, i, temp, sizeof(temp)/sizeof(wchar_t));
			   GetLongPathName(temp, temp2, MAX_PATH);
			   files.push_back(temp);
		}
	}

    return NOERROR;
}
