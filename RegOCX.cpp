// RegOCX.cpp : implementation file
// 
//功能：控件注册器的窗体类，在此加入注册，注销控件以及显示桌面等功能
// 作者：徐景周 　
//日期：2001年7月8日
//

#include "stdafx.h"
#include "scrgenius.h"
#include "RegOCX.h"
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegOCX dialog


CRegOCX::CRegOCX(CWnd* pParent /*=NULL*/)
	: CDialog(CRegOCX::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegOCX)
	m_FileName = _T("");
	//}}AFX_DATA_INIT
}


void CRegOCX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegOCX)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_UNREGOCX, m_UnRegOCX);
	DDX_Control(pDX, IDC_SHOWDESKTOP, m_ShowDesktop);
	DDX_Control(pDX, IDC_REGOCX, m_RegOCX);
	DDX_Control(pDX, IDC_BROWSE, m_Browse);
	DDX_Text(pDX, IDC_FILENAME, m_FileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegOCX, CDialog)
	//{{AFX_MSG_MAP(CRegOCX)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_SHOWDESKTOP, OnShowdesktop)
	ON_BN_CLICKED(IDC_REGOCX, OnRegocx)
	ON_BN_CLICKED(IDC_UNREGOCX, OnUnregocx)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegOCX message handlers

//浏览并找出要注册或注销的控件文件
void CRegOCX::OnBrowse() 
{
   CFileDialog fileDialog( TRUE,"*.OCX",NULL,NULL,"控件文件(*.ocx)|*.ocx|所有文件(*.*)|*.*||");
	if (fileDialog.DoModal() == IDOK) {
		m_FileName=fileDialog.GetPathName();	
	
	    UpdateData(FALSE); //把变量值更新到相应控件中
	}	
	
}

BOOL CRegOCX::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //初始化桌面窗体数为0
	m_nWndNum = 0;	

	//在此将文件浏览按钮设为扁平状,颜色和自定义窗体色相同
	m_Browse.SubclassDlgItem(IDC_BROWSE, this);
	m_Browse.SetActiveBgColor(RGB(240,200,90));
	m_Browse.SetInactiveBgColor(RGB(240,200,90));
	m_Browse.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse.SetIcon(IDI_FILEOPEN);
	m_Browse.SetBtnCursor(IDC_HAND); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//最小化或还原桌面上所有打开的窗体，以显示出桌面
void CRegOCX::OnShowdesktop() 
{
// 寻找桌面上正运行的所有窗本，并将正显示的存储到数组pWnd[]中
CWnd *pWnd[MAXNUM_RUNWINDOW], *pWndCur;
int nWndNum = 0;

  //将控件注册器的窗体提到最前
  SetForegroundWindow();

  pWnd[nWndNum]= GetWindow(GW_HWNDFIRST);

  while (pWnd[nWndNum] != NULL)
  {
   pWndCur = pWnd[nWndNum];
   CString str;
   pWndCur->GetWindowText(str);
   //最小化桌面窗体中不应包含程序主窗体
   if (str != "送给你..." && pWndCur->IsWindowVisible() && ! pWndCur->IsIconic() &&! pWndCur->GetParent() &&pWndCur != this &&pWndCur != GetDesktopWindow() &&! str.IsEmpty() &&str != "Program Manager")
   {
    // 列表数中窗体数加一
    nWndNum++;
    // 为了安全起见，窗体数不应超过总窗体数!!
    nWndNum = min(nWndNum, MAXNUM_RUNWINDOW-1);
   }
   pWnd[nWndNum] = pWndCur->GetWindow(GW_HWNDNEXT);
  }

  if (nWndNum)
  {
  // 将正在运行的所有桌面窗体最小化(自身除外)
  for (int i=0; i<nWndNum;++i)
  {
   pWnd[i]->ShowWindow(SW_SHOWMINNOACTIVE);
   m_pWnd[i] = pWnd[i];
  }
  m_nWndNum = nWndNum;
  }
  else
  {
  // 将已运行并被最小化的桌面窗体还原显示
  for (int i=m_nWndNum-1; i>=0; --i)
  {
   if (::IsWindow(m_pWnd[i]->m_hWnd) &&
   m_pWnd[i]->IsIconic())
   {
    m_pWnd[i]->ShowWindow(SW_RESTORE);
    m_pWnd[i]->UpdateWindow();
   }
  }
  }
  
}

//对已选控件进行系统注册
void CRegOCX::OnRegocx() 
{
//ActiveX控件的路径及文件名
  LPCTSTR pszDllName;

  //已选控件名是否为空
  if(m_FileName!="")
    pszDllName=m_FileName;
  else
  {
	MessageBox("请先选择您需要注册的控件名称！","提示",MB_OK);
	return;
  }

  //装载ActiveX控件
  HINSTANCE hLib = LoadLibrary(pszDllName);
  if (hLib < (HINSTANCE)HINSTANCE_ERROR)
  {
       MessageBox("不能载入该控件文件!","错误",MB_OK);
       return;
  }
  //获取注册函数DllRegisterServer地址
  FARPROC lpDllEntryPoint; 
  lpDllEntryPoint = GetProcAddress(hLib,_T("DllRegisterServer"));
  //调用注册函数DllRegisterServer
  if(lpDllEntryPoint!=NULL)
  {
       if(FAILED((*lpDllEntryPoint)()))
       {
            MessageBox("控件注册失败！","错误",MB_OK);
            FreeLibrary(hLib);
            return;
       }
       MessageBox("控件注册成功","提示",MB_OK);
  }
  else
  MessageBox("控件注册失败！","错误",MB_OK);
		
}

//对已注册控件进行系统注销
void CRegOCX::OnUnregocx() 
{
//ActiveX控件的路径及文件名
  LPCTSTR pszDllName;

  //已选控件名是否为空
  if(m_FileName!="")
    pszDllName=m_FileName;
  else
  {
	MessageBox("请先选择您需要注销的控件名称！","提示",MB_OK);
	return;
  }

  //装载ActiveX控件
  HINSTANCE hLib = LoadLibrary(pszDllName);
  if (hLib < (HINSTANCE)HINSTANCE_ERROR)
  {
       MessageBox("不能载入该控件文件!","错误",MB_OK);
       return;
  }
  //获取注销函数DllUnregisterServerr地址
  FARPROC lpDllEntryPoint; 
  lpDllEntryPoint = GetProcAddress(hLib,_T("DllUnregisterServer"));
  //调用注销函数DllUnRegisterServer
  if(lpDllEntryPoint!=NULL)
  {
       if(FAILED((*lpDllEntryPoint)()))
       {
            MessageBox("控件注销失败！","错误",MB_OK);
            FreeLibrary(hLib);
            return;
       }
       MessageBox("控件注销成功","提示",MB_OK);
  }
  else
  MessageBox("控件注销失败！","错误",MB_OK);
  
}


BOOL CRegOCX::DestroyWindow() 
{
   CRect rect1;  //对话框动画显示效果
   AnimEffect animation1;

	//在此加入对话框动画关闭效果
	GetWindowRect(rect1);
	rect1.DeflateRect(2,2);//动态方框显示
    animation1.DrawWireRects(rect1, CRect(rect1.CenterPoint().x-10,rect1.CenterPoint().y-10,rect1.CenterPoint().x+10,rect1.CenterPoint().y+10 ), 10);
//	animation1.Close(rect1); ////对话框动画效果显示关闭
//	Sleep(50); //延缓50毫秒后关闭
	
	
	return CDialog::DestroyWindow();
}

void CRegOCX::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rect1;  //对话框动画显示效果
   AnimEffect animation1;

	//在此加入对话框动画打开效果
	GetWindowRect(rect1 );
    animation1.Open( rect1 ); //对话框动画效果显示打开		
}
