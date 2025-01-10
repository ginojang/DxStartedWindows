#include "EXdefault.h"
#include "EFCmem.h"

#include "ResourceManager.h"

EXMEMDATA MemData;

void EFC_memInitialize( void )
{
	MEMSET( &MemData, 0, sizeof(MemData) );
}

void EFC_memFinalize( void )
{
}

void EFC_memReset( void )
{
}

void EFC_memCheck( void )
{
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

	return NULL;
}

ubyte *EFC_memGET( LPEXBUFF pBuff )
{
	if( pBuff == NULL ) 
	{
		return NULL;
	}

	return pBuff->pByte;
}


ubool EFC_memSET( LPEXBUFF pBuff, sint32 nSize )
{
	if( pBuff == NULL || nSize < 1)
		return FALSE;
	
	MEMSET(pBuff, 0, sizeof(EXBUFF));
	pBuff->nBuffID = g_resourceManager.AllocBuffer( nSize );

	if( pBuff->nBuffID == 0 )
	{
		return FALSE;
	}

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
		return NULL;
	}

	return pBuff;
}

void EFC_memFREE( LPEXBUFF pBuff )
{
	if( pBuff == NULL ) 
	{
		return;
	}

	if( pBuff->nBuffID != 0) 
	{
		g_resourceManager.ReleaseBuffer( pBuff->nBuffID );
	}
}

void EFC_memFreeALL( void )
{
	sint32 i;

	for( i=0; i<DF_MEM_COUNT; i++ ) 
	{
		EFC_memFREE( &MemData.xBuffs[i] );
	}
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
