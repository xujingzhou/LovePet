// ScrGenius.h : main header file for the ScrGenius application
//

#if !defined(AFX_ScrGenius_H__D363F3E5_49F3_11D3_8660_8A601C3D7832__INCLUDED_)
#define AFX_ScrGenius_H__D363F3E5_49F3_11D3_8660_8A601C3D7832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
/////////////////////////////////////////////////////////////////////////////
// CScrGeniusApp:
// See ScrGenius.cpp for the implementation of this class
//
//软件名称：爱圣
//作者：徐景周
//功能：控件注册器、显示/恢复桌面、文件加解密、定时关机、
//      简繁字体互转、窃取星号密码、BMP转文本、文件隐藏到位图、
//      文件分割器、光驱管理、系统管理等(扩充中...)
//
class CScrGeniusApp : public CWinApp
{
public:
	CScrGeniusApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrGeniusApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Generated message map functions
protected:	
// Implementation

	//{{AFX_MSG(CScrGeniusApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ScrGenius_H__D363F3E5_49F3_11D3_8660_8A601C3D7832__INCLUDED_)
