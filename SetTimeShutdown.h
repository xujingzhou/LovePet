#if !defined(AFX_SETTIMESHUTDOWN_H__C9072F20_7E12_11D5_9008_F484482AC9AC__INCLUDED_)
#define AFX_SETTIMESHUTDOWN_H__C9072F20_7E12_11D5_9008_F484482AC9AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTimeShutdown.h : header file
// 
//功能：Windows 9x/NT下定时、立即关机,重启计算机,开机程序自启动
// 作者：徐景周 　
//日期：2001年7月20日
//
/////////////////////////////////////////////////////////////////////////////
// CSetTimeShutdown dialog

#include "EllepticalButton.h" //椭圆按钮类

class CSetTimeShutdown : public CDialog
{
// Construction
public:
	CSetTimeShutdown(CWnd* pParent = NULL);   // standard constructor

	bool bSetTime;
// Dialog Data
	//{{AFX_DATA(CSetTimeShutdown)
	enum { IDD = IDD_SETTIME_SHUTDOWN };
	CEllepticalButton	m_ReBoot;
	CEllepticalButton	m_ShutDownOnce;
	CEllepticalButton	m_Cancel;
	CEllepticalButton	m_ShutDown;
	CDateTimeCtrl	m_Timer;
	CString	m_sNowTime;
	CTime	m_Time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetTimeShutdown)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetTimeShutdown)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonShutdown();
	afx_msg void OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShutdown();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnCancel();
	afx_msg void OnReboot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTIMESHUTDOWN_H__C9072F20_7E12_11D5_9008_F484482AC9AC__INCLUDED_)
