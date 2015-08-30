///
//Modify by 徐景周 2000.10
//功能：椭圆按钮显示效果
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "EllepticalButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// prototypes
COLORREF GetColour(double dAngle, COLORREF crBright, COLORREF crDark);

void DrawEllipse(CDC* pDC, CPoint p, LONG lHorizontalRadius,LONG lVerticalRadius, COLORREF crColour, BOOL bDashed = FALSE);
void DrawEllipse(CDC* pDC, CPoint p, LONG lHorizontalRadius, LONG lVerticalRadius,COLORREF crBright, COLORREF crDark);

// Calculate colour for a point at the given angle by performing a linear
// interpolation between the colours crBright and crDark based on the cosine
// of the angle between the light source and the point.
//
// Angles are measured from the +ve x-axis (i.e. (1,0) = 0 degrees, (0,1) = 90 degrees )
// But remember: +y points down!

COLORREF GetColour(double dAngle, COLORREF crBright, COLORREF crDark)
{
#define Rad2Deg	180.0/3.1415 
#define LIGHT_SOURCE_ANGLE	-2.356		// -2.356 radians = -135 degrees, i.e. From top left

	ASSERT(dAngle > -3.1416 && dAngle < 3.1416);
	double dAngleDifference = LIGHT_SOURCE_ANGLE - dAngle;

	if (dAngleDifference < -3.1415) dAngleDifference = 6.293 + dAngleDifference;
	else if (dAngleDifference > 3.1415) dAngleDifference = 6.293 - dAngleDifference;

	double Weight = 0.5*(cos(dAngleDifference)+1.0);

	BYTE Red	 = (BYTE) (Weight*GetRValue(crBright) + (1.0-Weight)*GetRValue(crDark));
	BYTE Green = (BYTE) (Weight*GetGValue(crBright) + (1.0-Weight)*GetGValue(crDark));
	BYTE Blue	= (BYTE) (Weight*GetBValue(crBright) + (1.0-Weight)*GetBValue(crDark));

	//TRACE("LightAngle = %0.0f, Angle = %3.0f, Diff = %3.0f, Weight = %0.2f, RGB %3d,%3d,%3d\n", 
	//		LIGHT_SOURCE_ANGLE*Rad2Deg, dAngle*Rad2Deg, dAngleDifference*Rad2Deg, Weight,Red,Green,Blue);

	return RGB(Red, Green, Blue);
}

void DrawEllipse(CDC* pDC, CPoint p, LONG lHorizontalRadius,LONG lVerticalRadius, COLORREF crColour, BOOL bDashed)
{
	const int nDashLength = 1;
	LONG lXoffset, lYoffset;
	int	nDash = 0;
	BOOL bDashOn = TRUE;

	//Check to see that the coordinates are valid
	ASSERT( (p.x + lHorizontalRadius <= LONG_MAX) && (p.y + lVerticalRadius <= LONG_MAX) );
	ASSERT( (p.x - lHorizontalRadius	>= LONG_MIN) && (p.y - lVerticalRadius	>= LONG_MIN) );

	long d1,d2; 
	long t1,t2,t3,t4,t5,t6,t7,t8,t9; 
	lXoffset = lHorizontalRadius; 
	lYoffset= 0; 
	t1 = lHorizontalRadius*lHorizontalRadius; t2 = 4*t1; t3 = 2*t2;
	t4 = lVerticalRadius*lVerticalRadius; 
	t5 = 4*t4; 
	t6 = 2*t5; 
	t7 = lHorizontalRadius*t5; t8 = 2*t7; 
	t9 = 0; d1 = t2 - t7 + t4; 
	d2 = t1 - t8 + t5;
	
	

	while (d2 < 0) 
	{/* region 1 of ellipse*/
		if (bDashOn) {
			pDC->SetPixelV(p.x + lXoffset, p.y + lYoffset, crColour);
			pDC->SetPixelV(p.x + lXoffset, p.y - lYoffset, crColour);
			pDC->SetPixelV(p.x - lXoffset, p.y + lYoffset, crColour);
			pDC->SetPixelV(p.x - lXoffset, p.y - lYoffset, crColour);
		}

		//Advance the error term and the constant X axis step
	
		 lYoffset++; 
		 t9 = t9 + t3; 
		 if (d1 < 0) { d1 = d1 + t9 + t2; d2 = d2 + t9; }
		 else 
		 { lXoffset --; t8 = t8 - t6; d1 = d1 - t8 + t9 + t2; d2 = d2 - t8 + t9 + t5; 
		 } 
		
		
		if (bDashed && (++nDash == nDashLength)) {
			nDash = 0;
			bDashOn = !bDashOn;
		}

	} 


	do
	{
		if (bDashOn) 
		{
			pDC->SetPixelV(p.x + lXoffset, p.y + lYoffset, crColour);
			pDC->SetPixelV(p.x + lXoffset, p.y - lYoffset, crColour);
			pDC->SetPixelV(p.x - lXoffset, p.y + lYoffset, crColour);
			pDC->SetPixelV(p.x - lXoffset, p.y - lYoffset, crColour);
		}

		lXoffset--;
		t8 = t8 - t6; 
		if (d2 < 0) 
		{ 
			lYoffset ++; t9 = t9 + t3; d2 = d2 - t8 + t5 + t9; 
		}
		else 
			d2 = d2 - t8 + t5; 
		
		if (bDashed && (++nDash == nDashLength)) {
			nDash = 0;
			bDashOn = !bDashOn;
		}

	} while (lXoffset>=0);	//Continue until halfwaX point

} 



void DrawEllipse(CDC* pDC, CPoint p, LONG lHorizontalRadius,LONG lVerticalRadius, COLORREF crBright, COLORREF crDark)
{
	LONG lXoffset, lYoffset;
	double	 dAngle ;
	const double Pi = 3.141592654, 
				 Pi_on_2 = Pi * 0.5,
				 Three_Pi_on_2 = Pi * 1.5;
	COLORREF crColour;

	//Check to see that the coordinates are valid
	ASSERT( (p.x + lHorizontalRadius <= LONG_MAX) && (p.y + lVerticalRadius <= LONG_MAX) );
	ASSERT( (p.x - lHorizontalRadius	>= LONG_MIN) && (p.y - lVerticalRadius	>= LONG_MIN) );

	long d1,d2; 
	long t1,t2,t3,t4,t5,t6,t7,t8,t9; 
	lXoffset = lHorizontalRadius; 
	lYoffset= 0; 
	t1 = lHorizontalRadius*lHorizontalRadius; t2 = 4*t1; t3 = 2*t2;
	t4 = lVerticalRadius*lVerticalRadius; 
	t5 = 4*t4; 
	t6 = 2*t5; 
	t7 = lHorizontalRadius*t5; t8 = 2*t7; 
	t9 = 0; d1 = t2 - t7 + t4; 
	d2 = t1 - t8 + t5;
	
	
	while (d2 < 0) 
	{
		dAngle = atan2(lYoffset, lXoffset);

		//Draw the current pixel, reflected across all eight arcs
		crColour = GetColour(dAngle, crBright, crDark);
		pDC->SetPixelV(p.x + lXoffset, p.y + lYoffset, crColour);

		crColour = GetColour(Pi_on_2 - dAngle, crBright, crDark);
		//pDC->SetPixelV(p.x + lYoffset, p.y + lXoffset, crColour);

		crColour = GetColour(Pi_on_2 + dAngle, crBright, crDark);
//		pDC->SetPixelV(p.x - lYoffset, p.y + lXoffset, crColour);

		crColour = GetColour(Pi - dAngle, crBright, crDark);
		pDC->SetPixelV(p.x - lXoffset, p.y + lYoffset, crColour);

		crColour = GetColour(-Pi + dAngle, crBright, crDark);
		pDC->SetPixelV(p.x - lXoffset, p.y - lYoffset, crColour);

		crColour = GetColour(-Pi_on_2 - dAngle, crBright, crDark);
		//pDC->SetPixelV(p.x - lYoffset, p.y - lXoffset, crColour);

		crColour = GetColour(-Pi_on_2 + dAngle, crBright, crDark);
//		pDC->SetPixelV(p.x + lYoffset, p.y - lXoffset, crColour);

		crColour = GetColour(-dAngle, crBright, crDark);
		pDC->SetPixelV(p.x + lXoffset, p.y - lYoffset, crColour);

		 lYoffset++; 
		 t9 = t9 + t3; 
		 if (d1 < 0) { d1 = d1 + t9 + t2; d2 = d2 + t9; }
		 else 
		 { 
			 lXoffset --; t8 = t8 - t6; d1 = d1 - t8 + t9 + t2; d2 = d2 - t8 + t9 + t5; 
		 } 

	} 


	do {

		dAngle = atan2(lYoffset, lXoffset);

		//Draw the current pixel, reflected across all eight arcs
		crColour = GetColour(dAngle, crBright, crDark);
		pDC->SetPixelV(p.x + lXoffset, p.y + lYoffset, crColour);

		crColour = GetColour(Pi_on_2 - dAngle, crBright, crDark);
		//pDC->SetPixelV(p.x + lYoffset, p.y + lXoffset, crColour);

		crColour = GetColour(Pi_on_2 + dAngle, crBright, crDark);
//		pDC->SetPixelV(p.x - lYoffset, p.y + lXoffset, crColour);

		crColour = GetColour(Pi - dAngle, crBright, crDark);
		pDC->SetPixelV(p.x - lXoffset, p.y + lYoffset, crColour);

		crColour = GetColour(-Pi + dAngle, crBright, crDark);
		pDC->SetPixelV(p.x - lXoffset, p.y - lYoffset, crColour);

		crColour = GetColour(-Pi_on_2 - dAngle, crBright, crDark);
		//pDC->SetPixelV(p.x - lYoffset, p.y - lXoffset, crColour);

		crColour = GetColour(-Pi_on_2 + dAngle, crBright, crDark);
//		pDC->SetPixelV(p.x + lYoffset, p.y - lXoffset, crColour);

		crColour = GetColour(-dAngle, crBright, crDark);
		pDC->SetPixelV(p.x + lXoffset, p.y - lYoffset, crColour);

		

	lXoffset--;
		t8 = t8 - t6; 
		if (d2 < 0) 
		{ 
			lYoffset ++; t9 = t9 + t3; d2 = d2 - t8 + t5 + t9; 
		}
		else 
			d2 = d2 - t8 + t5; 
	

	} while (lXoffset >=0);

} 

/////////////////////////////////////////////////////////////////////////////
// CEllepticalButton

CEllepticalButton::CEllepticalButton()
{
	m_bDrawDashedFocusCircle = TRUE;
}

CEllepticalButton::~CEllepticalButton()
{
	m_rgn.DeleteObject();
}

BEGIN_MESSAGE_MAP(CEllepticalButton, CButton)
	//{{AFX_MSG_MAP(CEllepticalButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEllepticalButton message handlers

void CEllepticalButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();

	m_bDefault=GetStyle() & (DWORD)BS_DEFPUSHBUTTON; //this is necessary as 
	// the default button style gets removed when u make it ownerdrawn

	ModifyStyle(0, BS_OWNERDRAW);


	CRect rect;
	GetClientRect(rect);
	// Resize the window to make it square
	//commented by Raghav
	//rect.bottom = rect.right = min(rect.bottom,rect.right);


	// Get the vital statistics of the window
	m_ptCentre = rect.CenterPoint();
	m_nHorizontalRadius	= rect.right/2-1;
	m_nVerticalRadius	= rect.bottom/2-1;

	// Set the window region so mouse clicks only activate the round section 
	// of the button
	m_rgn.DeleteObject(); 
	SetWindowRgn(NULL, FALSE);
	m_rgn.CreateEllipticRgnIndirect(rect);
	SetWindowRgn(m_rgn, TRUE);

	// Convert client coords to the parents client coords
	ClientToScreen(rect);
	CWnd* pParent = GetParent();
	if (pParent) pParent->ScreenToClient(rect);

	// Resize the window
	MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
}

void CEllepticalButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);
	
	CDC* pDC	 = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;
	UINT state = lpDrawItemStruct->itemState;
	UINT nStyle = GetStyle();
	int nHorizontalRadius = m_nHorizontalRadius;
	int nVerticalRadius = m_nVerticalRadius;
	int nSavedDC = pDC->SaveDC();

	pDC->SelectStockObject(NULL_BRUSH);
	pDC->FillSolidRect(rect, RGB(240,200,90)); //设置按钮的背景色

	// Draw the focus circle around the button either if it has the focus
	//or if it is the default button
	if ((state & ODS_FOCUS || m_bDefault)	&& m_bDrawDashedFocusCircle)
		 DrawEllipse(pDC, m_ptCentre, nHorizontalRadius--, nVerticalRadius--, RGB(0,0,0)); 
	


	// Draw the raised/sunken edges of the button (unless flat)
	if (nStyle & BS_FLAT) {
			DrawEllipse(pDC, m_ptCentre, nHorizontalRadius--, nVerticalRadius--, RGB(0,0,0));
			DrawEllipse(pDC, m_ptCentre, nHorizontalRadius--, nVerticalRadius--, ::GetSysColor(COLOR_3DHIGHLIGHT));
			
	} else {
		if ((state & ODS_SELECTED))	{
			DrawEllipse(pDC, m_ptCentre, nHorizontalRadius--, nVerticalRadius--, 
						 ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DHIGHLIGHT));
			
			DrawEllipse(pDC, m_ptCentre, nHorizontalRadius--, nVerticalRadius--, 
						 ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DLIGHT));
		} else {
			DrawEllipse(pDC, m_ptCentre, nHorizontalRadius--, nVerticalRadius--, 
						 ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));
							//RGB(255,255,255), ::GetSysColor(COLOR_3DDKSHADOW));
			DrawEllipse(pDC, m_ptCentre, nHorizontalRadius--, nVerticalRadius--, 
						 ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
			
		}
	}
	
	// draw the text if there is any
	CString strText;
	GetWindowText(strText);

	if (!strText.IsEmpty())
	{
		CRgn rgn;
		rgn.CreateEllipticRgn(m_ptCentre.x-nHorizontalRadius, m_ptCentre.y-nVerticalRadius, 
								m_ptCentre.x+nHorizontalRadius, m_ptCentre.y+nVerticalRadius);
		pDC->SelectClipRgn(&rgn);

		CSize Extent = pDC->GetTextExtent(strText);
		CPoint pt = CPoint( m_ptCentre.x - Extent.cx/2, m_ptCentre.y - Extent.cy/2 );

		if (state & ODS_SELECTED) pt.Offset(1,1);

		pDC->SetBkMode(TRANSPARENT);

		if (state & ODS_DISABLED)
			pDC->DrawState(pt, Extent, strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		else
			pDC->TextOut(pt.x, pt.y, strText);

		pDC->SelectClipRgn(NULL);
		rgn.DeleteObject();
	}

	// Draw the focus circle on the inside of the button
	if ((state & ODS_FOCUS) && m_bDrawDashedFocusCircle)
		DrawEllipse(pDC, m_ptCentre, nHorizontalRadius-2, nVerticalRadius-2, RGB(0,0,0), TRUE);
		


	pDC->RestoreDC(nSavedDC);
}

