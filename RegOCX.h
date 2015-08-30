#if !defined(AFX_REGOCX_H__A5FBFEE0_7DAC_11D5_9008_E230DBA088C1__INCLUDED_)
#define AFX_REGOCX_H__A5FBFEE0_7DAC_11D5_9008_E230DBA088C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegOCX.h : header file
// 
//功能：控件注册器的窗体类，在此加入注册，注销控件以及显示桌面等功能
// 作者：徐景周 　
//日期：2001年7月8日
//

/////////////////////////////////////////////////////////////////////////////
// CRegOCX dialog

#include "BtnST.h" //平面按钮类
#include "EllepticalButton.h" //椭圆按钮类

//桌面最大运行窗体数
#define MAXNUM_RUNWINDOW 256

class CRegOCX : public CDialog
{
// Construction
public:
	CRegOCX(CWnd* pParent = NULL);   // standard constructor

	//最小化或还原桌面所有窗体指针数组及数目
	CWnd * m_pWnd[MAXNUM_RUNWINDOW];
    int m_nWndNum;

// Dialog Data
	//{{AFX_DATA(CRegOCX)
	enum { IDD = IDD_REGOCX };
	//设置椭圆按钮
	CEllepticalButton	m_Cancel;
	CEllepticalButton	m_UnRegOCX;
	CEllepticalButton	m_ShowDesktop;
	CEllepticalButton	m_RegOCX;
	//设置平面按钮
	CButtonST	m_Browse;
	CString	m_FileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegOCX)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegOCX)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowdesktop();
	afx_msg void OnRegocx();
	afx_msg void OnUnregocx();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGOCX_H__A5FBFEE0_7DAC_11D5_9008_E230DBA088C1__INCLUDED_)
