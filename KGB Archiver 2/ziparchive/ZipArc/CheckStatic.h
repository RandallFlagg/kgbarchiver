/*
*
* CCheckStatic
* By Christian Graus 
* http://www.codeproject.com/staticctrl/
*****************************************************/
//
//	2001.10 Modified by Tadeusz Dracz
//

#if !defined(AFX_CHECKSTATIC_H__A6E0AD30_15AF_11D5_AE4A_0080ADB4DF70__INCLUDED_)
#define AFX_CHECKSTATIC_H__A6E0AD30_15AF_11D5_AE4A_0080ADB4DF70__INCLUDED_





#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckStatic.h : header file
//

#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CCheckStatic window

class CCheckStatic : public CStatic
{
// Construction
public:
	CCheckStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckStatic)
	public:
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	int AddItem(int ID);       // Adds a control to the custom list
	void ClearItems();         // Empties the custom list
	void SetCheck(BOOL check); // Sets the check state and toggles dialog items
	BOOL GetCheck() { return m_Check.GetCheck(); };
// Implementation
public:
	virtual ~CCheckStatic();
	  // Though shalt always call this before using my class.
	bool Init(bool bFlat = false, bool bDoNotHandleDisabling = false);
	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckStatic)

	//}}AFX_MSG
	bool create;
	CButton  m_Check;
	CRect    m_rcStatic;
	CArray <int, int> m_ItemID;
	CArray <int, int> m_IDList;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKSTATIC_H__A6E0AD30_15AF_11D5_AE4A_0080ADB4DF70__INCLUDED_)
