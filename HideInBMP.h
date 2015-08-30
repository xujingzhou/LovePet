#if !defined(AFX_HIDEINBMP_H__ECF114E0_F376_11D4_9004_C763984FA486__INCLUDED_)
#define AFX_HIDEINBMP_H__ECF114E0_F376_11D4_9004_C763984FA486__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HideInBMP.h : header file
//
#include "BtnST.h" //平面按钮类
#include "EllepticalButton.h" //椭圆按钮类
#include "GradientProgressCtrl.h" //渐变色进度条类
/////////////////////////////////////////////////////////////////////////////
// HideInBMP dialog

class HideInBMP : public CDialog
{
// Construction
public:
	HideInBMP(CWnd* pParent = NULL);   // standard constructor
	int Get();   //从位图中分离出文件
	void Put(char byte_buf); //文件隐藏到位图中

	CString strOpenBMPName;
	CString	m_DirPathString;
// Dialog Data
	//{{AFX_DATA(HideInBMP)
	enum { IDD = IDD_HIDEINBMP };
	CGradientProgressCtrl	m_Progress1;
	//}}AFX_DATA

	CButtonST m_SourceFile; //定义平面按钮变量
   //建立椭圆按钮
	CEllepticalButton m_HideFile,m_Ok,m_Cancel;

private:
	unsigned char state;
	int number;
	CFile m_File1;
	CFile m_File2;
	unsigned char byte_buf;
	unsigned char mask[8];
	unsigned char byte_bufs[5];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HideInBMP)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HideInBMP)
	afx_msg void OnDirbrowser();
	afx_msg void OnHideFile();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HIDEINBMP_H__ECF114E0_F376_11D4_9004_C763984FA486__INCLUDED_)
