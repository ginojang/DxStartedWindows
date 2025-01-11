#include "LegacyCore.h"
#include "LegacyDrawer.h"

#include "EFCgrp.h"
#include "EFCpixel.h"
#include "EFCimg.h"


void EFC_grpInitialize( void )
{
	EFC_grpSetFRAME( PixelData.hScreen );

	//EFC_grpSetRGB( RGB(255, 255, 255) );
	//EFC_grpFillSCREEN();
}

void EFC_grpFinalize( void )
{
}


//	Screen Orientation
void EFC_grpSetScreenOrientation(ScreenOrientation nMode)
{
	if(PixelData.nScreenOrientation != nMode)
	{
		PixelData.nScreenOrientation = nMode;

		if (PixelData.hLandScapeScreen)
		{
			MC_grpDestroyOffScreenFrameBuffer(PixelData.hLandScapeScreen);
		}

		if (EFC_grpGetOrientationMode() == SCREEN_ORIENTATION_LANDSCAPE)
		{//가로모드일 경우 뒤집어 준다.			

			if (PixelData.rtREAL.nW == 480 && PixelData.rtREAL.nH == 800)
			{
				PixelData.rtMAIN.nW = (sint16)PixelData.rtREAL.nH/2;
				PixelData.rtMAIN.nH = (sint16)PixelData.rtREAL.nW/2;

				PixelData.hLandScapeScreen = MC_grpCreateOffScreenFrameBuffer( PixelData.rtREAL.nW/2, PixelData.rtREAL.nH/2 );
				PixelData.hScreen = MC_grpCreateOffScreenFrameBuffer( PixelData.rtMAIN.nW, PixelData.rtMAIN.nH );
			}
			else
			{
				PixelData.rtMAIN.nW = (sint16)PixelData.rtREAL.nH;
				PixelData.rtMAIN.nH = (sint16)PixelData.rtREAL.nW;

				PixelData.hLandScapeScreen = MC_grpCreateOffScreenFrameBuffer( PixelData.rtREAL.nW, PixelData.rtREAL.nH );
				PixelData.hScreen = MC_grpCreateOffScreenFrameBuffer( PixelData.rtMAIN.nW, PixelData.rtMAIN.nH );
			}
			
			

			/*
			PixelData.hLandScapeScreen = MC_grpCreateOffScreenFrameBuffer( PixelData.rtREAL.nW, PixelData.rtREAL.nH );
			PixelData.rtMAIN.nW = (sint16)PixelData.rtREAL.nH;
			PixelData.rtMAIN.nH = (sint16)PixelData.rtREAL.nW;
			PixelData.hScreen = MC_grpCreateOffScreenFrameBuffer( PixelData.rtREAL.nH, PixelData.rtREAL.nW );
			*/
		} 
		else
		{
			if (PixelData.rtREAL.nW == 480 && PixelData.rtREAL.nH == 800)
			{
				PixelData.rtMAIN.nW = (sint16)PixelData.rtREAL.nW/2;
				PixelData.rtMAIN.nH = (sint16)PixelData.rtREAL.nH/2;
			}
			else
			{
				PixelData.rtMAIN.nW = (sint16)PixelData.rtREAL.nW;
				PixelData.rtMAIN.nH = (sint16)PixelData.rtREAL.nH;
			}
			
			PixelData.hScreen = MC_grpCreateOffScreenFrameBuffer( PixelData.rtMAIN.nW, PixelData.rtMAIN.nH );
		}

		PixelData.nBPP = g_legacyDrawer.GetFrameBpp( PixelData.hScreen );	

		PixelData.rtCLIP.nW = PixelData.rtMAIN.nW;
		PixelData.rtCLIP.nH = PixelData.rtMAIN.nH;

		LCD_W	= PixelData.rtMAIN.nW;
		LCD_H	= PixelData.rtMAIN.nH;

		stdX	= ((LCD_W)>>1);
		stdY	= ((LCD_H)>>1);
		midX	= (LCD_W>>1);
		midY	= (LCD_H>>1);

		if (EFC_grpGetOrientationMode() == SCREEN_ORIENTATION_LANDSCAPE)
		{
			menuX	= (LCD_W - 240)/2+40;
			menuY	= (LCD_H - 240)/2;
		}
		else 
		{
			menuX	= 0;
			menuY	= (LCD_H - 240)/2+20;
		}
	}
}
sint8 EFC_grpGetOrientationMode()
{
	return PixelData.nScreenOrientation;
}

void EFC_grpTransformMousePos(LPEXPOINT pPoint)
{
	if(EFC_grpGetOrientationMode() == SCREEN_ORIENTATION_LANDSCAPE){
		sint32 nX = pPoint->nX;
		sint32 nY = pPoint->nY;
		pPoint->nX = nY;
		pPoint->nY = (g_legacyDrawer.GetFrameWidth(PixelData.hLandScapeScreen)-1)-nX;
	}
}

void EFC_grpFLUSH( void )
{
	EXRECT *pRect;
	EXREGION rgn;

	pRect = &PixelData.rtMAIN;
	if( EFC_rgnSetREGION( &rgn, pRect->nX, pRect->nY, pRect->nW, pRect->nH, 0, 0 ) == FALSE ) 
	{
		return;
	}

	MC_grpFlushLcd( 0, PixelData.hScreen, rgn.nX, rgn.nY, rgn.nW, rgn.nH );
}

void EFC_grpGetANCHOR( LPEXRECT pRect, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nANCHOR )
{
	switch( nANCHOR & 0x000000F0 ) {
	case EN_TOP :
		break;
	case EN_VCENTER :
		nY -= (nH >> 1);
		break;
	case EN_BOTTOM :
		nY -= nH;
		break;
	}

	switch( nANCHOR & 0x0000000F ) {
	case EN_LEFT :
	case EN_MIX :
		break;
	case EN_HCENTER :
		nX -= (nW >> 1);
		break;
	case EN_RIGHT :
		nX -= nW;
		break;
	}

	EFC_rgnSetRECT( pRect, nX, nY, nW, nH );
}

ubool EFC_grpIsMAIN(sint32 hFrame)
{
	if( PixelData.hScreen == hFrame) {
		return TRUE;
	}

	return FALSE;
}

sint32 EFC_grpSetFRAME(sint32 hFrame )
{
	sint32 hPREV;

	hPREV = PixelData.hFrame;
	PixelData.hFrame = hFrame;

	EFC_grpInitMODE();
	EFC_grpReset();

	return hPREV;
}

void EFC_grpCopyAREA( sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy )
{
	EXREGION rgn;
	EXRECT rt;
	sint32 nWIDTH, nHEIGHT;

	EFC_rgnSetREGION( &rgn, nX, nY, nW, nH, nSx, nSy );

	nWIDTH = PixelData.rtMAIN.nW;
	nHEIGHT = PixelData.rtMAIN.nH;
	EFC_rgnSetRECT( &rt, 0, 0, nWIDTH, nHEIGHT );

	if( EFC_rgnInterREGION( &rgn, &rt, &rgn ) == FALSE ) {
		return;
	}

	EFC_pxlCopyArea( PixelData.hFrame, rgn.nX, rgn.nY, rgn.nW, rgn.nH, rgn.nSx, rgn.nSy );
}

void EFC_grpCopyFRAME( sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hFrame, sint32 nSx, sint32 nSy, ubool bTRANS )
{
	EXREGION rgn;

	if( EFC_grpIsMAIN(hFrame) == TRUE ) {
		nSy += PixelData.nANNUN;
	}

	EFC_rgnSetREGION( &rgn, nX, nY, nW, nH, nSx, nSy );
	if( EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE ) == FALSE ) {
		return;
	}

	EFC_pxlCopyFrameBuffer( PixelData.hFrame, rgn.nX, rgn.nY, rgn.nW, rgn.nH, hFrame, rgn.nSx, rgn.nSy, bTRANS );
}

void EFC_grpCopyBUFF( sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hFrame, sint32 nSx, sint32 nSy, sint32 nEx, sint32 nEy, ubool bTRANS )
{
	EXREGION rgn;
	sint32 nX1, nY1, nX2, nY2;

	if( EFC_grpIsMAIN(hFrame) == TRUE ) {
		nSy += PixelData.nANNUN;
	}

	EFC_rgnSetREGION( &rgn, nX, nY, nW, nH, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE ) == FALSE ) {
		return;
	}

	nX1 = nSx + ((nEx - nSx) * rgn.nSx) / nW;
	nY1 = nSy + ((nEy - nSy) * rgn.nSy) / nH;

	nX2 = nSx + ((nEx - nSx) * (rgn.nSx + rgn.nW)) / nW;
	nY2 = nSy + ((nEy - nSy) * (rgn.nSy + rgn.nH)) / nH;

	EFC_pxlCopyFrameScale( PixelData.hFrame, rgn.nX, rgn.nY, rgn.nW, rgn.nH, hFrame, nX1, nY1, nX2, nY2, bTRANS );
}

void EFC_grpSetCLIP( sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	EFC_rgnSetRECT( &PixelData.rtCLIP, nX, nY, nW, nH );
	if( EFC_rgnInterRECT( &PixelData.rtCLIP, &PixelData.rtMAIN, &PixelData.rtCLIP ) == FALSE ) {
		return;
	}
}

void EFC_grpSetLOCK( sint32 nLOCK )
{
	if( nLOCK == 0 ) {
		if( PixelData.nLOCK > 0 ) {
			PixelData.nLOCK--;
		} else {
			PixelData.nLOCK = 0;
		}
	} else {
		if( PixelData.nLOCK < 1 ) {
			PixelData.nLOCK = 1;
		} else {
			PixelData.nLOCK++;
		}
	}
}

void EFC_grpSetTRANS( sint32 nX, sint32 nY )
{
	EFC_rgnSetPOINT( &PixelData.ptTRANS, nX, nY );
}

void EFC_grpSetCOLOR( sint32 nCOLOR )
{
	PixelData.nCOLOR = nCOLOR;
}

void EFC_grpSetRGB( sint32 nRGB )
{
	PixelData.nCOLOR = RGB32RGB16( RGB_R(nRGB), RGB_G(nRGB), RGB_B(nRGB) );
}

void EFC_grpInitMODE( void )
{
	PixelData.nMODE = EN_MODE_NORMAL;
	PixelData.nEFF = 0;

	EFC_grpSetCLIP( PixelData.rtCLIP.nX, PixelData.rtCLIP.nY, PixelData.rtCLIP.nW, PixelData.rtCLIP.nH );
}

void EFC_grpSetMODE( sint32 nMODE, sint32 nEFF )
{
	if( PixelData.nLOCK > 0 ) {
		return;
	}

	if( (nMODE > 0) && (nMODE < EN_MODE_LAST) ) {
		PixelData.nMODE = (uint8)nMODE;
		switch( nMODE ) {
		case EN_MODE_ONE :
			PixelData.nEFF = RGB32RGB16( RGB_R(nEFF), RGB_G(nEFF), RGB_B(nEFF) );
			break;
		case EN_MODE_BRIGHT :
			PixelData.nRGB2[0] = MIN_CALC( MAX_CALC(0, nEFF), 256 );
			PixelData.nRGB2[1] = MIN_CALC( MAX_CALC(0, nEFF), 256 );
			PixelData.nRGB2[2] = MIN_CALC( MAX_CALC(0, nEFF), 256 );

			if( (PixelData.nRGB2[0] == 0) && (PixelData.nRGB2[1] == 0) && (PixelData.nRGB2[2] == 0) ) {
				EFC_grpSetMODE( EN_MODE_NORMAL, 0 );
			} else if( (PixelData.nRGB2[0] == 256) && (PixelData.nRGB2[1] == 256) && (PixelData.nRGB2[2] == 256) ) {
				EFC_grpSetMODE(EN_MODE_ONE, 0xFFFFFF);// RGB_WHITE );
			}

			break;
		case EN_MODE_DARK :
			PixelData.nRGB2[0] = MIN_CALC( MAX_CALC(0, nEFF), 256 );
			PixelData.nRGB2[1] = MIN_CALC( MAX_CALC(0, nEFF), 256 );
			PixelData.nRGB2[2] = MIN_CALC( MAX_CALC(0, nEFF), 256 );

			if( (PixelData.nRGB2[0] == 0) && (PixelData.nRGB2[1] == 0) && (PixelData.nRGB2[2] == 0) ) {
				EFC_grpSetMODE( EN_MODE_NORMAL, 0 );
			} else if( (PixelData.nRGB2[0] == 256) && (PixelData.nRGB2[1] == 256) && (PixelData.nRGB2[2] == 256) ) {
				EFC_grpSetMODE(EN_MODE_ONE, 0);// RGB_BLACK );
			}
			break;
		case EN_MODE_ALPHA :
			PixelData.nEFF = (uint16)(RANGE_CALC( nEFF, 0, 255 ));
			switch( PixelData.nEFF ) {				
			case 32:
			case 48 :
			case 64 :
			case 80 :
			case 96 :
				PixelData.nEFF = ((PixelData.nEFF - 32)>>4);
				EFC_grpSetMODE( EN_MODE_ALPHA_ETC, 0 );
				break;
			case 144 :
				PixelData.nEFF = 5;
				EFC_grpSetMODE( EN_MODE_ALPHA_ETC, 0 );
				break;
			case 160 :
				PixelData.nEFF = 6;
				EFC_grpSetMODE( EN_MODE_ALPHA_ETC, 0 );
				break;
			case 192 :			
				PixelData.nEFF = 7;
				EFC_grpSetMODE( EN_MODE_ALPHA_ETC, 0 );
				break;
			case 112 :
				EFC_grpSetMODE( EN_MODE_ALPHA_112, 0 );
				break;
			case 128 :
				EFC_grpSetMODE( EN_MODE_ALPHA_128, 0 );
				break;
			case 256:
				EFC_grpSetMODE( EN_MODE_NORMAL, 0 );
				break;			
			}
			break;
		case EN_MODE_GRAY :
			PixelData.nEFF = RGB32RGB16( RGB_R(nEFF), RGB_G(nEFF), RGB_B(nEFF) );
			break;
		case EN_MODE_GRAYGRADE :
			PixelData.nEFF = (uint16)RANGE_CALC( nEFF, 0, 256 );
			if( PixelData.nEFF == 0 ) {
				EFC_grpSetMODE( EN_MODE_NORMAL, 0 );
			} else if( PixelData.nEFF == 256 ) {
				EFC_grpSetMODE( EN_MODE_GRAY, 0 );
			}
			break;
		}		
	} else {
		EFC_grpInitMODE();
	}
}

void EFC_grpReset( void )
{
	EFC_grpSetMODE( EN_MODE_NORMAL, 0 );
}

void EFC_grpDrawPIXEL( sint32 nX, sint32 nY )
{
	EXREGION rgn;

	EFC_rgnSetREGION( &rgn, nX, nY, 1, 1, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE ) == FALSE ) {
		return;
	}

	EFC_pxlPutPixel( PixelData.hFrame, rgn.nX, rgn.nY );
}

void EFC_grpDrawLINE( sint32 nX1, sint32 nY1, sint32 nX2, sint32 nY2 )
{
	sint32 i, nMinX, nMinY, nMaxX, nMaxY, nNextX, nNextY, nSLOPE, nYB;

	if( nX1 == nX2 ) {
		nMinY = MIN_CALC( nY1, nY2 );
		nMaxY = MAX_CALC( nY1, nY2 );
		EFC_grpFillRECT( nX1, nMinY, 1, nMaxY - nMinY + 1 );
	} else if( nY1 == nY2 ) {
		nMinX = MIN_CALC( nX1, nX2 );
		nMaxX = MAX_CALC( nX1, nX2 );
		EFC_grpFillRECT( nMinX, nY1, nMaxX - nMinX + 1, 1 );
	} else {
		nSLOPE = ((nY2 - nY1) << 12) / (nX2 - nX1);
		nYB = nY1 - ((nSLOPE * nX1) >> 12);
		if( ABS32( nSLOPE ) < 4096 ) {
			if( nX1 < nX2 ) {
				for( i=nX1; i<=nX2; i++ ) {
					nNextY = ((nSLOPE * i) >> 12) + nYB;
					EFC_grpDrawPIXEL( i, nNextY );
				}
			} else {
				for( i=nX2; i<=nX1; i++ ) {
					nNextY = ((nSLOPE * i) >> 12) + nYB;
					EFC_grpDrawPIXEL( i, nNextY );
				}
			}
		} else {
			if( nY1 < nY2 ) {
				for( i=nY1; i<=nY2; i++ ) {
					nNextX = ((i - nYB) << 12) / nSLOPE;
					EFC_grpDrawPIXEL( nNextX, i );
				}
			} else {
				for( i=nY2; i<=nY1; i++ ) {
					nNextX = ((i - nYB) << 12) / nSLOPE;
					EFC_grpDrawPIXEL( nNextX, i );
				}
			}
		}
	}
}

void EFC_grpDrawRECT( sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	EFC_grpFillRECT( nX, nY, 1, nH );
	EFC_grpFillRECT( nX, nY, nW, 1 );
	EFC_grpFillRECT( nX + nW - 1, nY, 1, nH );
	EFC_grpFillRECT( nX, nY + nH - 1, nW, 1 );
}

void EFC_grpFillRECT( sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	EXREGION rgn;

	EFC_rgnSetREGION( &rgn, nX, nY, nW, nH, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE ) == FALSE ) {
		return;
	}

	EFC_pxlFillRect( PixelData.hFrame, rgn.nX, rgn.nY, rgn.nW, rgn.nH);
}

void EFC_grpDrawPOLYGON( sint32 nX, sint32 nY, LPEXPOINT pPTs, sint32 nCOUNT )
{
	sint32 i, nNEXT;

	for( i=(nCOUNT - 1); i>=0; i-- ) {
		nNEXT = ((i + 1) % nCOUNT);
		EFC_grpDrawLINE( nX + pPTs[i].nX, nY + pPTs[i].nY, nX + pPTs[nNEXT].nX, nY + pPTs[nNEXT].nY );
	}
}

void EFC_grpFillPOLYGON( sint32 nX, sint32 nY, LPEXPOINT pPTs, sint32 nCOUNT )
{
	sint32 i, j, nNextY;
	sint32 nMinY, nMaxY;
	sint32 nNODE, nNODEs[32], nTEMP;

	if( nCOUNT < 3 ) {
		return;
	}

	MEMSET( nNODEs, 0, sizeof(nNODEs) );

	nMinY = pPTs[0].nY;
	nMaxY = pPTs[0].nY;

	for( i=(nCOUNT - 1); i>=1; i-- ) {
		if( nMinY > pPTs[i].nY ) {
			nMinY = pPTs[i].nY;
		}

		if( nMaxY < pPTs[i].nY ) {
			nMaxY = pPTs[i].nY;
		}
	}

	for( nNextY=nMinY; nNextY<=nMaxY; nNextY++ ) {
		nNODE = 0;
		for( i=0, j=(nCOUNT - 1); i<nCOUNT; j=i++ ) {
			if( ((pPTs[i].nY < nNextY) && (pPTs[j].nY >= nNextY)) || ((pPTs[j].nY < nNextY) && (pPTs[i].nY >= nNextY)) ) {
				nNODEs[nNODE++] = (sint32)(pPTs[i].nX + (((((nNextY - pPTs[i].nY) * (pPTs[j].nX - pPTs[i].nX)) << 12) / (pPTs[j].nY - pPTs[i].nY)) >> 12));
			} else if( (pPTs[i].nY == nNextY) && (pPTs[j].nY == nNextY) ) {
				nNODEs[nNODE++] = pPTs[i].nX;
				nNODEs[nNODE++] = pPTs[j].nX;
			}
		}

		// SORT
		for( i=0; i<(nNODE - 1); i++ ) {
			for( j=(i + 1); j<nNODE; j++ ) {
				if( nNODEs[i] > nNODEs[j] ) {
					nTEMP = nNODEs[i];
					nNODEs[i] = nNODEs[j];
					nNODEs[j] = nTEMP;
				}
			}
		}

		for( i=0; i<nNODE; i+=2 ) {
			EFC_grpDrawLINE( nX + nNODEs[i], nY + nNextY, nX + nNODEs[i + 1], nY + nNextY );
		}
	}
}

void EFC_grpDrawRoundRECT( sint32 nTYPE, sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	switch( nTYPE ) {
	case 1 :
		// LINE
		EFC_grpFillRECT( nX + 1, nY, nW - 2, 1 );
		EFC_grpFillRECT( nX + 1, nY + nH - 1, nW - 2, 1 );
		EFC_grpFillRECT( nX, nY + 1, 1, nH - 2 );
		EFC_grpFillRECT( nX + nW - 1, nY + 1, 1, nH - 2 );
		// DOT
		EFC_grpFillRECT( nX + 1, nY + 1, 1, 1 );
		EFC_grpFillRECT( nX + 1, nY + nH - 2, 1, 1 );
		EFC_grpFillRECT( nX + nW - 2, nY + 1, 1, 1 );
		EFC_grpFillRECT( nX + nW - 2, nY + nH - 2, 1, 1 );
		break;
	case 2 :
		// TOP
		EFC_grpFillRECT( nX + 2, nY, nW - 4, 1 );
		EFC_grpFillRECT( nX + 2, nY + nH - 1, nW - 4, 1 );
		// TOP1
		EFC_grpFillRECT( nX + 1, nY + 1, 2, 1 );
		EFC_grpFillRECT( nX + nW - 3, nY + 1, 2, 1 );
		// BOTTOM1
		EFC_grpFillRECT( nX + 1, nY + nH - 2, 2, 1 );
		EFC_grpFillRECT( nX + nW - 3, nY + nH - 2, 2, 1 );
		// TOP2
		EFC_grpFillRECT( nX, nY + 2, 2, 1 );
		EFC_grpFillRECT( nX + nW - 2, nY + 2, 2, 1 );
		// BOTTOM2
		EFC_grpFillRECT( nX, nY + nH - 3, 2, 1 );
		EFC_grpFillRECT( nX + nW - 2, nY + nH - 3, 2, 1 );
		// LEFT, RIGHT
		EFC_grpFillRECT( nX, nY + 3, 1, nH - 6 );
		EFC_grpFillRECT( nX + nW - 1, nY + 3, 1, nH - 6 );
		break;
	case 3 :
		// TOP, BOTTOM
		EFC_grpFillRECT( nX + 4, nY, nW - 8, 1 );
		EFC_grpFillRECT( nX + 4, nY + nH - 1, nW - 8, 1 );
		// TOP1, BOTTOM1
		EFC_grpFillRECT( nX + 2, nY + 1, 3, 1 );
		EFC_grpFillRECT( nX + nW - 5, nY + 1, 3, 1 );
		EFC_grpFillRECT( nX + 2, nY + nH - 2, 3, 1 );
		EFC_grpFillRECT( nX + nW - 5, nY + nH - 2, 3, 1 );
		// TOP2, BOTTOM2
		EFC_grpFillRECT( nX + 2, nY + 2, 1, 1 );
		EFC_grpFillRECT( nX + nW - 3, nY + 2, 1, 1 );
		EFC_grpFillRECT( nX + 2, nY + nH - 3, 1, 1 );
		EFC_grpFillRECT( nX + nW - 3, nY + nH - 3, 1, 1 );
		// TOP3, BOTTOM3
		EFC_grpFillRECT( nX + 1, nY + 2, 1, 3 );
		EFC_grpFillRECT( nX + nW - 2, nY + 2, 1, 3 );
		EFC_grpFillRECT( nX + 1, nY + nH - 5, 1, 3 );
		EFC_grpFillRECT( nX + nW - 2, nY + nH - 5, 1, 3 );
		// LEFT, RIGHT
		EFC_grpFillRECT( nX, nY + 4, 1, nH - 8 );
		EFC_grpFillRECT( nX + nW - 1, nY + 4, 1, nH - 8 );
		break;
	}
}

void EFC_grpFillRoundRECT( sint32 nTYPE, sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	switch( nTYPE ) {
	case 1 :
		// TOP, BOTTOM
		EFC_grpFillRECT( nX + 1, nY, nW - 2, 1 );
		EFC_grpFillRECT( nX + 1, nY + nH - 1, nW - 2, 1 );
		// CENTER
		EFC_grpFillRECT( nX, nY + 1, nW, nH - 2 );
		break;
	case 2 :
		// TOP, BOTTOM
		EFC_grpFillRECT( nX + 2, nY, nW - 4, 1 );
		EFC_grpFillRECT( nX + 2, nY + nH - 1, nW - 4, 1 );
		// TOP1, BOTTOM1
		EFC_grpFillRECT( nX + 1, nY + 1, nW - 2, 1 );
		EFC_grpFillRECT( nX + 1, nY + nH - 2, nW - 2, 1 );
		// CENTER
		EFC_grpFillRECT( nX, nY + 2, nW, nH - 4 );
		break;
	case 3 :
		// TOP, BOTTOM
		EFC_grpFillRECT( nX + 4, nY, nW - 8, 1 );
		EFC_grpFillRECT( nX + 4, nY + nH - 1, nW - 8, 1 );
		// TOP1, BOTTOM1
		EFC_grpFillRECT( nX + 2, nY + 1, nW - 4, 1 );
		EFC_grpFillRECT( nX + 2, nY + nH - 2, nW - 4, 1 );
		// TOP2, BOTTOM2
		EFC_grpFillRECT( nX + 1, nY + 2, nW - 2, 2 );
		EFC_grpFillRECT( nX + 1, nY + nH - 4, nW - 2, 2 );
		// CENTER
		EFC_grpFillRECT( nX, nY + 4, nW, nH - 8 );
		break;
	}
}

void EFC_grpFillSCREEN( void )
{
	EXPOINT pt;
	sint32 nW, nH;

	pt = PixelData.ptTRANS;
	PixelData.ptTRANS.nX = 0;
	PixelData.ptTRANS.nY = 0;

	nW = g_legacyDrawer.GetFrameWidth( PixelData.hFrame );
	nH = g_legacyDrawer.GetFrameHeight( PixelData.hFrame );
	EFC_grpFillRECT( 0, 0, nW, nH );

	PixelData.ptTRANS = pt;
}

void EFC_grpFillGradientV( sint32 nX, sint32 nY, sint32 nW, sint32 nH, RGBVAL nRGB_T, RGBVAL nRGB_B )
{
	EXRECT rt;
	sint32 nR, nG, nB;
	sint32 nR1, nG1, nB1;
	sint32 nR2, nG2, nB2;
	sint32 i, nSy1, nSy2;

	if( nH < 1 ) {
		return;
	}

	nR1 = RGB_R( nRGB_T );
	nG1 = RGB_G( nRGB_T );
	nB1 = RGB_B( nRGB_T );

	nR2 = RGB_R( nRGB_B );
	nG2 = RGB_G( nRGB_B );
	nB2 = RGB_B( nRGB_B );

	EFC_rgnSetRECT( &rt, nX, nY, nW, nH );
	EFC_rgnInterRECT( &rt, &PixelData.rtMAIN, &rt );

	nSy1 = rt.nY;
	nSy2 = rt.nY + rt.nH;

	for( i=nSy1; i<nSy2; i++ ) {
		nR = (nR1 + (((nR2 - nR1) * (i - nY + 1)) / nH));
		nG = (nG1 + (((nG2 - nG1) * (i - nY + 1)) / nH));
		nB = (nB1 + (((nB2 - nB1) * (i - nY + 1)) / nH));

		EFC_grpSetRGB( RGB(nR, nG, nB) );
		EFC_grpFillRECT( nX, i, nW, 1 );
	}
}

void EFC_grpFillGradientH( sint32 nX, sint32 nY, sint32 nW, sint32 nH, RGBVAL nRGB_L, RGBVAL nRGB_R )
{
	EXRECT rt;
	sint32 nR, nG, nB;
	sint32 nR1, nG1, nB1;
	sint32 nR2, nG2, nB2;
	sint32 i, nSx1, nSx2;

	if( nW < 1 ) {
		return;
	}

	nR1 = RGB_R( nRGB_L );
	nG1 = RGB_G( nRGB_L );
	nB1 = RGB_B( nRGB_L );

	nR2 = RGB_R( nRGB_R );
	nG2 = RGB_G( nRGB_R );
	nB2 = RGB_B( nRGB_R );

	EFC_rgnSetRECT( &rt, nX, nY, nW, nH );
	EFC_rgnInterRECT( &rt, &PixelData.rtMAIN, &rt );

	nSx1 = rt.nX;
	nSx2 = rt.nX + rt.nW;

	for( i=nSx1; i<nSx2; i++ ) {
		nR = (nR1 + (((nR2 - nR1) * (i - nX + 1)) / nW));
		nG = (nG1 + (((nG2 - nG1) * (i - nX + 1)) / nW));
		nB = (nB1 + (((nB2 - nB1) * (i - nX + 1)) / nW));

		EFC_grpSetRGB( RGB(nR, nG, nB) );
		EFC_grpFillRECT( i, nY, 1, nH );
	}
}

void EFC_grpFillOpacityV( sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	sint32 i;

	for( i=0; i<26; i++ ) {
		EFC_grpSetMODE( EN_MODE_ALPHA, (255 - (10 * i)) );
		EFC_grpFillRECT( nX, nY + i, nW, 1 );
		EFC_grpFillRECT( nX, nY + nH - 1 - i, nW, 1 );
	}

	EFC_grpReset();
}

void EFC_grpFillOpacityVEX( sint32 nX, sint32 nY, sint32 nW, sint32 nH, RGBVAL nRGB_T, RGBVAL nRGB_B )
{
	sint32 nR, nG, nB;
	sint32 nR1, nG1, nB1;
	sint32 nR2, nG2, nB2;
	sint32 i/*, nSy1, nSy2*/;

	nR1 = RGB_R( nRGB_T );
	nG1 = RGB_G( nRGB_T );
	nB1 = RGB_B( nRGB_T );

	nR2 = RGB_R( nRGB_B );
	nG2 = RGB_G( nRGB_B );
	nB2 = RGB_B( nRGB_B );

	for( i=0; i<40; i++ ) {
		nR = (nR1 + (((nR2 - nR1) * (i+1)) / 40));
		nG = (nG1 + (((nG2 - nG1) * (i+1)) / 40));
		nB = (nB1 + (((nB2 - nB1) * (i+1)) / 40));

		EFC_grpSetRGB( RGB(nR, nG, nB) );
		EFC_grpFillRECT( nX, nY + i, nW, 1 );
		EFC_grpFillRECT( nX, nY + nH - 1 - i, nW, 1 );
	}

	EFC_grpReset();
}

void EFC_grpSetClipLINE( LPEXRECT pRECT, sint32 nX1, sint32 nX2, sint32 nY )
{
	if( (pRECT->nX == 0) && (pRECT->nY == 0) && (pRECT->nW == 0) && (pRECT->nH == 0) ) {
		EFC_rgnSetRECT( pRECT, nX1, nY, nX2, nY );
	} else if( (nX1 == 0) && (nX2 == 0) && (nY == 0) ) {
		EFC_grpDrawLINE( pRECT->nX, pRECT->nY, pRECT->nW, pRECT->nH );
		EFC_rgnSetRECT( pRECT, nX1, nY, nX2, nY );
	} else if( pRECT->nY == nY ) {
		sint32 nMIN, nMAX;
		nMIN = MIN_CALC( MIN_CALC( MIN_CALC( nX1, nX2 ), pRECT->nX ), pRECT->nW );
		nMAX = MAX_CALC( MAX_CALC( MAX_CALC( nX1, nX2 ), pRECT->nX ), pRECT->nW );
		EFC_rgnSetRECT( pRECT, nMIN, pRECT->nY, nMAX, pRECT->nH );
	} else {
		EFC_grpDrawLINE( pRECT->nX, pRECT->nY, pRECT->nW, pRECT->nH );
		EFC_rgnSetRECT( pRECT, nX1, nY, nX2, nY );
	}
}

void EFC_grpDrawARC( sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	EXREGION rgn;

	EFC_rgnSetREGION( &rgn, nX, nY, nW, nH, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE ) == FALSE ) {
		return;
	}

	{
		sint32 nNextW, nNextH, nNextC;
		sint32 i, nTempX = 0, nTempY = 0;
		sint32 nSx, nSy, nSw, nSh;
		sint32 nX1, nY1, nX2, nY2;
		sint32 n2W, n2H;

		n2W = (nW >> 1);
		n2H = (nH >> 1);

		nNextW = n2W + 1;
		nNextH = n2H + 1;
		nNextC = ((nNextW * nNextW) / (nNextW + nNextH));
		if( nNextC < 2 ) {
			nTempY = n2H;
		}

		nSx = nX + n2W - 1;
		nSy = nY + n2H;
		nSw = (nW & 0x00000001);
		nSh = (nH & 0x00000001);

		if( nSw == 0x00000001 ) {
			EFC_grpDrawPIXEL( nSx + 1, nSy - nNextH + 1 );
			EFC_grpDrawPIXEL( nSx + 1, nSy - nNextH + nH );
		}

		if( nSh == 0x00000001 ) {
			EFC_grpDrawPIXEL( nSx +   1 - (nNextW - 1), nSy );
			EFC_grpDrawPIXEL( nSx + nSw + (nNextW - 1), nSy );
		}

		for( nX1=1; nX1<nNextC; nX1++ ) {
			nY1 = EFC_mathCircleX( nX1, nNextW, nNextH );

			EFC_grpDrawPIXEL( nSx - nX1 +   1, nSy - nY1 );
			EFC_grpDrawPIXEL( nSx + nX1 + nSw, nSy - nY1 );
			EFC_grpDrawPIXEL( nSx - nX1 +   1, nSy + nY1 + nSh - 1 );
			EFC_grpDrawPIXEL( nSx + nX1 + nSw, nSy + nY1 + nSh - 1 );

			nTempX = nX1 + 1;
			nTempY = nY1 - 1;
		}

		for( nY2=nTempY; nY2>0; nY2-- ) {
			nX2 = EFC_mathCircleY( nY2, nNextW, nNextH );

			for( i=nTempX; i<nX2; i++ ) {
				EFC_grpDrawPIXEL( nSx - i +   1, nSy - nY2 );
				EFC_grpDrawPIXEL( nSx + i + nSw, nSy - nY2 );
				EFC_grpDrawPIXEL( nSx - i +   1, nSy + nY2 + nSh - 1 );
				EFC_grpDrawPIXEL( nSx + i + nSw, nSy + nY2 + nSh - 1 );
			}

			EFC_grpDrawPIXEL( nSx - nX2 +   1, nSy - nY2 );
			EFC_grpDrawPIXEL( nSx + nX2 + nSw, nSy - nY2 );
			EFC_grpDrawPIXEL( nSx - nX2 +   1, nSy + nY2 + nSh - 1 );
			EFC_grpDrawPIXEL( nSx + nX2 + nSw, nSy + nY2 + nSh - 1 );

			nTempX = nX2 + 1;
		}
	}
}

void EFC_grpDrawARC2( sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nLINES )
{
	EXREGION rgn;

	EFC_rgnSetREGION( &rgn, nX, nY, nW, nH, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE ) == FALSE ) {
		return;
	}

	{
		sint32 nNextW, nNextH, nNextC;
		sint32 nTempY = 0;
		sint32 nSx, nSy, nSw, nSh;
		sint32 nX1, nY1, nX2, nY2;
		sint32 n2W, n2H;
		EXRECT rt[4];

		MEMSET( rt, 0, sizeof(rt) );

		n2W = (nW >> 1);
		n2H = (nH >> 1);

		nNextW = n2W + 1;
		nNextH = n2H + 1;
		nNextC = ((nNextW * nNextW) / (nNextW + nNextH));
		if( nNextC < 2 ) {
			nTempY = n2H;
		}

		nSx = nX + n2W - 1;
		nSy = nY + n2H;
		nSw = (nW & 0x00000001);
		nSh = (nH & 0x00000001);

		if( nSh == 0x00000001 ) {
			EFC_grpSetClipLINE( &rt[0], nSx +   1 - (nNextW - 1), nSx +   1 - (nNextW - 1) + (nLINES - 1), nSy );
			EFC_grpSetClipLINE( &rt[2], nSx + nSw + (nNextW - 1), nSx + nSw + (nNextW - 1) - (nLINES - 1), nSy );
		}

		for( nX1=1; nX1<nNextC; nX1++ ) {
			nY1 = EFC_mathCircleX( nX1, nNextW, nNextH );
			nX2 = EFC_mathCircleY( nY1, nNextW - (nLINES - 1), nNextH - (nLINES - 1) );

			if( nLINES > (n2H - nY1 + 2) ) {
				EFC_grpSetClipLINE( &rt[0], nSx - nX1 + 1, nSx + nX1 + nSw, nSy - nY1 );
				EFC_grpSetClipLINE( &rt[2], nSx - nX1 + 1, nSx + nX1 + nSw, nSy + nY1 + nSh - 1 );
			} else {
				EFC_grpSetClipLINE( &rt[0], nSx - nX1 +   1, nSx - nX2 +   1, nSy - nY1 );
				EFC_grpSetClipLINE( &rt[1], nSx + nX1 + nSw, nSx + nX2 + nSw, nSy - nY1 );
				EFC_grpSetClipLINE( &rt[2], nSx - nX1 +   1, nSx - nX2 +   1, nSy + nY1 + nSh - 1 );
				EFC_grpSetClipLINE( &rt[3], nSx + nX1 + nSw, nSx + nX2 + nSw, nSy + nY1 + nSh - 1 );
			}

			nTempY = nY1 - 1;
		}

		for( nY2=nTempY; nY2>0; nY2-- ) {
			nX2 = EFC_mathCircleY( nY2, nNextW, nNextH );
			nX1 = EFC_mathCircleY( nY2, nNextW - (nLINES - 1), nNextH - (nLINES - 1) );

			if( nLINES > (n2H - nY2 + 2) ) {
				EFC_grpSetClipLINE( &rt[0], nSx - nX2 + 1, nSx + nX2 + nSw, nSy - nY2 );
				EFC_grpSetClipLINE( &rt[2], nSx - nX2 + 1, nSx + nX2 + nSw, nSy + nY2 + nSh - 1 );
			} else {
				EFC_grpSetClipLINE( &rt[0], nSx - nX2 +   1, nSx - nX1 +   1, nSy - nY2 );
				EFC_grpSetClipLINE( &rt[1], nSx + nX2 + nSw, nSx + nX1 + nSw, nSy - nY2 );
				EFC_grpSetClipLINE( &rt[2], nSx - nX2 +   1, nSx - nX1 +   1, nSy + nY2 + nSh - 1 );
				EFC_grpSetClipLINE( &rt[3], nSx + nX2 + nSw, nSx + nX1 + nSw, nSy + nY2 + nSh - 1 );
			}
		}

		EFC_grpSetClipLINE( &rt[0], 0, 0, 0 );
		EFC_grpSetClipLINE( &rt[1], 0, 0, 0 );

		EFC_grpSetClipLINE( &rt[2], 0, 0, 0 );
		EFC_grpSetClipLINE( &rt[3], 0, 0, 0 );
	}
}

void EFC_grpFillARC( sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	EXREGION rgn;

	EFC_rgnSetREGION( &rgn, nX, nY, nW, nH, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE ) == FALSE ) {
		return;
	}

	{
		sint32 nNextW, nNextH, nNextC;
		sint32 nTempY = 0;
		sint32 nSx, nSy, nSw, nSh;
		sint32 nX1, nY1, nX2, nY2;
		sint32 n2W, n2H;
		EXRECT rt[2];

		MEMSET( rt, 0, sizeof(rt) );

		n2W = (nW >> 1);
		n2H = (nH >> 1);

		nNextW = n2W + 1;
		nNextH = n2H + 1;
		nNextC = ((nNextW * nNextW) / (nNextW + nNextH));
		if( nNextC < 2 ) {
			nTempY = n2H;
		}

		nSx = nX + n2W - 1;
		nSy = nY + n2H;
		nSw = (nW & 0x00000001);
		nSh = (nH & 0x00000001);

		if( nSh == 0x00000001 ) {
			EFC_grpSetClipLINE( &rt[0], nSx + 1 - (nNextW - 1), nSx + nSw + (nNextW - 1), nSy );
		}

		for( nX1=1; nX1<nNextC; nX1++ ) {
			nY1 = EFC_mathCircleX( nX1, nNextW, nNextH );
			EFC_grpSetClipLINE( &rt[0], nSx - nX1 + 1, nSx + nX1 + nSw, nSy - nY1 );
			EFC_grpSetClipLINE( &rt[1], nSx - nX1 + 1, nSx + nX1 + nSw, nSy + nY1 + nSh - 1 );
			nTempY = nY1 - 1;
		}

		for( nY2=nTempY; nY2>0; nY2-- ) {
			nX2 = EFC_mathCircleY( nY2, nNextW, nNextH );
			EFC_grpSetClipLINE( &rt[0], nSx - nX2 + 1, nSx + nX2 + nSw, nSy - nY2 );
			EFC_grpSetClipLINE( &rt[1], nSx - nX2 + 1, nSx + nX2 + nSw, nSy + nY2 + nSh - 1 );
		}

		EFC_grpSetClipLINE( &rt[0], 0, 0, 0 );
		EFC_grpSetClipLINE( &rt[1], 0, 0, 0 );
	}
}

void EFC_grpFillSemiARC( sint32 nX, sint32 nY, sint32 nW, sint32 nH, ubool bUP )
{
	EXREGION rgn;

	EFC_rgnSetREGION( &rgn, nX, nY, nW, nH, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE ) == FALSE ) {
		return;
	}

	{
		sint32 nNextW, nNextH, nNextC;
		sint32 nTempY = 0;
		sint32 nSx, nSy, nSw;
		sint32 nX1, nY1, nX2, nY2;
		sint32 n2W, n2H;
		EXRECT rt[2];

		MEMSET( rt, 0, sizeof(rt) );

		n2W = (nW >> 1);
		n2H = nH;

		nNextW = n2W + 1;
		nNextH = n2H + 1;
		nNextC = ((nNextW * nNextW) / (nNextW + nNextH));
		if( nNextC < 2 ) {
			nTempY = n2H;
		}

		nSx = nX + n2W - 1;
		nSw = (nW & 0x00000001);

		if( bUP == TRUE ) {
			nSy = nY + n2H;

			for( nX1=1; nX1<nNextC; nX1++ ) {
				nY1 = EFC_mathCircleX( nX1, nNextW, nNextH );
				EFC_grpSetClipLINE( &rt[0], nSx - nX1 + 1, nSx + nX1 + nSw, nSy - nY1 );
				nTempY = nY1 - 1;
			}

			for( nY2=nTempY; nY2>0; nY2-- ) {
				nX2 = EFC_mathCircleY( nY2, nNextW, nNextH );
				EFC_grpSetClipLINE( &rt[0], nSx - nX2 + 1, nSx + nX2 + nSw, nSy - nY2 );
			}

			EFC_grpSetClipLINE( &rt[0], 0, 0, 0 );
		} else {
			nSy = nY;

			for( nX1=1; nX1<nNextC; nX1++ ) {
				nY1 = EFC_mathCircleX( nX1, nNextW, nNextH );
				EFC_grpSetClipLINE( &rt[1], nSx - nX1 + 1, nSx + nX1 + nSw, nSy + nY1 + - 1 );
				nTempY = nY1 - 1;
			}

			for( nY2=nTempY; nY2>0; nY2-- ) {
				nX2 = EFC_mathCircleY( nY2, nNextW, nNextH );
				EFC_grpSetClipLINE( &rt[1], nSx - nX2 + 1, nSx + nX2 + nSw, nSy + nY2 + - 1 );
			}

			EFC_grpSetClipLINE( &rt[1], 0, 0, 0 );
		}
	}
}

void EFC_grpDrawBEZIER( sint32 nX1, sint32 nY1, sint32 nX2, sint32 nY2, sint32 nX3, sint32 nY3, sint32 nX4, sint32 nY4, sint32 nCOUNT )
{
	sint32 i, nX, nY, nNextX, nNextY, nMAX = 100, nGRADE;

	if( nCOUNT < 1 ) {
		return;
	}

	nX = nNextX = nX1;
	nY = nNextY = nY1;

	nGRADE = (nMAX / nCOUNT);

	for( i=0; i<=nMAX; i+=nGRADE ) {
		nNextX = (((nMAX - i) * (nMAX - i) * (nMAX - i) * nX1) +
				(3 * i * (i - nMAX) * (i - nMAX) * nX2) + 
				(3 * i * i * (nMAX - i) * nX3) +
				(i * i * i * nX4)) / (nMAX * nMAX * nMAX);

		nNextY = (((nMAX - i) * (nMAX - i) * (nMAX - i) * nY1) +
				(3 * i * (i - nMAX) * (i - nMAX) * nY2) + 
				(3 * i * i * (nMAX - i) * nY3) +
				(i * i * i * nY4)) / (nMAX * nMAX * nMAX);

		EFC_grpDrawLINE( nX, nY, nNextX, nNextY );

		nX = nNextX;
		nY = nNextY;
	}

	EFC_grpDrawLINE( nNextX, nNextY, nX4, nY4 );
}

void EFC_grpDrawClipIMAGE( sint32 nX, sint32 nY, sint32 nW, sint32 nH, LPEXIMAGE pIMG, sint32 nSx, sint32 nSy, sint32 nPal )
{
	EXREGION rgn;

	if( nW > (pIMG->nW - nSx) ) {
		nW = (pIMG->nW - nSx);
	}

	if( nH > (pIMG->nH - nSy) ) {
		nH = (pIMG->nH - nSy);
	}

	EFC_rgnSetREGION( &rgn, nX, nY, nW, nH, nSx, nSy );
	if( EFC_rgnRealREGION( &rgn, EN_MIRROR_NONE ) == FALSE ) {
		return;
	}

	EFC_pxlDrawBuff( PixelData.hFrame, rgn.nX, rgn.nY, rgn.nW, rgn.nH, pIMG, rgn.nSx, rgn.nSy, 0, nPal);
}

void EFC_grpDrawIMAGE( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nANCHOR )
{
	EXRECT rt;
	EFC_grpGetANCHOR( &rt, nX, nY, pIMG->nW, pIMG->nH, nANCHOR );
	EFC_grpDrawClipIMAGE( rt.nX, rt.nY, rt.nW, rt.nH, pIMG, 0, 0, 0 );
}

void EFC_grpDrawSPRITE( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nANCHOR )
{
	EXRECT rt;
	EFC_grpGetANCHOR( &rt, nX, nY, pIMG->nW, pIMG->nH, nANCHOR );
	EFC_grpDrawClipIMAGE( rt.nX - pIMG->nMoveX, rt.nY - pIMG->nMoveY, rt.nW, rt.nH, pIMG, 0, 0, 0 );
}

void EFC_grpDrawSpritePal( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nPal )
{
	EXRECT rt;
	EFC_grpGetANCHOR( &rt, nX, nY, pIMG->nW, pIMG->nH, 0 );
	EFC_grpDrawClipIMAGE( rt.nX - pIMG->nMoveX, rt.nY - pIMG->nMoveY, rt.nW, rt.nH, pIMG, 0, 0, nPal );
}

void EFC_grpMirREGION( LPEXREGION pRGN, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy, sint32 nMIRROR )
{
	switch( nMIRROR ) {
	case EN_MIRROR_HORIZON :
		if( (pRGN->nX + pRGN->nW) < PixelData.rtMAIN.nW ) {
			pRGN->nSx = (sint16)nSx;
		} else {
			pRGN->nSx = (sint16)(nSx + nW - pRGN->nW);
		}
		break;
	case EN_MIRROR_VERTICAL :
		if( (pRGN->nY + pRGN->nH) < PixelData.rtMAIN.nH ) {
			pRGN->nSy = (sint16)nSy;
		} else {
			pRGN->nSy = (sint16)(nSy + nH - pRGN->nH);
		}
		break;
	case EN_MIRROR_BOTH :
		if( (pRGN->nX + pRGN->nW) < PixelData.rtMAIN.nW ) {
			pRGN->nSx = (sint16)nSx;
		} else {
			pRGN->nSx = (sint16)(nSx + nW - pRGN->nW);
		}
		if( (pRGN->nY + pRGN->nH) < PixelData.rtMAIN.nH ) {
			pRGN->nSy = (sint16)nSy;
		} else {
			pRGN->nSy = (sint16)(nSy + nH - pRGN->nH);
		}
		break;
	}
}

void EFC_grpDrawMirror( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nMIR, sint32 nPalette )
{
	EXMOVE rMov;
	EXREGION rgn;

	if(nPalette < 0)
		nPalette = 0;

	EFC_imgMIRROR(pIMG, &rMov, nMIR);

	if(pIMG->pBuff==NULL) return;

	EFC_rgnSetREGION( &rgn, nX - rMov.moveX, nY - rMov.moveY, pIMG->nW, pIMG->nH, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, nMIR ) == FALSE ) {
		return;
	}

	EFC_pxlDrawBuff( PixelData.hFrame, rgn.nX, rgn.nY, rgn.nW, rgn.nH, pIMG, rgn.nSx, rgn.nSy, nMIR, nPalette);
}
////////////////////////////////////////////////////////////

void EFC_grpFillBLUR(sint32 hFrame)
{
	sint32 i, j;
	sint32 nW, nH, nBPL, nBPW;
	sint32 nR, nG, nB;
	sint32 nX, nY, nPOS;

	nW = g_legacyDrawer.GetFrameWidth(hFrame);
	nH = g_legacyDrawer.GetFrameHeight(hFrame);
	
	nBPL = g_legacyDrawer.GetBplWithWidth( nW, PixelData.nBPP );
	nBPW = nBPL / (PixelData.nBPP >> 3);

	{
		uint16 *pDATA;
	
		pDATA = (uint16 *)g_legacyDrawer.GetFrameBuffer(hFrame);

		for( i=(nH - 1); i>=0; i-- ) {
			for( j=(nW - 1); j>=0; j-- ) {
				nR = nG = nB = 0;

				// LINE 1
				nY = (i - 1);

				nX = (j - 1);
				if( (nX >= 0) && (nY >= 0) ) {
					nPOS = nY * nBPW + nX;
					nR += COLOR16_R( pDATA[ nPOS ] );
					nG += COLOR16_G( pDATA[ nPOS ] );
					nB += COLOR16_B( pDATA[ nPOS ] );
				}

				nX = (j + 0);
				if( (nY >= 0) ) {
					nPOS = nY * nBPW + nX;
					nR += (COLOR16_R( pDATA[ nPOS ] ) << 1);
					nG += (COLOR16_G( pDATA[ nPOS ] ) << 1);
					nB += (COLOR16_B( pDATA[ nPOS ] ) << 1);
				}

				nX = (j + 1);
				if( (nX < nW) && (nY >= 0) ) {
					nPOS = nY * nBPW + nX;
					nR += COLOR16_R( pDATA[ nPOS ] );
					nG += COLOR16_G( pDATA[ nPOS ] );
					nB += COLOR16_B( pDATA[ nPOS ] );
				}

				// LINE 2
				nY = (i + 0);

				nX = (j - 1);
				if( (nX >= 0) ) {
					nPOS = nY * nBPW + nX;
					nR += (COLOR16_R( pDATA[ nPOS ] ) << 1);
					nG += (COLOR16_G( pDATA[ nPOS ] ) << 1);
					nB += (COLOR16_B( pDATA[ nPOS ] ) << 1);
				}

				nX = (j + 0);
				{
					nPOS = nY * nBPW + nX;
					nR += (COLOR16_R( pDATA[ nPOS ] ) << 2);
					nG += (COLOR16_G( pDATA[ nPOS ] ) << 2);
					nB += (COLOR16_B( pDATA[ nPOS ] ) << 2);
				}

				nX = (j + 1);
				if( (nX < nW) ) {
					nPOS = nY * nBPW + nX;
					nR += (COLOR16_R( pDATA[ nPOS ] ) << 1);
					nG += (COLOR16_G( pDATA[ nPOS ] ) << 1);
					nB += (COLOR16_B( pDATA[ nPOS ] ) << 1);
				}

				// LINE 3
				nY = (i + 1);

				nX = (j - 1);
				if( (nX >= 0) && (nY < nH) ) {
					nPOS = nY * nBPW + nX;
					nR += COLOR16_R( pDATA[ nPOS ] );
					nG += COLOR16_G( pDATA[ nPOS ] );
					nB += COLOR16_B( pDATA[ nPOS ] );
				}

				nX = (j + 0);
				if( (nY < nH) ) {
					nPOS = nY * nBPW + nX;
					nR += (COLOR16_R( pDATA[ nPOS ] ) << 1);
					nG += (COLOR16_G( pDATA[ nPOS ] ) << 1);
					nB += (COLOR16_B( pDATA[ nPOS ] ) << 1);
				}

				nX = (j + 1);
				if( (nX < nW) && (nY < nH) ) {
					nPOS = nY * nBPW + nX;
					nR += COLOR16_R( pDATA[ nPOS ] );
					nG += COLOR16_G( pDATA[ nPOS ] );
					nB += COLOR16_B( pDATA[ nPOS ] );
				}

				pDATA[ i * nBPW + j ] = (uint16)(((nR >> 15) << 11) | ((nG >> 9) << 5) | (nB >> 4));
			}
		}
	}
}
