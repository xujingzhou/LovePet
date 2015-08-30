//
//Modify by 徐景周 2000.10
//功能：系统区动态图标显示效果
//


/////////////////////////////////  Includes  //////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "ntray.h"
#include "CBmpMenu.h"  //菜单左边加入长条位图
#include "TransparentWnd.h"

//
//Modify by 徐景周 2000.10
//功能：系统区动态图标显示
//

/////////////////////////////////  Macros /////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


///////////////////////////////// Implementation //////////////////////////////
CTransparentWnd *pWnd; //CTransparentWnd类窗体指针

const UINT wm_TaskbarCreated = RegisterWindowMessage(_T("TaskbarCreated"));

IMPLEMENT_DYNAMIC(CTrayRessurectionWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(CTrayRessurectionWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CTrayRessurectionWnd)
	//}}AFX_MSG_MAP
  ON_REGISTERED_MESSAGE(wm_TaskbarCreated, OnTaskbarCreated)
END_MESSAGE_MAP()

LRESULT CTrayRessurectionWnd::OnTaskbarCreated(WPARAM wParam, LPARAM lParam)
{
  ASSERT(m_pTrayIcon);

  //Refresh the tray icon if necessary
  if (m_pTrayIcon->IsShowing())
  {
    m_pTrayIcon->HideIcon();
    m_pTrayIcon->ShowIcon();
  }

  return 0L;
}

CTrayRessurectionWnd::CTrayRessurectionWnd(CTrayNotifyIcon* pTrayIcon)
{
  //must have at valid tray notify instance
  ASSERT(pTrayIcon);

  //Store the values away
  m_pTrayIcon = pTrayIcon;
}


IMPLEMENT_DYNAMIC(CTrayTimerWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(CTrayTimerWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CTrayTimerWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTrayTimerWnd::CTrayTimerWnd(CTrayNotifyIcon* pTrayIcon, HICON* phIcons, int nNumIcons, DWORD dwDelay)
{
  m_nCurrentIconIndex = 0;

  //must have a valid tray notify instance
  ASSERT(pTrayIcon);

  //must have at least 1 icon
  ASSERT(nNumIcons);

  //array of icon handles must be valid
  ASSERT(phIcons);

  //must be non zero timer interval
  ASSERT(dwDelay);

  //Store the values away
  m_pTrayIcon = pTrayIcon;

  m_phIcons = new HICON[nNumIcons];
  CopyMemory(m_phIcons, phIcons, nNumIcons * sizeof(HICON));
  m_nNumIcons = nNumIcons;
  m_dwDelay = dwDelay;
}

CTrayTimerWnd::~CTrayTimerWnd()
{
  if (m_phIcons)
  {
    delete [] m_phIcons;
    m_phIcons = NULL;
  }
}

int CTrayTimerWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
  //create the animation timer
	m_nTimerID = SetTimer(1, m_dwDelay, NULL);

	return 0;
}

void CTrayTimerWnd::OnDestroy() 
{
  //kill the animation timer
  KillTimer(m_nTimerID);

   CFrameWnd::OnDestroy();
}

void CTrayTimerWnd::OnTimer(UINT nIDEvent) 
{
  //increment the icon index
  ++m_nCurrentIconIndex;
  m_nCurrentIconIndex = m_nCurrentIconIndex % m_nNumIcons;

  //update the tray icon
  m_pTrayIcon->m_NotifyIconData.uFlags = NIF_ICON;
  m_pTrayIcon->m_NotifyIconData.hIcon = m_phIcons[m_nCurrentIconIndex];
  Shell_NotifyIcon(NIM_MODIFY, &m_pTrayIcon->m_NotifyIconData);
}




IMPLEMENT_DYNAMIC(CTrayNotifyIcon, CObject)

CTrayNotifyIcon::CTrayNotifyIcon()
{
  memset(&m_NotifyIconData, 0, sizeof(m_NotifyIconData));
  m_bCreated = FALSE;
  m_bHidden = FALSE;
  m_pNotificationWnd = NULL;
  m_pResurrectionWnd = NULL;
  m_pTimerWnd = NULL;
  m_bAnimated = FALSE;
}

CTrayNotifyIcon::~CTrayNotifyIcon()
{
  DestroyTimerWindow();
  DestroyResurrectionWindow();
  RemoveIcon();
}

void CTrayNotifyIcon::HideIcon()
{
  ASSERT(m_bCreated);
	if (!m_bHidden) 
  {
		m_NotifyIconData.uFlags = NIF_ICON;
		Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);
		m_bHidden = TRUE;
	}
}

void CTrayNotifyIcon::ShowIcon()
{
  ASSERT(m_bCreated);
	if (m_bHidden) 
  {
		m_NotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);
		m_bHidden = FALSE;
	}
}

void CTrayNotifyIcon::RemoveIcon()
{
  if (m_bCreated)
  {
    m_NotifyIconData.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);
    m_bCreated = FALSE;
  }
}

void CTrayNotifyIcon::MoveToExtremeRight()
{
  HideIcon();
  ShowIcon();
}

BOOL CTrayNotifyIcon::Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON hIcon, UINT nNotifyMessage)
{
  //Create the ressurection window
  if (!CreateRessurectionWindow())
    return FALSE;

  //Make sure Notification window is valid
  ASSERT(pNotifyWnd && ::IsWindow(pNotifyWnd->GetSafeHwnd()));
  m_pNotificationWnd = pNotifyWnd;

  //Make sure we avoid conflict with other messages
  ASSERT(nNotifyMessage >= WM_USER);

  //Tray only supports tooltip text up to 64 characters
  ASSERT(_tcslen(pszTooltipText) <= 64);

  m_NotifyIconData.cbSize = sizeof(m_NotifyIconData);
  m_NotifyIconData.hWnd = pNotifyWnd->GetSafeHwnd();
  m_NotifyIconData.uID = uID;
  m_NotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
  m_NotifyIconData.uCallbackMessage = nNotifyMessage;
  m_NotifyIconData.hIcon = hIcon;
  _tcscpy(m_NotifyIconData.szTip, pszTooltipText);

  BOOL rVal = Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);
  m_bCreated = rVal;

  return rVal;
}

BOOL CTrayNotifyIcon::CreateRessurectionWindow()
{
  //Create the resurrection window 
  ASSERT(m_pResurrectionWnd == NULL);
  m_pResurrectionWnd = new CTrayRessurectionWnd(this);
  if (!m_pResurrectionWnd)
    return FALSE;
  if (!m_pResurrectionWnd->Create(NULL, _T("CTrayNotifyIcon Resurrection Notification Window")))
    return FALSE;

  return TRUE;
}

void CTrayNotifyIcon::DestroyResurrectionWindow()
{
  if (m_pResurrectionWnd)
  {
    m_pResurrectionWnd->SendMessage(WM_CLOSE);
    m_pResurrectionWnd = NULL;
  }
}

BOOL CTrayNotifyIcon::CreateTimerWindow(HICON* phIcons, int nNumIcons, DWORD dwDelay)
{
  //create the hidden window which will contain the timer which will do the animation
  ASSERT(m_pTimerWnd == NULL); 
  m_pTimerWnd = new CTrayTimerWnd(this, phIcons, nNumIcons, dwDelay);
  if (!m_pTimerWnd)
    return FALSE;
  if (!m_pTimerWnd->Create(NULL, _T("CTrayNotifyIcon Animation Notification Window")))
    return FALSE;

  return TRUE;
}

void CTrayNotifyIcon::DestroyTimerWindow()
{
  if (m_pTimerWnd)
  {
    m_pTimerWnd->SendMessage(WM_CLOSE);
    m_pTimerWnd = NULL;
  }
}

BOOL CTrayNotifyIcon::Create(CWnd* pNotifyWnd, UINT uID, LPCTSTR pszTooltipText, HICON* phIcons, int nNumIcons, DWORD dwDelay, UINT nNotifyMessage)
{
  //must be using at least 2 icons
  ASSERT(nNumIcons >= 2);

  if (!CreateTimerWindow(phIcons, nNumIcons, dwDelay))
    return FALSE;

  //let the normal Create function do its stuff
  BOOL bSuccess = Create(pNotifyWnd, uID, pszTooltipText, phIcons[0], nNotifyMessage);
  m_bAnimated = TRUE;
  return bSuccess;
}

BOOL CTrayNotifyIcon::SetTooltipText(LPCTSTR pszTooltipText)
{
  if (!m_bCreated)
    return FALSE;

  m_NotifyIconData.uFlags = NIF_TIP;
  _tcscpy(m_NotifyIconData.szTip, pszTooltipText);

  return Shell_NotifyIcon(NIM_MODIFY, &m_NotifyIconData);
}

BOOL CTrayNotifyIcon::SetTooltipText(UINT nID)
{
  CString sToolTipText;
  VERIFY(sToolTipText.LoadString(nID));

  return SetTooltipText(sToolTipText);
}

BOOL CTrayNotifyIcon::SetIcon(HICON hIcon)
{
  if (!m_bCreated)
    return FALSE;

  DestroyTimerWindow();
  m_bAnimated = FALSE;
  m_NotifyIconData.uFlags = NIF_ICON;
  m_NotifyIconData.hIcon = hIcon;

  return Shell_NotifyIcon(NIM_MODIFY, &m_NotifyIconData);
}

BOOL CTrayNotifyIcon::SetIcon(LPCTSTR lpIconName)
{
  HICON hIcon = AfxGetApp()->LoadIcon(lpIconName);

  return SetIcon(hIcon);
}

BOOL CTrayNotifyIcon::SetIcon(UINT nIDResource)
{
  HICON hIcon = AfxGetApp()->LoadIcon(nIDResource);

  return SetIcon(hIcon);
}

BOOL CTrayNotifyIcon::SetStandardIcon(LPCTSTR lpIconName)
{
  HICON hIcon = LoadIcon(NULL, lpIconName);

  return SetIcon(hIcon);
}

BOOL CTrayNotifyIcon::SetStandardIcon(UINT nIDResource)
{
  HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(nIDResource));

  return SetIcon(hIcon);
}

BOOL CTrayNotifyIcon::SetIcon(HICON* phIcons, int nNumIcons, DWORD dwDelay)
{
  ASSERT(nNumIcons >= 2);
  ASSERT(phIcons);

  if (!SetIcon(phIcons[0]))
    return FALSE;

  DestroyTimerWindow();
  if (!CreateTimerWindow(phIcons, nNumIcons, dwDelay))
    return FALSE;
  m_bAnimated = TRUE;

  return TRUE;
}

BOOL CTrayNotifyIcon::SetNotificationWnd(CWnd* pNotifyWnd)
{
  if (!m_bCreated)
    return FALSE;

  //Make sure Notification window is valid
  ASSERT(pNotifyWnd && ::IsWindow(pNotifyWnd->GetSafeHwnd()));

  m_pNotificationWnd = pNotifyWnd;
  m_NotifyIconData.hWnd = pNotifyWnd->GetSafeHwnd();
  m_NotifyIconData.uFlags = 0;

  return Shell_NotifyIcon(NIM_MODIFY, &m_NotifyIconData);
}

CString CTrayNotifyIcon::GetTooltipText() const
{
  CString sText;
  if (m_bCreated)
    sText = m_NotifyIconData.szTip;

  return sText;
}

HICON CTrayNotifyIcon::GetIcon() const
{
  HICON hIcon = NULL;
  if (m_bCreated)
  {
    if (m_bAnimated)
      hIcon = m_pTimerWnd->GetCurrentIcon();
    else
      hIcon = m_NotifyIconData.hIcon;
  }

  return hIcon;
}

CWnd* CTrayNotifyIcon::GetNotificationWnd() const
{
  return m_pNotificationWnd;
}

LRESULT CTrayNotifyIcon::OnTrayNotification(WPARAM wID, LPARAM lEvent)
{
  //Return quickly if its not for this tray icon
  if (wID != m_NotifyIconData.uID)
    return 0L;

  //As a default action use a menu resource with the same id 
  //as this was created with

  CMenu popmenu;
  if (!popmenu.LoadMenu(m_NotifyIconData.uID))
    return 0;
    //添加图标菜单

  CMenu * pSubMenu = popmenu.GetSubMenu(0);
  if (!pSubMenu) 
   return 0;
 

  if (lEvent == WM_RBUTTONUP)
  {
    //Clicking with right button brings up a context menu

    // Make first menu item the default (bold font)
 /*
  ::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

    //Display and track the popup menu
    CPoint pos;
    GetCursorPos(&pos);
    ::SetForegroundWindow(m_NotifyIconData.hWnd);  
    ::TrackPopupMenu(pSubMenu->m_hMenu, 0, pos.x, pos.y, 0, m_NotifyIconData.hWnd, NULL);
 */  

    //在系统区菜单左边添加长条位图(用于弹出式菜单)
  HBITMAP hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BAR));

	CString	m_csWidth;
	int		m_nStretch;
	BOOL	m_bShowBmp;
	BOOL	m_bShow4All;

	m_csWidth = CString("23");
	m_nStretch = 0;
	m_bShowBmp = TRUE;
	m_bShow4All = TRUE;


	CBmpMenu oMenu(atoi(m_csWidth.GetBuffer(0)), m_bShow4All, m_bShowBmp?hBitmap:0, (m_nStretch==0)?TRUE:FALSE);

	oMenu.LoadMenu(IDR_POPUP);

	 CBitmap bmp;	
	 if(pWnd->GetMuteFlag ()) //根据是否选中静音标志来替换位图显示
	   bmp.LoadBitmap(IDB_CHECKED1);    
	 else
       bmp.LoadBitmap(IDB_CHECK1);   
	 oMenu.SetMenuItemBitmaps( 5, MF_BYPOSITION, &bmp, &bmp);

	CPoint pt;
	GetCursorPos(&pt);
	 //删除第一级下拉菜单，只显示第二级及以下菜单弹出菜单
	oMenu.DeleteMenu ( 0 ,MF_BYPOSITION );
	oMenu.DrawMenuBar ();
    oMenu.TrackPopupMenu(0, pt.x, pt.y, AfxGetApp()->m_pMainWnd );
  } 
  else if (lEvent == WM_LBUTTONDBLCLK) 
  {
    // double click received, the default action is to execute first menu item
    if(!pWnd->GetShowPetFlag())  //同步获取的显示标志位为真显示宠物，为假不显示
	  ::SendMessage(m_NotifyIconData.hWnd, WM_COMMAND, pSubMenu->GetMenuItemID(0), 0);
    else
      ::SendMessage(m_NotifyIconData.hWnd, WM_COMMAND, pSubMenu->GetMenuItemID(0), 0);
  }

  return 1; // handled
}



