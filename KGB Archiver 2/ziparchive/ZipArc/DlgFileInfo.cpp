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
#include "DlgFileInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFileInfo dialog


CDlgFileInfo::CDlgFileInfo(CWnd* pParent /*=NULL*/)
: CResizableDialog(CDlgFileInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFileInfo)
	m_szFileName = _T("");
	//}}AFX_DATA_INIT
	m_pHeader = NULL;
	m_pArchive = NULL;
	m_bReadOnly = true;
}


void CDlgFileInfo::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFileInfo)
	DDX_Control(pDX, IDC_CHGDSP, m_change);
	DDX_Control(pDX, IDC_DISPLAYING, m_displaying);
	DDX_Control(pDX, IDC_INF, m_edit);
	DDX_Text(pDX, IDC_FILENAME, m_szFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFileInfo, CResizableDialog)
//{{AFX_MSG_MAP(CDlgFileInfo)
ON_BN_CLICKED(IDC_CHGDSP, OnChgdsp)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFileInfo message handlers

void CDlgFileInfo::OnChgdsp() 
{
	UINT nIDButton, nIDLabel;
	if (m_bInfo)
	{
		if (!m_pArchive && !m_pHeader)
			m_change.EnableWindow(FALSE);
		
		nIDButton = IDS_FIDISPINFO;
		nIDLabel = IDS_INFCOMM;
		m_edit.SetWindowText(m_szComment);
		if (!m_bReadOnly)
		{
			m_edit.SetReadOnly(FALSE);
			m_edit.SetFocus();
		}
	}
	else
	{
		nIDButton = IDS_FIDISPCOMM;
		nIDLabel = IDS_INFINF;
		if (!m_bReadOnly) // disabled anyway
		{
			m_edit.GetWindowText(m_szComment);
			m_edit.SetReadOnly();
		}
		m_edit.SetWindowText(m_szInfo);
	}
	m_displaying.SetWindowText(CString((LPCTSTR)nIDLabel));
	m_change.SetWindowText(CString((LPCTSTR)nIDButton));
	m_bInfo = !m_bInfo;
}


BOOL CDlgFileInfo::OnInitDialog() 
{
	
	if (m_pArchive)
	{
		m_szFileName = m_pArchive->GetArchivePath();
		SetWindowText(CString((LPCTSTR)IDS_INFOARCH));
	}
	else
	{
		if (m_pHeader)
			m_szFileName = m_pHeader->GetFileName();
		else
			m_szFileName.LoadString(IDS_INFOMFCOM);
		SetWindowText(CString((LPCTSTR)IDS_INFOFILE));
	}
	
	
	CResizableDialog::OnInitDialog();
	
	if (m_pArchive || m_pHeader)
	{
		if (m_pHeader)
		{
			CString szEncr, szDir, szMeth, szAttr, szSystem, szEff;
			szEncr.LoadString(m_pHeader->IsEncrypted() ? IDS_YES : IDS_NO);
			szDir.LoadString(m_pHeader->IsDirectory() ? IDS_DIR : IDS_FILE);
			szMeth.LoadString(IDS_METH0 + m_pHeader->m_uMethod);
			// 	if (m_pHeader->m_uMethod != 0)
			// 	{
			CString szTemp;
			
			if (!m_pHeader->CompressionEfficient())
				szEff.LoadString(IDS_COMPRINEFF);
			else
			{
				szEff.LoadString(IDS_COMPREFF);
				if (m_pHeader->IsEncrypted() && 
					m_pHeader->GetDataSize() == m_pHeader->m_uUncomprSize)
					szTemp.Format(IDS_COMPREXTR, m_pHeader->GetEncryptedInfoSize());
			}
			szEff.Format(IDS_COMPRINFO, szEff + szTemp);
			// 	}
			CTime t(m_pHeader->GetTime());
			DWORD uAttr = m_pHeader->GetSystemAttr();
			TCHAR d = _T('-');
			szAttr = uAttr & FILE_ATTRIBUTE_READONLY ? _T('r') : d;
			szAttr += uAttr & FILE_ATTRIBUTE_ARCHIVE ? _T('a') : d;
			szAttr += uAttr & FILE_ATTRIBUTE_HIDDEN ? _T('h') : d;
			szAttr += uAttr & FILE_ATTRIBUTE_SYSTEM ? _T('s') : d;
			switch (m_pHeader->GetSystemCompatibility())
			{
			case 0:
				szSystem = _T("MS-DOS and OS/2 (FAT / VFAT / FAT32 file systems)");
				break;
			case 1:
				szSystem = _T("Amiga");
				break;
			case 2:
				szSystem = _T("OpenVMS");
				break;
			case 3:
				szSystem = _T("Unix");
				break;
			case 4:
				szSystem = _T("VM/CMS");
				break;
			case	 5:
				szSystem = _T("Atari ST");
				break;
			case 6:
				szSystem = _T("OS/2 H.P.F.S");
				break;
			case 7:
				szSystem = _T("Macintosh");
				break;
			case 8:
				szSystem = _T("Z-System");
				break;
			case 9:
				szSystem = _T("CP/M");
				break;
			case 10:
				szSystem = _T("Windows NTFS");
				break;
			default:
				szSystem.LoadString(IDS_UNKNOWN);
			}
			CString szC = CZipArcApp::FormatSize(m_pHeader->m_uComprSize), szU = CZipArcApp::FormatSize(m_pHeader->m_uUncomprSize);
			
			m_szInfo.Format(IDS_FILEINFO, szDir, szEncr, m_pHeader->m_uDiskStart + 1, CZipArcApp::FormatSize(m_pHeader->m_uOffset),szC,
				szU, m_pHeader->GetCompressionRatio(), 
				szMeth, szEff, m_pHeader->m_uCrc32, m_pHeader->m_uCrc32, t.Format(_T("%d %b %Y, %X")), 
				szAttr, m_pHeader->GetOriginalAttributes(), m_pHeader->GetOriginalAttributes() >> 16, szSystem,
				((float)(m_pHeader->m_uVersionMadeBy & 0x00FF))/10, ((float)(m_pHeader->m_uVersionNeeded & 0x00FF))/10);
			
		}
		else 
		{
			CString szType, szSFX;
			CZipCentralDir::Info info;
			m_pArchive->GetCentralDirInfo(info);
			if (!m_pArchive->GetSegmMode())
				szType.LoadString(IDS_ARINORMAL);
			else
			{
				int iMode = m_pArchive->GetSegmMode();
				szType.Format(IDS_ARIDS, abs(iMode) == 2 ? _T("split") : _T("spanned"), CString((LPCTSTR)(iMode > 0 ? IDS_ARICR : IDS_ARIEX)));
			}
			
			CWaitCursor cw;		
			ZIP_U16_U64 iCount = m_pArchive->GetCount();
			
			// we need to calculate this
			ZIP_U32_U64 uTotalDirSize = m_pArchive->GetCentralDirSize(false);
			ZIP_U32_U64 uTotalUncompressed = 0, uTotalCompressed = 0;
			if (iCount)
			{
				for (ZIP_U16_U64 i = 0; i < iCount; i++)		
				{
					CZipFileHeader* pFh = m_pArchive->GetFileInfo(i);
					uTotalDirSize += pFh->GetSize();
					uTotalUncompressed += pFh->m_uUncomprSize;
					uTotalCompressed += pFh->m_uComprSize;
					
				}
				
			}
#if _MSC_VER < 1300
			double ratio = (double)(__int64)(uTotalUncompressed ? ((uTotalCompressed)*100 / uTotalUncompressed): 0);
#else
			double ratio = uTotalUncompressed ? (((double)uTotalCompressed)*100 / uTotalUncompressed): 0;
#endif
			m_szInfo.Format(IDS_ARCHINFO, szType, CString((LPCTSTR)(m_pArchive->IsReadOnly() ? IDS_YES : IDS_NO)),
				CZipArcApp::FormatSize(m_pArchive->GetCount()), m_pArchive->GetSegmMode() >= 0 ? m_pArchive->GetCurrentDisk() : info.m_uLastDisk + 1, CZipArcApp::FormatSize(uTotalUncompressed), ratio, 
				CZipArcApp::FormatSize(uTotalDirSize), CString((LPCTSTR)(info.m_bOnDisk ? IDS_YES : IDS_NO)));
		}
		m_szInfo.Replace(_T("\n"), _T("\r\n"));
		m_bInfo = false;
	}
	
	
	AddAnchor(IDC_INF, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_CHGDSP, BOTTOM_LEFT, BOTTOM_LEFT);
	AddAnchor(IDOK,BOTTOM_RIGHT, BOTTOM_RIGHT);
	AddAnchor(IDC_FILENAME,TOP_LEFT, TOP_RIGHT);
	EnableSaveRestore(_T("File Info Dialog"));
	
	
	OnChgdsp();	
	m_edit.SetLimitText(65535); // max global comment length
	if (m_pHeader)
		m_szComment = m_pHeader->GetComment();
	else if (m_pArchive)
		m_szComment = m_pArchive->GetGlobalComment();
	
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgFileInfo::OnOK() 
{
	if (!m_bReadOnly && !m_bInfo)
		m_edit.GetWindowText(m_szComment);
	
	CResizableDialog::OnOK();
}
