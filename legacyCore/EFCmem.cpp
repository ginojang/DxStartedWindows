#include "LegacyCore.h"
#include "ResourceManager.h"


ubyte *EFC_memGET( LPEXBUFF pBuff )
{
	return g_resourceManager.GetBufferBits(pBuff);
}

ubool EFC_memSET(LPEXBUFF pBuff, sint32 nSize)
{
	pBuff = EFC_memALLOC(nSize);
	return TRUE;
}
LPEXBUFF EFC_memALLOC(sint32 nSize)
{
	return g_resourceManager.AllocBuffer(nSize);
}

LPEXBUFF EFC_memCOPY( LPEXBUFF pBuff )
{
	return g_resourceManager.AllocWithBuffer(pBuff);
}

void EFC_memFREE( LPEXBUFF pBuff )
{
	g_resourceManager.ReleaseBuffer(pBuff);
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
