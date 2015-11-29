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

#if !defined(AFX_INFOBUTTON_H__B7A0381A_5766_4C59_BFAC_0F977A807E02__INCLUDED_)
#define AFX_INFOBUTTON_H__B7A0381A_5766_4C59_BFAC_0F977A807E02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CInfoButton window

class CInfoButton : public CButton
{
// Construction
public:
	CInfoButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoButton)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetInfo(UINT nID,  LPCTSTR lpszIconName = IDI_ASTERISK);
	virtual ~CInfoButton();

	// Generated message map functions
protected:
	LPCTSTR m_lpszStandardIcon;
	UINT m_InfoID;
	//{{AFX_MSG(CInfoButton)
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOBUTTON_H__B7A0381A_5766_4C59_BFAC_0F977A807E02__INCLUDED_)
