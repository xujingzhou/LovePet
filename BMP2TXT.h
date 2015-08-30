#if !defined(AFX_BMP2TXT_H__5BEFA200_C968_11D4_9000_C3A311B28F6A__INCLUDED_)
#define AFX_BMP2TXT_H__5BEFA200_C968_11D4_9000_C3A311B28F6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMP2TXT.h : header file
//
#include "BtnST.h" //平面按钮类
#include "EllepticalButton.h" //椭圆按钮类
#include "GradientProgressCtrl.h" //渐变色进度条类
/////////////////////////////////////////////////////////////////////////////
// CBMP2TXT dialog

class CBMP2TXT : public CDialog
{
// Construction
public:
	CBMP2TXT(CWnd* pParent = NULL);   // standard constructor
    ~CBMP2TXT();

	CString strOpenBMPName;
    CString strSaveTXTName;
	CString	m_DirPathString;
	CString	m_FilePathString;

	LPBITMAPFILEHEADER	m_lpBMFileHeader; //位图文件头
	LPBITMAPINFOHEADER	m_lpBMInfoHeader; //位图信息头

	BOOL Read(CString strBMPName); //读取位图文件
	BOOL ConvertToText(CString strBMPName,CString strTXTName);//转换位图为ASCII
private:
	char *m_pFileBuffer;

//	LPBITMAPFILEHEADER	m_lpBMFileHeader;
//	LPBITMAPINFOHEADER	m_lpBMInfoHeader;
	char				*m_pData;
	LPLOGPALETTE		m_lpLogPalette;
	BOOL m_bSetPalette;

	int m_nBytesPerLine;
// Dialog Data
	//{{AFX_DATA(CBMP2TXT)
	enum { IDD = IDD_BMP2TXT };
	CGradientProgressCtrl	m_ProgressBar;
	//}}AFX_DATA

	CButtonST m_SourceFile,m_DescFile; //定义平面按钮
	//建立椭圆按钮
	CEllepticalButton m_Ok,m_Cancel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMP2TXT)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBMP2TXT)
	afx_msg void OnDirbrowser();
	afx_msg void OnFilebrowser();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMP2TXT_H__5BEFA200_C968_11D4_9000_C3A311B28F6A__INCLUDED_)
