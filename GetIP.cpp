// GetIP.cpp : implementation file
//
//Modify by 徐景周 2000.10
//功能：利用系统鼠标钩子得到星号框内容

#include "stdafx.h"
#include "scrgenius.h"
#include "GetIP.h"
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// GetIP dialog


GetIP::GetIP(CWnd* pParent /*=NULL*/)
	: CDialog(GetIP::IDD, pParent)
{
	//{{AFX_DATA_INIT(GetIP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Capture=false;
}


void GetIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetIP)
	DDX_Control(pDX, IDC_GETIP, m_editip);
	DDX_Control(pDX, IDOK,  m_Button);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GetIP, CDialog)
	//{{AFX_MSG_MAP(GetIP)
    ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GetIP message handlers

HHOOK hHook=NULL;
HWND hWndDisplay=NULL;
char* HostString;
bool bHost = false;
char szText[32000];
char* szIPNum ;
bool GetPointText(POINT pt);
extern "C" LRESULT WINAPI HookProc(int nCode,WPARAM wParam,LPARAM lParam); 
//bool FormatCharToIP(char* szText);

BOOL GetIP::OnInitDialog()
{
	CDialog::OnInitDialog();
	//拖动宠物图标时设置
    m_Static.SubclassDlgItem(IDC_LOVEPET,this);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR GetIP::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void GetIP::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rect;
	m_Static.GetWindowRect(&rect);
    POINT pt = point;
	ClientToScreen( &pt );
	if (rect.PtInRect(pt))
	{
	   ::SetCursor(AfxGetApp()->LoadCursor(IDC_PET));
       SetCapture();
	   m_Capture = true;  
	   hWndDisplay = m_editip.GetSafeHwnd();
	   HookInstaller();
	   }

	CDialog::OnLButtonDown(nFlags, point);
}

void GetIP::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if(m_Capture)
	{
		ReleaseCapture();
        m_Capture = false;
		HookUninstaller();
	}
	CDialog::OnLButtonUp(nFlags, point);
}

HHOOK GetIP::HookInstaller()
{

	hHook=SetWindowsHookEx(WH_MOUSE,HookProc,NULL,0);
	return hHook;

}

BOOL GetIP::HookUninstaller()
{
	int bUnHook;
	if(hHook)
	{
		bUnHook = UnhookWindowsHookEx(hHook);
		if(bUnHook)
		{
			hWndDisplay=NULL;
			hHook=NULL;
		}
	}
	return bUnHook;

}

extern "C" LRESULT WINAPI HookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	//定义为鼠标HOOK
	LPMOUSEHOOKSTRUCT pMouseEvent = (MOUSEHOOKSTRUCT FAR *)lParam;
	if(nCode>=0)
	{
					
		   if(GetPointText(pMouseEvent->pt))
			{
			     //显示星号内容
				SendMessage(hWndDisplay,WM_SETTEXT,0,(LPARAM)(LPCTSTR)szText);
			
				//如果是IP地址，则解析它并显示出来
           //     if(FormatCharToIP(szText))
			//		SendMessage(hWndDisplay,WM_SETTEXT,0,(LPARAM)(LPCTSTR)szIPNum);
			}
			else
			{
				 SendMessage(hWndDisplay,WM_SETTEXT,0,(LPARAM)_T(""));
			}

	}
	
	return CallNextHookEx(hHook,nCode,wParam,lParam);
}



bool GetPointText(POINT pt)
{  
	HWND  dWnd; 
	
	dWnd= WindowFromPoint(pt);
    if(dWnd!=NULL)
	{
      if(SendMessage(dWnd,WM_GETTEXT,32000,(LPARAM)(LPCTSTR)szText)>0) 
		  return true; 
	     
	}
	return false;
}

/*
  //解析网址为IP地址
bool FormatCharToIP(char* szText)
{
    PHOSTENT hostinfo;
	RASCONN rasconn; 
	DWORD dNumber,dSize ;
	int first,end;
	bHost = false;
	CString szString=CString(szText);
	if(szString.Find(_T("."))==-1)
		return false;
	if ((first = szString.Find(_T("://")))>1)
		first=first+3;
	else
		first=0;
	szString=szString.Mid(first);
    if ((end=szString.Find(_T("/"))) >1)
		szString=szString.Left(end);
	if (szString.Find(_T(":")) >= 1)
		return false;
	rasconn.dwSize = sizeof(RASCONN); 
	dSize = sizeof(rasconn);
	if (RasEnumConnections(&rasconn,&dSize,&dNumber)!=0) 
		return false;
	if (dNumber==0)
		return false;
	
	
     if((hostinfo = gethostbyname(szString)) != NULL)        
	 	 {
			 szIPNum = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			 HostString = szString.GetBuffer(30);
			 szString.ReleaseBuffer();
			 bHost = true;
			 return true;
		 }
      return false; 	 
}
*/

BOOL GetIP::DestroyWindow() 
{
  //解决有时意外的鼠标左键未松，而窗口已关闭时造成的鼠标HOOK未释放的错误
	 if(m_Capture)
	{
		ReleaseCapture();
        m_Capture = false;
		HookUninstaller();
	}
	 CRect rc5;  //对话框动画显示效果
     AnimEffect anim5;

    //在此加入对话框动画关闭效果
	GetWindowRect(rc5 );
	rc5.DeflateRect(2,2);//动态方框显示
    anim5.DrawWireRects(rc5, CRect(rc5.CenterPoint().x-10,rc5.CenterPoint().y-10,rc5.CenterPoint().x+10,rc5.CenterPoint().y+10 ), 10);
//	anim5.Close(rc5); ////对话框动画效果显示关闭 
// 	Sleep(50); //延缓50毫秒后关闭

	return CDialog::DestroyWindow();
}

void GetIP::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rc5;  //对话框动画显示效果
   AnimEffect anim5;

	//在此加入对话框动画打开效果
	GetWindowRect(rc5 );
    anim5.Open( rc5 ); //对话框动画效果显示打开

}
