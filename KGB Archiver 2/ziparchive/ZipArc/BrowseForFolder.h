/*
* BrowseForFolder.h
*
* You are free to use and distribute. May not be sold for profit. 
*****************************************************/


#if !defined(AFX_BROWSEFORFOLDER_H__67074462_2EEE_11D3_A25C_F4E7CA30A942__INCLUDED_)
#define AFX_BROWSEFORFOLDER_H__67074462_2EEE_11D3_A25C_F4E7CA30A942__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBrowseForFolder  
{
public:
	void SetNewStyle(bool val);
	// parent's HWND
	HWND hWndOwner;
	// directory to select
	CString strStartupDir;
	void SetStatusBar(bool val);
	void SetEditBox(bool val);
	// title of the browse dialog
	CString strTitle;
	// the same as Image variable in the BROWSEINFO structure ( see help )
	int Image;
	// directory to explore from
	CString strDirRoot;
	bool GetFolder(CString &returnPath);
	CBrowseForFolder();
	virtual ~CBrowseForFolder();

protected:
	UINT flags;
	void GetPidl(CString name, LPITEMIDLIST);
	static int CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
private:

};


#endif // !defined(AFX_BROWSEFORFOLDER_H__67074462_2EEE_11D3_A25C_F4E7CA30A942__INCLUDED_)
