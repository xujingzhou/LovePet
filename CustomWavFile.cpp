// CustomWavFile.cpp : implementation file
//
//Modify by 徐景周 2000.10
//功能：自定制情话及文字

#include "stdafx.h"
#include "scrgenius.h"
#include "CustomWavFile.h"
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString strOpenWavName="";  //情话文件名
CString strWavFile=""; //不含路径的情话文件名
bool bOriginWavPlay=true;   //是否播放原始的情话标志位
CString strPath=""; //主执行程序所在的路径
/////////////////////////////////////////////////////////////////////////////
// CCustomWavFile dialog


CCustomWavFile::CCustomWavFile(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomWavFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomWavFile)
	m_TextDescription = _T("");
	m_FilePathString1 = _T("");
	//}}AFX_DATA_INIT
    bOriginWavPlay=true; //是否恢复原始情话文件的初始值为真
}


void CCustomWavFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomWavFile)
	DDX_Control(pDX, IDC_EDIT2, m_TextDescriptionEdit);
	DDX_Text(pDX, IDC_EDIT2, m_TextDescription);
	DDV_MaxChars(pDX, m_TextDescription, 40);
	DDX_Text(pDX, IDC_EDIT1, m_FilePathString1); //情话文件一编辑框变量
    DDX_Control(pDX, IDOK,  m_OK);
	DDX_Control(pDX, IDCANCEL,  m_Cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomWavFile, CDialog)
	//{{AFX_MSG_MAP(CCustomWavFile)
	ON_BN_CLICKED(IDC_FILEBROWSER1, OnFilebrowser1)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomWavFile message handlers

void CCustomWavFile::OnFilebrowser1() 
{
  CFileDialog fileDialog(TRUE,NULL,NULL,NULL,"情话文件(*.wav)|*.wav||");

	if (fileDialog.DoModal() == IDOK) {
		strOpenWavName = fileDialog.GetPathName(); //含路径
		strWavFile=fileDialog.GetFileName(); //不含路径
		m_FilePathString1 = strOpenWavName;
		UpdateData(FALSE);
	}	
}

void CCustomWavFile::OnOK() 
{
  m_TextDescriptionEdit.GetWindowText(m_TextDescription); //赋字符值
   if (strOpenWavName.IsEmpty() && m_TextDescription =="")
	{
		AfxMessageBox("\n 抱歉,情话文件名和文字不能同时为空!\r\n 否则，请按恢复按钮来恢复原始情话. ");
		return;
	}

   bOriginWavPlay=false; //播放定制的情话.


  if(strWavFile!=""||m_TextDescription!="")
  {
   //建一数据文件，保存当前文件名（不含路径），第二次运行程序时自动播放
   CStdioFile TxtFile;

    //创建情话数据文件，保存路径设为主执行程序所在路径
    if (!TxtFile.Open(strPath+ "\\lovepet.dat", CFile::modeCreate | CFile::modeWrite | CFile::typeText))
    {
		CString strErrorMessage;
		strErrorMessage = "创建情话数据文件: "+strPath+"lovepet.dat 失败 !";
		AfxMessageBox(strErrorMessage);
    }
	else
	{
    //写入情话文件名（不含路径，及默认当前路径，好一起发给对方）和文字表达
    if(strWavFile!="" && m_TextDescription!="")
	{
	 TxtFile.WriteString(strWavFile);
	 TxtFile.WriteString ("$$$");
	 TxtFile.WriteString (m_TextDescription);
	}
	else if(m_TextDescription!="")
	{
     TxtFile.WriteString ("$$$");
	 TxtFile.WriteString (m_TextDescription);
	}
	else
	{
     TxtFile.WriteString(strWavFile);
	 TxtFile.WriteString ("$$$");
	}

	TxtFile.Close();
	}
  }

	CDialog::OnOK();
}

//获取选中的情话文件名
CString CCustomWavFile::GetWaveFile()
{
	CString sWaveFile="";
	
	if(strOpenWavName!="")
		sWaveFile=strOpenWavName;

	return sWaveFile;
}

//获取是否播放定制情话文件标志
bool CCustomWavFile::GetOriginPlayFlag ()
{
	bool bOriginWavePlayFlag=true;
	
	if(!bOriginWavPlay)
		bOriginWavePlayFlag=bOriginWavPlay;

	return bOriginWavePlayFlag;

}	

void CCustomWavFile::OnCancel() 
{
  CStdioFile TxtFile;

  //情话数据文件存在，则删除它
  if (TxtFile.Open(strPath+"\\lovepet.dat", CFile::modeRead|CFile::typeText))
    {
	
	TxtFile.Close();
	//在恢复原始情话时，删除自定制情话数据文件
    TxtFile.Remove(strPath+"\\lovepet.dat");
    }
   	

	CDialog::OnCancel();
}

BOOL CCustomWavFile::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //此处获取主程序所在路径,存在全局变量strPath中
	GetModuleFileName(NULL,strPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	strPath.ReleaseBuffer ();
    int nPos;
	nPos=strPath.ReverseFind ('\\');
	strPath=strPath.Left (nPos);

   //在此将文件浏览按钮设为扁平状,颜色和自定义窗体色相同
	m_SourceFile.SubclassDlgItem(IDC_FILEBROWSER1, this);
	m_SourceFile.SetActiveBgColor(RGB(240,200,90));
	m_SourceFile.SetInactiveBgColor(RGB(240,200,90));
	m_SourceFile.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_SourceFile.SetIcon(IDI_FILEOPEN);
//	m_SourceFile.SetFlat(FALSE); //不扁平显示
	m_SourceFile.SetBtnCursor(IDC_HAND); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCustomWavFile::DestroyWindow() 
{
	CRect rc4;  //对话框动画显示效果
    AnimEffect anim4;

   //在此加入对话框动画关闭效果
	GetWindowRect(rc4 );
	rc4.DeflateRect(2,2);//动态方框显示
    anim4.DrawWireRects(rc4, CRect(rc4.CenterPoint().x-10,rc4.CenterPoint().y-10,rc4.CenterPoint().x+10,rc4.CenterPoint().y+10 ), 10);
//	anim4.Close(rc4); ////对话框动画效果显示关闭
//  Sleep(50); //延缓50毫秒后关闭
	
	return CDialog::DestroyWindow();
}

void CCustomWavFile::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rc4;  //对话框动画显示效果
   AnimEffect anim4;


	//在此加入对话框动画打开效果
	GetWindowRect(rc4 );
    anim4.Open( rc4 ); //对话框动画效果显示打开

}
