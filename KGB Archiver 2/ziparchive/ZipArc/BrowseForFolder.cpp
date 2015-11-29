/*
* BrowseForFolder.cpp
*
* You are free to use and distribute. May not be sold for profit. 
*****************************************************/


#include "stdafx.h"
#include "BrowseForFolder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifndef BIF_NEWDIALOGSTYLE
	#define BIF_NEWDIALOGSTYLE     0x0040
#endif
#ifndef BIF_SHAREABLE
	#define BIF_SHAREABLE          0x8000
#endif
CBrowseForFolder::CBrowseForFolder()
{
	Image = 0;
	flags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_SHAREABLE;
	hWndOwner = NULL;
}

CBrowseForFolder::~CBrowseForFolder()
{
}

// browse for a folder
bool CBrowseForFolder::GetFolder(CString &returnPath)
{
	LPITEMIDLIST pidlRoot = NULL;
	// get the pidl of the root 
	GetPidl(strDirRoot, pidlRoot);
	
	TCHAR strBuff[MAX_PATH + 1];
	
	returnPath.Empty();
	LPMALLOC pMalloc;
	if (SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		BROWSEINFO brInfo= 
		{
			hWndOwner, 
				pidlRoot, 
				strBuff, 
				strTitle,
				flags,
				BrowseCallbackProc,
				(LPARAM)&strStartupDir,
				Image
		};
		if (flags & 0x0040)
			OleInitialize(NULL);
		LPITEMIDLIST pidl = SHBrowseForFolder(&brInfo);
		if (flags & 0x0040)
			OleUninitialize();

		
		if (pidl)
		{
			SHGetPathFromIDList(pidl, strBuff);
			pMalloc->Free(pidl);
			returnPath = strBuff;
		}
		if (pidlRoot) 
			pMalloc->Free(pidlRoot);
		pMalloc->Release();					
	}
	
	if (!returnPath.IsEmpty())
		return true;
	else 
		return false;
}

void CBrowseForFolder::GetPidl(CString name, LPITEMIDLIST pidl)
{
	LPSHELLFOLDER pshf;
	ULONG chEaten;

#ifdef _UNICODE
	OLECHAR* oleRoot = name.GetBuffer(name.GetLength());
#else
	OLECHAR oleRoot[MAX_PATH];
	// convert path to Unicode string
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, name, -1,
		oleRoot, MAX_PATH);
#endif	
	if (SUCCEEDED(SHGetDesktopFolder(&pshf)))
	{
		// get pidl
		pshf->ParseDisplayName(hWndOwner, NULL, oleRoot, &chEaten, 
			&pidl, NULL);
	}
	else return;
	pshf->Release();
}

// set if the EditBox is present in the browse dialog
void CBrowseForFolder::SetEditBox(bool val)
{
	if (val) 
		flags |= BIF_EDITBOX;
	else 
		flags &= ~BIF_EDITBOX;
}

// set if the StatusBar is present in the browse dialog
// doesn't work with BIF_NEWDIALOGSTYLE
void CBrowseForFolder::SetStatusBar(bool val)
{
	if (val) 
		flags |= BIF_STATUSTEXT;
	else 
		flags &= ~BIF_STATUSTEXT;
}

int CALLBACK CBrowseForFolder::BrowseCallbackProc(HWND hwnd, UINT uMsg, 
												  LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
		case BFFM_INITIALIZED: 
			{
				// set the initial directory
				CString* pString = (CString*)lpData;
				if (!pString) 
					return 0;
				LPTSTR szDir = pString->GetBuffer(pString->GetLength());
				// WParam is TRUE since you are passing a path.
				// It would be FALSE if you were passing a pidl.
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)szDir);             
				break;       
			}
		case BFFM_SELCHANGED: 
			{
				// Set the status window to the currently selected path.
				TCHAR szDir[MAX_PATH];
				if (SHGetPathFromIDList((LPITEMIDLIST) lParam , szDir)) 
					SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
				break;            
			}            
		default:
			break;         
	}         
	return 0;
}



void CBrowseForFolder::SetNewStyle(bool val)
{
	if (val) 
		flags |= 0x0040;
	else 
		flags &= ~0x0040;
}

