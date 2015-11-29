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

#if !defined(AFX_SIGNAL_H__EC536E5A_2997_4498_A7DD_2D3C92897E16__INCLUDED_)
#define AFX_SIGNAL_H__EC536E5A_2997_4498_A7DD_2D3C92897E16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxmt.h>

class CSignal : public CEvent  
{
public:
	bool WaitForDeactivate(DWORD dwTimeOut = INFINITE); // time in miliseconds
	bool WaitForActivate(DWORD dwTimeOut = INFINITE); // time in miliseconds
	bool IsActive();
	void Deactivate();
	void Activate();
	CSignal(bool bInitiallyActive = false);
	virtual ~CSignal();

protected:
	bool WaitFor(bool bActive, DWORD dwTimeOut);
};

#endif // !defined(AFX_SIGNAL_H__EC536E5A_2997_4498_A7DD_2D3C92897E16__INCLUDED_)
