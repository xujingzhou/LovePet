//********************************************************************************

/*
Modified by 徐景周 2000.9
功能：动态透明位图显示及WAV资源文件播放
*/

#if !defined(AFX_TRANSPARENTWND_H__INCLUDED_)
#define AFX_TRANSPARENTWND_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "NTray.h"  // 加入系统托盘支持头文件
#include "HyperLink.h" //加入超链接头文件

class CTransparentWnd : public CWnd
{

public:
	CTransparentWnd();
    int xcounter,ycounter;
	bool GetMuteFlag(); //获取是否静音情话标志位
	bool GetShowPetFlag(); //获取是否显示宠物的标志位
	void CreateTransparent(LPCTSTR pTitle, RECT &rect);
	void SetupRegion(CDC *pDC); //透明位图
	void StartBitmap(void);//初始图像显示 
	void Reset(void); //静止时图像显示
	void DoSet(void);  //改变计数器
	void DoWalk(void); //动态图像显示
	void Idle(void);//播放声音时显示的图像
    void SoundPlay(void);	//WAV音频文件播放
	void WinShutdown(UINT ShutdownFlag);  //Windows NT/98下关机，注销涵数
	void WinAnimation(bool ShowFlag); //窗体显示/隐藏的动画效果
	CString GetTextDescription(); //获取用户自定制的文字描述信息
	CBitmap m_bmpDraw;
	int m_iAniSeq;
//	CFont m_fontLogo;
	CPoint m_ptDest;
	int m_iLastDirection;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransparentWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTransparentWnd();

protected:
	HICON			m_hIcon[1];  //系统托盘支持设置变量
	CTrayNotifyIcon m_TrayIcon;

	// Generated message map functions
protected:
	//{{AFX_MSG(CTransparentWnd)
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShow();
	afx_msg void OnRegister();
	afx_msg void OnShutdown();
	afx_msg void OnEject();
	afx_msg void OnExit();
	afx_msg void OnFileSendMail();
	afx_msg void OnMute();
	afx_msg void OnUpdateMute(CCmdUI* pCmdUI);
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg void OnGetip();
	afx_msg void OnBmp2txt();
	afx_msg void OnHelp();
	afx_msg void OnCustomwavfile();
	afx_msg void OnBig2gb();
	afx_msg void OnHideinbmp();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEdstar();
	afx_msg void OnRegocx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
