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
#include "DlgOptGen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptGen dialog


CDlgOptGen::CDlgOptGen(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgOptGen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptGen)
	//}}AFX_DATA_INIT
}


void CDlgOptGen::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptGen)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_ALWAYS, m_pOptions->m_bAlwaysSave);
	DDX_Check(pDX, IDC_ASSOCIATE, m_pOptions->m_bAssociate);
	DDX_Check(pDX, IDC_ASSUMETD, m_pOptions->m_bTDOnRemovable);

}


BEGIN_MESSAGE_MAP(CDlgOptGen, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CDlgOptGen)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptGen message handlers
