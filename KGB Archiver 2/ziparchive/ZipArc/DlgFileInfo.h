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
#if !defined(AFX_DLGFILEINFO_H__9050616D_6259_4C96_8831_79C85ABBDC0E__INCLUDED_)
#define AFX_DLGFILEINFO_H__9050616D_6259_4C96_8831_79C85ABBDC0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\ZipArchive\ZipArchive.h"



#include "ResizableDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFileInfo dialog

class CDlgFileInfo : public CResizableDialog
{
// Construction
public:
	bool m_bReadOnly;
	CDlgFileInfo(CWnd* pParent = NULL);   // standard constructor
	CZipFileHeader* m_pHeader;
	CZipArchive* m_pArchive;
	CString	m_szInfo;
	CString	m_szComment;



// Dialog Data
	//{{AFX_DATA(CDlgFileInfo)
	enum { IDD = IDD_FILEINFO };
	CButton	m_change;
	CStatic	m_displaying;
	CEdit	m_edit;
	CString	m_szFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFileInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bInfo;

	// Generated message map functions
	//{{AFX_MSG(CDlgFileInfo)
	afx_msg void OnChgdsp();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILEINFO_H__9050616D_6259_4C96_8831_79C85ABBDC0E__INCLUDED_)
