#include "LegacyCore.h"
#include "EFCmain.h"

#include "EFCkey.h"
#include "EFCpixel.h"
#include "EFCgrp.h"
#include "EFCthr.h"
#include "EFCfs.h"
#include "EFCfnt.h"


EXMAINDATA MainData;


void EFC_mainInitialize( void )
{
	MEMSET( &MainData, 0, sizeof(EXMAINDATA) );

	MainData.isDebugDraw = FALSE;


	// 아래 순서 바꾸지 말자
	EFC_knlInitialize();
	EFC_fntInitialize();

	//
	EFC_pxlInitialize();

	EFC_thrInitialize(); 
	
	//
#if _CHECK
	EFC_lnkInitialize();
#endif

	EFC_grpInitialize();

	EFC_keyInitialize();
	/// 초기화 추가시

	
	/// 쓰레드 실행
	//EFC_thrPLAY();

}

void EFC_mainFinalize( void )
{
	if( MainData.bFINAL == TRUE ) {
		return;
	}

	MainData.bFINAL = TRUE;

	EFC_keyFinalize();
#if _CHECK
	EFC_lnkFinalize();
#endif
	//
	EFC_thrFinalize();
	EFC_grpFinalize();
	//
	
	EFC_pxlFinalize();
	//
	EFC_fntFinalize();
	EFC_knlFinalize();
	
}

void EFC_mainPAUSE( void )
{
	if( MainData.nEXIT > 0 ) {
		if( MainData.nEXIT == 1 ) {
			EFC_knlEXIT();
		}
		MainData.nEXIT++;
		return;
	}

	EFC_miscBackLight( FALSE );

	//EFC_thrSTOP();
	//EFC_mdaPAUSE();
}

void EFC_mainRESUME( void )
{
	//EFC_grpSetCLIP( PixelData.rtMAIN.nX, PixelData.rtMAIN.nY, PixelData.rtMAIN.nW, PixelData.rtMAIN.nH );

	EFC_miscBackLight( TRUE );

/*
	EFC_grpSetRGB( RGB(0, 0, 0) );
	EFC_grpFillSCREEN();*/


	//EFC_mdaRESUME();
	//EFC_thrPLAY();
}



void EFC_mainKeyPRESS( sint32 nKEY )
{
	if( MainData.nKEY == nKEY ) {
		return;
	}

	MainData.nKEY = nKEY;
	
	EFC_keyADD( MainData.nKEY );
}

void EFC_TouchADD( LPEXPOINT pPoint, sint32 nType )
{
	sint32 nPOS;

	if (MainData.xTOUCH.nCOUNT == TOUCH_COUNT)
	{
		MainData.xTOUCH.nPOS = (MainData.xTOUCH.nPOS + 1) % TOUCH_COUNT;
		MainData.xTOUCH.nCOUNT--;
	}

	nPOS = (MainData.xTOUCH.nPOS + MainData.xTOUCH.nCOUNT) % TOUCH_COUNT;
	MainData.xTOUCH.nCOUNT++;

	MainData.xTOUCH.xDATA[nPOS].nType = nType;
	MainData.xTOUCH.xDATA[nPOS].stPoint.nX = pPoint->nX;
	MainData.xTOUCH.xDATA[nPOS].stPoint.nY = pPoint->nY;
}

LPEXTOUCHDATA EFC_TouchGET( void )
{
	LPEXTOUCHDATA pTOUCH = NULL;

	if( MainData.xTOUCH.nCOUNT > 0 ) {
		pTOUCH = &MainData.xTOUCH.xDATA[MainData.xTOUCH.nPOS];
		MainData.xTOUCH.nPOS = (MainData.xTOUCH.nPOS + 1) % TOUCH_COUNT;
		MainData.xTOUCH.nCOUNT--;
	}

	return pTOUCH;
}

void EFC_TouchReset( void )
{
	MEMSET( &MainData.xTOUCH, 0, sizeof(EXTOUCHLIST) );
}

void EFC_mainTouch(LPEXPOINT pPoint, sint32 nType)
{
	//EFC_grpTransformMousePos(pPoint);
	EFC_TouchADD( pPoint, nType );
}

void EFC_mainKeyRELEASE( void )
{
	MainData.nKEY = 0;
	EFC_keyADD( MainData.nKEY );
}

void EFC_mainSetNEXT( sint32 nNEXT )
{
#if _CHECK
	if( LinkData.xLINKs[LinkData.nCURR].destroy != NULL ) {
		LinkData.xLINKs[LinkData.nCURR].destroy();
	}

	//EFC_mdaVIBRATE(250);
	LinkData.bNEXT = TRUE;
	LinkData.nPREV = LinkData.nCURR;
	LinkData.nCURR = nNEXT;
#endif

	EFC_grpSetTRANS( 0, 0 );
	EFC_keyReset();
	EFC_TouchReset();

	//MC_knlGetFreeMemory();
	MainData.stateCount = 0;

#if _CHECK
	if( LinkData.xLINKs[LinkData.nCURR].init != NULL ) {
		LinkData.xLINKs[LinkData.nCURR].init();
	}
#endif
}

/*
void EFC_mainFLUSH( void )
{
#if defined( USE_TOUCH_ROTATE )
	{
		sint32 i, j, nW, nH, nSX, nSX1, nSX2;
		uint16 *pDATA2;

		EFC_mainTouchDRAW();

		pDATA2 = JC_imgGET( &PixelData.imgBACK );

		nW = EFC_GRP_GET_FRAME_BUFFER_WIDTH( PixelData.hScreen );
		nH = EFC_GRP_GET_FRAME_BUFFER_HEIGHT( PixelData.hScreen );

		if( PixelData.nBPP != 16 ) {
			uint32 *pDATA1, *pCOLOR;
			pDATA1 = (uint32 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( PixelData.hScreen );
			pCOLOR = (uint32 *)JC_memGET( PixelData.pCOLORS );
			//			for( i=0; i<nH; i++ ) {		// 400
			//				for( j=0; j<nW; j++ ) {	// 240
			//					pDATA1[i * nW + j] = pCOLOR[ pDATA2[(nW - 1 - j) * nH + i] ];
			//				}
			//			}
			for( i=0, nSX=0; i<nH; i++, nSX+=nW ) { // 400
				for( j=0, nSX1=nSX, nSX2=((nW - 1) * nH + i); j<nW; j++, nSX1++, nSX2-=nH ) {	// 240
					pDATA1[nSX1] = pCOLOR[ pDATA2[nSX2] ];
				}
			}
		} else {
			uint16 *pDATA1;
			pDATA1 = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( PixelData.hScreen );
			//			for( i=0; i<nH; i++ ) {		// 400
			//				for( j=0; j<nW; j++ ) {	// 240
			//					pDATA1[i * nW + j] = pDATA2[(nW - 1 - j) * nH + i];
			//				}
			//			}
			for( i=0, nSX=0; i<nH; i++, nSX+=nW ) { // 400
				for( j=0, nSX1=nSX, nSX2=((nW - 1) * nH + i); j<nW; j++, nSX1++, nSX2-=nH ) {	// 240
					pDATA1[nSX1] = pDATA2[nSX2];
				}
			}
		}

		MC_grpCopyFrameBuffer( PixelData.hReal, 0, PixelData.nANNUN, nW, nH, PixelData.hScreen, 0, PixelData.nANNUN, &PixelData.hGC );

		MC_grpFlushLcd( 0, PixelData.hReal, 0, PixelData.nANNUN, EFC_GRP_GET_FRAME_BUFFER_WIDTH( PixelData.hReal ), EFC_GRP_GET_FRAME_BUFFER_HEIGHT( PixelData.hReal ) );
		return;
	}

	// COPY SCREEN FRAME BUFF
	if( PixelData.nBPP != 16 ) {
		uint32 *pDATA1;
		uint16 *pDATA2;
		uint32 *pCOLOR;
		sint32 nSX1, nSY1, nSX2, nSY2, nEX1, nEY1;

		pDATA1 = (uint32 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( PixelData.hScreen );
		pCOLOR = (uint32 *)JC_memGET( PixelData.pCOLORS );

		pDATA2 = JC_imgGET( &PixelData.imgBACK );

		for( nSY1=(PixelData.imgBACK.nW * (PixelData.nANNUN + PixelData.imgBACK.nH - 1)), nEY1=(nSY1 - (PixelData.imgBACK.nW * PixelData.imgBACK.nH)), nSY2=(PixelData.imgBACK.nW * (PixelData.imgBACK.nH - 1)); nSY1>nEY1; nSY1-=PixelData.imgBACK.nW, nSY2-=PixelData.imgBACK.nW ) {
			for( nSX1=(nSY1 + PixelData.imgBACK.nW - 1), nEX1=(nSY1 - 1), nSX2=(nSY2 + PixelData.imgBACK.nW - 1); nSX1>nEX1; nSX1--, nSX2-- ) {
				pDATA1[nSX1] = pCOLOR[ pDATA2[nSX2] ];
			}
		}
	}

	// FLUSH LCD
#if defined( SKT_WIPI )
	MC_grpFlushLcd( 0, PixelData.hScreen, 0, PixelData.nANNUN, PixelData.imgBACK.nW, PixelData.imgBACK.nH );
#elif defined( KTF_WIPI )
	MC_grpFillRect( PixelData.hReal, 0, PixelData.nANNUN, 1, 1, &PixelData.hGC );
	MC_grpFillRect( PixelData.hReal, PixelData.rtMAIN.nW - 1, PixelData.nANNUN + PixelData.rtMAIN.nH - 1, 1, 1, &PixelData.hGC );
	MC_grpFlushLcd( 0, PixelData.hScreen, 0, PixelData.nANNUN, PixelData.imgBACK.nW, PixelData.imgBACK.nH );
#elif defined( LGT_WIPI )
	MC_grpCopyFrameBuffer( PixelData.hReal, 0, PixelData.nANNUN, PixelData.rtMAIN.nW, PixelData.rtMAIN.nH, PixelData.hScreen, 0, PixelData.nANNUN, &PixelData.hGC );
	MC_grpFlushLcd( 0, PixelData.hReal, 0, PixelData.nANNUN, PixelData.imgBACK.nW, PixelData.imgBACK.nH );
#endif
}*/

void EFC_mainDRAW( void )
{
	// 한번 종료 코드 들어갔을 경우
	if( MainData.bFINAL == TRUE ) {
		return;
	}

	if( MainData.nEXIT > 0 ) {
		if( MainData.nEXIT == 1 ) {
			EFC_knlEXIT();
		}
		MainData.nEXIT++;
		return;
	}


#if _CHECK
	// TOUCH
	while( MainData.xTOUCH.nCOUNT > 0 ) {
		if( LinkData.xLINKs[LinkData.nCURR].mTouch != NULL ) {
			LinkData.xLINKs[LinkData.nCURR].mTouch( EFC_TouchGET() );
		} else {
			// 터치 함수가 없을 경우 터치를 누적하지 않는다.
			EFC_TouchReset();
		}
	}

	// KEY
	while( EFC_keyCOUNT() > 0 ) {
		if( LinkData.xLINKs[LinkData.nCURR].mKeyPressed != NULL ) {
			LinkData.xLINKs[LinkData.nCURR].mKeyPressed( EFC_keyGET() );
		} else {
			// 키 함수가 없을 경우 키를 누적하지 않는다.
			EFC_keyReset();
		}
	}

	// RUN
	do {
		LinkData.bNEXT = FALSE;
		if( LinkData.xLINKs[LinkData.nCURR].mRun != NULL ) {
			LinkData.xLINKs[LinkData.nCURR].mRun();
		}
	} while( LinkData.bNEXT == TRUE );

	// MEDIA
	//EFC_mdaRUN()

	// DRAW
	if( LinkData.xLINKs[LinkData.nCURR].mPaint != NULL ) {
		LinkData.xLINKs[LinkData.nCURR].mPaint();
	}

#endif


	EFC_grpFLUSH();
	MainData.stateCount++;
}

void EFC_mainInitTOUCH( void )
{
	MEMSET( &MainData.xTOUCHKEY, 0, sizeof(MainData.xTOUCHKEY) );
}

void EFC_mainSetTOUCH( sint32 nKEY, sint32 nKEYS, sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	LPEXTOUCHKEYDATA pDATA;
	if( MainData.xTOUCHKEY.nTOUCHES < TOUCH_COUNT ) 
	{
		pDATA = &MainData.xTOUCHKEY.xDATA[MainData.xTOUCHKEY.nTOUCHES++];
		EFC_rgnSetRECT( &pDATA->rt, nX - 2, nY - 2, nW + 4, nH + 4 );
		pDATA->nKEY = nKEY;
		pDATA->nKEYS = nKEYS;
	}
}


ubool EFC_mainKeyTOUCH( LPEXPOINT pPoint, sint32 nType )
{
	LPEXTOUCHKEYDATA pDATA;
	sint32 i, j;
	ubool isFind = FALSE;
	
	for( i=(MainData.xTOUCHKEY.nTOUCHES - 1); i>=0; i-- ) 
	{
		pDATA = &MainData.xTOUCHKEY.xDATA[i];
		if( EFC_rgnIsInRECT( &pDATA->rt, pPoint->nX, (pPoint->nY - PixelData.nANNUN) ) == TRUE
			&& pDATA->nTouchType == nType)
		{
			for( j=0; j<pDATA->nKEYS; j++ )
			{
				EFC_mainKeyPRESS( pDATA->nKEY );
			}

			isFind = TRUE;
			break;
		}
	}

	return isFind;	
}
