// SetTimeShutdown.cpp : implementation file
// 
//功能：Windows 9x/NT下定时、立即关机,重启计算机,开机程序自启动
// 作者：徐景周 　
//日期：2001年7月20日
//
#include "stdafx.h"
#include "scrgenius.h"
#include "SetTimeShutdown.h"
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//设置开机程序自启动键值位置和其下新建子键值(可设为程序名)
const TCHAR gcszAutoRunKey[]= _T( "Software\\Microsoft\\windows\\CurrentVersion\\Run" );
const TCHAR	gcszWindowClass[] = _T("lovePet");
/////////////////////////////////////////////////////////////////////////////
// CSetTimeShutdown dialog


CSetTimeShutdown::CSetTimeShutdown(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTimeShutdown::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTimeShutdown)
	m_sNowTime = _T("");
	m_Time = CTime(2001,7,21,20,06,0);
	//}}AFX_DATA_INIT
}


void CSetTimeShutdown::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTimeShutdown)
	DDX_Control(pDX, IDC_REBOOT, m_ReBoot);
	DDX_Control(pDX, IDC_SHUTDOWN, m_ShutDownOnce);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_BUTTON_SHUTDOWN, m_ShutDown);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_Timer);
	DDX_Text(pDX, IDC_STATIC_NOW, m_sNowTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_Time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTimeShutdown, CDialog)
	//{{AFX_MSG_MAP(CSetTimeShutdown)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, OnButtonShutdown)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_SHUTDOWN, OnShutdown)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_REBOOT, OnReboot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTimeShutdown message handlers

BOOL CSetTimeShutdown::OnInitDialog() 
{
	CDialog::OnInitDialog();	

    //启动计时器，一秒钟更新一次
    SetTimer(0,1000,NULL);	

	bSetTime=false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//开机时程序自动运行
BOOL StartUpAutoRun(BOOL bIsAdd)
{
	HKEY hKey;
	LONG lRet, lRet2;
	DWORD dwLength, dwDataType;
	TCHAR szItemValue[MAX_PATH], szPrevValue[MAX_PATH];
	TCHAR szBuffer[MAX_PATH];

	// 得到程序全路径名
	GetModuleFileName( NULL, szItemValue, MAX_PATH );


	// 打开注册表键
		lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, gcszAutoRunKey,
			0, KEY_READ | KEY_WRITE, &hKey );
		if( lRet != ERROR_SUCCESS )
			return FALSE;

	// 查询自动运行项目是否存在
		dwLength = sizeof( szBuffer );
		lRet = RegQueryValueEx( hKey, gcszWindowClass, 
			NULL, &dwDataType, (LPBYTE)szBuffer, &dwLength );

	// 添加
		if( bIsAdd == TRUE ) 
		{
	// 自动运行项目不存在
			if( lRet != ERROR_SUCCESS )
				lRet2 = RegSetValueEx( hKey, gcszWindowClass,
				0, REG_SZ, (LPBYTE)szItemValue, strlen( szItemValue ) );
			else 
			{
	// 存在, 比较二者是否相同
				dwLength = sizeof( szPrevValue );
				lRet2 = RegQueryValueEx( hKey, gcszWindowClass, 
					0, &dwDataType,(LPBYTE)szPrevValue, &dwLength );
	// 不相同则替换
				if( lstrcmp( szItemValue, szPrevValue ) ) 
				{
					lRet2 = RegDeleteValue( hKey, gcszWindowClass );
					lRet2 = RegSetValueEx( hKey, gcszWindowClass, 
						0, REG_SZ,(LPBYTE)szItemValue, strlen( szItemValue ) );
				}
			}
		}
	// 删除
		else 
	// 自动运行项目已存在则删除
			if( lRet == ERROR_SUCCESS )
				lRet2 = RegDeleteValue( hKey, gcszWindowClass );

	// 关闭注册表键
		RegCloseKey( hKey );

		if( lRet2 != ERROR_SUCCESS )
			return FALSE;


	return TRUE;
}

//WINDOWS NT/98下安全关机或注销的调用涵数
void WinShutdown(UINT ShutdownFlag)
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

void CSetTimeShutdown::OnTimer(UINT nIDEvent) 
{
  int hour,min,sec;
  int yhour,ymin,ysec;
    
    CTime nowtime = CTime::GetCurrentTime();
	m_sNowTime = nowtime.Format(_T("%Y-%m-%d %H:%M:%S"));

	UpdateData(FALSE);

	if(bSetTime)
	{
	hour = nowtime.GetHour();
	min	 = nowtime.GetMinute();
	sec  = nowtime.GetSecond();

	yhour = m_Time.GetHour();
	ymin  = m_Time.GetMinute();
	ysec  = m_Time.GetSecond();

	//如果时间到，则关机
	if(hour == yhour && min == ymin )
	    WinShutdown(EWX_SHUTDOWN);
	}

	
	CDialog::OnTimer(nIDEvent);
}

//定时关机按钮按下时
void CSetTimeShutdown::OnButtonShutdown() 
{
	if(m_sNowTime.IsEmpty())
	  return;

    bSetTime=true;

	ShowWindow(SW_HIDE);//隐藏该窗体

	//判断是否选中开机自启动项，并做相应处理
	int Status;
	CButton * check1 = ( CButton * )GetDlgItem( IDC_CHECK1 );
	Status=check1->GetCheck();
	if (Status==1) //添加子键
	   StartUpAutoRun(true);
	else  //删除子键
	   StartUpAutoRun(false);
}

void CSetTimeShutdown::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
    UpdateData(true);	//更新控件显示

	*pResult = 0;
}

//当按下立即关机按钮时，则马上关机
void CSetTimeShutdown::OnShutdown() 
{
    //判断是否选中开机自启动项，并做相应处理
	int Status;
	CButton * check1 = ( CButton * )GetDlgItem( IDC_CHECK1 );
	Status=check1->GetCheck();
	if (Status==1) //添加子键
	   StartUpAutoRun(true);
	else  //删除子键
	   StartUpAutoRun(false);
   
	//关机
   WinShutdown(EWX_SHUTDOWN);
}

BOOL CSetTimeShutdown::DestroyWindow() 
{
   CRect rect3;  //对话框动画显示效果
   AnimEffect animation3;

	//在此加入对话框动画关闭效果
	GetWindowRect(rect3);
	rect3.DeflateRect(2,2);//动态方框显示
    animation3.DrawWireRects(rect3, CRect(rect3.CenterPoint().x-10,rect3.CenterPoint().y-10,rect3.CenterPoint().x+10,rect3.CenterPoint().y+10 ), 10);
//	animation3.Close(rect3); ////对话框动画效果显示关闭
//	Sleep(50); //延缓50毫秒后关闭

	return CDialog::DestroyWindow();
}

void CSetTimeShutdown::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	CRect rect3;  //对话框动画显示效果
    AnimEffect animation3;

	//在此加入对话框动画打开效果
	GetWindowRect(rect3 );
    animation3.Open( rect3 ); //对话框动画效果显示打开
	
}

//关闭该对话框
void CSetTimeShutdown::OnCancel() 
{
    //判断是否选中开机自启动项，并做相应处理
	int Status;
	CButton * check1 = ( CButton * )GetDlgItem( IDC_CHECK1 );
	Status=check1->GetCheck();
	if (Status==1) //添加子键
	   StartUpAutoRun(true);
	else  //删除子键
	   StartUpAutoRun(false);
	
	CDialog::OnCancel();
}

//重新启动计算机
void CSetTimeShutdown::OnReboot() 
{
   //判断是否选中开机自启动项，并做相应处理
	int Status;
	CButton * check1 = ( CButton * )GetDlgItem( IDC_CHECK1 );
	Status=check1->GetCheck();
	if (Status==1) //添加子键
	   StartUpAutoRun(true);
	else  //删除子键
	   StartUpAutoRun(false);
   
	//重新启动
   WinShutdown(EWX_REBOOT);
	
}
