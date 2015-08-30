#if !defined(AFX_GETIP_H__D1215E40_C93D_11D4_9000_E66FE00DB38A__INCLUDED_)
#define AFX_GETIP_H__D1215E40_C93D_11D4_9000_E66FE00DB38A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetIP.h : header file
//

#include "EllepticalButton.h" //椭圆按钮类
/////////////////////////////////////////////////////////////////////////////
// GetIP dialog

class GetIP : public CDialog
{
// Construction
public:
	
	GetIP(CWnd* pParent = NULL);	// standard constructor
    BOOL HookUninstaller();
	HHOOK HookInstaller();
	CStringArray csArray;
// Dialog Data
	//{{AFX_DATA(GetIP)
	enum { IDD = IDD_GetIP };
	CEdit	m_editip;
	//}}AFX_DATA
		
	//建立椭圆按钮
	CEllepticalButton m_Button;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GetIP)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_String;
	CWnd m_Static;
	BOOL m_Capture;
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(GetIP)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETIP_H__D1215E40_C93D_11D4_9000_E66FE00DB38A__INCLUDED_)
