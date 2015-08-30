// Big2GB.cpp : implementation file
//
//Modify by 徐景周 2000.10
//功能：简繁字体互转

#include "stdafx.h"
#include "scrgenius.h"
#include "Big2GB.h"
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Big2GB dialog


Big2GB::Big2GB(CWnd* pParent /*=NULL*/)
	: CDialog(Big2GB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBIG2GBDlg)
	m_SourceFileName = _T("");
	m_DescFileName = _T("");
	m_Result = 1; //默认为简体
	//}}AFX_DATA_INIT
	bBigToGB=TRUE;
	strLine.SetSize(0);
	m_lineNumber=CalLineNumber();
	m_text="\n";
}


void Big2GB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Big2GB)
	DDX_Text(pDX, IDC_EDIT1, m_SourceFileName);
	DDX_Text(pDX, IDC_EDIT2, m_DescFileName);
	DDX_Radio(pDX, IDC_RADIO1, m_Result);
	DDX_Control(pDX, IDOK,  m_Ok);
	DDX_Control(pDX, IDCANCEL,  m_Cancel);
	DDX_Control(pDX, IDC_BUTTON1,  m_View);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Big2GB, CDialog)
	//{{AFX_MSG_MAP(Big2GB)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DIRBROWSER, OnDirbrowser)
	ON_BN_CLICKED(IDC_FILEBROWSER, OnFilebrowser)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Big2GB message handlers

BOOL Big2GB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//在此将文件浏览两按钮设为扁平状,颜色和自定义窗体色相同
	m_SourceFile.SubclassDlgItem(IDC_DIRBROWSER, this);
	m_SourceFile.SetActiveBgColor(RGB(240,200,90));
	m_SourceFile.SetInactiveBgColor(RGB(240,200,90));
	m_SourceFile.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_SourceFile.SetIcon(IDI_FILEOPEN);
//	m_SourceFile.SetFlat(FALSE); //不扁平显示
	m_SourceFile.SetBtnCursor(IDC_HAND); 

	m_DescFile.SubclassDlgItem(IDC_FILEBROWSER, this);
	m_DescFile.SetActiveBgColor(RGB(240,200,90));
	m_DescFile.SetInactiveBgColor(RGB(240,200,90));
	m_DescFile.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_DescFile.SetIcon(IDI_FILEOPEN);
//	m_DescFile.SetFlat(FALSE); //不扁平显示
	m_DescFile.SetBtnCursor(IDC_HAND);


    //locate the resource of "GBTOBIG" 定位资源 "GBTOBIG"
	hRes=FindResource(NULL,"GBTOBIG",RT_RCDATA);//IDR_GBTOBIG
	hGBTable=LoadResource(0,hRes);
	pGBTable=(LPWORD)LockResource(hGBTable);
	if(pGBTable==NULL)
		AfxMessageBox("fail to load GB2BIG Resource");

	//locate the resource of "BIGTOGB" 定位资源 "BIGTOGB"
	hRes=FindResource(NULL,"BIGTOGB",RT_RCDATA);
	hBIGTable=LoadResource(0,hRes);
	pBIGTable=(LPWORD)LockResource(hBIGTable);
	if(pBIGTable==NULL)
		AfxMessageBox("fail to load Big2GB Resource");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Big2GB::OnClose() 
{
	UnlockResource(hGBTable); 
	FreeResource(hGBTable);   	
	UnlockResource(hBIGTable); 
	FreeResource(hBIGTable);  
	
	CDialog::OnClose();
}

//源文件名
void Big2GB::OnDirbrowser() 
{
CFileDialog fileDialog(TRUE,NULL,NULL,NULL,"文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");

	if (fileDialog.DoModal() == IDOK) {
		strOpenTXTName = fileDialog.GetPathName();
		m_SourceFileName = strOpenTXTName;
		UpdateData(FALSE);

        m_text=_T("");
	}		
}
//另存为目标文件名
void Big2GB::OnFilebrowser() 
{
CFileDialog fileDialog( FALSE,"*.txt",NULL,NULL,"文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	if (fileDialog.DoModal() == IDOK) {
		strSaveTXTName=fileDialog.GetPathName();		
		m_DescFileName = strSaveTXTName;
		
		UpdateData(FALSE);
	}		
}

void Big2GB::OnOK() 
{
   if (strSaveTXTName.IsEmpty() || strOpenTXTName.IsEmpty ())
	{
		AfxMessageBox("\n 抱歉,请选择要转换的文件名和转换后另存为的文件名! ");
		return;
	}
   //读文件
    TCHAR	buffer[0xff]="\0";
	CStdioFile	file(m_SourceFileName, CFile::modeRead|CFile::typeBinary);
	DWORD	count = 0;
	DWORD	Length = file.GetLength();
	do 
	{	memset(buffer,'\0',sizeof(buffer));
		count=file.Read(buffer, sizeof(buffer)-1);
		m_text += buffer;
	}
	while (count == sizeof(buffer)-1);

	m_text+="\n";
	DWORD tmp=m_text.GetLength();
	m_lineNumber=CalLineNumber();
	//简繁体转换
    if(m_Result==1)
       BigtoGB();   
    else
       GBtoBig() ;      
	//写入文件
   	try {
			CStdioFile	file(m_DescFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
			m_text="";
			for(long i=0;i<m_lineNumber;i++)
			{
				m_text+=strLine[i];
				m_text+='\r';  //每一行后要加上一换行符，否则会出现乱码现象
				m_text+='\n';
			}
			file.WriteHuge(m_text, m_text.GetLength());
			AfxMessageBox("文件转换成功! ");
		} 
	catch (...)
		{
			return ;
		}

   
//	CDialog::OnOK();
}

CString Big2GB::Range(long start, long end)
{
	return m_text.Mid(start,end-start);
}



CString Big2GB::GetText()
{
	return m_text;
}

int Big2GB::GetTextLength()
{
	return m_text.GetLength();
}

long Big2GB::CalLineNumber()
{
	strLine.SetSize(0);
	CString strTemp;
	BOOL bReturn=FALSE;
	if(!GetTextLength())
		return 0;
	long line_num=0;
	int index=0;
	do
	{
		TCHAR c=m_text.GetAt(index++);
		if(c=='\t')
			strTemp+="        ";
		else if(c!='\n')
			strTemp+=c;
		else
		{	
			bReturn=TRUE;
			line_num++;;
			strLine.Add(strTemp.Left(strTemp.GetLength()-1));
			strTemp="\0";
		}
	}
	while(index<GetTextLength());
	if(!bReturn)
		strLine.Add(strTemp);

	return strLine.GetSize();
}

long Big2GB::GetLineNumber()
{
	return m_lineNumber;
}

void Big2GB::BigtoGB() 
{
	CalLineNumber();

	if(m_text=="\n")
		return;
	for(int Line=0;Line<GetLineNumber();Line++)
	{
		BYTE* SourceSTR;
		SourceSTR=(BYTE* )((LPCTSTR)strLine[Line]);
		int size=strLine[Line].GetLength();
		if(size)
		{	
			BYTE* DestSTR=new BYTE[size+1];
			unsigned i=0;
			do
			{
				// is English 是英文字符
				if(SourceSTR[i]<0xA1||(SourceSTR[i+1]<0x40)) 
				{
					DestSTR[i]=SourceSTR[i];
					i++;
				}
				else //是BIG5的汉字码
				{
					DestSTR[i+1]=HIBYTE(pBIGTable[(SourceSTR[i]-0xA1)*0xBF+SourceSTR[i+1]-0x40]);
					DestSTR[i]=LOBYTE(pBIGTable[(SourceSTR[i]-0xA1)*0xBF+SourceSTR[i+1]-0x40]);
					i+=2;
				}
			}
			while(i<strlen((char*)(SourceSTR)));
            DestSTR[i]='\0';

			strLine[Line]=DestSTR;
			delete DestSTR;
		}
	}
}

void Big2GB::GBtoBig() 
{
	CalLineNumber();

	if(m_text=="\n")
		return;
	for(int Line=0;Line<GetLineNumber();Line++)
	{
		BYTE* SourceSTR;
		SourceSTR=(BYTE* )((LPCTSTR)strLine[Line]);
		int size=strLine[Line].GetLength();
		if(size)
		{	
			BYTE* DestSTR=new BYTE[size+1];
			unsigned i=0;
			do
			{
				// is English 是英文字符
				if(SourceSTR[i]<0xA1||(SourceSTR[i+1]<0xA1)) 
				{
					DestSTR[i]=SourceSTR[i];
					i++;
				}
				else if(SourceSTR[i]>0xA1&&SourceSTR[i]<0xB0) //是GB2312的汉字码
				{
					DestSTR[i+1]=HIBYTE(pGBTable[(SourceSTR[i]-0xA1)*0x5E+SourceSTR[i+1]-0xA1]);
					DestSTR[i]=LOBYTE(pGBTable[(SourceSTR[i]-0xA1)*0x5E+SourceSTR[i+1]-0xA1]);
					i+=2;
				}
				else
				{
					DestSTR[i+1]=HIBYTE(pGBTable[(SourceSTR[i]-0xA7)*0x5E+SourceSTR[i+1]-0xA1]);
					DestSTR[i]=LOBYTE(pGBTable[(SourceSTR[i]-0xA7)*0x5E+SourceSTR[i+1]-0xA1]);
					i+=2;
				}
			}
			while(i<strlen((char*)(SourceSTR)));
	        DestSTR[i]='\0';

			strLine[Line]=DestSTR;
			delete DestSTR;
		}
	}

}
//查看转换后的文件
void Big2GB::OnButton1() 
{
	 if (m_DescFileName.IsEmpty())
	{
		AfxMessageBox("\n 抱歉,没有发现转换后的文件名! ");
		return;
	}
HINSTANCE result =ShellExecute(NULL, _T("open"), m_DescFileName, NULL,NULL, SW_SHOW);	
}

void Big2GB::OnRadio1() 
{
  m_Result=0;
	
}

void Big2GB::OnRadio2() 
{
  m_Result=1;	
}


void Big2GB::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
    CRect rc2;  //对话框动画显示效果
    AnimEffect anim2;

	//在此加入对话框动画打开效果
	GetWindowRect(rc2 );
    anim2.Open( rc2 ); //对话框动画效果显示打开
	
}

BOOL Big2GB::DestroyWindow() 
{
   CRect rc2;  //对话框动画显示效果
   AnimEffect anim2;

	//在此加入对话框动画关闭效果
	GetWindowRect(rc2 );
	rc2.DeflateRect(2,2);//动态方框显示
    anim2.DrawWireRects(rc2, CRect(rc2.CenterPoint().x-10,rc2.CenterPoint().y-10,rc2.CenterPoint().x+10,rc2.CenterPoint().y+10 ), 10);
//	anim2.Close(rc2); ////对话框动画效果显示关闭 
//  Sleep(50); //延缓50毫秒后关闭

	return CDialog::DestroyWindow();
}

void Big2GB::OnDestroy() 
{
	CDialog::OnDestroy();
	
}
