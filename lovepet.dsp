# Microsoft Developer Studio Project File - Name="lovepet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=lovepet - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lovepet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lovepet.mak" CFG="lovepet - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lovepet - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "lovepet - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lovepet - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "lovepet - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "lovepet - Win32 Release"
# Name "lovepet - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\About.cpp
# End Source File
# Begin Source File

SOURCE=.\AnimEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Big2GB.cpp
# End Source File
# Begin Source File

SOURCE=.\BMP2TXT.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\CBmpMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomWavFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DirDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EDStar.cpp
# End Source File
# Begin Source File

SOURCE=.\EllepticalButton.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSplit.cpp
# End Source File
# Begin Source File

SOURCE=.\Flatcombobox.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\GetIP.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\HideInBMP.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\lovePet.rc
# End Source File
# Begin Source File

SOURCE=.\Ntray.cpp
# End Source File
# Begin Source File

SOURCE=.\RegOCX.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrGenius.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrGeniusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetTimeShutdown.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticFilespec.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StringEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TransparentWnd.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\About.h
# End Source File
# Begin Source File

SOURCE=.\AnimEffect.h
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\Big2GB.h
# End Source File
# Begin Source File

SOURCE=.\BMP2TXT.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\CBmpMenu.h
# End Source File
# Begin Source File

SOURCE=.\CustomWavFile.h
# End Source File
# Begin Source File

SOURCE=.\DirDialog.h
# End Source File
# Begin Source File

SOURCE=.\EDStar.h
# End Source File
# Begin Source File

SOURCE=.\EllepticalButton.h
# End Source File
# Begin Source File

SOURCE=.\FileSplit.h
# End Source File
# Begin Source File

SOURCE=.\Flatcombobox.h
# End Source File
# Begin Source File

SOURCE=.\FlatEdit.h
# End Source File
# Begin Source File

SOURCE=.\GetIP.h
# End Source File
# Begin Source File

SOURCE=.\GradientProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\HideInBMP.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\Ntray.h
# End Source File
# Begin Source File

SOURCE=.\RegOCX.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScrGenius.h
# End Source File
# Begin Source File

SOURCE=.\ScrGeniusDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetTimeShutdown.h
# End Source File
# Begin Source File

SOURCE=.\StaticFilespec.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringEx.h
# End Source File
# Begin Source File

SOURCE=.\TransparentWnd.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Big2GB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp2txt.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cancel.ico
# End Source File
# Begin Source File

SOURCE=.\res\check.bmp
# End Source File
# Begin Source File

SOURCE=.\res\check1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Checked.bmp
# End Source File
# Begin Source File

SOURCE=.\res\checked1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\custom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drop.cur
# End Source File
# Begin Source File

SOURCE=.\res\drop1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\drop2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\edstar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eject.bmp
# End Source File
# Begin Source File

SOURCE=.\res\email.bmp
# End Source File
# Begin Source File

SOURCE=.\res\exit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fileopen.ico
# End Source File
# Begin Source File

SOURCE=.\res\filespli.bmp
# End Source File
# Begin Source File

SOURCE=.\res\getip.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gril.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\help.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hide.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HideInBM.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\lovepet.ico
# End Source File
# Begin Source File

SOURCE=.\res\lovepet.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ok.ico
# End Source File
# Begin Source File

SOURCE=.\res\pet.cur
# End Source File
# Begin Source File

SOURCE=.\res\register.bmp
# End Source File
# Begin Source File

SOURCE=.\res\regocx.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rose.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rose.ico
# End Source File
# Begin Source File

SOURCE=.\res\show.bmp
# End Source File
# Begin Source File

SOURCE=.\res\shutdown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk12.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk13.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk14.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk15.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk17.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk18.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk19.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk20.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk21.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk22.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk23.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\walk9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\west11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\west12.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\love1.wav
# End Source File
# Begin Source File

SOURCE=.\res\love2.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section lovepet : {130D7743-5F5A-11D1-B676-00A0C9697233}
# 	2:21:DefaultSinkHeaderFile:hhopen.h
# 	2:16:DefaultSinkClass:CHhopen
# End Section
# Section lovepet : {130D7741-5F5A-11D1-B676-00A0C9697233}
# 	2:5:Class:CHhopen
# 	2:10:HeaderFile:hhopen.h
# 	2:8:ImplFile:hhopen.cpp
# End Section
