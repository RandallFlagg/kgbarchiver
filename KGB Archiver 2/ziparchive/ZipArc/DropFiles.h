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
#if !defined(AFX_DROPFILES_H__05BC07EF_D436_46B6_B1CC_8878B5A8E6A4__INCLUDED_)
#define AFX_DROPFILES_H__05BC07EF_D436_46B6_B1CC_8878B5A8E6A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDropFiles : public CObject  
{
public:
	void SetMoveFilesMode(bool bMove);
	bool IsMoveFiles();
	//	bool CopyToClipboard();
	static UINT s_uPDEFormat;
	bool CreateFromClipboard();
	CStringArray* m_pszaFiles;
	DWORD m_uDropEffect;
	void Serialize(CArchive& ar);
	DECLARE_SERIAL(CDropFiles)
	CDropFiles();
	CDropFiles(bool bWinNT);
	virtual ~CDropFiles();
	bool m_bWinNT;
};

#endif // !defined(AFX_DROPFILES_H__05BC07EF_D436_46B6_B1CC_8878B5A8E6A4__INCLUDED_)
