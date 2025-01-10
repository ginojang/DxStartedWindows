#include "EXdefault.h"
#include "EFCmem.h"

EXMEMDATA MemData;

void EFC_memInitialize( void )
{
	MEMSET( &MemData, 0, sizeof(MemData) );
	MemData.nTOTAL = MC_knlGetTotalMemory();
	MemData.nFREE = MC_knlGetFreeMemory();
	MemData.nUSEDMEM = 0;
}

void EFC_memFinalize( void )
{
	EFC_memFreeALL();
}

void EFC_memReset( void )
{
	if( MemData.bLOCK == TRUE ) {
		MemData.bLOCK = FALSE;
		MemData.nCURR = MC_knlGetFreeMemory();
	}
}

void EFC_memCheck( void )
{
	MemData.nCURR = MC_knlGetFreeMemory();
}

LPEXBUFF EFC_memEMPTY( void )
{
	sint32 i;

	for( i=0; i<DF_MEM_COUNT; i++ ) 
	{
		if( EFC_memGET( &MemData.xBuffs[i] ) == NULL ) 
		{
			return &MemData.xBuffs[i];
		}
	}

	//assert_msg(0, "EFC_memEMPTY : 메모리 배열 부족");
	return NULL;
}

ubyte *EFC_memGET( LPEXBUFF pBuff )
{
	if( pBuff == NULL ) 
	{
		return NULL;
	}


#if defined( KT_WIPI )
	if( pBuff->nBuffID == 0 )
	{
		return pBuff->pByte;
	}

	pBuff->pByte = GETDATA( pBuff->nBuffID );
#endif

	return pBuff->pByte;
}


ubool EFC_memSET( LPEXBUFF pBuff, sint32 nSize )
{
	if( pBuff == NULL ) {
		return FALSE;
	}

	MEMSET( pBuff, 0, sizeof(EXBUFF) );
	if( nSize < 1 ) {
		return FALSE;
	}

	pBuff->nBuffID = MALLOC( nSize );

	//assert_msg(pBuff->nBuffID, "EFC_memSET : 메모리 부족 MALLOC실패");

	if( pBuff->nBuffID == 0 )
	{
		return FALSE;
	}

	MemData.bLOCK = TRUE;
	
	pBuff->pByte = (M_Byte *)GETDATA( pBuff->nBuffID );

	//assert_msg(pBuff->pByte, "EFC_memSET : GETDATA실패");

	if( pBuff->pByte == NULL ) 
	{		
		return FALSE;
	}

	MEMSET( pBuff->pByte, 0, nSize );
	pBuff->nSize = nSize;

	return TRUE;
}

LPEXBUFF EFC_memCOPY( LPEXBUFF pBuff )
{
	LPEXBUFF pEmpty;

	if( EFC_memGET( pBuff ) == NULL ) 
	{
		return NULL;
	}

	pEmpty = EFC_memALLOC( pBuff->nSize );
	if( (EFC_memGET( pEmpty ) == NULL) || (EFC_memGET( pBuff ) == NULL) )
	{
		return NULL;
	}

	MEMCPY( pEmpty->pByte, pBuff->pByte, pBuff->nSize );
	return pEmpty;
}

LPEXBUFF EFC_memALLOC( sint32 nSize )
{
	LPEXBUFF pBuff;

	pBuff = EFC_memEMPTY();

	if( (pBuff == NULL) || (EFC_memSET( pBuff, nSize ) == FALSE) ) 
	{
		//assert_msg(pBuff, "EFC_memALLOC : 메모리 부족", nSize);
		return NULL;
	}

	MemData.bLOCK = TRUE;
	MemData.nUSEDMEM += nSize;
	MemData.nALLOC++;

	return pBuff;
}

void EFC_memFREE( LPEXBUFF pBuff )
{
	if( pBuff == NULL ) 
	{
		return;
	}

	if( pBuff->nSize > 0 ) 
	{
		if( pBuff->nBuffID != 0) 
		{
			sint32 nSize = pBuff->nSize;
			FREE( pBuff->nBuffID );
			MemData.nALLOC--;
			MemData.bLOCK = TRUE;
			MemData.nUSEDMEM -= nSize;
		}
	}

	MEMSET( pBuff, 0, sizeof(EXBUFF) );
}

void EFC_memFreeALL( void )
{
	sint32 i;

	for( i=0; i<DF_MEM_COUNT; i++ ) 
	{
		EFC_memFREE( &MemData.xBuffs[i] );
	}



	MEMSET( MemData.xBuffs, 0, sizeof(MemData.xBuffs) );
}

void EFC_fxtCHANGE( LPEXBUFF pBuff, schar szOld, schar szNew )
{
	schar *pChar, *pPrev;
	schar szBuff[2];

	szBuff[0] = szOld;
	szBuff[1] = 0;

	pChar = (schar *)EFC_memGET( pBuff );
	if( pChar == NULL )
	{
		return;
	}

	pPrev = pChar;
	while( pChar != NULL ) {
		pChar = (schar *)STRSTR( (schar *)pPrev, szBuff );
		if( pChar == NULL ) {
			break;
		}

		pChar[0] = szNew;

		pChar++;
		pPrev = pChar;
	}
}
