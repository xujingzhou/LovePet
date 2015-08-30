// ScrGenius.cpp : Defines the class behaviors for the application.
//
//软件名称：爱圣
//作者：徐景周
//功能：控件注册器、显示/恢复桌面、文件加解密、定时关机、
//      简繁字体互转、窃取星号密码、BMP转文本、文件隐藏到位图、
//      文件分割器、光驱管理、系统管理等(扩充中...)
//
#include "stdafx.h"
#include "ScrGenius.h"
#include "ScrGeniusDlg.h"
#include "TransparentWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrGeniusApp

BEGIN_MESSAGE_MAP(CScrGeniusApp, CWinApp)
	//{{AFX_MSG_MAP(CScrGeniusApp)	
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrGeniusApp construction

CScrGeniusApp::CScrGeniusApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CScrGeniusApp object

CScrGeniusApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CScrGeniusApp initialization

BOOL CScrGeniusApp::InitInstance()
{
	srand(time(NULL));
    
	//用互斥量来判断程序是否正在运行，是则退出
    HANDLE m_hMutex=CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS) { return FALSE; }
    
	//设置对话框背景和文本颜色
	SetDialogBkColor(RGB(240,200,90),RGB(0,0,0));

	CTransparentWnd* pFrame = new CTransparentWnd;
	m_pMainWnd = pFrame;
    
	// create and load the frame with its resources
	CRect rect(0, 0, 100, 100);
	pFrame->CreateTransparent("送给你...", rect);//IDB_MASK, IDB_BACK);
	pFrame->CenterWindow();  //初始显示窗体位置
	pFrame->ShowWindow(SW_SHOW);
	
	return TRUE;
}
