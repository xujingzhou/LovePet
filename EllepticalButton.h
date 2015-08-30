#if !defined(AFX_ROUNDBUTTON_H__5254170E_59CF_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_ROUNDBUTTON_H__5254170E_59CF_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

////
//Modify by 徐景周 2000.10
//功能：椭圆按钮显示效果
//
/////////////////////////////////////////////////////////////////////////////

// CEllepticalButton window

class CEllepticalButton : public CButton
{
// Construction
public:
	CEllepticalButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEllepticalButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEllepticalButton();
  BOOL m_bDefault;//maintains whether this button is the default button or not

	CRgn   m_rgn;
	CPoint m_ptCentre;
	int    m_nHorizontalRadius;
	int    m_nVerticalRadius;
	BOOL   m_bDrawDashedFocusCircle;

	// Generated message map functions
protected:
	//{{AFX_MSG(CEllepticalButton)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUNDBUTTON_H__5254170E_59CF_11D1_ABBA_00A0243D1382__INCLUDED_)
