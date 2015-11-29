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
#include "Signal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSignal::CSignal(bool bInitiallyActive):CEvent(bInitiallyActive, TRUE)
{

}

CSignal::~CSignal()
{

}

void CSignal::Activate()
{
	SetEvent();
}

void CSignal::Deactivate()
{
	ResetEvent();
}

bool CSignal::IsActive()
{
	return Lock(0) != 0;
}


bool CSignal::WaitForActivate(DWORD dwTimeOut)
{
	return WaitFor(true, dwTimeOut);
}

bool CSignal::WaitForDeactivate(DWORD dwTimeOut)
{
	return WaitFor(false, dwTimeOut);
}


bool CSignal::WaitFor(bool bActive, DWORD dwTimeOut)
{
	DWORD dw;
	bool bCheckTime = dwTimeOut != INFINITE;
	if (bCheckTime)
		dw = GetTickCount() + dwTimeOut;
	for (;;)
	{
		MSG msg;
		if (bCheckTime && GetTickCount() > dw)
			return false;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!(AfxGetApp()->PumpMessage()))
			{
				::PostQuitMessage(0);
				return false;
			}
		}
		LONG idleCount = 0;
		while (AfxGetApp()->OnIdle(idleCount++));
		if (Lock(0) == (bActive ? TRUE : FALSE))
			return true;
	}


}
