// FlatComboBox.h : header file
//
//功能：扁平组合框显示类
// 修改人：徐景周 　
//日期：2001年7月18日
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FLATCOMBOBOX_H_INCLUDED)
#define FLATCOMBOBOX_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FC_DRAWNORMAL	0x00000001
#define FC_DRAWRAISED	0x00000002
#define FC_DRAWPRESSD	0x00000004

/////////////////////////////////////////////////////////////////////////////
// CFlatComboBox window

class CFlatComboBox : public CComboBox
{
// Construction
public:
	CFlatComboBox();

// Attributes
public:
	bool		m_bLBtnDown;
	COLORREF	m_clrHilite;
	COLORREF	m_clrShadow;
	COLORREF    m_clrDkShad;
	COLORREF	m_clrButton;

// Operations
public:
	void DrawCombo(DWORD dwStyle, COLORREF clrTopLeft, COLORREF clrBottomRight);
	int Offset();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlatComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlatComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlatComboBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(FLATCOMBOBOX_H_INCLUDED)
