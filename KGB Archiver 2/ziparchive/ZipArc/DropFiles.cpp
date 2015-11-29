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
#include "DropFiles.h"
#include <afxadv.h> // for CSharedFile
#include "..//ZipArchive//ZipAutoBuffer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDropFiles , CObject, 1)
UINT CDropFiles::s_uPDEFormat = RegisterClipboardFormat(CFSTR_PREFERREDDROPEFFECT);

CDropFiles::CDropFiles(bool bWinNT):m_bWinNT(bWinNT)
{
	m_uDropEffect = DROPEFFECT_COPY;
	m_pszaFiles = NULL;
}

CDropFiles::CDropFiles()
{
	m_uDropEffect = DROPEFFECT_COPY;
	m_bWinNT = false; // will work also under NT
	m_pszaFiles = NULL;
}


CDropFiles::~CDropFiles()
{
}

void CDropFiles::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_uDropEffect;
	}
	else
	{	// loading code
		ar >> m_uDropEffect;
	}
	m_pszaFiles->Serialize(ar);
}

bool CDropFiles::CreateFromClipboard()
{
	ASSERT(m_pszaFiles);
	m_pszaFiles->RemoveAll();
	m_uDropEffect = DROPEFFECT_COPY;
	if (!s_uPDEFormat)
		return false;
	if (!IsClipboardFormatAvailable(CF_HDROP))
		return false;
	
	if (!OpenClipboard(AfxGetMainWnd()->m_hWnd))
		return false;
	HGLOBAL hglb = GetClipboardData(CF_HDROP);
	if (!hglb)
	{
		CloseClipboard();
		return false;
	}
	HDROP hdrop = (HDROP)GlobalLock(hglb);
	bool bRet = true;
	if (hdrop)
	{
		UINT cFiles = ::DragQueryFile(hdrop, (UINT) - 1, NULL, 0);
		
		CString szFile;
		
		for (UINT count = 0; count < cFiles; count++)
		{
			::DragQueryFile(hdrop, count, szFile.GetBuffer(MAX_PATH), MAX_PATH);
			szFile.ReleaseBuffer();
			m_pszaFiles->Add(szFile);
		}
		GlobalUnlock(hdrop);
		
		if (IsClipboardFormatAvailable(s_uPDEFormat))
		{
			hglb = GetClipboardData(s_uPDEFormat);
			if (hglb)
			{
				LPDWORD pDword = (LPDWORD)GlobalLock(hglb);
				if (pDword)
				{
					m_uDropEffect = *pDword;
					GlobalUnlock(pDword);					
				}
			}
		}
	}
	else
		bRet = false;
	CloseClipboard();
	if (!m_pszaFiles->GetSize())
		return false;

	return bRet;
}

//bool CDropFiles::CopyToClipboard()
//{
//	ASSERT(m_pszaFiles);
//	if (!m_pszaFiles->GetSize())
//		return false;
//	CSharedFile sf;
//	try
//	{
//		DROPFILES dobj = { 20, { 0, 0 }, 0, m_bWinNT ? 1 : 0 };
//		sf.Write(&dobj, sizeof(dobj));
//		int iCount = m_pszaFiles->GetSize();
//		for (int i = 0; i < iCount; i++)
//		{
//			CString& szString = (*m_pszaFiles)[i];
//			if (m_bWinNT)
//			{
//				int iBufSize = szString.GetLength() * 2;
//				if (!iBufSize)
//					continue;
//				iBufSize += 2; // NULL
//				CZipAutoBuffer buf (iBufSize);
//				if (MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szString, -1, (LPWSTR)(LPCTSTR)buf, iBufSize))
//				{
//					sf.Write(buf, iBufSize);
//				}
//			}
//			else
//				sf.Write((LPCTSTR)szString, szString.GetLength() + 1);
//		}
//		WORD end = 0;
//		sf.Write(&end, 2);
//	}
//	catch (CException* e)
//	{
//		e->Delete();
//		return false;
//	}
//
//	if (OpenClipboard(AfxGetMainWnd()->m_hWnd) && EmptyClipboard())
//	{
//		HGLOBAL hglb = GlobalAlloc  ( GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DWORD) );
//		if (hglb)
//		{
//			DWORD *pdw = (DWORD *)GlobalLock(hglb);
//			if (pdw)
//				*pdw = m_uDropEffect;
//		
//			GlobalUnlock(hglb);
//			SetClipboardData(s_uPDEFormat, hglb);
//		}
//
//		SetClipboardData(CF_HDROP, sf.Detach());
//		CloseClipboard();
//		return true;
//	}
//	return false;
//}

bool CDropFiles::IsMoveFiles()
{
	return m_uDropEffect == DROPEFFECT_MOVE;
}

void CDropFiles::SetMoveFilesMode(bool bMove)
{
	m_uDropEffect = bMove ? DROPEFFECT_MOVE : DROPEFFECT_COPY;
}
