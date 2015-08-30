*********************************************************************
/*
Modified by 徐景周 2000.9
功能：动态透明位图显示及WAV资源文件播放
*/

#include "stdafx.h"
#include "TransparentWnd.h"
#include <assert.h>
#include "ScrGenius.h"
#include "ScrGeniusDlg.h"
#include "BCMenu.h"  //添加位图菜单
#include <afxpriv.h> //添加对话框控件的更新响应
#include "GetIP.h"
#include "BMP2TXT.H"
#include "About.h"
#include "CustomWavFile.h"
#include "Big2GB.h"
#include "HideInBMP.h"
#include "FileSplit.h"
#include "RegOCX.h"
#include "EDStar.h"
#include "SetTimeShutdown.h"
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BCMenu popmenu;  //定义位图菜单变量

#define WM_TRAYNOTIFY WM_USER + 100  //自定义系统托盘消息函数

CWnd * pFrame  ;//得到当前窗体指针
CCustomWavFile *pCustomWavFile; //播放定制情话类指针

bool bWavPlay ; //声音播放标志位
bool bDragDrop; //拖动标志位
CPoint CurPoint; //记录当前鼠标位置
bool bShow=false; //窗体显示还是隐藏标志位，用于判断是否进行窗体动画
bool bMute=false; //情话静音标志位
bool bEject=true; //弹出还是关闭光驱标志位
CString strWavFileName=""; //从lovepet.dat数据文件中得到自定制情话文件名变量
CString strTextDescription="";//和文字描述变量.
//CString strTextPlay="送有情人"; //宠物上面显示的文字信息
//********************************************************************************
//* Constructor
//********************************************************************************

CTransparentWnd::CTransparentWnd()
{
	bMute=false;
	m_iAniSeq=0;
//	m_fontLogo.CreateFont(13, 0, 90, 0, FW_NORMAL, 0, 0,0,0,0,0,0,0, "宋体");
}


//********************************************************************************
//* Destructor
//********************************************************************************

CTransparentWnd::~CTransparentWnd()
{
}


BEGIN_MESSAGE_MAP(CTransparentWnd, CWnd)
	//{{AFX_MSG_MAP(CTransparentWnd)
	ON_MESSAGE(WM_TRAYNOTIFY, OnTrayNotification)  //自定义系统托盘消息涵数 
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOVING()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDR_SHOW, OnShow)
	ON_COMMAND(IDR_REGISTER, OnRegister)
	ON_COMMAND(IDR_SHUTDOWN, OnShutdown)
	ON_COMMAND(IDR_EJECT, OnEject)
	ON_COMMAND(IDR_EXIT, OnExit)
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_COMMAND(IDR_MUTE, OnMute)
	ON_UPDATE_COMMAND_UI(IDR_MUTE, OnUpdateMute)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_COMMAND(IDR_GETIP, OnGetip)
	ON_COMMAND(IDR_BMP2TXT, OnBmp2txt)
	ON_COMMAND(IDR_HELP, OnHelp)
	ON_COMMAND(IDR_CUSTOMWAVFILE, OnCustomwavfile)
	ON_COMMAND(IDR_BIG2GB, OnBig2gb)
	ON_COMMAND(IDR_HIDEINBMP, OnHideinbmp)
	ON_WM_MOUSEMOVE()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(IDR_EDSTAR, OnEdstar)
	ON_COMMAND(IDR_REGOCX, OnRegocx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CTransparentWnd::OnKickIdle(WPARAM wparam, LPARAM lparam)
	{
		UpdateDialogControls(this, false);
		return 0;
	}
//********************************************************************************
//* CreateTransparent()
//*
//* Creates the main application window transparent
//********************************************************************************
void CTransparentWnd::CreateTransparent(LPCTSTR pTitle, RECT &rect)
{
	// 创建一个隐藏窗口
	CreateEx(   0,  
		AfxRegisterWndClass(0,AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
				pTitle,
				WS_POPUP ,
				rect,
				NULL,
				NULL,
			    NULL);

   StartBitmap(); //显示初始玫瑰图像

}

//********************************************************************************
//* SetupRegion()
//*
//* Set the Window Region for transparancy outside the mask region
//********************************************************************************
void CTransparentWnd::SetupRegion(CDC *pDC)
{
	CDC					memDC;
	CBitmap			&cBitmap=m_bmpDraw;
	CBitmap*		pOldMemBmp = NULL;
	COLORREF		col,colMask;
	CRect				cRect;
	int					x, y;
	CRgn				wndRgn, rgnTemp;

	GetWindowRect(&cRect);
	CPoint ptOrg=cRect.TopLeft();

	BITMAP bmInfo;
	cBitmap.GetObject(sizeof(bmInfo),&bmInfo);
	CRect rcNewWnd=CRect(ptOrg,CSize(bmInfo.bmWidth,bmInfo.bmHeight));

	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
	colMask=memDC.GetPixel(0,0);

	wndRgn.CreateRectRgn(0, 0, rcNewWnd.Width(), rcNewWnd.Height());
	for(x=0; x<=rcNewWnd.Width(); x++)
	{
		for(y=0; y<=rcNewWnd.Height(); y++)
		{
			col = memDC.GetPixel(x, y);
			if(col == colMask)
			{
				rgnTemp.CreateRectRgn(x, y, x+1, y+1);
				wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_XOR);
				rgnTemp.DeleteObject();	
			}
		}
	}
	if (pOldMemBmp) memDC.SelectObject(pOldMemBmp);
	SetWindowRgn((HRGN)wndRgn, TRUE);
	MoveWindow(rcNewWnd);
}

//WAV音频文件播放
void CTransparentWnd::SoundPlay(void)
{
  CString strTemp; 

	if(bMute) //当选择了情话静音时，不放声音
	{
	}
	else if(pCustomWavFile->GetOriginPlayFlag() ) //如选中恢复按钮,则播放原始情话 
	{
    
	//初始化时，判断是否有用户自定制的情话数据文件(放在此，是为了反复测情话文件是否存在)
	CStdioFile TxtFile;
    //打开情话数据文件
    if (TxtFile.Open("lovepet.dat", CFile::modeRead|CFile::typeText))
    {
	//读入用户自定制的情话文件名（不含路径，及在当前路径寻找并播放它）
    //和文字描述信息.
    TxtFile.ReadString (strTemp);
	//字符从0记起，故不加1 ,分离出相应字符串信息
	strWavFileName=strTemp.Left(strTemp.Find("$$$"));
	if(strTemp.Find( "$$$")>=0)
	  strTextDescription=strTemp.Right(strTemp.GetLength()-(strTemp.Find("$$$")+3));
	TxtFile.Close();

	//判断用户自定制的情话语音文件是否在当前路径
	if(!TxtFile.Open(strWavFileName,CFile::modeRead))
	{
	 //	CString strErrorMessage;
	//	strErrorMessage = "自定制情话文件:" + strWavFileName +"不存在或不和主程序(lovepet.exe)在一起!";
	//	AfxMessageBox(strErrorMessage);
	 
        strWavFileName="" ; //情话语音文件不存在，付于其空值		
	}
	else
		TxtFile.Close (); //文件存在，则关闭它，否则会播不出声音
   }
   else
      strWavFileName="" ; //情话数据文件不存在，付于其空值
	
    if(strWavFileName!="") //情话数据不为空并且自定制情话文件存在则播放它
	{
		SetTimer(2,5000,NULL); //设置自定制情话播放的时间为5秒
	   //用下API函数播放
    	sndPlaySound(strWavFileName,SND_ASYNC|SND_NODEFAULT  ); 
	}
	else if(bWavPlay)  //否则，播放原始情圣情话一
	{    //先关闭原声音播放
		PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  ); 
    	SetTimer(2,10700,NULL); //设置第一次播放声音时宠物休息的时间10.7秒
	   //资源WAV文件的ID须加双引号，用下API函数播放
    	PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_ASYNC|SND_NODEFAULT  ); 
	}
	else  //否则，播放原始情圣情话二
	{
		 //先关闭原声音播放
		PlaySound("IDR_WAVE2",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  ); 
      	SetTimer(2,12000,NULL); //设置第二次播放声音时宠物休息的时间12秒钟
	   //资源WAV文件的ID须加双引号，用下API函数播放
    	PlaySound("IDR_WAVE2",AfxGetResourceHandle(),SND_RESOURCE|SND_ASYNC |SND_NODEFAULT  ); 
	}
	}
	else  //播放定制情话
	{
		//定制情话文件一如为空，则播放原始情话文件一
      if(pCustomWavFile->GetWaveFile ().IsEmpty ()) 
	  { //先关闭原声音播放
		PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  ); 
    	SetTimer(2,10700,NULL); //设置第一次播放声音时宠物休息的时间10.7秒
	   //资源WAV文件的ID须加双引号，用下API函数播放
    	PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_ASYNC|SND_NODEFAULT  ); 
	  }
	  else //不为空，则播放自定制情话文件
	  {  
    	SetTimer(2,5000,NULL); //设置自定制情话播放的时间为5秒
	   //用下API函数播放
    	sndPlaySound(pCustomWavFile->GetWaveFile(),SND_ASYNC|SND_NODEFAULT  ); 
	  }

	}
		
}

//初始显示玫瑰图像
void CTransparentWnd::StartBitmap(void)
{
	char szBmp[20];
    CString strTemp; 

    sprintf(szBmp,"ROSE");
	m_bmpDraw.DeleteObject();
	m_bmpDraw.LoadBitmap(szBmp);
	CWindowDC dc(this);
	SetupRegion(&dc);
	Invalidate();

	SoundPlay(); //播放相应的WAV文件
}

void CTransparentWnd::Reset(void)
{
    Idle();
	SoundPlay(); //播放相应的WAV文件
}

//播放WAV文件时显示的动态图像
void CTransparentWnd::Idle(void)
{
  char szBmp[20];

    sprintf(szBmp,"WEST1");
	m_bmpDraw.DeleteObject();
	m_bmpDraw.LoadBitmap(szBmp);
	CWindowDC dc(this);
	SetupRegion(&dc);
	Invalidate();
}

void CTransparentWnd::DoSet(void)
{
	int iSW=GetSystemMetrics(SM_CXFULLSCREEN);
	int iSH=GetSystemMetrics(SM_CYFULLSCREEN);

	KillTimer(1);
	SetTimer(2,600,NULL);  //设置图像改变间隔时间

}

//获取用户自定制文字描述字符串
CString CTransparentWnd::GetTextDescription()
{
  return strTextDescription;
}

void CTransparentWnd::DoWalk(void)
{
	int iSW=GetSystemMetrics(SM_CXFULLSCREEN);
	int iSH=GetSystemMetrics(SM_CYFULLSCREEN);

	CRect rcW;
	GetWindowRect(rcW);

	SetTimer(2,600,NULL);  //恢复原图像改变时的间隔时间

	if(rcW.left<50) xcounter=10;
	if(rcW.top<50) ycounter=10;
	if( rcW.left <50) m_iLastDirection=1;  //向右移动
	
	if(rcW.left>iSW-50) xcounter=-10;
	if(rcW.top>iSH-50) ycounter=-10;
    if(rcW.left >iSW-50) m_iLastDirection=0; //向左移动
	
	CPoint ptOffset(xcounter,ycounter);
 	rcW+=ptOffset;
//	MoveWindow(rcW);  //移动窗体

	char szBmp[20];
  
	if(bDragDrop) //宠物实时拖动时显示的图像	 
      sprintf(szBmp,"GRIL");
    else if(m_iLastDirection==1) //宠物向右移动时显示的图像
	{
	  if((m_iAniSeq%24)>16) //图像动态四动一静
	    sprintf(szBmp,"WALK%d",m_iAniSeq%8+5); //不动时图像(后八个)
	  else
	  {
		sprintf(szBmp,"WALK%d",m_iAniSeq%4+1); //动时图像
		MoveWindow(rcW);//为运动图像时方移动窗体(前四个)
	  }
	}
	else//宠物向左移动时显示的图像
	{
      if((m_iAniSeq%24)>16)
        sprintf(szBmp,"WALK%d",m_iAniSeq%8+17); //不动时图像(后八个)
	  else
	  {
		sprintf(szBmp,"WALK%d",m_iAniSeq%4+13); //动时图像
	    MoveWindow(rcW);//为运动图像时方移动窗体(前四个)
	  }
	}

	m_bmpDraw.DeleteObject();
	m_bmpDraw.LoadBitmap(szBmp);
	CWindowDC dc(this);
	SetupRegion(&dc);
	Invalidate();
}


//********************************************************************************
//* CTransparentWnd message handlers
//********************************************************************************

void CTransparentWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

    SetTimer(3,60,NULL);  //拖动时同时显示窗体的计数器3
	//鼠标移动到窗体时改变鼠标指针
	::SetCursor(AfxGetApp()->LoadCursor(IDC_DROP));
	SetCapture(); //设置鼠标全屏捕捉
    CurPoint=point; //记下按下时的鼠标坐标

	//实现无标题拖动
//	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y)); 
}

int CTransparentWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

     	//系统区动态显示的图标文件
	m_hIcon[0]=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
	//创建系统区图标
	if (!m_TrayIcon.Create(this, 
				IDR_POPUP, 
				_T("爱圣V3.0"), 
				m_hIcon, 
				1, //一幅图像
				1000, //延时1秒变化
				WM_TRAYNOTIFY))
	{
		AfxMessageBox(_T("错误：创建系统图标失败！"), MB_OK | MB_ICONSTOP);
		return -1;
	}

	xcounter=10,ycounter=10;
	m_iLastDirection=1;  //宠物跑动的方向标志位，开始为向右
	SetTimer(1,3000,NULL);  //初始时的玫瑰图像显示时间3秒
	bWavPlay=true;  //声音开始播放标志位初始为真
	bDragDrop=false;  //宠物拖动标志初始为假
	bShow=true; //窗体显示动画标志位初始为真，及滑向系统区
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); //窗体总在总前面

	return 0;
}

BOOL CTransparentWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect	rect;
	GetWindowRect(&rect);

	CDC memDC;
	CBitmap			&cBitmap=m_bmpDraw;;
	CBitmap*		pOldMemBmp = NULL;
	CFont* pOldMemFont=NULL;

	memDC.CreateCompatibleDC(pDC);
	pOldMemBmp = memDC.SelectObject(&cBitmap);
/*	//宠物上方显示文字信息
	pOldMemFont = memDC.SelectObject(&m_fontLogo);
	CString szT=strTextPlay;
	int iLen=szT.GetLength ();
	CString strTemp=szT.Left (2);
	szT=szT.Right (iLen-2)+strTemp;
	CRect rcD(3,2,16,33);
	memDC.SetBkMode(TRANSPARENT);
	memDC.DrawText(szT,rcD,DT_LEFT|DT_WORDBREAK);
*/
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	if (pOldMemBmp) memDC.SelectObject( pOldMemBmp );
//	if (pOldMemFont) memDC.SelectObject( pOldMemFont );

	return TRUE;
}

void CTransparentWnd::OnPaint()
{
	CPaintDC dc(this);

}


void CTransparentWnd::OnTimer(UINT nIDEvent) 
{
  POINT point;
  CRect rcW;

	GetWindowRect(rcW);

	switch(nIDEvent)
	{
	case(1)://judge
		DoSet();
	
		break;
	case(2)://walk
		if(m_iAniSeq%24)  //奔跑还是静止播放声音
			DoWalk();
        else if(m_iAniSeq%59) //播放情话
		{   
		    bWavPlay=!bWavPlay;
			Reset();						
		}
		
		break; 
    case(3):
		bDragDrop=true;  //宠物拖动标志为真
        ::GetCursorPos(&point);  //实现拖动时窗体跟着移动
		MoveWindow(point.x-CurPoint.x ,point.y-CurPoint.y ,rcW.Width(),rcW.Height() ,true); 

		break;
	default:
		break;
	}
	m_iAniSeq++;  //图像循环显示变量增加及复位
	if(m_iAniSeq>60) m_iAniSeq=0;

	CWnd::OnTimer(nIDEvent);
}

void CTransparentWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

	//关闭程序对话框
void CTransparentWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDown(nFlags, point);

//	CScrGeniusDlg dlg;  
//	if(IDOK!=dlg.DoModal())
//		DestroyWindow();

	//添加右击时弹出快捷菜单
 /* CMenu menu;
    menu.LoadMenu(IDR_POPUP);
 
  CMenu* pSubMenu = menu.GetSubMenu(0);

    // Make first menu item the default (bold font)
    ::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

    //Display and track the popup menu
    CPoint pos;
    GetCursorPos(&pos);
 //   ::SetForegroundWindow(::FindWindow(NULL,"窈窕淑女，君子好求!"));  
    ::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, ::FindWindow(NULL,"窈窕淑女，君子好求!"), NULL);
  */

	//添加右键快捷图标菜单(用于下拉式菜单)
  popmenu.LoadMenu(IDR_POPUP);
  popmenu.ModifyODMenu(NULL,IDR_EJECT,IDB_EJECT);
  if(bMute) 
	popmenu.ModifyODMenu(NULL, IDR_MUTE,IDB_CHECKED);
  else
    popmenu.ModifyODMenu(NULL, IDR_MUTE,IDB_CHECK);
  popmenu.ModifyODMenu(NULL, IDR_CUSTOMWAVFILE,IDB_CUSTOM);
  popmenu.ModifyODMenu(NULL, IDR_GETIP,IDB_GETIP);
  popmenu.ModifyODMenu(NULL, IDR_HELP,IDB_HELP);
  popmenu.ModifyODMenu(NULL, IDR_BIG2GB,IDB_BIG2GB);
  popmenu.ModifyODMenu(NULL, IDR_BMP2TXT,IDB_BMP2TXT);
  popmenu.ModifyODMenu(NULL,IDR_EXIT,IDB_EXIT);
  popmenu.ModifyODMenu(NULL,IDR_REGISTER,IDB_REGISTER);
  popmenu.ModifyODMenu(NULL,IDR_SHUTDOWN,IDB_SHUTDOWN);
  popmenu.ModifyODMenu(NULL,IDR_HIDEINBMP,IDB_HIDEINBMP);
  popmenu.ModifyODMenu(NULL,IDR_SHOW,IDB_SHOW);
  popmenu.ModifyODMenu(NULL,IDR_REGOCX,IDB_REGOCX);
  popmenu.ModifyODMenu(NULL,IDR_EDSTAR,IDB_EDSTAR);
  //用原邮件发送ID号,换为文件分割图标
  popmenu.ModifyODMenu(NULL,ID_FILE_SEND_MAIL,IDB_FILESPLIT); 
  
  //SK modified for Unicode
 // popmenu.LoadToolbar(IDR_TOOLBAR);
  ClientToScreen(&point);
  BCMenu *psub = (BCMenu *)popmenu.GetSubMenu(0); 
  psub->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
  popmenu.DestroyMenu();
}

void CTransparentWnd::OnMoving(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnMoving(fwSide, pRect);
	
}

void CTransparentWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
    
	bDragDrop=false;  //宠物拖动标志为假
	ReleaseCapture();
	KillTimer(3);  //关闭拖动时的图像显示记数器3 
	// SetCursor(LoadCursor(NULL, IDC_ARROW)); //恢复鼠标正常显示
	
	CWnd::OnLButtonUp(nFlags, point);
}

LRESULT CTransparentWnd::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
  //删除系统区创建的图标映射涵数
  return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

//获取宠物是否显示的标志位
bool CTransparentWnd::GetShowPetFlag ()
{
  bool bShowFlag=true;

       if(!bShow)
		   bShowFlag=bShow;

	   return bShowFlag;
}

//显示或隐藏宠物
void CTransparentWnd::OnShow() 
{
	pFrame=AfxGetApp()->GetMainWnd();
	if(!bShow) //是否进行窗体动画,窗体已显示则不进行
	{
		WinAnimation(false); //窗体动画从系统区滑出 
		pFrame->ShowWindow(SW_SHOW);
	}
    else //是否进行窗体动画,窗体已隐藏则不进行
	{
        pFrame->ShowWindow(SW_HIDE);
	    WinAnimation(true); //窗体动画滑入到系统区中 
	}
	bShow=!bShow;  //显示隐藏标志位取反
	
}

//WINDOWS NT/98下安全关机或注销的调用涵数
void CTransparentWnd::WinShutdown(UINT ShutdownFlag)
{
    OSVERSIONINFO oi;
    oi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&oi);
	  //如果是NT/2000下需先设置相关权限
     if (oi.dwPlatformId == VER_PLATFORM_WIN32_NT) 
	 {
       HANDLE handle;
       TOKEN_PRIVILEGES tkp;

       OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &handle);
       LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

       tkp.PrivilegeCount = 1;  // one privilege to set    
       tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
       AdjustTokenPrivileges(handle, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	 }

	   //WIN98下关机或注销时可直接调用下面涵数既可
		::ExitWindowsEx(ShutdownFlag,0);

}

//注销当前用户
void CTransparentWnd::OnRegister() 
{
  
//	if(MessageBox("您是否真的要注销当前用户?","用户注销",
//		                         MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2)==IDOK)
//	{   
	   WinShutdown(EWX_LOGOFF);
	   CTransparentWnd::DestroyWindow();
//	}
	
}

//关闭计算机
void CTransparentWnd::OnShutdown() 
{
 //   if(MessageBox("您是否真的要关闭计算机?","关闭系统",
//		                         MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON2)==IDOK)
								
	   //  WinShutdown(EWX_SHUTDOWN);
	CSetTimeShutdown dlg;
    //此对话框只显示一次
	if( !FindWindow(NULL,"定时关机"))
	     dlg.DoModal();
	else
		::ShowWindow (::FindWindow(NULL,"定时关机"),SW_SHOW);
}

//弹出或关闭CD-ROM
void CTransparentWnd::OnEject() 
{
   if(bEject)
   {
	if(mciSendString("set cdaudio door open",NULL,0,NULL)!=0)
		  MessageBox("弹出CD-ROM出错!");
   }
   else
   {
	 if(mciSendString("set cdaudio door closed",NULL,0,NULL)!=0)
		  MessageBox("关闭CD-ROM出错");
   }
   bEject=!bEject;
	
}

//关闭程序对话框
void CTransparentWnd::OnExit() 
{
	
	CScrGeniusDlg dlg;  
	
      //只显示一次关闭对话框
	if( !FindWindow(NULL,"愿天下有情人终成眷属 -- 未来工作室(Future Studio)"))
	  if(IDOK!=dlg.DoModal())
	    DestroyWindow();
}

//原为邮件发送功能，现改为打开文件分割与合并对话框
void CTransparentWnd::OnFileSendMail() 
{
 // CHyperLink m_Mail; 

 //   m_Mail.GotoURL(_T("mailto:jz_x@china.com"), SW_SHOW);

	CFileSplit dlg;  

	  //此对话框只显示一次
	if( !FindWindow(NULL,"文件分割器"))
	     dlg.DoModal();

}

//获取静音情话的标志位
bool CTransparentWnd::GetMuteFlag ()
{
	bool bMuteFlag=false;

	if(bMute)
		bMuteFlag=bMute;

	return bMuteFlag;
}

//关闭或打开情话播放
void CTransparentWnd::OnMute() 
{
	bMute=!bMute; //情话静音标志取反	
	if(bMute)
	{
	  if(bWavPlay) 
		  //关闭原情话播放一
		PlaySound("IDR_WAVE1",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  ); 
	  else  
		 //先关闭原情话播放二
		PlaySound("IDR_WAVE2",AfxGetResourceHandle(),SND_RESOURCE|SND_PURGE|SND_NODEFAULT  );  	 

	  SetTimer(2,600,NULL);
	}
}

void CTransparentWnd::OnUpdateMute(CCmdUI* pCmdUI) 
{ 

   pCmdUI->SetCheck (bMute);
	
}

//窃取星号密码对话框
void CTransparentWnd::OnGetip() 
{
	GetIP dlg;  
	
	//此对话框只显示一次
  if( !FindWindow(NULL,"窃取星号密码"))
	    dlg.DoModal();
	
}

//位图转换为文本对话框
void CTransparentWnd::OnBmp2txt() 
{
	CBMP2TXT dlg; 

	//此对话框只显示一次
  if( !FindWindow(NULL,"位图转换为文本"))
	    dlg.DoModal();
	
}

void CTransparentWnd::OnHelp() 
{
	CAbout dlg; 

	//此对话框只显示一次
  if( !FindWindow(NULL,"帮助"))
	    dlg.DoModal();
	
}

//在显示窗体动画效果前，先寻找系统区位置
BOOL CALLBACK FindTrayWnd(HWND hwnd, LPARAM lParam)
{
    TCHAR szClassName[256];
    GetClassName(hwnd, szClassName, 255);

    // 比较窗口类名
    if (_tcscmp(szClassName, _T("TrayNotifyWnd")) == 0)
    {
        CRect *pRect = (CRect*) lParam;
        ::GetWindowRect(hwnd, pRect);
        return TRUE;
    }

    // 当找到时钟窗口时表示可以结束了
    if (_tcscmp(szClassName, _T("TrayClockWClass")) == 0)
    {
        CRect *pRect = (CRect*) lParam;
        CRect rectClock;
        ::GetWindowRect(hwnd, rectClock);
        pRect->right = rectClock.left;
        return FALSE;
    }
 
    return TRUE;
}

//显示窗口动画效果的涵数
void CTransparentWnd::WinAnimation(bool ShowFlag) 
{
    CRect rect(0,0,0,0);

    // 查找托盘窗口 
    CWnd* pWnd = FindWindow("Shell_TrayWnd", NULL);
    if (pWnd)
    {
        pWnd->GetWindowRect(rect);
		EnumChildWindows(pWnd->m_hWnd, FindTrayWnd, (LPARAM)&rect);
		//rect 为托盘区矩形
		CRect rcWnd;
		GetWindowRect(rcWnd);
		if(ShowFlag) //窗体滑向系统区
		  DrawAnimatedRects(GetSafeHwnd(),IDANI_CAPTION,rcWnd,rect);
		else   //窗体从系统区滑出
		  DrawAnimatedRects(GetSafeHwnd(),IDANI_CAPTION,rect,rcWnd);
    }
}


void CTransparentWnd::OnCustomwavfile() 
{
  CCustomWavFile dlg; 

	//此对话框只显示一次
  if( !FindWindow(NULL,"定制自我情话"))
	    dlg.DoModal();
	
}

void CTransparentWnd::OnBig2gb() 
{
	Big2GB dlg; 

	//此对话框只显示一次
  if( !FindWindow(NULL,"两岸通--简繁字体互转"))
	    dlg.DoModal();
	
}

void CTransparentWnd::OnHideinbmp() 
{
  HideInBMP dlg;

   	//此对话框只显示一次
  if( !FindWindow(NULL,"将文件隐藏到位图中"))
	  dlg.DoModal();
	
}

void CTransparentWnd::OnMouseMove(UINT nFlags, CPoint point) 
{	
	CWnd::OnMouseMove(nFlags, point);
}

void CTransparentWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
    CRect rc0;  //对话框动画显示效果
    AnimEffect anim0;

	GetWindowRect(rc0);
	anim0.Open (rc0);  //显示对话框动态打开动画效果	
}

//文件加解密对话框
void CTransparentWnd::OnEdstar() 
{
	CEDStar dlg;

   	//此对话框只显示一次
  if( !FindWindow(NULL,"加密之星袖珍版"))
	  dlg.DoModal();
	
}

//控件注册器对话框
void CTransparentWnd::OnRegocx() 
{
  CRegOCX dlg;

   	//此对话框只显示一次
  if( !FindWindow(NULL,"控件注册器"))
	  dlg.DoModal();
}
