// kgb_arch_conf.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

static char *loadString(int id){
	CString cs;
	cs.LoadString(id);
	return (char *)cs.GetString();
}


// Ckgb_arch_confApp:
// See kgb_arch_conf.cpp for the implementation of this class
//

class Ckgb_arch_confApp : public CWinApp
{
public:
	Ckgb_arch_confApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Ckgb_arch_confApp theApp;