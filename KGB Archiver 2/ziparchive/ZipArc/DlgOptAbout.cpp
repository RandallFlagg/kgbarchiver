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
#include "DlgOptAbout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptAbout dialog


CDlgOptAbout::CDlgOptAbout(CWnd* pParent /*=NULL*/)
	: CSAPrefsSubDlg(CDlgOptAbout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOptAbout)
	//}}AFX_DATA_INIT
}


void CDlgOptAbout::DoDataExchange(CDataExchange* pDX)
{
	CSAPrefsSubDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptAbout)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptAbout, CSAPrefsSubDlg)
	//{{AFX_MSG_MAP(CDlgOptAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptAbout message handlers

BOOL CDlgOptAbout::OnInitDialog() 
{

	// TODO: [changelog] ziparchive updated to 2.0 (zip64, aes)
	CSAPrefsSubDlg::OnInitDialog();
	CString s;
	s = _T("<font color='255,255,255' face='arial' size='12' align='center'>");
	s += _T("<br><font size='28' face='Comic Sans MS' style='bi' color='255,204,51'>ZipArc</font><br>");
	s += _T("<font size='14' face='Courier New' color='209,180,50'>VERSION 2.0</font><p>");
	s += _T("Copyright (c) 2000 - 2006 by <a href='mailto:tdracz@artpol-software.com?SUBJECT=ZipArc' style='b'><font style='u'>Tadeusz Dracz</font></a><p>");
	s += _T("<vspace size='5'><font color='250,150,18'>");
	s += _T("<font style='bi'>web:   </font><font style='u'><a href='http://www.artpol-software.com' color='255,200,100'>http://www.artpol-software.com</a></font><br>");
	s += _T("</font>");
	s += _T("<p><font align='center' size='12' face='Tahoma' color='102,204,254'>This program was created with <br><a href='http://www.artpol-software.com' background='102,204,254' color='255,255,255'><font size='15' style='bu' color='51,255,51' face='Comic Sans MS'>The ZipArchive Library</font></a> <br>and is a demonstration of its possibilities<br></font>");
	s += _T("</font>");
	s += _T("<p><font size='20' face='Comic Sans MS' style='u' color='250,250,100'>Credits</font><br>");
	s += _T("<font size='11' face='Comic Sans MS' color='250,250,250'>in no particular order</font><p>");
	s += _T("<font size='12' color='255,255,102'>");
	s += _T("<font style='b'>Dr Brian Gladman</font><br>for<br><font color='255,51,51'>AES Support in the ZipArchive Library</font><br>");
	s += _T("<a href='http://fp.gladman.plus.com/AES/index.htm' color='255,255,255' style='u'>http://fp.gladman.plus.com/AES/index.htm</a><p>");
	s += _T("<font style='b'>Smaller Animals Software, Inc.</font><br>for<br><font color='255,51,51'>Netscape-like options dialog</font><br>");
	s += _T("<a href='http://www.codeproject.com/property/saprefs.asp' color='255,255,255' style='u'>http://www.codeproject.com/property/saprefs.asp</a><p>");
	s += _T("<font style='b'>Chris Maunder</font><br>for<br><font color='255,51,51'>CProgressBar</font><br>");
	s += _T("<a href='http://www.codeproject.com' color='255,255,255' style='u'>http://www.codeproject.com</a><p>");
	s += _T("<font style='b'>Christian Graus</font><br>for<br><font color='255,51,51'>CCheckStatic</font><br>");
	s += _T("<a href='http://www.codeproject.com/staticctrl/ccheckstatic.asp' color='255,255,255' style='u'>http://www.codeproject.com/staticctrl/ccheckstatic.asp</a><p>");
	s += _T("<font style='b'>Douglas Peterson</font><br>for<br><font color='255,51,51'>CSortableObArray</font><br>");
	s += _T("<a href='http://www.codeguru.com/cpp_mfc/sortable_cobarray.shtml' color='255,255,255' style='u'>http://www.codeguru.com/cpp_mfc/sortable_cobarray.shtml</a><p>");
	s += _T("<font style='b'>Marc Richarme</font><br>for<br><font color='255,51,51'>this Credits Control</font><br>");
	s += _T("<a href='http://www.codeproject.com/dialog/ccreditsctrl.asp' color='255,255,255' style='u'>http://www.codeproject.com/dialog/ccreditsctrl.asp</a><p>");
	s += _T("<p></font>");
	s += _T("<font color='100,75,25'>___</font>");
	s += _T("<font color='150,115,50'>___</font>");
	s += _T("<font color='200,155,75'>___</font>");
	s += _T("<font color='255,204,102'>_______</font>");
	s += _T("<font color='200,155,75'>___</font>");
	s += _T("<font color='150,115,50'>___</font>");
	s += _T("<font color='100,75,25'>___</font>");
	s += _T("<p>");
	
	m_wndCredits.m_bCanScroll = TRUE;
	m_wndCredits.SetDataString(s);
	m_wndCredits.SetDefaultBkColor(RGB(51, 51, 51));
	m_wndCredits.m_crInternalTransparentColor = RGB(51,51,51);
	m_wndCredits.SetDefaultLinkCursor();
	int i = m_wndCredits.m_nTimerSpeed;
	m_wndCredits.m_nTimerSpeed = 100;
	CWaitCursor cw;
	m_wndCredits.Create(WS_EX_CLIENTEDGE,WS_VISIBLE|WS_CHILD,IDC_FRAME,this, 0, IDC_MOVE);
	m_wndCredits.m_nTimerSpeed = i;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

