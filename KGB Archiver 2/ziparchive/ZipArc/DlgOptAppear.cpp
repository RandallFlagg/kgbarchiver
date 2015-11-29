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
#include "ziparc.h"
#include "DlgOptAppear.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptAppear dialog


CDlgOptAppear::CDlgOptAppear(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgOptAppear::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptAppear)
	//}}AFX_DATA_INIT
}


void CDlgOptAppear::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptAppear)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_MAXIMIZED, m_pOptions->m_bMaximized);
	DDX_Check(pDX, IDC_SORTED, m_pOptions->m_bListSorted);

}


BEGIN_MESSAGE_MAP(CDlgOptAppear, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CDlgOptAppear)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptAppear message handlers
