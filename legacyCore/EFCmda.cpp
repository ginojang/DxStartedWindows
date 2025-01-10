#include "EXdefault.h"
#include "EFCmda.h"


#define IDS_START	0
// EFCmda.c
#define MDA_BACKS -1
#define MDA_EFFECTS 5
#define MDA_RESERVE 1

#ifndef MC_MDA_STATUS_STARTED
#define MC_MDA_STATUS_STARTED 2
#endif

EXMDADATA MdaData;


void EFC_mdaSET( LPEXPLAY pPLAY, sint32 nID, ubool bREPEAT, sint32 nPLAY, sint32 nDEPTH );

void EFC_mdaMediaCB( MC_MdaClip* pClip, sint32 nSTATUS )
{
}

void EFC_mdaSET( LPEXPLAY pPLAY, sint32 nID, ubool bREPEAT, sint32 nPLAY, sint32 nDEPTH )
{
	pPLAY->nID = nID;
	pPLAY->bREPEAT = bREPEAT;
	pPLAY->nPLAY = (sint8)nPLAY;
	pPLAY->nDEPTH = (sint8)nDEPTH;
}

void EFC_mdaSetKEYTONE( void )
{
}

void EFC_mdaUnSetKEYTONE( void )
{
}


void EFC_mdaInitialize( sint32 nBufSize )
{
	
}


void EFC_mdaFinalize( void )
{
	EFC_mdaStopALL();
	EFC_mdaFreeALL();

	if( MdaData.pCLIP != NULL ) {
		MC_mdaClipFree( MdaData.pCLIP );
	}

	EFC_mdaUnSetKEYTONE();

	MEMSET( &MdaData, 0, sizeof(EXMDADATA) );
}

inline sint32 EFC_mdaGetINDEX( sint32 nID )
{
	sint32 nIDX;
	nIDX = (nID - IDS_START);

	if(nIDX >= 100)
	{
		nIDX -= 71;
	}
	else if( nIDX > (DF_MDA_COUNT - 1) ) {
		return -1;
	}
	return nIDX;
}

inline sint32 EFC_mdaLOAD( sint32 nID )
{
	sint32 nFIND, nRET;
	schar szBuff[64];

	return 0;
}

void EFC_mdaFREE(sint32 nID)
{

}

void EFC_mdaFreeALL( void )
{
	sint32 i;
	MEMSET( MdaData.pDATAs, 0, sizeof(MdaData.pDATAs) );
}

inline void EFC_mdaSetPLAY( sint32 nID, ubool bREPEAT, sint32 nPLAY, sint32 nDEPTH )
{
	EFC_mdaSET( &MdaData.xResv, nID, bREPEAT, nPLAY, nDEPTH );
	EFC_mdaSET( &MdaData.xBack, -1, FALSE, MDA_EFFECTS, 0 );
}

void EFC_mdaPlayBACK( sint32 nID )
{
	if( (MdaData.xPlay.nDEPTH > 0) || (MdaData.xPlay.nID == nID) || (MdaData.xBack.nID == nID) ) {
		return;
	}
	
	if( (MdaData.xPlay.nID > 0) && (MdaData.xPlay.nPLAY > 0) ) {
		return;
	}

	EFC_mdaSET( &MdaData.xBack, nID, TRUE, MDA_RESERVE, 0 );
}

void EFC_mdaPlayCONTINUE( sint32 nID )
{
	if( (MdaData.xPlay.nDEPTH > 0) || (MdaData.xPlay.nID == nID) || (MdaData.xBack.nID == nID) ) {
		return;
	}

	if( (MdaData.xPlay.nID > 0) && (MdaData.xPlay.nPLAY > 0) ) {
		return;
	}

	EFC_mdaSET( &MdaData.xBack, nID, TRUE, MDA_BACKS, 0 );
}

// 사용시 끝나는 시점에서 STOP 해줘야 한다
void EFC_mdaPlayREPEAT( sint32 nID )
{
	/*
	if( MdaData.xPlay.nID == nID ) {
		return;
	}

	EFC_mdaSetPLAY( nID, TRUE, MDA_BACKS, 1 );*/
}

void EFC_mdaPlayEFFECT( sint32 nID )
{
	//EFC_mdaSetPLAY( nID, FALSE, MDA_EFFECTS, 0 );
}

void EFC_mdaPlayDATA( sint32 nID )
{
	//EFC_mdaSetPLAY( nID, FALSE, MDA_BACKS, 0 );
}

inline void EFC_mdaPlayREAL( void )
{
	/*
	sint32 nRET;
	sint32 nFIND;

	if( MdaData.xPlay.nID > 0 ) {
		sint32 nRand = EFC_knlGetRAND()%(1000*10);

		EFC_mdaStopREAL();

		nFIND = EFC_mdaLOAD( MdaData.xPlay.nID );

		if( nFIND < 0 ) 
		{
			return;
		}

#if !defined(ANDROID)
		//RETOUCH_11
		nRET = MC_mdaClipPutData( MdaData.pCLIP, EFC_memGET(MdaData.pDATAs[nFIND]), MdaData.pDATAs[nFIND]->nSize );
		if( nRET < 0 ) 
		{
			return;
		}
#endif

		if( EFC_mdaVOLUME( 0 ) == FALSE )
		{
			return;
		}

		MdaData.nSTATUS = MC_MDA_STATUS_STARTED;

		MdaData.actSndTime = ThreadData.nNOW + 5000 + nRand;

		nRET = MC_mdaPlay( MdaData.pCLIP, MdaData.xPlay.bREPEAT );

		if (nRET < 0)
		{
			assert_msg(FALSE, "MC_mdaPlay  실패!   %d", nRET);
			return ;
		}
	}
	*/
}

inline void EFC_mdaStopREAL( void )
{
	
#if !defined( ANDROID )
	if( MdaData.nSTATUS == MC_MDA_STATUS_STOPPED ) 
	{
		return;
	}
#endif

	if( MdaData.pCLIP != NULL ) 
	{
		sint32 nRst = MC_mdaStop( MdaData.pCLIP );
		MdaData.nSTATUS = MC_MDA_STATUS_STOPPED;

#if defined( ANDROID )
		MC_mdaClipFreePlayer( MdaData.pCLIP );
		MC_mdaClipFree( MdaData.pCLIP );
		MdaData.pCLIP = NULL;
#elif defined( KT_WIPI )
		MC_mdaClipClearData( MdaData.pCLIP );
#elif defined( LGT_WIPI )
		nRst = MC_mdaClipClearData( MdaData.pCLIP );
		nRst = MC_mdaClipFreePlayer( MdaData.pCLIP );
#endif
	}
}

inline void EFC_mdaPLAY( sint32 nID, ubool bREPEAT, sint32 nPLAY, sint32 nDEPTH )
{
	sint32 nFIND;

	EFC_mdaSET( &MdaData.xPlay, nID, bREPEAT, nPLAY, nDEPTH );

	EFC_mdaPlayREAL();
}

void EFC_mdaStopALL( void )
{
	EFC_mdaSET( &MdaData.xResv, -1, FALSE, MDA_EFFECTS, 0 );
	EFC_mdaSET( &MdaData.xPlay, -1, FALSE, MDA_EFFECTS, 0 );
	EFC_mdaSET( &MdaData.xBack, -1, FALSE, MDA_EFFECTS, 0 );

	EFC_mdaStopREAL();
}

void EFC_mdaStopWAIT( void )
{
	EFC_mdaSET( &MdaData.xResv, -1, FALSE, MDA_EFFECTS, 0 );
	EFC_mdaSET( &MdaData.xPlay, IDS_START, FALSE, MDA_EFFECTS, 0 );
	EFC_mdaSET( &MdaData.xBack, -1, FALSE, MDA_EFFECTS, 0 );

	EFC_mdaStopREAL();
}

void EFC_mdaPAUSE( void )
{
	EFC_mdaUnSetKEYTONE();
	EFC_mdaStopREAL();
}

void EFC_mdaRESUME( void )
{
	EFC_mdaSetKEYTONE();
	if( MdaData.xPlay.bREPEAT == TRUE ) {
		EFC_mdaSET( &MdaData.xResv, MdaData.xPlay.nID, MdaData.xPlay.bREPEAT, MdaData.xPlay.nPLAY, MdaData.xPlay.nDEPTH );
	}
}

void EFC_mdaRUN( void )
{
	/*
	if( MdaData.xResv.nID > 0 ) 
	{
		EFC_mdaPLAY( MdaData.xResv.nID, MdaData.xResv.bREPEAT, MdaData.xResv.nPLAY, MdaData.xResv.nDEPTH );
		EFC_mdaSET( &MdaData.xResv, -1, FALSE, MDA_EFFECTS, 0 );
		EFC_mdaSET( &MdaData.xBack, -1, FALSE, MDA_EFFECTS, 0 );
	}

	if( MdaData.xBack.nID > 0 ) {
		if( MdaData.xBack.nPLAY < 1 ) {
			EFC_mdaPLAY( MdaData.xBack.nID, MdaData.xBack.bREPEAT, MdaData.xBack.nPLAY, MdaData.xBack.nDEPTH );
			EFC_mdaSET( &MdaData.xBack, -1, FALSE, MDA_EFFECTS, 0 );
		}
	}

	if( MdaData.nDIFFs > 500 ) {
		MdaData.nDIFFs = 0;
		if( (MdaData.xPlay.nID > 0) && (MdaData.xPlay.nPLAY > 0) ) {
			MdaData.xPlay.nPLAY--;
		}
		if( (MdaData.xBack.nID > 0) && (MdaData.xBack.nPLAY > 0) ) {
			MdaData.xBack.nPLAY--;
		}
	}

	MdaData.nDIFFs += ThreadData.nDIFF;
	*/
}

void EFC_mdaVIBRATE( sint32 nTimeOut )
{
	/*
	if( MdaData.bVIBRATE == FALSE ) {
		return;
	}

	MC_mdaVibrator( MdaData.nVIBRATEPOWER, nTimeOut );
	*/
}

ubool EFC_mdaVOLUME( sint32 nSTATUS )
{
	sint32 nVOLUME = 0;

	MdaData.nVOLUME = (sint8)((MdaData.nVOLUME + MdaData.nVOLUMEs + nSTATUS + 1) % (MdaData.nVOLUMEs + 1));

	if( MdaData.nVOLUME == 0 ) 
	{
		EFC_mdaStopALL();
		return FALSE;
	}

#if defined( KTF_NOT_VOLUMES )
	return TRUE;
#endif

	if( MdaData.nVOLUMEs > 0 )
	{
		nVOLUME = ((MdaData.nVOLUME * 100) / MdaData.nVOLUMEs);
	}

#if defined(ANDROID)
	MC_mdaSetVolume( nVOLUME );

	if( MdaData.pCLIP != NULL ) {
		MC_mdaClipSetVolume( MdaData.pCLIP, nVOLUME );
	}
#elif defined( SKT_WIPI )
	if( MdaData.pCLIP != NULL ) {
		MC_mdaClipSetVolume( MdaData.pCLIP, nVOLUME );
	}
#elif defined( KT_WIPI )
	if( MdaData.pCLIP != NULL ) {
		MC_mdaClipSetVolume( MdaData.pCLIP, nVOLUME );
	}
#elif defined( LGT_WIPI )
	if( MdaData.pCLIP != NULL ) {
		MC_mdaClipSetVolume( MdaData.pCLIP, nVOLUME );
	}
#endif

	return TRUE;
}