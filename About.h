#if !defined(AFX_ABOUT_H__50ECEE60_C992_11D4_9000_DDA11EA2727D__INCLUDED_)
#define AFX_ABOUT_H__50ECEE60_C992_11D4_9000_DDA11EA2727D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// About.h : header file
//
#include "EllepticalButton.h" //椭圆按钮类
#include "HyperLink.h" //超链接类
/////////////////////////////////////////////////////////////////////////////
// CAbout dialog

class CAbout : public CDialog
{
// Construction
public:
	CAbout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAbout)
	enum { IDD = IDD_ABOUT };
	CString	m_edit;
	//}}AFX_DATA

	//建立椭圆按钮
	CEllepticalButton m_Button,m_ButtonHelp;
	CHyperLink m_Mail; //在此设置标签的超连接

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbout)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAbout)
	virtual BOOL OnInitDialog();
	afx_msg void OnHelpfile();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUT_H__50ECEE60_C992_11D4_9000_DDA11EA2727D__INCLUDED_)
