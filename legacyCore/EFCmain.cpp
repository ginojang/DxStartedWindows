#include "LegacyCore.h"
#include "LegacyDrawer.h"

#include "EFCmain.h"

#include "EFCkey.h"
#include "EFCgrp.h"
#include "EFCfnt.h"


EXMAINDATA MainData;


void EFC_mainInitialize( void )
{
	MEMSET( &MainData, 0, sizeof(EXMAINDATA) );

	MainData.isDebugDraw = FALSE;

	//
	
	//
#if _CHECK
	EFC_lnkInitialize();
#endif
	EFC_keyInitialize();
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
}

void EFC_mainPAUSE( void )
{
}

void EFC_mainRESUME( void )
{
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


void EFC_mainDRAW( void )
{
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
		if( EFC_rgnIsInRECT( &pDATA->rt, pPoint->nX, pPoint->nY ) == TRUE
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
