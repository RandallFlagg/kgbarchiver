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
#include "ZipArc.h"

#include "ZipArcDoc.h"
#include "ZipArcView.h"
#include "resource.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZipArcView

int CZipArcView::m_iColums[NCOLS] = {-1};

IMPLEMENT_DYNCREATE(CZipArcView, CListView)

BEGIN_MESSAGE_MAP(CZipArcView, CListView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CZipArcView)
	ON_WM_CREATE()
	ON_MESSAGE(WM_REDISPLAY,OnRedisplay)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, OnOdcachehint)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_WM_DROPFILES()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_MESSAGE(WM_SETREPORT,OnSetreport)
	ON_COMMAND(ID_SELECTALL, OnSelectall)
	ON_MESSAGE(WM_SHOWHIDEVIEW,OnShowhideview)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILEINFO, OnUpdateViewFileinfo)
	ON_MESSAGE(WM_MODIFYMENU,OnModifymenu)
	ON_UPDATE_COMMAND_UI(ID_SELECTALL, OnUpdateSelectall)
	ON_UPDATE_COMMAND_UI(ID_FILE_INFORMATION, OnUpdateNotHidden)
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_SEARCHSELECT, OnUpdateNotHidden)
	ON_COMMAND(ID_COMMAND_RENAME, OnCommandRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZipArcView construction/destruction

CZipArcView::CZipArcView()
{
	m_bHidden = m_bHoldUpdate = m_bRefreshNeeded = false;

}

CZipArcView::~CZipArcView()
{
}

BOOL CZipArcView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_OWNERDATA | LVS_SHAREIMAGELISTS |LVS_REPORT |LVS_SHOWSELALWAYS |LVS_EDITLABELS;
	cs.dwExStyle |= WS_EX_ACCEPTFILES;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CZipArcView drawing

void CZipArcView::OnDraw(CDC* pDC)
{
	CZipArcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

void CZipArcView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CImageList* pil = &CZipArcApp::GetFrame()->m_small;
	if (pil->m_hImageList)
		GetListCtrl().SetImageList(pil, LVSIL_SMALL);
// 	pil = &CZipArcApp::GetFrame()->m_large;
// 	if (pil->m_hImageList)
// 		GetListCtrl().SetImageList(pil, LVSIL_NORMAL);
	GetDocument()->Init(this);
}

/////////////////////////////////////////////////////////////////////////////
// CZipArcView diagnostics

#ifdef _DEBUG
void CZipArcView::AssertValid() const
{
	CListView::AssertValid();
}

void CZipArcView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CZipArcDoc* CZipArcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CZipArcDoc)));
	return (CZipArcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZipArcView message handlers


int CZipArcView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetListCtrl().SetExtendedStyle(LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES
			|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
	for (int i = 0; i < NCOLS; i++)
		InsertColumn(i, IDS_COL1 + i, (i == 0 || i == NCOLS - 1) ? LVCFMT_LEFT : LVCFMT_RIGHT );
	if (m_iColums[0] == -1) // initializing needed
	{
	 	GetListCtrl().SetRedraw(FALSE);
		int iPaddingSize = 4*GetSystemMetrics(SM_CXDLGFRAME);
		CDC* cdc = this->GetDC();
		
		CFont *pFont = this->GetFont();
		ASSERT(pFont);
		CFont* pPrevFont = cdc->SelectObject(pFont);

 		TEXTMETRIC tm;
 		cdc->GetTextMetrics(&tm);
		int nItems = GetListCtrl().GetItemCount();
		int iColumn;
		for (iColumn = 0; iColumn < NCOLS; iColumn++)
		{
			HDITEM hdi;
			TCHAR  lpBuffer[512];
			hdi.mask = HDI_TEXT | HDI_FORMAT;
			hdi.pszText = lpBuffer;
			hdi.cchTextMax = 512;
			GetListCtrl().GetHeaderCtrl()->GetItem(iColumn, &hdi);
			
			m_iColums[iColumn] = cdc->GetOutputTextExtent(lpBuffer).cx + 2*tm.tmAveCharWidth; // header width
	// 		if (hdi.fmt & HDF_OWNERDRAW)
	// 		{
	// 			CRect r;
	// 			m_ctlHeaderCtrl.GetItemRect(i, &r);
	// 			aSizes[iItem] += r.Height(); // approx. the width of the triangle
	// 		}

			
		}



		for (iColumn = 0; iColumn < NCOLS - 1; iColumn++) // leave Comment col
		{
			CString sz;
			switch (iColumn)
			{
			case 0:
				{
					for (int i = 0; i < 40; i ++)
						sz += _T("K"); // average
					break;
				}
			case 1:
				sz = _T("999 999 999");
				break;
			case 2:
				sz = _T("99 OOO 9999, 99:99:99");
				break;
			case 3:
				sz = _T("aaaa");
				break;
			case 4:
				sz = _T("100%");
				break;
			}
			int width = cdc->GetOutputTextExtent(sz).cx;
				if (width > m_iColums[iColumn]) 
					m_iColums[iColumn] = width;
			m_iColums[iColumn] += iPaddingSize;	
		}
		cdc->SelectObject(pPrevFont);
		this->ReleaseDC(cdc);

	 	GetListCtrl().SetRedraw();
	 	GetListCtrl().RedrawWindow();	
	}
	for (int iColumn = 0; iColumn < NCOLS; iColumn++)
		GetListCtrl().SetColumnWidth(iColumn, m_iColums[iColumn]);	
	return 0;
}






int CZipArcView::InsertColumn(int nCol, UINT nIDColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	CString col;
	col.LoadString(nIDColumnHeading);
	return GetListCtrl().InsertColumn(nCol, col, nFormat, nWidth, nSubItem);
}


void CZipArcView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CListView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if (bActivate)
	{
		if (m_bRefreshNeeded)
		{
			m_bRefreshNeeded = false; // needed, before Onredisplay is reached another OnActivateView is called when opening or creating
			PostMessage(WM_REDISPLAY);
		}
		CZipArcApp::GetFrame()->ChangeSaveToFinalize(GetDocument()->GetSegmMode() != 0);
	}
}

LONG CZipArcView::OnRedisplay( UINT uParam, LONG lParam)
{
	CWaitCursor wc;
	m_bRefreshNeeded = false;
	ClearCache();
// 	GetListCtrl().SetRedraw(FALSE);
	GetListCtrl().SetItemCount((int)GetDocument()->GetCount());
// 	GetListCtrl().SetRedraw();
	UpdateItems();
	UpdateSelText();
	return 0;
}

void CZipArcView::BuildSelectedArray(CZipIndexesArray &dwArray)
{
	POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
	while (pos)
		dwArray.Add(GetListCtrl().GetNextSelectedItem(pos));


}

void CZipArcView::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}
		CChildFrame* pFrame = (CChildFrame*)GetParentFrame();
		if (!pFrame)
		{
			ASSERT(FALSE);
			return;
		}

		CMenu* pPopup = pFrame->GetParentFrame()->GetMenu()->GetSubMenu(pFrame->GetMenuIndex());
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}


void CZipArcView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CZipArcDoc* pDoc = GetDocument();
	if (GetListCtrl().GetSelectedCount() == 1)
		pDoc->OnViewFileinfo();
	*pResult = 0;
}

void CZipArcView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVDISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LVITEM* pItem= &(pDispInfo->item);
	int iItemIndx= pItem->iItem;
	*pResult = 0;
	if (pItem->mask & LVIF_TEXT || pItem->mask & LVIF_IMAGE)
	{
		if (GetDocument()->IsModifyingInProgress())
			return;

		CZipArcViewCache av;
		if (!m_cache.Lookup(iItemIndx, av))
			if (!FillAvWithFh(av, iItemIndx))
			{
				*pResult = 1;
				return;
			}
		
		if (pItem->mask & LVIF_TEXT) //valid text buffer?
			lstrcpyn(pItem->pszText, av[pItem->iSubItem], pItem->cchTextMax);
		if (pItem->mask & LVIF_IMAGE)
			pItem->iImage = av.m_iImage;
	}
	
}

void CZipArcView::OnOdcachehint(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCACHEHINT * pCacheHint = (NMLVCACHEHINT *)pNMHDR;
	*pResult = 0;
	if (GetDocument()->IsModifyingInProgress())
		return;
	CMap<int, int, CZipArcViewCache, CZipArcViewCache> newcache;
	for (int i = pCacheHint->iFrom; i <= pCacheHint->iTo; i++)
	{
		CZipArcViewCache av;
		bool bAdd = true;
		if (!m_cache.Lookup(i, av))
			if (!FillAvWithFh(av, i))
				bAdd = false;
		if (bAdd)
			newcache[i] = av;

	}
	ClearCache();
	POSITION pos = newcache.GetStartPosition();
	while (pos)
	{
		int i;
		CZipArcViewCache av;
		newcache.GetNextAssoc(pos, i, av);
		m_cache[i] = av;
	}


	
}

bool CZipArcView::FillAvWithFh(CZipArcViewCache& av, ZIP_U16_U64 idx, bool bFast)
{
	CZipFileHeader* pFh = GetDocument()->GetFileInfo(idx);
	if (!pFh)
		return false;
	
	av[0] = (LPCTSTR)pFh->GetFileName();
	av[1] = CZipArcApp::FormatSize(pFh->m_uUncomprSize);

	if (!bFast)
	{
		CTime t(pFh->GetTime());
		av[2] = t.Format(_T("%d %b %Y, %X"));
		DWORD uAttr = pFh->GetSystemAttr();
		TCHAR d = _T('-');
		av[3] = uAttr & FILE_ATTRIBUTE_READONLY ? _T('r') : d;
		av[3] += uAttr & FILE_ATTRIBUTE_ARCHIVE ? _T('a') : d;
		av[3] += uAttr & FILE_ATTRIBUTE_HIDDEN ? _T('h') : d;
		av[3] += uAttr & FILE_ATTRIBUTE_SYSTEM ? _T('s') : d;
		av[4].Format(_T("%.0f%%"), pFh->GetCompressionRatio());
	}
	av[5] = pFh->GetComment();
	if (av[5].GetLength() > 255)
		av[5] = av[5].Left(255);
	SHFILEINFO    shfi;
	UINT flags = SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES;
	if (pFh->IsDirectory())
		SHGetFileInfo(_T("TMP") ,  FILE_ATTRIBUTE_DIRECTORY , &shfi, sizeof(SHFILEINFO), flags);
	else
		SHGetFileInfo(av[0],  FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFO), flags);
	av.m_iImage = shfi.iIcon;
	return true;
}

void CZipArcView::OnViewRefresh() 
{
	if (m_bHidden && !GetDocument()->IsActionInProgress())
		OnShowhideview(1, 0); // just in case (will redisplay)
	else
		OnRedisplay(0, 0);
}

void CZipArcView::DeselectAllItems()
{
	m_bHoldUpdate = true;
	POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
	while (pos)
		SelectItem(GetListCtrl().GetNextSelectedItem(pos), false);
	m_bHoldUpdate = false;

}

void CZipArcView::SelectItem(ZIP_U16_U64 iItem, bool bSelect)
{
	if (iItem > INT_MAX)
		return;
	GetListCtrl().SetItemState((int)iItem, bSelect ? LVIS_SELECTED | LVIS_FOCUSED: ~LVIS_SELECTED, LVIS_SELECTED | (bSelect ? LVIS_FOCUSED : 0));
// 	if (bSelect)
// 		GetListCtrl().EnsureVisible(iItem, FALSE);
}

void CZipArcView::OnDropFiles(HDROP hDropInfo) 
{
	CListView::OnDropFiles(hDropInfo);
	if (GetDocument()->IsActionInProgress())
		return;
	CWaitCursor wait;
	UINT numFiles = DragQueryFile(hDropInfo, UINT(-1), NULL, 0);
	UINT maxPath;
	int poz = 0;
	DeselectAllItems();
	CStringArray* pArray = new CStringArray;
	pArray->Add(_T("")); // for SetRootPath
	for (UINT nFile = 0; nFile < numFiles; nFile++)
	{
		maxPath = DragQueryFile(hDropInfo, nFile, NULL, 0) + 1;
		CString nameFile;
		DragQueryFile(hDropInfo, nFile, nameFile.GetBuffer(maxPath) , maxPath);
		nameFile.ReleaseBuffer();
		pArray->Add(nameFile);
	}
	if (pArray->GetSize() > 1)
	{
		CZipPathComponent zpc(pArray->GetAt(1));
		pArray->SetAt(0, zpc.GetFilePath());
	}

	DragFinish(hDropInfo);
	GetDocument()->OnCommandAdd(pArray);
	
}


void CZipArcView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if (m_bHoldUpdate)
		return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uChanged & LVIF_STATE && (pNMListView->uNewState & LVIS_SELECTED || pNMListView->uOldState & LVIS_SELECTED))
		UpdateSelText();
	
}

void CZipArcView::UpdateSelText()
{
	CString sz;
	sz.Format(ID_INDICATOR_SEL, GetListCtrl().GetSelectedCount(), GetDocument()->GetCount());
	((CChildFrame*)GetParentFrame())->SetSelString(sz);
}

void CZipArcView::UpdateItems()
{
	int iTop = GetListCtrl().GetTopIndex();
	GetListCtrl().RedrawItems(iTop, iTop + GetListCtrl().GetCountPerPage());
	GetListCtrl().Invalidate();
	GetListCtrl().UpdateWindow();
}

void CZipArcView::ClearCache()
{
	m_cache.RemoveAll();
}

LONG CZipArcView::OnSetreport( UINT uParam, LONG lParam)
{
	GetDocument()->SetReport(uParam != 0);
	return 0;
}

void CZipArcView::OnSelectall() 
{
	m_bHoldUpdate = true;
	SetRedraw(FALSE);
	int iCount = GetListCtrl().GetItemCount();
	for (int i = 0; i < iCount; i++)
		SelectItem(i, true);
	SetRedraw();
	m_bHoldUpdate = false;
	UpdateSelText();
}


void CZipArcView::SetView(int iView) 
{ 
	// Get the current window style. 
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE); 
 
	// Only set the window style if the view bits have changed. 
	if ((dwStyle & LVS_TYPEMASK) != (DWORD)iView) 
		SetWindowLong(m_hWnd, GWL_STYLE, 
		   (dwStyle & ~LVS_TYPEMASK) | (DWORD)iView); 
} 




LONG CZipArcView::OnShowhideview( UINT uParam, LONG lParam)
{
	if ((uParam != 0) != m_bHidden)
		return 0;
	if (uParam) // show
	{
		m_bHidden = false;
		SetView(vsReport);
		OnRedisplay(0, 0);
		

	}
	else
	{
		// hide - there is a virtual list which will display incorrectly if modifying in progress
		m_bHidden = true;
		GetListCtrl().SetItemCount(0);
		SetView(vsIcon);// hide header
	}
	
	return 0;
}

BOOL CZipArcView::OnEraseBkgnd(CDC* pDC) 
{
		
	if (m_bHidden)
	{
		CBrush backBrush(GetSysColor(COLOR_BTNFACE));
		CBrush* pOldBrush = pDC->SelectObject(&backBrush);
		CRect rect;
		pDC->GetClipBox(&rect);     // Erase the area needed

		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
		pDC->SelectObject(pOldBrush);
		return TRUE;	
	}
	else
		return CListView::OnEraseBkgnd(pDC);

}


void CZipArcView::OnPaint() 
{
	

	if (m_bHidden)
	{
		CPaintDC dc(this); // device context for painting
        int nSavedDC = dc.SaveDC();
        
        CRect rc;
        GetWindowRect( &rc );
        ScreenToClient( &rc );

        rc.top += 30;
       
        CString strText;
        strText.LoadString( IDS_MODIFYING );
        
        dc.SetTextColor( ::GetSysColor( COLOR_WINDOWTEXT ) );
        dc.SetBkColor( ::GetSysColor( COLOR_BTNFACE ) );
        dc.SelectStockObject( ANSI_VAR_FONT );
        dc.DrawText( strText, -1, 
                     rc, 
                     DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP );
        
        dc.RestoreDC(nSavedDC);
	}
	else
		Default();

}

void CZipArcView::OnUpdateViewFileinfo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bHidden && GetListCtrl().GetSelectedCount() );	
}

LONG CZipArcView::OnModifymenu( UINT uParam, LONG lParam)
{
	GetDocument()->ModifyMenu(uParam != 0);
	return 0;
}

void CZipArcView::OnUpdateSelectall(CCmdUI* pCmdUI) 
{
	ZIP_U16_U64 iCount = GetDocument()->GetCount();
	pCmdUI->Enable(!m_bHidden && iCount != GetListCtrl().GetSelectedCount() && iCount);
	
}

void CZipArcView::OnUpdateNotHidden(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bHidden);	
}


void CZipArcView::SelectArray(CZipIndexesArray &ar)
{
	DeselectAllItems();
	m_bHoldUpdate = true;
	SetRedraw(FALSE);
	ZIP_ARRAY_SIZE_TYPE iCount = ar.GetSize();
	ZIP_U16_U64 nMin = ZIP_U16_U64(-1);
	for (ZIP_ARRAY_SIZE_TYPE i = 0; i < iCount; i++)
	{
		ZIP_U16_U64 nIdx = ar[i];
		if (nMin > nIdx)
			nMin = nIdx;
		SelectItem(nIdx, true);
	}
	GetListCtrl().EnsureVisible((int)nMin, FALSE);
	SetRedraw();
	m_bHoldUpdate = false;
	UpdateSelText();	
}

void CZipArcView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	if (GetDocument()->IsReadOnly() || GetDocument()->IsActionInProgress())
		*pResult = 1;
	else
		*pResult = 0;
}

void CZipArcView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	if (pDispInfo->item.pszText)
		GetDocument()->Rename(pDispInfo->item.iItem, pDispInfo->item.pszText);
	*pResult = 0;
}

void CZipArcView::OnCommandRename() 
{
	int i = GetListCtrl().GetSelectionMark();
	if (i!=-1)
		GetListCtrl().EditLabel(i);
}

// there is editing in place
bool CZipArcView::CanProcessDEL()
{
	CWnd * pWnd = CWnd::GetFocus();
	if (pWnd != NULL)
	{
		ASSERT_VALID(pWnd);
		TCHAR lpClassName[32];
		::GetClassName(pWnd->GetSafeHwnd(), lpClassName, 32);
		
		if (!_tcscmp(lpClassName, _T("Edit")))
		{
			CWnd* pParent = pWnd->GetParent();
			if (pParent)
			{
				::GetClassName(pParent->GetSafeHwnd(), lpClassName, 32);
				if (!_tcscmp(lpClassName, _T("SysListView32")) && pParent->GetParent() == GetParent())
				{
					((CEdit*)pWnd)->SendMessage(WM_KEYDOWN, VK_DELETE, 0);		
					return false;
				}
			}
		}
			
	}
	return true;
	
}
