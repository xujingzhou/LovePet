// EDStar.cpp : implementation file
//
//功能：通用文件加解密
// 作者：徐景周 　
//日期：2001年7月18日
//

#include "stdafx.h"
#include "scrgenius.h"
#include "EDStar.h"
#include "DirDialog.h" //目录浏览对话框
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEDStar dialog


CEDStar::CEDStar(CWnd* pParent /*=NULL*/)
	: CDialog(CEDStar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEDStar)
	m_Password = _T("");
	m_SourcePath = _T("");
	m_TargetPath = _T("");
	//}}AFX_DATA_INIT
}


void CEDStar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEDStar)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_ENCRYPT, m_Encrypt);
	DDX_Control(pDX, IDC_DECRYPT, m_Decrypt);
	DDX_Control(pDX, IDC_BROWSE2, m_Browse2);
	DDX_Control(pDX, IDC_BROWSE1, m_Browse1);
	DDX_Control(pDX, IDC_PATH2, m_TargetPath1);
	DDX_Control(pDX, IDC_PATH1, m_SourcePath1);
	DDX_Control(pDX, IDC_PASSWORD, m_Password1);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDV_MaxChars(pDX, m_Password, 16);
	DDX_Text(pDX, IDC_PATH1, m_SourcePath);
	DDX_Text(pDX, IDC_PATH2, m_TargetPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEDStar, CDialog)
	//{{AFX_MSG_MAP(CEDStar)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BROWSE1, OnBrowse1)
	ON_BN_CLICKED(IDC_BROWSE2, OnBrowse2)
	ON_BN_CLICKED(IDC_ENCRYPT, OnEncrypt)
	ON_BN_CLICKED(IDC_DECRYPT, OnDecrypt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEDStar message handlers

BOOL CEDStar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    //把两个文件选择路径的静态框，设置内部显示方式为,
	//在显示长路径名时，省略中间路径，只显示两边路径。
    m_SourcePath1.SubclassDlgItem (IDC_PATH1, this);
	m_SourcePath1.SetPath (TRUE);
    m_TargetPath1.SubclassDlgItem (IDC_PATH2, this);
    m_TargetPath1.SetPath (TRUE);	

	 //在此将文件浏览两按钮设为平面状,颜色和自定义窗体色相同
	m_Browse1.SubclassDlgItem(IDC_BROWSE1, this);
	m_Browse1.SetActiveBgColor(RGB(240,200,90));
	m_Browse1.SetInactiveBgColor(RGB(240,200,90));
	m_Browse1.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse1.SetIcon(IDI_FILEOPEN);
	m_Browse1.SetBtnCursor(IDC_HAND); 

	m_Browse2.SubclassDlgItem(IDC_BROWSE2, this);
	m_Browse2.SetActiveBgColor(RGB(240,200,90));
	m_Browse2.SetInactiveBgColor(RGB(240,200,90));
	m_Browse2.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse2.SetIcon(IDI_FILEOPEN);
	m_Browse2.SetBtnCursor(IDC_HAND);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//加密文件算法涵数
BOOL CEDStar::EncryptFile(CString filename,char *password)
{
	SetFileAttributes(filename,FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_NORMAL);
	CFile m_file;
	if(m_file.Open(filename,CFile::modeReadWrite|CFile::typeBinary))
	{
		char m_buffer[3001];		
		m_file.Read(m_buffer,3000);
		//判断文件头八个字符是否为指定字符，
		//是的话，则是加密过的，不用再加密了。
		if(strncmp(m_buffer,"加密之星",8)==0)
		{
			m_file.Close();
			return false;
		}
	
		//取头3000字节按位于密码相与,
		//并将其存于文件的未尾处。
		int m_len=strlen(password);
		for(int i=0;i<3000;i++)
			m_buffer[i]^=password[i%m_len];
		m_len=m_file.GetLength();		
		m_file.SeekToEnd();
		m_file.Write(m_buffer,3000);

        //在加密后的文件开头处设置一特征字符串,
		//并将其它一些重要特征字符于一固定字符
		//串PW相与后,再将其存入文件开头处。
		memset(m_buffer,0,3001);
		strcpy(m_buffer,"加密之星");
		int *data=(int *)&m_buffer[8];
		data[0]=m_len;
		data[1]=strlen(password);
		strcpy((char *)&m_buffer[2*sizeof(int)+8],password);
		int temp=(2*sizeof(int)+8)+strlen(password);
		char * PW="xujingzhou";
		int m_PWlen=strlen(PW);
		for(int j=8;j<= temp;j++)
			m_buffer[j]^=PW[j%m_PWlen];
		m_file.SeekToBegin();
		if(m_len<3000)		
			m_file.Write(m_buffer,m_len);
		else
		    m_file.Write(m_buffer,3000);

		m_file.Close();
		
		return true;
	}
	return false;
}

//解密文件算法涵数
BOOL CEDStar::DecryptFile(CString filename,char *password)
{
	SetFileAttributes(filename,FILE_ATTRIBUTE_ARCHIVE|FILE_ATTRIBUTE_NORMAL);
 
	CFile m_file;
	if(m_file.Open(filename,CFile::modeReadWrite|CFile::typeBinary))
	{
		char m_buffer[3001];		
		memset(m_buffer,0,3001);
		m_file.Read(m_buffer,3000);
		//判断文件头八个字符是否为加密后指定特征字
		//符串，是的话，则是加密过的，可以解密了，
		//否则就不能解密。
		if(strncmp(m_buffer,"加密之星",8)!=0)
		{
			m_file.Close();
			return FALSE;
		}
		//计算出特征字后其它一些重要字节的长度，
        //并将其解密后，与指定密码比较，判断是
        // 否相同。
		int temp=(2*sizeof(int)+8)+strlen(password);
		char * PW="xujingzhou";
		int m_PWlen=strlen(PW);
        for(int j=8;j<=temp;j++)
			m_buffer[j]^=PW[j%m_PWlen];
		int *data=(int *)&m_buffer[8];
		int m_len=data[1];
		if((m_len!=(int)strlen(password)) || (strcmp((char *)&m_buffer[8+2*sizeof(int)],password)!=0))
		{
			m_file.Close();
			return FALSE;
		}

		//从加密后的文件未尾处取出3000字节，
		//将解密后，重写入文件开头处既可。
		m_len=data[0];
		long m_pos=m_file.Seek(-3000,CFile::end);
		int  m_number=m_file.Read(m_buffer,3000);
		int  m_passlen=strlen(password);
		for(int i=0;i<3000;i++)
			m_buffer[i]^=password[i%m_passlen];
		m_file.SeekToBegin();
		m_file.Write(m_buffer,3000);
		m_file.SetLength(m_len);
		m_file.Close();

		return TRUE;
	}
	return FALSE;
}

BOOL CEDStar::DestroyWindow() 
{
	CRect rect2;  //对话框动画显示效果
    AnimEffect animation2;

	//在此加入对话框动画关闭效果
	GetWindowRect(rect2);
	rect2.DeflateRect(2,2);//动态方框显示
    animation2.DrawWireRects(rect2, CRect(rect2.CenterPoint().x-10,rect2.CenterPoint().y-10,rect2.CenterPoint().x+10,rect2.CenterPoint().y+10 ), 10);
 //	animation2.Close(rect2); ////对话框动画效果显示关闭
//	Sleep(50); //延缓50毫秒后关闭
	
	return CDialog::DestroyWindow();
}

void CEDStar::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
    CRect rect2;  //对话框动画显示效果
    AnimEffect animation2;

	//在此加入对话框动画打开效果
	GetWindowRect(rect2 );
    animation2.Open( rect2 ); //对话框动画效果显示打开
	
}

//选择加解密文件的对话框
void CEDStar::OnBrowse1() 
{
    //设置对话框参数并显示
	static char BASED_CODE filter[] = _T("所有文件(*.*)|*.*||");

	CFileDialog dlg(TRUE,
					NULL, 
					NULL, 
					OFN_HIDEREADONLY	| 
					OFN_OVERWRITEPROMPT | 
					OFN_FILEMUSTEXIST,
					filter,
					0);

	//得到加解密的源文件给m_SourcePath
	//并显示它到对话框的相应路径框中　
	if (dlg.DoModal() == IDOK) {
		m_SourcePath = dlg.GetPathName();
		m_Filename=dlg.GetFileName ();
	    UpdateData(FALSE);
	}	
}

//显示选择加解密后保存路径对话框
void CEDStar::OnBrowse2() 
{
    CDirDialog	dlg;
	if (dlg.DoBrowse(this) == IDOK) {
		//得到目标路径给m_TargetPath
		//并显示它到对话框的相应路径框中　
		m_TargetPath = dlg.m_strPath;
		UpdateData(FALSE);
	}
	
}

//进行文件加密操作
void CEDStar::OnEncrypt() 
{
    UpdateData(true); //将对话框中各控件值传给相应变量
	if(m_SourcePath == "" || m_TargetPath == "" || m_Password == "")
	{
		MessageBox("请先选择加密文件和加密后的保存路径，\n然后设置加密密码既可！","提示");
		return ;
	}

	//将源文件复制到要保存的目标路径后，再进行加密操作
	CString tempPath;
    if(m_TargetPath.Right (1)=='\\')
       tempPath=m_TargetPath+m_Filename;
	else
       tempPath=m_TargetPath+'\\'+m_Filename;
	CopyFile(m_SourcePath,tempPath,false);

	if(EncryptFile(tempPath,m_Password.GetBuffer (0)))
	{
		MessageBox("文件加密成功！","提示");
		//清空相应变量值，以免连按加、解密按钮
       m_SourcePath = "";
	   m_Password ="";
       UpdateData(false); //将改变的变量值传给相应控件
	}
	else
	{
		MessageBox("文件加密失败！","提示");
		if(tempPath != m_SourcePath)
		   DeleteFile(tempPath);  //路径不同时，删除已复制文件
	}

}

//进行文件解密操作
void CEDStar::OnDecrypt() 
{
	UpdateData(true); //将对话框中各控件值传给相应变量
    if(m_SourcePath == "" || m_TargetPath == "" || m_Password == "")
	{
		MessageBox("请先选择解密文件和解密后的保存路径，\n然后设置解密密码既可！","提示");
		return ;
	}
    
	//将源文件复制到要保存的目标路径后，再进行解密操作
	CString tempPath;
    if(m_TargetPath.Right (1)=='\\')
       tempPath=m_TargetPath+m_Filename;
	else
       tempPath=m_TargetPath+'\\'+m_Filename;
	CopyFile(m_SourcePath,tempPath,false);

	if(DecryptFile(tempPath,m_Password.GetBuffer (0)))
	{
		MessageBox("文件解密成功！","提示");
		//清空相应变量值，以免连按加、解密按钮
        m_SourcePath ="";
	    m_Password ="";
        UpdateData(false); //将改变的变量值传给相应控件
	}
	else
	{
		MessageBox("文件解密失败！","提示");
		if(tempPath != m_SourcePath)
		   DeleteFile(tempPath);  //路径不同时，删除已复制文件
	}

}

