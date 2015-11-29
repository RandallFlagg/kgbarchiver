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


#if !defined(AFX_ZIPARC_H__1C0BF0C9_E855_4892_9DA2_9FB764B281FE__INCLUDED_)
#define AFX_ZIPARC_H__1C0BF0C9_E855_4892_9DA2_9FB764B281FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "Options.h"
/////////////////////////////////////////////////////////////////////////////
// CZipArcApp:
// See ZipArc.cpp for the implementation of this class
//
#include "MainFrm.h"

class CZipArcApp : public CWinApp
{
	class CCommandLineInfoZipArc : public CCommandLineInfo
	{
	public:
		CCommandLineInfoZipArc()
		{
			m_nShellCommand = FileNothing;
			m_bRegister = false;
		}
		bool m_bRegister;
		void ParseParam( LPCTSTR lpszParam, BOOL bFlag, BOOL bLast )
		{
			if (bFlag && lstrcmp(lpszParam, _T("register")) == 0)
			{
				m_bRegister = true;
			}
			else
				CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
		}
	
	};
public:
	static CString FormatSize(ZIP_U32_U64 iSize);

	static bool GetTempPath(CString& szTemp);
	bool Register(bool bRegister, bool bQuiet = false);
	COptions m_options;
	static COptions* GetOptions()
	{
		return &((CZipArcApp*)AfxGetApp())->m_options;
	}
	static CMainFrame* GetFrame()
	{
		return (CMainFrame*)AfxGetMainWnd();
	}
	CZipArcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZipArcApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CZipArcApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void UnregisterFileTypes();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZIPARC_H__1C0BF0C9_E855_4892_9DA2_9FB764B281FE__INCLUDED_)
