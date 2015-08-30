// FileSplit.cpp : implementation file
//
//Modify by 徐景周 ，2001.2.13
//功能：文件分割为多种格式的大小，并将其合并在一起
//
#include "stdafx.h"
#include "scrgenius.h"
#include "FileSplit.h"
#include "DirDialog.h" //目录浏览对话框
#include "AnimEffect.h" //对话框动画效果显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileSplit dialog


CFileSplit::CFileSplit(CWnd* pParent /*=NULL*/)
	: CDialog(CFileSplit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileSplit)
	m_parts = _T("状态显示");
	m_path = _T("");
	m_targetpath = _T("");
	m_split = TRUE;
	//}}AFX_DATA_INIT
}


void CFileSplit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileSplit)
	DDX_Control(pDX, IDC_COMBOX_FILESIZE, m_FileSize);
	DDX_Control(pDX, IDC_PROGRESS, pProgress);
	DDX_Control(pDX, IDC_BUTTON_MERGE, m_buttMerge);
	DDX_Control(pDX, IDCANCEL, m_buttExit);
	DDX_Control(pDX, IDOK, m_buttSplit);
	DDX_Control(pDX, IDC_BROWSE, m_Browse);
	DDX_Control(pDX, IDC_BROWSE2, m_Browse2);
	DDX_Control(pDX, IDC_RADIO3, m_RadioSplit);
	DDX_Control(pDX, IDC_PATH, m_edit);
	DDX_Text(pDX, IDC_PARTS, m_parts);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_PATH2, m_targetpath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileSplit, CDialog)
	//{{AFX_MSG_MAP(CFileSplit)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_BROWSE2, OnBrowse2)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BUTTON_MERGE, OnButtonMerge)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSplit message handlers

BOOL CFileSplit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_split) 
		m_RadioSplit.SetCheck(1);

	CButton *pButtonMerge = (CButton*)(GetDlgItem(IDC_BUTTON_MERGE));
	pButtonMerge->EnableWindow(FALSE);

	m_path =_T("");
	m_filename =_T("");

	//在此初始化渐变色进度条
	pProgress.SetRange32(1,500);
    pProgress.SetBkColor(RGB(240,200,90));
	pProgress.ShowPercent(true);	
	pProgress.SetPos(500);

	//设置选择文件大小扁平下拉组合框默认初值为第1个
	m_FileSize.SetCurSel(0);

	//在此将文件浏览两按钮设为平面状,颜色和自定义窗体色相同
	m_Browse.SubclassDlgItem(IDC_DIRBROWSER, this);
	m_Browse.SetActiveBgColor(RGB(240,200,90));
	m_Browse.SetInactiveBgColor(RGB(240,200,90));
	m_Browse.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse.SetIcon(IDI_FILEOPEN);
//	m_Browse.SetFlat(FALSE); //不扁平显示
	m_Browse.SetBtnCursor(IDC_HAND); 

	m_Browse2.SubclassDlgItem(IDC_FILEBROWSER, this);
	m_Browse2.SetActiveBgColor(RGB(240,200,90));
	m_Browse2.SetInactiveBgColor(RGB(240,200,90));
	m_Browse2.SetTooltipText(IDS_TOOLTIP_BROWER);
	m_Browse2.SetIcon(IDI_FILEOPEN);
//	m_Browse2.SetFlat(FALSE); //不扁平显示
	m_Browse2.SetBtnCursor(IDC_HAND);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//选择待分割或合并的源文件
void CFileSplit::OnBrowse() 
{
	//设置分割文件时对话框中显示的文件扩展名
	static char BASED_CODE szSplitFilter[] = _T("待分割文件(*.*)|*.*||");
	//设置合并文件时对话框中显示的文件扩展名
	static char BASED_CODE szMergeFilter[] = _T("待合并文件(1_*.*)|1_*.*||");

	CString filter;
	if (!m_split)
		filter = szMergeFilter;
	else 
		filter = szSplitFilter;

	CFileDialog dlg(TRUE,
					NULL, 
					NULL, 
					OFN_HIDEREADONLY	| 
					OFN_OVERWRITEPROMPT | 
					OFN_FILEMUSTEXIST,
					filter,
					0);

	//获取源文件名及路径
	if (dlg.DoModal() == IDOK) {
		m_path = dlg.GetPathName();
		m_filename = dlg.GetFileName();
		UpdateData(FALSE);
	}
	
}

//选择目标文件夹
void CFileSplit::OnBrowse2() 
{
   CDirDialog	dlg;

	if (dlg.DoBrowse(this) == IDOK) {
		//获取目标路径
		m_targetpath = dlg.m_strPath;
		UpdateData(FALSE);
	}	
	
}

//获取用户选择文件的分割大小
UINT  CFileSplit::GetSplitFileSize()
{
  
    switch(m_FileSize.GetCurSel())
	{
	case 0:
		return 1400000;
    case 1:
		return 1200000;
    case 2:
		return 1000000;
	case 3:
		return 800000;
	default:
		return 1400000;
	}
	
}

// 文件合并涵数 
int CFileSplit::MergeMe() 
{
	CWaitCursor wait;
	CFile destFile;
	CFileException ex;

	BYTE buffer[140000];
	DWORD dwRead;
	UINT nCount = 140000;
	UINT newlen = 1400000;
	char buff [20];
	long l = 1;
	CString name;
	CString pref;
	CString newpath;
		
	UpdateData(TRUE);

	//判断文件是否存在
	if (!m_path.IsEmpty()) {
		if (!m_SourceFile.Open(m_path, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
      		::AfxMessageBox(szError);
			m_edit.SetFocus();
			m_edit.SetSel(0, -1);
			return 1;
		}
		//获取文件名
		m_filename = m_path.Right((m_path.GetLength() - m_path.ReverseFind('\\')) - 1);
		//关闭源文件
		m_SourceFile.Close();
	}

	//获取路径名
	newpath = m_path.Left(m_path.GetLength() - m_filename.GetLength());

	if (!m_targetpath.IsEmpty()) {
		//判断合并文件名是否正确
		if (!m_filename.IsEmpty() && m_filename.Left(2) != _T("1_")) {
			::AfxMessageBox(_T("待合并的源文件名不对..."), MB_ICONERROR);
			return 1;
		}
		else if(m_filename.IsEmpty()) {
			MessageBox(_T("请选择待合并的源文件."), _T("文件分割器"), MB_ICONEXCLAMATION);
			return 1;
		}
		//获取新的文件名
		m_filename = m_filename.Right(m_filename.GetLength() - 2);
		//判断选择目录未尾是否已有"\"符
		if(m_targetpath.Right(1)=='\\')
		    m_path = m_targetpath + m_filename;
		else
			m_path = m_targetpath + _T("\\") + m_filename;
		//建立目标文件
		if (!destFile.Open(m_path,  
			CFile::modeWrite		| 
			CFile::shareExclusive	| 
			CFile::typeBinary		| 
			CFile::modeCreate, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			::AfxMessageBox(szError);
			return 0;
		}
	}
	else if(m_path.IsEmpty())
	{//源文件选择为空
		MessageBox(_T("请选择待合并的源文件."), _T("文件分割器"), MB_ICONEXCLAMATION);
		return 1;
	}
	if (m_targetpath.IsEmpty()) {//目标文件选择为空
		MessageBox(_T("请选择合并后要保存到的目标文件夹."), _T("文件分割器"), MB_ICONEXCLAMATION);
		return 1;
	}
	//开始合并
	do {
		//通过动态增加的数字变量来得到分割文件名，并打开它
		pref = _ltoa(l, buff, 10);
		pref += _T("_");
		if (!m_SourceFile.Open(newpath + pref + m_filename,  
			CFile::modeRead		| 
			CFile::shareExclusive	| 
			CFile::typeBinary, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			destFile.Close();
			m_path = _T("");
			m_filename = _T("");
//			pProgress.SetPos(0);
			newpath = _T("");
//			m_parts = _T("");
			UpdateData(FALSE);

			return 0;
		}
		else
			//产生一个新名
			name = _T(newpath + pref + m_filename);
		do  {//写到目标文件中
			dwRead = m_SourceFile.Read(buffer, nCount);
			destFile.Write(buffer, dwRead);
		}while (dwRead > 0);

		m_SourceFile.Close();

		// 设置进度条范围为0到500.
		pProgress.SetRange(0, 500);
		// 设置进度条移动位置
		for (int i = 0; i < 500; i++)
			pProgress.SetPos(i);
		m_parts = _ltoa(l, buff, 10);
		m_parts += _T("个文件已合并");
		UpdateData(FALSE);
		l++;
		UpdateWindow();
	}while (l < 500);
		
	return 0;
}

//文件分割涵数
int CFileSplit::SplitMe() 
{
	CWaitCursor wait;
	// 目标文件
	CFile destFile;
	// 文件出错的信息变量
	CFileException ex;

	DWORD dwRead;
	UINT newlen;
	char buff [20];
	char b [20];
	long l = 1;
	CString name;
	UINT len = 0;
//	CGradientProgressCtrl *pProgress = (CProgressCtrl*) GetDlgItem(IDC_PROGRESS);

	UpdateData(TRUE);

	//获取文件分割后的大小，定义相对应变量数值
    newlen=GetSplitFileSize();
	UINT nCount = newlen/10;
    BYTE buffer[140000];

	//判断是否已选择源文件及是否能打开，并得不到文件大小
	if (!m_path.IsEmpty()) {
		if (!m_SourceFile.Open(m_path, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
      		::AfxMessageBox(szError);
			m_edit.SetFocus();
			m_edit.SetSel(0, -1);
			return 1;
		}
		len = m_SourceFile.GetLength();
	}
	else {
		MessageBox(_T("请选择待分割的源文件."), _T("文件分割器"), MB_ICONEXCLAMATION);
		return 1;
	}
	if (m_targetpath.IsEmpty()) {
		MessageBox(_T("请选择分割后保存到的目标文件夹."), _T("文件分割器"), MB_ICONEXCLAMATION);
		return 1;
	}
	//判断源文件大小是否够分割的大小
	if (len < newlen) {
		CString length = _itoa(len, b, 10);
		MessageBox(_T("文件长度为 " + length + " 字节，不够指定的分割大小， 没有必要再进行分割."), _T("文件分割器"), MB_ICONEXCLAMATION);
		m_SourceFile.Close();
		m_path = _T("");
		m_filename = _T("");
		UpdateData(FALSE);
		return 1;
	}

	//开始分割
	do {
		//动态改变分割后的保存文件名
		name = _ltoa(l, buff, 10);
		name += _T("_");
		CString newpath; 

		//判断选择目录未尾是否已有"\"符
		if(m_targetpath.Right(1)=='\\')
			newpath = m_targetpath;
		else
			newpath = m_targetpath + _T("\\");
		if (!destFile.Open(newpath + name + m_SourceFile.GetFileName(),  
			CFile::modeWrite		| 
			CFile::shareExclusive	| 
			CFile::typeBinary		|
			CFile::modeCreate, &ex)) {
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			::AfxMessageBox(szError);
			m_SourceFile.Close();
			return 1;
		}
		do {
			dwRead = m_SourceFile.Read(buffer, nCount);
			destFile.Write(buffer, dwRead);
		}//当大小小于指定分割大小时继续　
		while (dwRead > 0 && destFile.GetLength() < newlen); 
		destFile.Close();
		// 设置进度条范围
		pProgress.SetRange(0, len /newlen*10);
		// 设置进度条移动位置
		pProgress.SetPos(l);
		m_parts = _ltoa(l , buff, 10);
		m_parts += _T("个文件生成");
		UpdateData(FALSE);
		l++;
		UpdateWindow();
	}
	while (dwRead > 0);
		// 关闭源文件
		m_SourceFile.Close();

	m_path = _T("");
	m_filename = _T("");
	pProgress.SetPos(len /newlen*10);
//	m_parts = _T("");
	UpdateData(FALSE);

	return 0;
}

//开始分割文件
void CFileSplit::OnOk() 
{
	 //分割文件
	int error = SplitMe();

	//判断结果是否成功
	if (!error) 
		MessageBox(_T("文件已被成功分割!"), _T("文件分割器"), MB_ICONEXCLAMATION);
		
	
}

//开始合并文件
void CFileSplit::OnButtonMerge() 
{
	//合并文件
	int error = MergeMe();

	//判断结果是否成功
	if (!error) 
		MessageBox(_T("文件合并成功!"), _T("文件分割器"), MB_ICONEXCLAMATION);
	
	
}

//选中文件合并单选框
void CFileSplit::OnRadio4() 
{
  	CButton *pButtonMerge = (CButton*)(GetDlgItem(IDC_BUTTON_MERGE));
	CButton *pButtonSplit = (CButton*)(GetDlgItem(IDOK));
	
	//当选择合并文件时，置选择文件大小的下位框为无效状态
	m_FileSize.EnableWindow(false); 
	//设置其它按钮的状态,激活合并按钮，挂起分割按钮
	pButtonSplit->EnableWindow(FALSE);
	pButtonMerge->EnableWindow(TRUE);
	m_split = FALSE;
	m_path = _T("");
	UpdateData(FALSE);	
}

//选中文件分割单选框
void CFileSplit::OnRadio3() 
{
	CButton *pButtonMerge = (CButton*)(GetDlgItem(IDC_BUTTON_MERGE));
	CButton *pButtonSplit = (CButton*)(GetDlgItem(IDOK));
	
	//当选择分割文件时，恢复选择文件大小的下位框为有效状态
	m_FileSize.EnableWindow(true); 
	//设置其它按钮的状态,激活分割按钮，挂起合并按钮
	pButtonSplit->EnableWindow(TRUE);
	pButtonMerge->EnableWindow(FALSE);
	m_split = TRUE;
	m_path = _T("");
	UpdateData(FALSE);	
}

BOOL CFileSplit::DestroyWindow() 
{
   CRect rc0;  //对话框动画显示效果
   AnimEffect anim0;

	//在此加入对话框动画关闭效果
	GetWindowRect(rc0);
	rc0.DeflateRect(2,2);//动态方框显示
    anim0.DrawWireRects(rc0, CRect(rc0.CenterPoint().x-10,rc0.CenterPoint().y-10,rc0.CenterPoint().x+10,rc0.CenterPoint().y+10 ), 10);
//	anim0.Close(rc0); ////对话框动画效果显示关闭
//	Sleep(50); //延缓50毫秒后关闭
	
	return CDialog::DestroyWindow();
}

void CFileSplit::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
   CRect rc0;  //对话框动画显示效果
   AnimEffect anim0;

	//在此加入对话框动画打开效果
	GetWindowRect(rc0 );
    anim0.Open( rc0 ); //对话框动画效果显示打开	
}
