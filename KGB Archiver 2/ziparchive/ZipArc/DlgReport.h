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

#if !defined(AFX_DlgReport_H__4CF42759_3792_4C63_9119_5A5DD5B6E979__INCLUDED_)
#define AFX_DlgReport_H__4CF42759_3792_4C63_9119_5A5DD5B6E979__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReport.h : header file
//
#include "ResizableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgReport dialog

class CDlgReport : public CResizableDialog
{
// Construction
public:
	int m_iType;
	bool m_bEnableEmpty;
	CDlgReport(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgReport)
	enum { IDD = IDD_REPORT };
	CString	m_szReport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgReport)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgReport)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DlgReport_H__4CF42759_3792_4C63_9119_5A5DD5B6E979__INCLUDED_)
