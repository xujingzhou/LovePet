/*
Modified by 徐景周 2000.12
功能：对话框动画效果显示
*/
//////////////////////////////////////////////////////////////////////

#define _ANIM_INTERNAL_

#include "stdafx.h"
#include "AnimEffect.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define FIXED_SHIFT 16
#define FIXED int

typedef struct tag_MATRIXF {
	FIXED fM11;
	FIXED fM12;
	int	  iM13;
	FIXED fM21;
	FIXED fM22;
	int   iM23;
	int   iM31;
	int   iM32;
	int   iM33;
} MATRIXF;

const FIXED fixed1= 1 << FIXED_SHIFT;
const MATRIXF matrix1 = { fixed1, 0, 0, 0, fixed1, 0, 0, 0, 1 };
const POINT point0 = { 0, 0 };

int		fixedMul(int iMultiplicand, FIXED fMultiplier);
FIXED	fixedDiv(int iNumerator, int iDenominator);
POINT	operator*( const POINT &point, const MATRIXF &matrix);
MATRIXF	operator*( const MATRIXF &matrix1, const MATRIXF &matrix2);
MATRIXF	mix( const MATRIXF &matrix1, const MATRIXF &matrix2, FIXED fMix);
POINT	mix( const POINT &point1, const POINT &point2, FIXED fMix);

MATRIXF scaleMatrix( FIXED scaleX, FIXED scaleY, POINT ptOrg = point0 );
MATRIXF rotateMatrix( FIXED angle, POINT ptOrg = point0 );
MATRIXF offsetMatrix( int offsetX, int offsetY );

BOOL	efSpinFrame( AnimData *pAnimData );
BOOL	efVortexFrames( AnimData *pAnimData );
BOOL	efScatterGatherFrames( AnimData *pAnimData );
BOOL	efSpikeFrames( AnimData *pAnimData );
BOOL	efFireworxFrames( AnimData *pAnimData );



inline POINT operator+(POINT &point1, POINT &point2)
{
	POINT ptResult;
	ptResult.x = point1.x + point2.x;
	ptResult.y = point1.y + point2.y;
	return ptResult;
}

inline POINT operator-(POINT &point1, POINT &point2)
{
	POINT ptResult;
	ptResult.x = point1.x - point2.x;
	ptResult.y = point1.y - point2.y;
	return ptResult;
}

inline MATRIXF& operator *=(MATRIXF &matrix1, const MATRIXF &matrix2)
{
	return matrix1 = matrix1*matrix2;
}


void drawBox( HDC hDC, POINT &ptCenter, POINT &ptRelRightTop );
void drawPoly( HDC hDC, POINT *pPts, DWORD dwPoints );

//画动态变化的方框线
void  AnimEffect::DrawWireRects(LPRECT lprcFrom, LPRECT lprcTo, UINT nMilliSecSpeed)
{
    const int nNumSteps = 10;

	GdiFlush();
    Sleep(50);  // Let the desktop window sort itself out

	// if hwnd is null - "you have the CON".
	HDC hDC = ::GetDC(NULL);

	// Pen size, urmmm not too thick
    HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0,0,0));

	int nMode = ::SetROP2(hDC, R2_NOT);
	HPEN hOldPen = (HPEN) ::SelectObject(hDC, hPen);

	for (int i = 0; i < nNumSteps; i++)
	{
        double dFraction = (double) i / (double) nNumSteps;

        RECT transition;
        transition.left   = lprcFrom->left + (int)((lprcTo->left - lprcFrom->left) * dFraction);
        transition.right  = lprcFrom->right + (int)((lprcTo->right - lprcFrom->right) * dFraction);
        transition.top    = lprcFrom->top + (int)((lprcTo->top - lprcFrom->top) * dFraction);
        transition.bottom = lprcFrom->bottom + (int)((lprcTo->bottom - lprcFrom->bottom) * dFraction);

		POINT pt[5];
		pt[0] = CPoint(transition.left, transition.top);
		pt[1] = CPoint(transition.right,transition.top);
		pt[2] = CPoint(transition.right,transition.bottom);
		pt[3] = CPoint(transition.left, transition.bottom);
		pt[4] = CPoint(transition.left, transition.top);

		// We use Polyline because we can determine our own pen size
		// Draw Sides
		::Polyline(hDC,pt,5);

		GdiFlush();

		Sleep(nMilliSecSpeed);

		// UnDraw Sides
		::Polyline(hDC,pt,5);

		GdiFlush();
	}

	::SetROP2(hDC, nMode);
	::SelectObject(hDC, hOldPen);

	::ReleaseDC(NULL,hDC);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AnimEffect::AnimEffect()
{
	m_hDC = ::GetDC(NULL);
	m_hPen = ::CreatePen( PS_SOLID, 1, RGB(0,0,0) );

	Defaults();
}

AnimEffect::~AnimEffect()
{
	::DeleteObject( m_hPen );
	::ReleaseDC( NULL, m_hDC );
}

void AnimEffect::Defaults()
{
	m_rcScreen = CRect(0,0,GetSystemMetrics(SM_CXFULLSCREEN),GetSystemMetrics(SM_CYFULLSCREEN));
	m_iAfterimages = 1;//6;
	m_iTotalSteps = 20;//50;
	m_iDelay = 1;//5;
	Effect( Random );
}

void AnimEffect::Setup(int iSteps, int iAfterimages, int iDelay)
{
	if (iSteps > 255) iSteps = 255;
	else if (iSteps < 1) iSteps = 1;
	m_iTotalSteps = iSteps;

	if (iAfterimages > 32) iAfterimages = 32;
	else if (iAfterimages < 0) iAfterimages = 0;
	m_iAfterimages = iAfterimages;

	if (iDelay > 100) iDelay = 100;
	else if (iDelay < 0) iDelay = 0;
}

void AnimEffect::Effect( EffectType effect )
{
	switch ( effect ) {
	case Random:
		break;
	case Spin:
		m_iParameter = 360;
		break;
	case Vortex:
		m_iParameter = 180;
		break;
	case ScatterGather:
		m_iParameter = 4;
		break;
	case Spike:
		m_iParameter = 180;
		break;
	case Fireworks:
		m_iParameter = 360;
		break;
	}
	m_Effect = effect;
}

void AnimEffect::ChooseFunc()
{
	bool bRandom = false;

	if (m_Effect == Random) {
		bRandom = true;
		Effect((EffectType)(rand() % 5));
	}

	switch (m_Effect) {
	case Spin:
		m_Func = efSpinFrame;
		break;
	case Vortex:
		m_Func = efVortexFrames;
		break;
	case ScatterGather:
		m_Func = efScatterGatherFrames;
		break;
	case Spike:
		m_Func = efSpikeFrames;
		break;
	case Fireworks:
		m_Func = efFireworxFrames;
		break;
	default:
		m_Func = 0;
	}

	if (bRandom) {
		m_Effect = Random;
	}
}


void AnimEffect::Open( const CRect &rcWnd )
{
	HPEN oldpen;
	int	 oldrop2;
	AnimData ad;

	oldpen = (HPEN)::SelectObject(m_hDC, m_hPen);
	oldrop2= ::SetROP2(m_hDC, R2_NOT);

	ChooseFunc();
	ad.bOpen = TRUE;
	ad.hDC = m_hDC;
	ad.iAfterimages = m_iAfterimages;
	ad.iTotalSteps = m_iTotalSteps;
	ad.rcWnd = rcWnd;
	ad.ptCenter = rcWnd.CenterPoint();
	ad.ptRelRightTop = CPoint( rcWnd.right - ad.ptCenter.x,rcWnd.top - ad.ptCenter.y );
	ad.iParameter = m_iParameter;

	Animate(ad);
	
	::SetROP2(m_hDC, oldrop2);
	::SelectObject(m_hDC, oldpen);
}

void AnimEffect::Close( const CRect &rcWnd )
{
	HPEN oldpen;
	int	 oldrop2;
	AnimData ad;

	oldpen = (HPEN)::SelectObject(m_hDC, m_hPen);
	oldrop2= ::SetROP2(m_hDC, R2_NOT);

	ChooseFunc();
	ad.bOpen = FALSE;
	ad.hDC = m_hDC;
	ad.iAfterimages = m_iAfterimages;
	ad.iTotalSteps = m_iTotalSteps;
	ad.rcWnd = rcWnd;
	ad.ptCenter = rcWnd.CenterPoint();
	ad.ptRelRightTop = CPoint( rcWnd.right - ad.ptCenter.x,rcWnd.top - ad.ptCenter.y );
	ad.iParameter = m_iParameter;

	Animate(ad);
	
	::SetROP2(m_hDC, oldrop2);
	::SelectObject(m_hDC, oldpen);
}

void AnimEffect::Animate(AnimData &animData)
{
	animData.animType = AnimInit;
	m_Func(&animData);

	if (animData.bOpen) {
		for(int frame=0;frame < animData.iTotalSteps+animData.iAfterimages;frame++) {
			// draw 
			if (frame < animData.iTotalSteps) {
				animData.animType = AnimDraw;
				animData.iStep = frame;
				m_Func( &animData );
				::GdiFlush();
			}
			Sleep(m_iDelay);
			// erase
			if (frame >= animData.iAfterimages) {
				animData.animType = AnimErase;
				animData.iStep = frame - animData.iAfterimages;
				m_Func( &animData );
				::GdiFlush();
			}
		}
	} else {
		for(int frame=animData.iTotalSteps+animData.iAfterimages-1;frame >= 0 ;frame--) {
			// draw 
			if (frame >= animData.iAfterimages) {
				animData.animType = AnimDraw;
				animData.iStep = frame - animData.iAfterimages;
				m_Func( &animData );
				::GdiFlush();
			}
			Sleep(m_iDelay);
			// erase
			if (frame < animData.iTotalSteps) {
				animData.animType = AnimErase;
				animData.iStep = frame;
				m_Func( &animData );
				::GdiFlush();
			}
		}
	}

	animData.animType = AnimTerm;
	m_Func(&animData);
}
//////////////////////////////////////////////////////////////////////
// Supp. functions
//////////////////////////////////////////////////////////////////////

int		fixedMul(int iMultiplicand, FIXED fMultiplier)
{
	return MulDiv(iMultiplicand, fMultiplier, 65536);
}

FIXED	fixedDiv(int iNumerator, int iDenominator)
{
	if ( iNumerator == 0 || iDenominator == 0) return 0;

	return MulDiv(65536,iNumerator,iDenominator);
}

POINT	operator*( const POINT &point, const MATRIXF &matrix)
{
	POINT ptResult;
	ptResult.x = fixedMul(point.x, matrix.fM11) + fixedMul(point.y,matrix.fM21) + matrix.iM31;
	ptResult.y = fixedMul(point.x, matrix.fM12) + fixedMul(point.y,matrix.fM22) + matrix.iM32;
	return ptResult;
}

MATRIXF	operator*( const MATRIXF &m1, const MATRIXF &m2)
{
	MATRIXF mtRes;

	mtRes.fM11 = fixedMul(m1.fM11,m2.fM11) + fixedMul(m1.fM12,m2.fM21);
	mtRes.fM12 = fixedMul(m1.fM11,m2.fM12) + fixedMul(m1.fM12,m2.fM22);
	mtRes.iM13 = 0;
	mtRes.fM21 = fixedMul(m1.fM21,m2.fM11) + fixedMul(m1.fM22,m2.fM21);
	mtRes.fM22 = fixedMul(m1.fM21,m2.fM12) + fixedMul(m1.fM22,m2.fM22);
	mtRes.iM23 = 0;
	mtRes.iM31 = fixedMul(m1.iM31,m2.fM11) + fixedMul(m1.iM32,m2.fM21) + m2.iM31;
	mtRes.iM32 = fixedMul(m1.iM31,m2.fM12) + fixedMul(m1.iM32,m2.fM22) + m2.iM32;
	mtRes.iM33 = 1;

	return mtRes;
}

MATRIXF	mix( const MATRIXF &matrix1, const MATRIXF &matrix2, FIXED fMix)
{
	MATRIXF mtRes;

	mtRes.fM11 = fixedMul(matrix1.fM11, fMix) + fixedMul(matrix2.fM11,fixed1-fMix);
	mtRes.fM12 = fixedMul(matrix1.fM12, fMix) + fixedMul(matrix2.fM12,fixed1-fMix);
	mtRes.iM13 = fixedMul(matrix1.iM13, fMix) + fixedMul(matrix2.iM13,fixed1-fMix);
	mtRes.fM21 = fixedMul(matrix1.fM21, fMix) + fixedMul(matrix2.fM21,fixed1-fMix);
	mtRes.fM22 = fixedMul(matrix1.fM22, fMix) + fixedMul(matrix2.fM22,fixed1-fMix);
	mtRes.iM23 = fixedMul(matrix1.iM23, fMix) + fixedMul(matrix2.iM23,fixed1-fMix);
	mtRes.iM31 = fixedMul(matrix1.iM31, fMix) + fixedMul(matrix2.iM31,fixed1-fMix);
	mtRes.iM32 = fixedMul(matrix1.iM32, fMix) + fixedMul(matrix2.iM32,fixed1-fMix);
	mtRes.iM33 = fixedMul(matrix1.iM33, fMix) + fixedMul(matrix2.iM33,fixed1-fMix);

	return mtRes;
}

POINT	mix( const POINT &point1, const POINT &point2, FIXED fMix)
{
	POINT ptRes;

	ptRes.x = fixedMul(point1.x,fMix) + fixedMul(point2.x,fixed1-fMix);
	ptRes.y = fixedMul(point1.y,fMix) + fixedMul(point2.y,fixed1-fMix);

	return ptRes;
}

MATRIXF offsetMatrix( int offsetX, int offsetY )
{
	MATRIXF mRes = matrix1;

	mRes.iM31 = offsetX;
	mRes.iM32 = offsetY;

	return mRes;
}

MATRIXF scaleMatrix( FIXED scaleX, FIXED scaleY, POINT ptOrg)
{
	MATRIXF mRes = matrix1;

	mRes.fM11 = scaleX;
	mRes.fM22 = scaleY;

	return offsetMatrix(-ptOrg.x,-ptOrg.y) * mRes * offsetMatrix(ptOrg.x,ptOrg.y);
}

MATRIXF rotateMatrix( FIXED angle, POINT ptOrg)
{
	MATRIXF mRes = matrix1;

	double dAngle = (angle / 65536.0) * 3.141592654 / 180.0;
	FIXED fCos = (FIXED)(65536.0 * cos( dAngle ));
	FIXED fSin = (FIXED)(65536.0 * sin( dAngle ));

	mRes.fM11 =  fCos;
	mRes.fM21 =  -fSin;
	mRes.fM12 =  fSin;
	mRes.fM22 =  fCos;

	return offsetMatrix(-ptOrg.x,-ptOrg.y) * mRes * offsetMatrix(ptOrg.x,ptOrg.y);
}

/////////////////////////////////////////////////////////
// Effects
/////////////////////////////////////////////////////////
BOOL	efSpinFrame( AnimData *pAnimData )
{
	if (pAnimData->animType == AnimDraw ||
		pAnimData->animType == AnimErase ) {
		POINT ptRect[4];

		ptRect[1].x = ptRect[0].x = pAnimData->ptRelRightTop.x;
		ptRect[1].y = -(ptRect[0].y = pAnimData->ptRelRightTop.y);

		MATRIXF matrix;
		FIXED	fxScale;
		fxScale = fixedDiv(pAnimData->iStep,pAnimData->iTotalSteps);

		matrix = scaleMatrix( fxScale, fxScale ) * 	rotateMatrix( 
			fixedDiv( pAnimData->iParameter * ( pAnimData->iStep - pAnimData->iTotalSteps ), pAnimData->iTotalSteps ) );

		ptRect[0] = ptRect[0] * matrix;
		ptRect[1] = ptRect[1] * matrix;
		ptRect[2] = pAnimData->ptCenter + ptRect[0];
		ptRect[3] = pAnimData->ptCenter + ptRect[1];
		ptRect[0] = pAnimData->ptCenter - ptRect[0];
		ptRect[1] = pAnimData->ptCenter - ptRect[1];

		drawPoly(pAnimData->hDC,ptRect,4);
	}

	return TRUE;
}

BOOL efVortexFrames( AnimData *pAnimData )
{
	switch ( pAnimData->animType ) {
		case AnimInit :
			*(MATRIXF*)pAnimData->bBuffer = rotateMatrix( fixed1 * 72 , pAnimData->ptCenter);
			break;
		case AnimDraw:
		case AnimErase: 
			{
				POINT ptBoxRel;

				ptBoxRel.x = pAnimData->ptRelRightTop.x * pAnimData->iStep / pAnimData->iTotalSteps;
				ptBoxRel.y = pAnimData->ptRelRightTop.y * pAnimData->iStep / pAnimData->iTotalSteps;

				MATRIXF matrix;
				FIXED	fxScale;

				fxScale = fixedDiv( (pAnimData->iTotalSteps - pAnimData->iStep) * 4,
					pAnimData->iTotalSteps * 3 );
				
				matrix = offsetMatrix( pAnimData->ptRelRightTop.x, pAnimData->ptRelRightTop.y ) *
					  scaleMatrix( fxScale, fxScale, pAnimData->ptCenter ) *
					  rotateMatrix( fixedDiv(pAnimData->iParameter * pAnimData->iStep, pAnimData->iTotalSteps) , pAnimData->ptCenter );

				POINT ptBoxCenter;
				ptBoxCenter = pAnimData->ptCenter * matrix;

				for( int iLoop = 0; iLoop < 5; iLoop++) {
					drawBox( pAnimData->hDC, ptBoxCenter, ptBoxRel );
					ptBoxCenter = ptBoxCenter * *(MATRIXF *)pAnimData->bBuffer;
				}

				break;
			}
	}

	return TRUE;
}

BOOL efScatterGatherFrames( AnimData *pAnimData )
{
	if ( pAnimData->animType == AnimDraw ||
		pAnimData->animType == AnimErase ) {
		int iDivisor = pAnimData->iParameter;
		POINT ptBoxRel;

		ptBoxRel.x = pAnimData->ptRelRightTop.x *  pAnimData->iStep / (pAnimData->iTotalSteps * iDivisor);
		ptBoxRel.y = pAnimData->ptRelRightTop.y *  pAnimData->iStep / (pAnimData->iTotalSteps * iDivisor);

		MATRIXF matrix;
		FIXED fxScale;

		fxScale = fixedDiv( pAnimData->iTotalSteps * 3 - pAnimData->iStep * 2, pAnimData->iTotalSteps );
		matrix = scaleMatrix( fxScale, fxScale ) * offsetMatrix( pAnimData->ptCenter.x, pAnimData->ptCenter.y );

		for(int iRow = 0; iRow < iDivisor; iRow++) {
			for( int iCol=0; iCol < iDivisor; iCol++) {
				POINT ptTileCenter;

				ptTileCenter.x = (iRow * 2 - iDivisor + 1) * pAnimData->ptRelRightTop.x / iDivisor;
				ptTileCenter.y = (iCol * 2 - iDivisor + 1) * pAnimData->ptRelRightTop.y / iDivisor;
				ptTileCenter = ptTileCenter * matrix;

				drawBox( pAnimData->hDC, ptTileCenter, ptBoxRel );
			}
		}
	}

	return TRUE;
}

BOOL efSpikeFrames( AnimData *pAnimData )
{
	struct SpikeData {
		POINT ptTriangleEnd[16][3], ptEndCenter[16], ptTriangleCenter[16];
		MATRIXF matrixCircle[16];
	};

	switch (pAnimData->animType) {
	case AnimInit: {
		int xLeft	= pAnimData->rcWnd.left;
		int xRight	= pAnimData->rcWnd.right;
		int yTop	= pAnimData->rcWnd.bottom;
		int yBottom = pAnimData->rcWnd.top;

		POINT *pTriangle = ((SpikeData*)pAnimData->bBuffer)->ptTriangleEnd[0],
			  *pCenter	 = ((SpikeData*)pAnimData->bBuffer)->ptEndCenter;
		
		for( int iNdx = 0; iNdx < 16; iNdx++, pTriangle +=3, pCenter++) {
			pTriangle[0] = pAnimData->ptCenter;

			if (iNdx < 4) {
				pTriangle[1].y = pTriangle[2].y = yTop;
				pTriangle[1].x = (xLeft * (4 - iNdx) + xRight * iNdx) / 4;
				pTriangle[2].x = (xLeft * (3 - iNdx) + xRight * (iNdx+1)) / 4;
			} else if (iNdx < 8) {
				pTriangle[1].x = pTriangle[2].x = xRight;
				pTriangle[1].y = (yTop * (8 - iNdx) + yBottom * (iNdx-4)) / 4;
				pTriangle[2].y = (yTop * (7 - iNdx) + yBottom * (iNdx-3)) / 4;
			} else if (iNdx < 12) {
				pTriangle[1].y = pTriangle[2].y = yBottom;
				pTriangle[1].x = (xRight * (12 - iNdx) + xLeft * (iNdx-8)) / 4;
				pTriangle[2].x = (xRight * (11 - iNdx) + xLeft * (iNdx-7)) / 4;
			} else {
				pTriangle[1].x = pTriangle[2].x = xLeft;
				pTriangle[1].y = (yBottom * (16 - iNdx) + yTop * (iNdx-12)) / 4;
				pTriangle[2].y = (yBottom * (15 - iNdx) + yTop * (iNdx-11)) / 4;
			}

			pCenter->x = (pTriangle[0].x + pTriangle[1].x + pTriangle[2].x) / 3;
			pCenter->y = (pTriangle[0].y + pTriangle[1].y + pTriangle[2].y) / 3;
		}

		pCenter = ((SpikeData*)pAnimData->bBuffer)->ptTriangleCenter;
		POINT ptTrgCenter;

		ptTrgCenter.x = pAnimData->ptCenter.x;
		ptTrgCenter.y = pAnimData->ptCenter.y + (pAnimData->ptRelRightTop.x + pAnimData->ptRelRightTop.y) * 4/5;

		for( iNdx =0;iNdx < 16; iNdx++ ) {
			MATRIXF matrix;

			matrix = rotateMatrix( (33 * fixed1) + (-22 * fixed1) * iNdx, pAnimData->ptCenter );
			pCenter[iNdx] = ptTrgCenter * matrix;

			POINT ptTemp = pCenter[iNdx] - ((SpikeData*)pAnimData->bBuffer)->ptEndCenter[iNdx];
			((SpikeData*)pAnimData->bBuffer)->matrixCircle[iNdx] = offsetMatrix( ptTemp.x, ptTemp.y );
		}

		break;
	}
	case AnimDraw:
	case AnimErase: {
		POINT ptTriangle[3];
		FIXED fixedFactor;

		MATRIXF matrix;
		FIXED fxScale;
		fxScale = fixedDiv(pAnimData->iStep, pAnimData->iTotalSteps);

		if (pAnimData->iStep < pAnimData->iTotalSteps / 2) {
			fixedFactor = (fixed1 - fixedDiv(pAnimData->iStep*2, pAnimData->iTotalSteps)) *
				pAnimData->iParameter;

			for( int iNdx=0;iNdx < 16; iNdx++ ) {
				matrix = scaleMatrix( fxScale, fxScale, ((SpikeData*)pAnimData->bBuffer)->ptEndCenter[iNdx] ) *
					rotateMatrix( fixedFactor, ((SpikeData*)pAnimData->bBuffer)->ptEndCenter[iNdx] );
				matrix = matrix * ((SpikeData*)pAnimData->bBuffer)->matrixCircle[iNdx];

				for(int iAngle=0;iAngle < 3;iAngle++) {
					ptTriangle[iAngle] = 
						((SpikeData*)pAnimData->bBuffer)->ptTriangleEnd[iNdx][iAngle] * matrix;
				}

				drawPoly( pAnimData->hDC, ptTriangle, 3 );
			}
		} else {
			fixedFactor = fixedDiv(pAnimData->iStep*2 - pAnimData->iTotalSteps, pAnimData->iTotalSteps);

			for( int iNdx=0; iNdx < 16; iNdx++) {
				matrix = scaleMatrix( fxScale, fxScale, ((SpikeData*)pAnimData->bBuffer)->ptEndCenter[iNdx]);
				matrix *= mix( matrix1, ((SpikeData*)pAnimData->bBuffer)->matrixCircle[iNdx], fixedFactor );

				for( int iAngle=0; iAngle < 3; iAngle++ ) {
					ptTriangle[iAngle] = 
						((SpikeData*)pAnimData->bBuffer)->ptTriangleEnd[iNdx][iAngle] * matrix;
				}

				drawPoly(pAnimData->hDC, ptTriangle, 3 );
			}
		}


		break;
	}
	}

	return TRUE;
}

BOOL efFireworxFrames( AnimData *pAnimData )
{
	const int NRECT = 10;
	struct FWData {
		POINT ptCenter[NRECT];
	};

	switch ( pAnimData->animType ) {
	case AnimInit: {
		POINT *ptCenter = ((FWData*)pAnimData->bBuffer)->ptCenter;
		POINT ptRectCenter;
		
		ptRectCenter.x = pAnimData->ptCenter.x;
		ptRectCenter.y = pAnimData->ptCenter.y + (pAnimData->ptRelRightTop.x + pAnimData->ptRelRightTop.y) * 5/3;

		for(int iNdx=0;iNdx < NRECT;iNdx++) {
			MATRIXF matrix = rotateMatrix( iNdx * (360 * fixed1 / NRECT), pAnimData->ptCenter );

			ptCenter[iNdx] = ptRectCenter * matrix;
			
		}
		break;
	}
	case AnimDraw:
	case AnimErase: {
		POINT ptTemp;
		FIXED fixedFactor = fixedDiv( pAnimData->iStep, pAnimData->iTotalSteps );

		MATRIXF matrix;
		POINT ptRect[4], ptTmp[4];
		ptRect[0].x = ptRect[3].x = -pAnimData->ptRelRightTop.x;
		ptRect[1].x = ptRect[2].x = pAnimData->ptRelRightTop.x;
		ptRect[0].y = ptRect[1].y = pAnimData->ptRelRightTop.y;
		ptRect[2].y = ptRect[3].y = -pAnimData->ptRelRightTop.y;

		for(int iNdx=0; iNdx < NRECT; iNdx++) {
			matrix = scaleMatrix( fixedFactor, fixedFactor ) *
				rotateMatrix( (fixed1-fixedFactor)*pAnimData->iParameter);

			ptTemp = mix(pAnimData->ptCenter, ((FWData*)pAnimData->bBuffer)->ptCenter[iNdx], fixedFactor);
			matrix = matrix * offsetMatrix( ptTemp.x, ptTemp.y );

			for(int iAngle=0; iAngle < 4; iAngle++) 
				ptTmp[iAngle] = ptRect[iAngle] * matrix;
			
			drawPoly( pAnimData->hDC, ptTmp, 4 );
		}

		break;
	}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////
// Primitives
/////////////////////////////////////////////////////////

void drawBox( HDC hDC, POINT &ptCenter, POINT &ptRelRightTop )
{
	if (ptRelRightTop.x == 0 && ptRelRightTop.y == 0) return;

	POINT ptTemp[4];

	ptTemp[0].x = ptCenter.x - ptRelRightTop.x;
	ptTemp[0].y = ptCenter.y + ptRelRightTop.y;
	ptTemp[1].x = ptCenter.x + ptRelRightTop.x;
	ptTemp[1].y = ptCenter.y + ptRelRightTop.y;
	ptTemp[2].x = ptCenter.x + ptRelRightTop.x;
	ptTemp[2].y = ptCenter.y - ptRelRightTop.y;
	ptTemp[3].x = ptCenter.x - ptRelRightTop.x;
	ptTemp[3].y = ptCenter.y - ptRelRightTop.y;

	::MoveToEx( hDC, ptTemp[3].x, ptTemp[3].y , NULL );
	::PolylineTo( hDC, ptTemp, 4 );
}

void drawPoly( HDC hDC, POINT *pPts, DWORD dwPoints )
{
	if (pPts == NULL || dwPoints == 0) return;

	if (pPts[dwPoints-1].x == pPts[0].x &&
		pPts[dwPoints-1].y == pPts[0].y ) return;
	::MoveToEx(hDC, pPts[dwPoints-1].x,pPts[dwPoints-1].y,NULL);
	::PolylineTo( hDC, pPts, dwPoints );
}
