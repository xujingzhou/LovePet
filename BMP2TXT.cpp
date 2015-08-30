// BMP2TXT.cpp : implementation file
//Modify by 徐景周 2000.10
//功能：位图转换为ASCII文本

#include "stdafx.h"
#include "scrgenius.h"
#include "BMP2TXT.h"
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBMP2TXT dialog


CBMP2TXT::CBMP2TXT(CWnd* pParent /*=NULL*/)
	: CDialog(CBMP2TXT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBMP2TXT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	//初始化各变量值
	m_DirPathString = _T(""); //选择位图文件名变量
	m_FilePathString = _T(""); //另存为文本变量

	m_bSetPalette = TRUE;
    m_lpBMFileHeader = NULL;
    m_lpBMInfoHeader = NULL;
    m_lpLogPalette = NULL;
    m_nBytesPerLine = 0;
    m_pData = NULL;
    m_pFileBuffer = NULL;
}

//析构函数, 释放不为空的数据和调色盘内存空间
CBMP2TXT::~CBMP2TXT()
{
    if (m_lpLogPalette)
    {
        LocalFree(m_lpLogPalette);
    }

    if (m_pFileBuffer)
    {
        LocalFree(m_pFileBuffer);
    }
}

void CBMP2TXT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBMP2TXT)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
	DDX_Text(pDX, IDC_EDIT1, m_DirPathString); //位图选择编辑框变量
	DDX_Text(pDX, IDC_EDIT2, m_FilePathString);//文本保存编辑框变量
	DDX_Control(pDX, IDOK,  m_Ok);
	DDX_Control(pDX, IDCANCEL,  m_Cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBMP2TXT, CDialog)
	//{{AFX_MSG_MAP(CBMP2TXT)
	ON_BN_CLICKED(IDC_DIRBROWSER, OnDirbrowser)
	ON_BN_CLICKED(IDC_FILEBROWSER, OnFilebrowser)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMP2TXT message handlers
//打开位图选择对话框
void CBMP2TXT::OnDirbrowser() 
{
	CFileDialog fileDialog(TRUE,NULL,NULL,NULL,"位图文件(*.bmp)|*.bmp||");

	if (fileDialog.DoModal() == IDOK) {
		strOpenBMPName = fileDialog.GetPathName();
		m_DirPathString = strOpenBMPName;
		UpdateData(FALSE);
	}	
}

//另存为文本对话框
void CBMP2TXT::OnFilebrowser() 
{
  CFileDialog fileDialog(FALSE,"*.txt",NULL,NULL,"文本文件(*.txt)|*.txt||");
	if (fileDialog.DoModal() == IDOK) {
		strSaveTXTName=fileDialog.GetPathName();		
		m_FilePathString = strSaveTXTName;
		
		UpdateData(FALSE);
	}	
}

void CBMP2TXT::OnOK() 
{
  if (strSaveTXTName.IsEmpty() || strOpenBMPName.IsEmpty ())
	{
		AfxMessageBox("\n 抱歉,请选择位图文件名和另存为的文本文件名! ");
		return;
	}
  
  CString strTemp;
    strTemp="文本文件:"+strSaveTXTName+"转换成功!";
	if (!ConvertToText(strOpenBMPName,strSaveTXTName))
	{
		AfxMessageBox("抱歉, 转换失败!");
	}
	else
        AfxMessageBox(strTemp);
	
//	CDialog::OnOK();
}

//读取 BMP文件数据, 各种指针赋值和计算逻辑调色盘
//暂不支持 JPEG 或 PNG格式
BOOL CBMP2TXT::Read(CString strBMPName)
{
    CFile File;
    //按只读方式打开文件
    BOOL bResult = File.Open(strBMPName, CFile::modeRead);
	if (!bResult)
	{
		CString strErrorMessage;
		strErrorMessage = "打开文件:" + strBMPName + "失败 !";
		AfxMessageBox(strErrorMessage);
		return FALSE;
	}

    //取得文件长度
    int nFileLength = File.GetLength();

    //按文件长度申请空间
    m_pFileBuffer = (char *)LocalAlloc(LPTR, nFileLength);
    if (!m_pFileBuffer)
    {
		AfxMessageBox("申请必须的内存空间失败 !");
        return FALSE;
    }

    //读取文件所有数据
    int nBytesRead = File.Read(m_pFileBuffer, nFileLength);
    if (nBytesRead != nFileLength)
    {
		AfxMessageBox("读取文件内容失败 !");
        return FALSE;
    }

    //文件头指针赋值
    m_lpBMFileHeader = (LPBITMAPFILEHEADER)m_pFileBuffer;

    //判断文件类型
    if (m_lpBMFileHeader->bfType != 0x4d42) // 'BM'
    {
		CString strErrorMessage;
		strErrorMessage = "文件:" + strBMPName + "不是有效的位图文件 !";
		AfxMessageBox(strErrorMessage);
        return FALSE;
    }

    //信息头指针赋值
    m_lpBMInfoHeader = (LPBITMAPINFOHEADER)(m_pFileBuffer + sizeof(BITMAPFILEHEADER));
    
    //计算每行占用的字节数 (m_lpBMInfoHeader的biSizeImage成员有时为空不可用)
    //m_nBytesPerLine = m_lpBMInfoHeader->biSizeImage / m_lpBMInfoHeader->biHeight;
    m_nBytesPerLine = m_lpBMInfoHeader->biWidth * m_lpBMInfoHeader->biBitCount / 8;
    if (m_nBytesPerLine % 4 != 0)
        m_nBytesPerLine = (m_nBytesPerLine / 4 + 1) * 4;

    //数据指针赋值
    m_pData = m_pFileBuffer + m_lpBMFileHeader->bfOffBits;

    //为调色盘申请空间
    m_lpLogPalette = (LPLOGPALETTE)LocalAlloc(LPTR, sizeof(LOGPALETTE) + 256 * sizeof(PALETTEENTRY));
    m_lpLogPalette->palVersion = 0x300;

    //判断是否需使用调色盘
    switch (m_lpBMInfoHeader->biBitCount)
    {
    case 0: //JPEG 或 PNG 格式
        m_bSetPalette = FALSE;
        break;
        
    case 1:
        m_lpLogPalette->palNumEntries = 2;
        m_bSetPalette = TRUE;
        break;
        
    case 4:
        m_lpLogPalette->palNumEntries = 16;
        m_bSetPalette = TRUE;
        break;
        
    case 8:
        m_lpLogPalette->palNumEntries = 256;
        m_bSetPalette = TRUE;
        break;
        
    case 16:
    case 24:
    case 32:
        m_bSetPalette = FALSE;
        break;
        
    default:
		AfxMessageBox("文件色彩数不可识别 !");
        return FALSE;
    }
        
    //申请临时空间以处理调色盘
    char *pPalette = m_pFileBuffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    if (!pPalette)
	{
		AfxMessageBox("申请必须的内存空间失败 !");
        return FALSE;
	}
        
    RGBQUAD rgbQuad[256];
    //调色盘赋值
    memcpy(rgbQuad, pPalette, sizeof(PALETTEENTRY) * m_lpLogPalette->palNumEntries);
    for (int i = 0; i < m_lpLogPalette->palNumEntries; i ++)
    {                                               
        m_lpLogPalette->palPalEntry[i].peBlue = rgbQuad[i].rgbBlue;
        m_lpLogPalette->palPalEntry[i].peGreen = rgbQuad[i].rgbGreen;
        m_lpLogPalette->palPalEntry[i].peRed = rgbQuad[i].rgbRed;
        m_lpLogPalette->palPalEntry[i].peFlags = rgbQuad[i].rgbReserved;
    }

    return TRUE;
}


// 将给定 BMP文件转换为文本文件, 仅支持 256色格式和真彩格式
BOOL CBMP2TXT::ConvertToText(CString strBMPName,CString strTXTName)
{
    //字符调色盘
    BYTE CharPalette[16];
    CharPalette[0] = '#';
    CharPalette[1] = 'M';
    CharPalette[2] = '@';
    CharPalette[3] = 'H';
    CharPalette[4] = 'X';
    CharPalette[5] = '$';
    CharPalette[6] = '%';
    CharPalette[7] = '+';
    CharPalette[8] = '/';
    CharPalette[9] = ';';
    CharPalette[10] = ':';
    CharPalette[11] = '=';
    CharPalette[12] = '-';
    CharPalette[13] = ',';
    CharPalette[14] = '.';
    CharPalette[15] = ' ';
    

    CStdioFile TxtFile;

    //创建文本文件
    if (!TxtFile.Open(strTXTName, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
    {
		CString strErrorMessage;
		strErrorMessage = "创建文本文件:" + strTXTName + "失败 !";
		AfxMessageBox(strErrorMessage);
        return FALSE;
    }

    //读取 BMP文件数据
    if (!Read(strBMPName))
    {
        return FALSE;
    }

    //设置渐变色进度条
    m_ProgressBar.SetPos(1);

    //取得 BMP数据指针
    BYTE *pData = (BYTE *)m_pData;
    BYTE *pLine = pData;


    for (int i = m_lpBMInfoHeader->biHeight - 1; i >= 0; i--)
    {
        //计算每行的数据指针
        pLine = pData + i * m_nBytesPerLine;
        CString strLineText;
        for (int j = 0; j < m_lpBMInfoHeader->biWidth; j++)
        {
            int nRed, nGreen, nBlue, nValue;

            //计算当前象素的 RGB三分量的值
            switch (m_lpBMInfoHeader->biBitCount)
            {
            case 24:
                nRed = *pLine++;
                nGreen = *pLine++;
                nBlue = *pLine++;
                break;

            case 8:
                nRed = m_lpLogPalette->palPalEntry[*pLine].peRed;
                nGreen = m_lpLogPalette->palPalEntry[*pLine].peGreen;
                nBlue = m_lpLogPalette->palPalEntry[*pLine].peBlue;
                pLine++;
                break;

            default:
				AfxMessageBox("抱歉, 仅支持 8位和24位位图文件 !");
                return FALSE;
            }

            //计算灰度值
            nValue = (nRed * 30 + nGreen * 59 + nBlue * 11) / 100;

            //转换到文本
            strLineText += CharPalette[nValue / 16];
        }

        //写入文本文件
        TxtFile.WriteString(strLineText);
        TxtFile.WriteString("\n");

		//改变文件转换进度条显示(由大到小)
		if(i<=0)
		  m_ProgressBar.SetPos(10);
		else if((i%(m_lpBMInfoHeader->biHeight/8+1))==0)
		  m_ProgressBar.StepIt();
		  
    }

    TxtFile.Close();

    return TRUE;
}

BOOL CBMP2TXT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   //在此将文件浏览两按钮设为平面状,颜色和自定义窗体色相同
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
	
	 //在此初始化渐变色进度条
	    m_ProgressBar.SetRange32(1,10);
		m_ProgressBar.SetBkColor(RGB(240,200,90));
		m_ProgressBar.ShowPercent(true);
		m_ProgressBar.SetStep(1);		
		m_ProgressBar.SetPos(10);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBMP2TXT::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rc3;  //对话框动画显示效果
   AnimEffect anim3;

	//在此加入对话框动画打开效果
	GetWindowRect(rc3 );
    anim3.Open( rc3 ); //对话框动画效果显示打开

}

BOOL CBMP2TXT::DestroyWindow() 
{
   CRect rc3;  //对话框动画显示效果
   AnimEffect anim3;

	//在此加入对话框动画关闭效果
	GetWindowRect(rc3 );
	rc3.DeflateRect(2,2);//动态方框显示
    anim3.DrawWireRects(rc3, CRect(rc3.CenterPoint().x-10,rc3.CenterPoint().y-10,rc3.CenterPoint().x+10,rc3.CenterPoint().y+10 ), 10);
//	anim3.Close(rc3); ////对话框动画效果显示关闭
//	Sleep(50); //延缓50毫秒后关闭

	return CDialog::DestroyWindow();
}
