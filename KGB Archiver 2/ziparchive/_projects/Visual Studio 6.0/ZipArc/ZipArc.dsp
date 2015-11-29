# Microsoft Developer Studio Project File - Name="ZipArc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ZipArc - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ZipArc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZipArc.mak" CFG="ZipArc - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZipArc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ZipArc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ZipArc - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "ZipArc - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ZipArc - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "ZIP_ARCHIVE_MFC" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ZipArc - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "ZIP_ARCHIVE_MFC" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x415 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "ZipArc - Win32 Unicode Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ZipArc___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "ZipArc___Win32_Unicode_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_UNICODE" /D "ZIP_ARCHIVE_MFC" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ZipArc - Win32 Unicode Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ZipArc___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "ZipArc___Win32_Unicode_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_UNICODE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "ZIP_ARCHIVE_MFC" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ZipArc - Win32 Release"
# Name "ZipArc - Win32 Debug"
# Name "ZipArc - Win32 Unicode Release"
# Name "ZipArc - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BrowseForFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CreditsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGetFrom.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNew.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptAppear.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptCompr.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptGen.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptSmart.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPassword.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReplace.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgReport.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSave.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DropFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoButton.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MMStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\RegistrySerialize.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableGrip.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableMinMax.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableState.cpp
# End Source File
# Begin Source File

SOURCE=.\SAPrefsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SAPrefsStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\SAPrefsSubDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Signal.cpp
# End Source File
# Begin Source File

SOURCE=.\SortableObArray.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ZipArc.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipArc.odl
# End Source File
# Begin Source File

SOURCE=.\ZipArc.rc
# End Source File
# Begin Source File

SOURCE=.\ZipArcDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ZipArcView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BrowseForFolder.h
# End Source File
# Begin Source File

SOURCE=.\CheckStatic.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CreditsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgContainer.h
# End Source File
# Begin Source File

SOURCE=.\DlgExtract.h
# End Source File
# Begin Source File

SOURCE=.\DlgFileInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgGetFrom.h
# End Source File
# Begin Source File

SOURCE=.\DlgNew.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptAbout.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptAppear.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptCompr.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptGen.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptSmart.h
# End Source File
# Begin Source File

SOURCE=.\DlgPassword.h
# End Source File
# Begin Source File

SOURCE=.\DlgReplace.h
# End Source File
# Begin Source File

SOURCE=.\DlgReport.h
# End Source File
# Begin Source File

SOURCE=.\DlgSave.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelect.h
# End Source File
# Begin Source File

SOURCE=.\DropFiles.h
# End Source File
# Begin Source File

SOURCE=.\InfoButton.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MMStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\Options.h
# End Source File
# Begin Source File

SOURCE=.\ProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\RegistrySerialize.h
# End Source File
# Begin Source File

SOURCE=.\ResizableDialog.h
# End Source File
# Begin Source File

SOURCE=.\ResizableGrip.h
# End Source File
# Begin Source File

SOURCE=.\ResizableLayout.h
# End Source File
# Begin Source File

SOURCE=.\ResizableMinMax.h
# End Source File
# Begin Source File

SOURCE=.\ResizableState.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SAPrefsDialog.h
# End Source File
# Begin Source File

SOURCE=.\SAPrefsStatic.h
# End Source File
# Begin Source File

SOURCE=.\SAPrefsSubDlg.h
# End Source File
# Begin Source File

SOURCE=.\Signal.h
# End Source File
# Begin Source File

SOURCE=.\SortableObArray.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\ZipArc.h
# End Source File
# Begin Source File

SOURCE=.\ZipArcDoc.h
# End Source File
# Begin Source File

SOURCE=.\ZipArcView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\except.ico
# End Source File
# Begin Source File

SOURCE=.\res\modified.ico
# End Source File
# Begin Source File

SOURCE=.\res\move.cur
# End Source File
# Begin Source File

SOURCE=.\res\multidis.ico
# End Source File
# Begin Source File

SOURCE=.\res\readonly.ico
# End Source File
# Begin Source File

SOURCE=.\res\report.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ZipArc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ZipArc.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ZipArcDoc.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ZipArc.reg
# End Source File
# End Target
# End Project
# Section ZipArc : {05AAD2C6-9D37-4FE4-8032-FDF8F0B1D750}
# 	2:11:HyperLink.h:HyperLink.h
# 	2:17:CLASS: CHyperLink:CHyperLink
# 	2:25:CLASS: tagHYPERLINKCOLORS:tagHYPERLINKCOLORS
# 	2:24:TYPEDEF: HYPERLINKCOLORS:HYPERLINKCOLORS
# 	2:13:HyperLink.cpp:HyperLink.cpp
# 	2:19:Application Include:ziparc.h
# End Section
# Section ZipArc : {9D1D7D39-F15C-46A5-9A6E-94794786048A}
# 	2:21:RegistrySerialize.cpp:RegistrySerialize.cpp
# 	2:19:RegistrySerialize.h:RegistrySerialize.h
# 	2:19:Application Include:ZipArc.h
# 	2:25:CLASS: CRegistrySerialize:CRegistrySerialize
# End Section
# Section ZipArc : {72ADFD6C-2C39-11D0-9903-00A0C91BC942}
# 	1:25:CG_IDR_POPUP_ZIP_ARC_VIEW:235
# End Section
# Section ZipArc : {81652207-79E0-472A-ACF9-63A94253E9F8}
# 	2:17:SortableObArray.h:SortableObArray.h
# 	2:25:TYPEDEF: SOA_SORTFUNCTION:SOA_SORTFUNCTION
# 	2:40:CLASS: CTypedSortableObArray<class TYPE>:CTypedSortableObArray<class TYPE>
# 	2:23:CLASS: CSortableObArray:CSortableObArray
# 	2:19:Application Include:ziparc.h
# 	2:19:SortableObArray.cpp:SortableObArray.cpp
# End Section
# Section ZipArc : {899C2737-77D1-4BB6-8A43-3B8C27F9775F}
# 	2:23:CLASS: CResizableDialog:CResizableDialog
# 	2:17:ResizableDialog.h:ResizableDialog.h
# 	2:19:Application Include:ziparc.h
# 	2:19:ResizableDialog.cpp:ResizableDialog.cpp
# End Section
# Section ZipArc : {D7FD97CE-75A0-44E7-9EA9-8C0173C0F2A6}
# 	2:23:CLASS: CBrowseForFolder:CBrowseForFolder
# 	2:19:BrowseForFolder.cpp:BrowseForFolder.cpp
# 	2:19:Application Include:ziparc.h
# 	2:17:BrowseForFolder.h:BrowseForFolder.h
# End Section
# Section ZipArc : {5132F1F2-7484-403A-9B91-D7DEDD45B5CC}
# 	2:13:ProgressBar.h:ProgressBar.h
# 	2:19:CLASS: CProgressBar:CProgressBar
# 	2:19:Application Include:ZipArc.h
# 	2:15:ProgressBar.cpp:ProgressBar.cpp
# End Section
# Section ZipArc : {13749CD0-D8F8-4296-AB61-E15F90D4A3F4}
# 	2:11:Options.cpp:Options.cpp
# 	2:15:CLASS: COptions:COptions
# 	2:9:Options.h:Options.h
# 	2:19:Application Include:ZipArc.h
# End Section
