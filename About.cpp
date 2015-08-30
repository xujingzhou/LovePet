// About.cpp : implementation file
//
//Modify by 徐景周 2000.9
//功能：帮助对话框实现

#include "stdafx.h"
#include "scrgenius.h"
#include "About.h"
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAbout dialog


CAbout::CAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CAbout::IDD, pParent)
{
	CString str;
	str="序言：\r\n  爱圣为一纯绿色免费软件，您可以任意传播和复制。免费软件，谢谢使用！谢谢支持！~O~ \r\n\r\n功能简介：\r\n  爱圣3.0版，集娱乐、休闲、实用性于一身。功能多多，快快下载一试便知!您想向您心目中的她(他)表达爱意或想讨其欢心吗？那就把全新正式版的爱圣送给她(他)，一切尽在不言中，可以让她(他)打开音箱或戴上耳机来收听情话(注:宠物实时拖动时会变为美女头像)。其中实用功能主要有：宠物实时拖动、系统区图标快捷菜单操作、显示/隐藏宠物、定制自我情话、暂停情话播放、两岸通--简繁字体互转、窃取星号密码、位图转换为文本、文件隐藏到位图中、文件分割与合并、控件注册器(新)、显示/恢复桌面(新)、文件加解密(新)、定时关机(新)、开机自启动(新)、发送邮件、光驱管理、系统管理等等。\r\n\r\n作者后话：\r\n  随着该软件的逐渐升级，爱圣已不在仅局限于娱乐、爱情软件中，而志在向全方位的实用工具软件发展。但是，同样，免费软件也需要用户的积极支持。因此，如果您在使用它后，如果觉得有什么好的意见或建议，请写信给我（jz_x@china.com）。我会在下一个版本中修改或增加其功能，这样，才能保证它会不断得到升级更新，不断成长。否则的话，它只会在沉默中灭亡，这个版本就可能会是其最终版!";
//	str=str+"\r\n  版本";
	//{{AFX_DATA_INIT(CAbout)
	m_edit = _T(str);
	//}}AFX_DATA_INIT
}


void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbout)
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_MAIL, m_Mail); //标签超连接变量
	DDX_Control(pDX, IDOK,  m_Button);	
	DDX_Control(pDX, IDC_HELPFILE,  m_ButtonHelp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbout, CDialog)
	//{{AFX_MSG_MAP(CAbout)
	ON_BN_CLICKED(IDC_HELPFILE, OnHelpfile)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbout message handlers

BOOL CAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();

	  //加入标签的超连
    m_Mail.SetURL(_T("mailto:jz_x@china.com"));
	m_Mail.SetUnderline(FALSE);	
     
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//打开爱圣帮助文件
void CAbout::OnHelpfile() 
{
  HINSTANCE result =ShellExecute(NULL, _T("open"), "lovepet.chm", NULL,NULL, SW_SHOW);
  
  if((UINT)result <= HINSTANCE_ERROR)
    AfxMessageBox("\n 抱歉,帮助文件lovePet.chm不存在或不在当前路径下！");
}


BOOL CAbout::DestroyWindow() 
{

	CRect rc;  //对话框动画显示效果
    AnimEffect anim;	

	//在此加入对话框动画关闭效果
	GetWindowRect(rc );
	rc.DeflateRect(2,2);//动态方框显示
    anim.DrawWireRects(rc, CRect(rc.CenterPoint().x-10,rc.CenterPoint().y-10,rc.CenterPoint().x+10,rc.CenterPoint().y+10 ), 10);
//	anim.Close(rc); ////对话框动画效果显示关闭 
//	Sleep(50); //延缓50毫秒后关闭

	return CDialog::DestroyWindow();
}

void CAbout::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	CRect rc;  //对话框动画显示效果
    AnimEffect anim;
	
    //在此加入对话框动画打开效果
	GetWindowRect(rc );
    anim.Open( rc ); //对话框动画效果显示打开
	
}

void CAbout::OnDestroy() 
{
	CDialog::OnDestroy();
	
}
