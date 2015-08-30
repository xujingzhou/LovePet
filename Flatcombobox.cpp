// FlatComboBox.cpp : implementation file
//
//功能：扁平组合框显示类
// 修改人：徐景周 　
//日期：2001年7月18日
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FlatComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlatComboBox

CFlatComboBox::CFlatComboBox()
{
	m_bLBtnDown  = false;
}

CFlatComboBox::~CFlatComboBox()
{
}

BEGIN_MESSAGE_MAP(CFlatComboBox, CComboBox)
	//{{AFX_MSG_MAP(CFlatComboBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlatComboBox message handlers

void CFlatComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(1,10,NULL);
	CComboBox::OnMouseMove(nFlags, point);
}

void CFlatComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = true;
	CComboBox::OnLButtonDown(nFlags, point);
}

void CFlatComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = false;
	Invalidate();
	CComboBox::OnLButtonUp(nFlags, point);
}

void CFlatComboBox::OnTimer(UINT nIDEvent) 
{
	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	static bool bPainted = false;

	// OnLButtonDown, show pressed.
	if (m_bLBtnDown==true) {
		KillTimer (1);
		if (bPainted == true) {
			DrawCombo(FC_DRAWPRESSD, ::GetSysColor(COLOR_BTNSHADOW),
				::GetSysColor(COLOR_BTNHIGHLIGHT));
			bPainted = false;
		}
		return;
	}

	// If mouse leaves, show flat.
	if (!rcItem.PtInRect(pt)) {
		KillTimer (1);
		if (bPainted == true) {
			DrawCombo(FC_DRAWNORMAL, ::GetSysColor(COLOR_BTNFACE),
				::GetSysColor(COLOR_BTNFACE));
			bPainted = false;
		}
		return;
	}

	// On mouse over, show raised.
	else {
		if (bPainted == true)
			return;
		else {
			bPainted = true;
			DrawCombo(FC_DRAWRAISED, ::GetSysColor(COLOR_BTNSHADOW),
				::GetSysColor(COLOR_BTNHIGHLIGHT));
		}
	}
	
	CComboBox::OnTimer(nIDEvent);
}

void CFlatComboBox::OnPaint() 
{
	Default();
	DrawCombo(FC_DRAWNORMAL, RGB(240,200,90),RGB(240,200,90));
		//::GetSysColor(COLOR_BTNFACE),::GetSysColor(COLOR_BTNFACE));
}

void CFlatComboBox::DrawCombo(DWORD dwStyle, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	CRect rcItem;
	GetClientRect(&rcItem);
	CDC* pDC = GetDC();
	
	// Cover up dark 3D shadow.
	pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
	rcItem.DeflateRect(1,1);
	
	if (!IsWindowEnabled()) {
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
	}
	
	else {
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNFACE),
			::GetSysColor(COLOR_BTNFACE));
	}

	// Cover up dark 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	rcItem.left = rcItem.right-Offset();
	pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNFACE),
		::GetSysColor(COLOR_BTNFACE));
	
	// Cover up normal 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNFACE),
		::GetSysColor(COLOR_BTNFACE));
	
	if (!IsWindowEnabled()) {
		return;
	}

	switch (dwStyle)
	{
	case FC_DRAWNORMAL:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
		rcItem.left -= 1;
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
		break;

	case FC_DRAWRAISED:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNHIGHLIGHT),
			::GetSysColor(COLOR_BTNSHADOW));
		break;

	case FC_DRAWPRESSD:
		rcItem.top -= 1;
		rcItem.bottom += 1;
		rcItem.OffsetRect(1,1);
		pDC->Draw3dRect(rcItem, ::GetSysColor(COLOR_BTNSHADOW),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
		break;
	}

	ReleaseDC(pDC);
}

int CFlatComboBox::Offset()
{
	// Thanks to Todd Brannam for this suggestion...
	return ::GetSystemMetrics(SM_CXHTHUMB);
}
