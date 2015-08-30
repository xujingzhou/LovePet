/ ScrGeniusDlg.cpp : implementation file
//
//Modify by 徐景周 2000.10
//功能：退出对话框实现

#include "stdafx.h"
#include "ScrGenius.h"
#include "ScrGeniusDlg.h"
//#include <afxpriv.h>
#include "TransparentWnd.h"
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTransparentWnd *pTextDescription; //用于得到用户自定制文字信息

/////////////////////////////////////////////////////////////////////////////
// ScrGenius dialog

CScrGeniusDlg::CScrGeniusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScrGeniusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ScrGenius)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScrGeniusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ScrGenius)
	DDX_Control(pDX, IDOK,  m_Button1);
	DDX_Control(pDX, IDCANCEL,  m_Button2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScrGeniusDlg, CDialog)
	//{{AFX_MSG_MAP(ScrGenius)
	ON_WM_PAINT()
//	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_WM_SHOWWINDOW()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
LRESULT CScrGeniusDlg::OnKickIdle(WPARAM wparam, LPARAM lparam)
	{
		UpdateDialogControls(this, false);
		return 0;
	}
*/

/////////////////////////////////////////////////////////////////////////////
// ScrGenius message handlers

BOOL CScrGeniusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//在此将两按钮设为不平面状显示,颜色和自定义窗体色相同
	m_Button1.SubclassDlgItem(IDC_DIRBROWSER, this);
	m_Button1.SetActiveBgColor(RGB(240,200,90));
	m_Button1.SetInactiveBgColor(RGB(240,200,90));
	m_Button1.SetInactiveFgColor(RGB(0,0,255));
	m_Button1.SetActiveFgColor(RGB(0,255,0));
	m_Button1.SetTrickButton(true); //设此按钮为随鼠标自移动郊果,不让鼠标碰上
//  m_Button1.SetIcon(IDI_CANCEL); 
	m_Button1.SetFlat(false); //不扁平显示 

	m_Button2.SubclassDlgItem(IDC_FILEBROWSER, this);
	m_Button2.SetActiveBgColor(RGB(240,200,90));
	m_Button2.SetInactiveBgColor(RGB(240,200,90));
	m_Button2.SetInactiveFgColor(RGB(0,0,255));
	m_Button2.SetActiveFgColor(RGB(0,255,0));
//	m_Button2.SetIcon(IDI_OK);	
	m_Button2.SetTooltipText(IDS_TOOLTIP_EXIT);
   	m_Button2.SetFlat(false); //不扁平显示
	m_Button2.SetBtnCursor(IDC_HAND);

	SetLogoFont("宋体", 17); //初始化设置显示3D字体名称和大小

	// 下面代码为建立特殊窗体显示（椭圆）
	
//	CRect				cRect;
//	CRgn				wndRgn, rgnTemp;

//	GetWindowRect(&cRect);
//	wndRgn.CreateEllipticRgn(0, 0,cRect.Width(),cRect.Height());
//	SetWindowRgn((HRGN)wndRgn, TRUE);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

//设置字体属性
void CScrGeniusDlg::SetLogoFont(CString Name, int nHeight, int nWeight, BYTE bItalic, BYTE bUnderline)
{
  if(m_fontLogo.m_hObject) m_fontLogo.Detach();

  m_fontLogo.CreateFont(nHeight, 0, 0, 0, nWeight, bItalic, bUnderline,0,0,0,0,0,0, Name);
} // End of SetLogoFont

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CScrGeniusDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{  //在此显示3D文字
	CRect rectDlg;
    GetWindowRect(rectDlg);
    ScreenToClient(rectDlg);
	CPaintDC dc(this);

    CString ShowText="有缘，所以认识你！投缘，所以爱上你！";
	//在此设置用户自定制显示的情话文字信息
	if(pTextDescription->GetTextDescription ()!="")
       ShowText=pTextDescription->GetTextDescription ();
   
    CFont* OldFont = dc.SelectObject(&m_fontLogo);

    CSize sizeText = dc.GetTextExtent(ShowText, strlen(ShowText));

    dc.DrawState(CPoint((rectDlg.Width()-sizeText.cx)/2, 2), 
                 rectDlg.Size(), 
                 ShowText, 
                  DSS_DISABLED, TRUE, 0, (CBrush*)NULL);

    dc.SelectObject(OldFont);

		CDialog::OnPaint();
	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScrGeniusDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CScrGeniusDlg::DestroyWindow() 
{
   CRect rc1;  //对话框动画显示效果
   AnimEffect anim1;

	//在此加入对话框动画关闭效果
    GetWindowRect( rc1 );
	rc1.DeflateRect(2,2);//动态方框显示
    anim1.DrawWireRects(rc1, CRect(rc1.CenterPoint().x-10,rc1.CenterPoint().y-10,rc1.CenterPoint().x+10,rc1.CenterPoint().y+10 ), 10);
//	anim1.Close(rc1); ////对话框动画效果显示关闭 
//  Sleep(100); //延缓100毫秒后关闭

	return CDialog::DestroyWindow();
}

void CScrGeniusDlg::OnCancel() 
{	
    
 // CScrGeniusDlg::ShowWindow (SW_HIDE);

	CDialog::OnCancel();
}

void CScrGeniusDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rc1;  //对话框动画显示效果
   AnimEffect anim1;

	//在此加入对话框动画打开效果
    GetWindowRect( rc1 );
    anim1.Open( rc1 ); //对话框动画效果显示打开
	
}
