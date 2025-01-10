#include "EXdefault.h"
#include "EFCknl.h"

#include "EFCmem.h"
#include "EFCpixel.h"
#include "EFCimg.h"


// EFCknl.c
EXKNLDATA KnlData;

uint16 g_nCOS[180] = {
0x1000, 0x0FFD, 0x0FF6, 0x0FE9, 0x0FD8, 0x0FC1, 0x0FA6, 0x0F86, 0x0F61, 0x0F37,
0x0F08, 0x0ED5, 0x0E9D, 0x0E61, 0x0E20, 0x0DDB, 0x0D91, 0x0D43, 0x0CF1, 0x0C9B,
0x0C41, 0x0BE3, 0x0B82, 0x0B1D, 0x0AB4, 0x0A48, 0x09D9, 0x0967, 0x08F2, 0x087A,
0x0800, 0x0782, 0x0703, 0x0681, 0x05FE, 0x0578, 0x04F1, 0x0469, 0x03DE, 0x0353,
0x02C7, 0x023A, 0x01AC, 0x011D, 0x008E, 0x0000, 0xFF72, 0xFEE3, 0xFE54, 0xFDC6,
0xFD39, 0xFCAD, 0xFC22, 0xFB97, 0xFB0F, 0xFA88, 0xFA02, 0xF97F, 0xF8FD, 0xF87E,
0xF801, 0xF786, 0xF70E, 0xF699, 0xF627, 0xF5B8, 0xF54C, 0xF4E3, 0xF47E, 0xF41D,
0xF3BF, 0xF365, 0xF30F, 0xF2BD, 0xF26F, 0xF225, 0xF1E0, 0xF19F, 0xF163, 0xF12B,
0xF0F8, 0xF0C9, 0xF09F, 0xF07A, 0xF05A, 0xF03F, 0xF028, 0xF017, 0xF00A, 0xF003,
0xF000, 0xF003, 0xF00A, 0xF017, 0xF028, 0xF03F, 0xF05A, 0xF07A, 0xF09F, 0xF0C9,
0xF0F8, 0xF12B, 0xF163, 0xF19F, 0xF1E0, 0xF225, 0xF26F, 0xF2BD, 0xF30F, 0xF365,
0xF3BF, 0xF41D, 0xF47E, 0xF4E3, 0xF54C, 0xF5B8, 0xF627, 0xF699, 0xF70E, 0xF786,
0xF800, 0xF87E, 0xF8FD, 0xF97F, 0xFA02, 0xFA88, 0xFB0F, 0xFB97, 0xFC22, 0xFCAD,
0xFD39, 0xFDC6, 0xFE54, 0xFEE3, 0xFF72, 0x0000, 0x008E, 0x011D, 0x01AC, 0x023A,
0x02C7, 0x0353, 0x03DE, 0x0469, 0x04F1, 0x0578, 0x05FE, 0x0681, 0x0703, 0x0782,
0x07FF, 0x087A, 0x08F2, 0x0967, 0x09D9, 0x0A48, 0x0AB4, 0x0B1D, 0x0B82, 0x0BE3,
0x0C41, 0x0C9B, 0x0CF1, 0x0D43, 0x0D91, 0x0DDB, 0x0E20, 0x0E61, 0x0E9D, 0x0ED5,
0x0F08, 0x0F37, 0x0F61, 0x0F86, 0x0FA6, 0x0FC1, 0x0FD8, 0x0FE9, 0x0FF6, 0x0FFD };

void EFC_knlInitialize( void )
{
	MEMSET( &KnlData, 0, sizeof(KnlData) );

	KnlData.isFirst = TRUE;

#if defined( LGT_WIPI ) // 2.0 초기 버전 콜 수신 안되는 경우 
	MC_knlSetSystemProperty( "CALL_REJECT", "0" );
	MC_knlSetSystemProperty( "NO_MESSAGE", "0" );
#endif

	KnlData.nRand = MC_knlCurrentTime();

	EFC_miscBackLight( TRUE );
}

void EFC_knlFinalize( void )
{
	EFC_miscBackLight( FALSE );
}

sint32 EFC_knlGetRAND( void )
{
	return (sint32)(((KnlData.nRand = (KnlData.nRand * 214013L + 2531011L)) >> 16) & 0x00007FFF); 
}

void EFC_knlEXIT( void )
{
	//EFC_mainFinalize();
	MC_knlExit( 27 );
}

sint32 EFC_knlTouchEvent( LPEXPOINT pPOINT, sint32 type, sint32 param1, sint32 param2 )
{
	return -1;
}

sint32 EFC_knlGetCRC( LPEXBUFF pBuff, sint32 Offset, sint32 Size )
{
	sint32 value = 0x1;
	sint32 s1 = 1;
	sint32 s2 = 0;
	sint32 k;
	ubyte *bData;

	bData = EFC_memGET( pBuff );

	while( Size > 0 ) {
		k = (Size < 5552) ? Size : 5552;
		Size -= k;

		while( --k >= 0 ) {
			s1 += (bData[Offset] & 0x000000ff);
			s2 += s1;
			Offset++;
		}

		s1 %= 65521;
		s2 %= 65521;
	}

	value = ((s2 << 16) | s1);
	return value;
}

void EFC_miscBackLight( ubool bON )
{
	if( bON == TRUE ) {
	//	MC_miscBackLight( 0, MC_LIGHT_ALWAYS_ON, 0, 2000 );
	} else {
	//	MC_miscBackLight( 0, MC_LIGHT_DEFAULT, 0, 0 );
	}
}

ubool EFC_rgnIsInRECT( LPEXRECT pRect, sint32 nX, sint32 nY )
{
	sint32 nL, nT, nR, nB;

	nL = pRect->nX;
	nT = pRect->nY;
	nR = pRect->nX + pRect->nW - 1;
	nB = pRect->nY + pRect->nH - 1;

	if( (nX < nL) || (nX > nR) || (nY < nT) || (nY > nB) ) {
		return FALSE;
	}

	return TRUE;
}

void EFC_rgnSetPOINT( LPEXPOINT pPT, sint32 nX, sint32 nY )
{
	pPT->nX = (sint16)nX;
	pPT->nY = (sint16)nY;
}

void EFC_rgnRealPOINT( LPEXPOINT pPT, sint32 nX, sint32 nY )
{
	if( EFC_grpIsMAIN( PixelData.pFrame ) == TRUE ) {
		EFC_rgnSetPOINT( pPT,
			nX + PixelData.ptTRANS.nX,
			nY + PixelData.ptTRANS.nY + PixelData.nANNUN );
	} else {
		EFC_rgnSetPOINT( pPT, nX, nY );
	}
}

ubool EFC_rgnSetRECT( LPEXRECT pRect, sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	if( (nW > 0) && (nH > 0) ) {
		pRect->nX = (sint16)nX;
		pRect->nY = (sint16)nY;
		pRect->nW = (sint16)nW;
		pRect->nH = (sint16)nH;
		return TRUE;
	}

	MEMSET( pRect, 0, sizeof(EXRECT) );
	return FALSE;
}

ubool EFC_rgnInterRECT( LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2 )
{
	sint32 n1L, n1T, n1R, n1B;
	sint32 n2L, n2T, n2R, n2B;

	n1L = pRect1->nX;
	n1T = pRect1->nY;
	n1R = pRect1->nX + pRect1->nW - 1;
	n1B = pRect1->nY + pRect1->nH - 1;

	n2L = pRect2->nX;
	n2T = pRect2->nY;
	n2R = pRect2->nX + pRect2->nW - 1;
	n2B = pRect2->nY + pRect2->nH - 1;

	// CALC
	n1L = MAX_CALC( n1L, n2L );
	n1T = MAX_CALC( n1T, n2T );
	n1R = MIN_CALC( n1R, n2R );
	n1B = MIN_CALC( n1B, n2B );

	return EFC_rgnSetRECT( pRect, n1L, n1T, n1R - n1L + 1, n1B - n1T + 1 );
}

ubool EFC_rgnUnionRECT( LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2 )
{
	sint32 n1L, n1T, n1R, n1B;
	sint32 n2L, n2T, n2R, n2B;

	if( (pRect1->nW < 1) || (pRect1->nH < 1) ) {
		return EFC_rgnSetRECT( pRect, pRect2->nX, pRect2->nY, pRect2->nW, pRect2->nH );
	}

	if( (pRect2->nW < 1) && (pRect2->nH < 1) ) {
		return EFC_rgnSetRECT( pRect, pRect1->nX, pRect1->nY, pRect1->nW, pRect1->nH );
	}

	n1L = pRect1->nX;
	n1T = pRect1->nY;
	n1R = pRect1->nX + pRect1->nW - 1;
	n1B = pRect1->nY + pRect1->nH - 1;

	n2L = pRect2->nX;
	n2T = pRect2->nY;
	n2R = pRect2->nX + pRect2->nW - 1;
	n2B = pRect2->nY + pRect2->nH - 1;

	// CALC
	n1L = MIN_CALC( n1L, n2L );
	n1T = MIN_CALC( n1T, n2T );
	n1R = MAX_CALC( n1R, n2R );
	n1B = MAX_CALC( n1B, n2B );

	return EFC_rgnSetRECT( pRect, n1L, n1T, n1R - n1L + 1, n1B - n1T + 1 );
}

ubool EFC_rgnPlusRECT( LPEXRECT pRect, LPEXRECT pRect1, sint32 nX, sint32 nY )
{
	sint32 n1L, n1T, n1R, n1B;

	n1L = pRect1->nX;
	n1T = pRect1->nY;
	n1R = pRect1->nX + pRect1->nW - 1;
	n1B = pRect1->nY + pRect1->nH - 1;

	// CALC
	n1L = MIN_CALC( n1L, nX );
	n1T = MIN_CALC( n1T, nY );
	n1R = MAX_CALC( n1R, nX );
	n1B = MAX_CALC( n1B, nY );

	return EFC_rgnSetRECT( pRect, n1L, n1T, n1R - n1L + 1, n1B - n1T + 1 );
}

ubool EFC_rgnRealRECT( LPEXRECT pRect, LPEXRECT pRect1 )
{
	EXREGION rgn;
	EFC_rgnSetREGION( &rgn, pRect1->nX, pRect1->nY, pRect1->nW, pRect1->nH, 0, 0 );
	EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE );
	return EFC_rgnGetRECT( pRect, &rgn );
}

ubool EFC_rgnSetREGION( LPEXREGION pRgn, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy )
{
	if( (nW > 0) && (nH > 0) ) {
		pRgn->nX = (sint16)nX;
		pRgn->nY = (sint16)nY;
		pRgn->nW = (sint16)nW;
		pRgn->nH = (sint16)nH;
		pRgn->nSx = (sint16)nSx;
		pRgn->nSy = (sint16)nSy;
		return TRUE;
	}

	MEMSET( pRgn, 0, sizeof(EXREGION) );
	return FALSE;
}

ubool EFC_rgnInterREGION( LPEXREGION pRgn, LPEXRECT pRect, LPEXREGION pRegion )
{
	sint32 nBackL, nBackT, nBackR, nBackB;
	sint32 nL, nT, nR, nB, nSx, nSy;

	// 드로잉 영역
	nBackL = pRect->nX;
	nBackT = pRect->nY;
	nBackR = pRect->nX + pRect->nW - 1;
	nBackB = pRect->nY + pRect->nH - 1;

	// 이미지 영역
	nL = pRegion->nX;
	nT = pRegion->nY;
	nR = pRegion->nX + pRegion->nW - 1;
	nB = pRegion->nY + pRegion->nH - 1;
	nSx = pRegion->nSx;
	nSy = pRegion->nSy;

	// CALC
	nSx = MAX_CALC( (nSx + nBackL - nL), nSx );
	nL = MAX_CALC( nL, nBackL );
	nSy = MAX_CALC( (nSy + nBackT - nT), nSy );
	nT = MAX_CALC( nT, nBackT );
	nR = MIN_CALC( nR, nBackR );
	nB = MIN_CALC( nB, nBackB );

	return EFC_rgnSetREGION( pRgn, nL, nT, nR - nL + 1, nB - nT + 1, nSx, nSy );
}

void EFC_rgnSetTRANS( LPEXPOINT pPT, sint32 nX, sint32 nY )
{
	if( EFC_grpIsMAIN( PixelData.pFrame ) == TRUE ) {
		EFC_rgnSetPOINT( pPT, nX + PixelData.ptTRANS.nX, nY + PixelData.ptTRANS.nY );
	} else {
		EFC_rgnSetPOINT( pPT, nX, nY );
	}
}

void EFC_rgnSetANNUN( LPEXPOINT pPT, sint32 nX, sint32 nY )
{
	if( EFC_grpIsMAIN( PixelData.pFrame ) == TRUE ) {
		EFC_rgnSetPOINT( pPT, nX, nY + PixelData.nANNUN );
	} else {
		EFC_rgnSetPOINT( pPT, nX, nY );
	}
}

//ubool EFC_rgnRealREGION( LPEXREGION pRgn )
//{
//	EXPOINT pt;
//	EXRECT rt;
//
//	EFC_rgnSetRECT( &rt, 0, 0, 0x00007FFF, 0x00007FFF );
//	EFC_rgnInterRECT( &rt, &PixelData.rtCLIP, &rt );
//
//	EFC_rgnSetTRANS( &pt, 0, 0 );
//	EFC_rgnSetREGION( pRgn, pRgn->nX + pt.nX, pRgn->nY + pt.nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy );
//	EFC_rgnInterREGION( pRgn, &rt, pRgn );
//
//	EFC_rgnSetANNUN( &pt, 0, 0 );
//	return EFC_rgnSetREGION( pRgn, pRgn->nX + pt.nX, pRgn->nY + pt.nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy );
//}

ubool EFC_rgnRealREGION( LPEXREGION pRgn, sint32 nMIRROR )
{
	EXREGION rgn;

	pRgn->nX = (sint16)(pRgn->nX + PixelData.ptTRANS.nX);
	pRgn->nY = (sint16)(pRgn->nY + PixelData.ptTRANS.nY);

	switch( nMIRROR ) {
	case EN_MIRROR_NONE :
		EFC_rgnInterREGION( pRgn, &PixelData.rtCLIP, pRgn );
		break;
	case EN_MIRROR_HORIZON :
		EFC_rgnSetREGION( &rgn, pRgn->nX, pRgn->nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy );
		EFC_rgnInterREGION( pRgn, &PixelData.rtCLIP, pRgn );
		if( (rgn.nX + rgn.nW) < (PixelData.rtCLIP.nX + PixelData.rtCLIP.nW) ) {
			pRgn->nSx = rgn.nSx;
		} else {
			pRgn->nSx = (sint16)MAX_CALC( 0, ((rgn.nX + rgn.nW) - (PixelData.rtCLIP.nX + PixelData.rtCLIP.nW)) );
		}
		break;
	case EN_MIRROR_VERTICAL :
		EFC_rgnSetREGION( &rgn, pRgn->nX, pRgn->nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy );
		EFC_rgnInterREGION( pRgn, &PixelData.rtCLIP, pRgn );
		if( (rgn.nY + rgn.nH) < (PixelData.rtCLIP.nY + PixelData.rtCLIP.nH) ) {
			pRgn->nSy = rgn.nSy;
		} else {
			pRgn->nSy = (sint16)MAX_CALC( 0, ((rgn.nY + rgn.nH) - (PixelData.rtCLIP.nY + PixelData.rtCLIP.nH)) );
		}
		break;
	case EN_MIRROR_BOTH :
		EFC_rgnSetREGION( &rgn, pRgn->nX, pRgn->nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy );
		EFC_rgnInterREGION( pRgn, &PixelData.rtCLIP, pRgn );
		if( (rgn.nX + rgn.nW) < (PixelData.rtCLIP.nX + PixelData.rtCLIP.nW) ) {
			pRgn->nSx = rgn.nSx;
		} else {
			pRgn->nSx = (sint16)MAX_CALC( 0, ((rgn.nX + rgn.nW) - (PixelData.rtCLIP.nX + PixelData.rtCLIP.nW)) );
		}
		if( (rgn.nY + rgn.nH) < (PixelData.rtCLIP.nY + PixelData.rtCLIP.nH) ) {
			pRgn->nSy = rgn.nSy;
		} else {
			pRgn->nSy = (sint16)MAX_CALC( 0, ((rgn.nY + rgn.nH) - (PixelData.rtCLIP.nY + PixelData.rtCLIP.nH)) );
		}
		break;
	}

	if( (pRgn->nW < 1) || (pRgn->nH < 1) ) {
		return FALSE;
	}

	return TRUE;
}

ubool EFC_rgnGetRECT( LPEXRECT pRect, LPEXREGION pRegion )
{
	return EFC_rgnSetRECT( pRect, pRegion->nX, pRegion->nY, pRegion->nW, pRegion->nH );
}

sint32 EFC_mathiSqrt( uint32 nX )
{
	uint32 m = 0x40000000;
	uint32 r = 0, nr;

	do {
		nr = m + r;
		r >>= 1;

		if( nr <= nX ) {
			nX -= nr;
			r += m;
		}

		m >>= 2;
	} while( m != 0 );

	return ((sint32)r);
}
// 타원 방정식
// x^2 / a^2 + y^2 / b^2 = 1
sint32 EFC_mathCircleX( sint32 nX, sint32 nW, sint32 nH )
{
	return EFC_mathiSqrt( nH * nH * (nW * nW - nX * nX) / (nW * nW) );

//	return EFC_mathiSqrt( (((nW * nH) - (nH * nX)) * ((nW * nH) + (nH * nX))) / (nW * nW) );
}

sint32 EFC_mathCircleY( sint32 nY, sint32 nW, sint32 nH )
{
	return EFC_mathiSqrt( nW * nW * (nH * nH - nY * nY) / (nH * nH) );

//	return EFC_mathiSqrt( ((nW * nH - nW * nY) * (nW * nH + nW * nY)) / (nH * nH) );
}

sint32 EFC_mathSIN( sint32 nANGLE )
{
	nANGLE += 270;
	while( nANGLE < 0 ) {
		nANGLE += 360;
	}
	nANGLE %= 360;

	return (sint16)g_nCOS[(nANGLE >> 1)];
}

sint32 EFC_mathCOS( sint32 nANGLE )
{
	while( nANGLE < 0 ) {
		nANGLE += 360;
	}
	nANGLE %= 360;

	return (sint16)g_nCOS[(nANGLE >> 1)];
}

void EFC_mathNORMAL( LPEXPOINT pt, sint32 nDIST )
{
	sint64 nCALC;
	sint32 nX, nY;

	if( pt->nX == 0 ) {
		nX = 0;
		nY = nDIST;
	} else if( pt->nY == 0 ) {
		nX = nDIST;
		nY = 0;
	} else {
		nCALC = ((sint64)nDIST * nDIST * pt->nX * pt->nX) / (((sint64)pt->nX * pt->nX) + ((sint64)pt->nY * pt->nY));
		nX = EFC_mathiSqrt( (sint32)nCALC );

		nCALC = ((sint64)nDIST * nDIST) - (nX * nX);
		nY = EFC_mathiSqrt( (sint32)nCALC );
	}

	if( pt->nX < 0 ) {
		pt->nX = (sint16)-nX;
	} else {
		pt->nX = (sint16)nX;
	}

	if( pt->nY < 0 ) {
		pt->nY = (sint16)-nY;
	} else {
		pt->nY = (sint16)nY;
	}
}

void EFC_mathMOVE( LPEXPOINT ptMOVE, LPEXPOINT ptCURR, LPEXPOINT ptNEXT, LPEXPOINT ptDIRECT, sint32 nMOVE, uint8 nRATE1, uint16 nRATE2 )
{
	EXPOINT ptCALC;

	EFC_rgnSetPOINT( &ptCALC, (sint32)ptNEXT->nX - (sint32)ptCURR->nX, (sint32)ptNEXT->nY - (sint32)ptCURR->nY );

	EFC_mathNORMAL( &ptCALC, (sint32)nRATE1 * 100 );
	EFC_mathNORMAL( ptDIRECT, (sint32)nRATE2 * 100 );

	EFC_rgnSetPOINT( ptDIRECT, (sint32)ptCALC.nX + (sint32)ptDIRECT->nX, (sint32)ptCALC.nY + (sint32)ptDIRECT->nY );

	EFC_rgnSetPOINT( ptMOVE, ptDIRECT->nX, ptDIRECT->nY );
	EFC_mathNORMAL( ptMOVE, nMOVE );
}
