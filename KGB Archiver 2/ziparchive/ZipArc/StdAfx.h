
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
#if !defined(AFX_STDAFX_H__381013D5_3622_4456_B966_A8C434928849__INCLUDED_)
#define AFX_STDAFX_H__381013D5_3622_4456_B966_A8C434928849__INCLUDED_

#if _MSC_VER > 1000
#ifndef WINVER
	#define WINVER 0x0400
#endif
#pragma once
#endif // _MSC_VER > 1000

#define WM_PROGRESS WM_USER + 1
#define WM_REDISPLAY WM_USER + 2
#define WM_MODIFYMENU WM_USER + 3
#define WM_SETREPORT WM_USER + 4
#define WM_FINISHED WM_USER + 5
#define WM_SHOWHIDEVIEW WM_USER + 6
#define WM_SETDEFAULT WM_USER + 7

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#ifdef _DEBUG
/**
	A structure for code execution time measurement;
*/	
struct CDebugTimer
	{
		DWORD m_ticks;
		CDebugTimer(){m_ticks = GetTickCount();}
		void Stop(bool bDisplay = true)
		{
			m_ticks = GetTickCount() - m_ticks;
			if (bDisplay)
			{
					CString sz;
					sz.Format(_T("%d"), m_ticks);
					AfxMessageBox(sz);
	
			}
	}
};

#endif //_DEBUG

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__381013D5_3622_4456_B966_A8C434928849__INCLUDED_)
