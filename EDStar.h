#if !defined(AFX_EDSTAR_H__C3037040_7DB6_11D5_9008_851E79159E58__INCLUDED_)
#define AFX_EDSTAR_H__C3037040_7DB6_11D5_9008_851E79159E58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EDStar.h : header file
//
//
//功能：通用文件加解密
// 作者：徐景周 　
//日期：2001年7月18日
//
/////////////////////////////////////////////////////////////////////////////
// CEDStar dialog

#include "StaticFilespec.h" //扩展静态框类
#include "FlatEdit.h" //扁平编辑框显示类
#include "BtnST.h" //平面按钮类
#include "EllepticalButton.h" //椭圆按钮类

class CEDStar : public CDialog
{
// Construction
public:
	CEDStar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEDStar)
	enum { IDD = IDD_EDSTAR };
	//设置大陆椭圆按钮
	CEllepticalButton	m_Cancel;
	CEllepticalButton	m_Encrypt;
	CEllepticalButton	m_Decrypt;
	//设置平面按钮
	CButtonST	m_Browse2;
	CButtonST	m_Browse1;
	//设置扩展静态标签
	CStaticFilespec	m_TargetPath1;
	CStaticFilespec	m_SourcePath1;
	//设置平面编辑框
	CFlatEdit	m_Password1;
	CString	m_Password;
	CString	m_SourcePath;
	CString	m_TargetPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEDStar)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CString m_Filename; //对原文件进行备份文件名
	//加密文件算法涵数
    BOOL EncryptFile(CString filename,char *password);
	//解密文件算法涵数
    BOOL DecryptFile(CString filename,char *password);

	// Generated message map functions
	//{{AFX_MSG(CEDStar)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBrowse1();
	afx_msg void OnBrowse2();
	afx_msg void OnEncrypt();
	afx_msg void OnDecrypt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDSTAR_H__C3037040_7DB6_11D5_9008_851E79159E58__INCLUDED_)
