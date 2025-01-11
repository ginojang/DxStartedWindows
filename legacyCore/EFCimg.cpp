#include "LegacyCore.h"

#include "EFCimg.h"
#include "ABMReader.h"
#include "ResourceManager.h"

#define MAX_IMAGES 256

void EFC_imgLoadARRAY( LPEXIMAGE pIMGs, schar *pStr, sint32 *pIDXs, sint32 nCount, ubool bORDER, ubool bMIRROR )
{
	TBMPREADER xTBmp;
	EXFILE xFile;
	sint32 i, nIDX = 0, nIMG = 0;
	ubool bBREAK = FALSE;

	MEMSET( &xFile, 0, sizeof(xFile) );
	xFile.pBuff = g_resourceManager.LoadFromAssets( pStr );
	if( EFC_memGET( xFile.pBuff ) == NULL ) 
	{
		return;
	}

	MEMSET( &xTBmp, 0, sizeof(xTBmp) );
	ABMReader_Load( &xFile, &xTBmp );

	for( i=0; i<nCount; i++ ) {
		while( nIDX < pIDXs[i] ) {
			if( ABMReader_skip( &xFile, &xTBmp ) == FALSE ) {
				bBREAK = TRUE;
				break;
			}

			nIDX++;
			if( bORDER == FALSE ) {
				nIMG++;
			}
		}

		if( bBREAK == TRUE ) {
			break;
		}

		EFC_imgFREE( &pIMGs[nIMG] );

		if( bMIRROR == FALSE ) {
			if( ABMReader_real( &xFile, &pIMGs[nIMG], &xTBmp) == FALSE ) {
				bBREAK = TRUE;
			}
		} else {
			if( ABMReader_next( &xFile, &pIMGs[nIMG], &xTBmp ) == FALSE ) {
				bBREAK = TRUE;
			}
		}

		if( bBREAK == TRUE ) 
		{
			break;
		}

		pIMGs[nIMG].nMoveX = xTBmp.refX;
		pIMGs[nIMG].nMoveY = xTBmp.refY;
		pIMGs[nIMG].nPalette = 0;

		nIDX++;
		nIMG++;
	}

	ABMReader_Free( &xTBmp );
	EFC_memFREE( xFile.pBuff );
}

void EFC_imgLoadCOUNT( LPEXIMAGE pIMGs, schar *pStr, sint32 nIDX, sint32 nCount )
{
	sint32 nARRAYs[MAX_IMAGES];
	sint32 i;

	for( i=0; i<nCount; i++ ) {
		nARRAYs[i] = nIDX + i;
	}

	EFC_imgLoadARRAY( pIMGs, pStr, nARRAYs, nCount, TRUE, TRUE );
}

void EFC_imgLoadIMAGE( LPEXIMAGE pIMGs, schar *pStr, sint32 nIDX )
{
	sint32 nARRAYs[1];

	nARRAYs[0] = nIDX;

	EFC_imgLoadARRAY( pIMGs, pStr, nARRAYs, 1, TRUE, TRUE );
}

void EFC_imgFREE( LPEXIMAGE pIMG )
{
	EFC_memFREE( pIMG->pBuff );

	MEMSET( pIMG, 0, sizeof(EXIMAGE) );
}

void EFC_imgFreeARRAY( LPEXIMAGE pIMGs, sint32 nCount )
{
	sint32 i;

	for( i=nCount-1; i>=0; i-- ) {
		EFC_imgFREE( &pIMGs[i] );
	}

	MEMSET( pIMGs, 0, (sizeof(EXIMAGE) * nCount) );
}
/*

sint32 EFC_imgGetMIRROR( sint32 nCurr, sint32 nNext )
{
	sint32 nCALC[4];

	nCALC[0] = (nNext & 0x00000003);
	nCALC[1] = ((5 - nNext) & 0x00000003);
	nCALC[2] = ((nNext + 2) & 0x00000003);
	nCALC[3] = ((3 - nNext) & 0x00000003);

	return nCALC[nCurr];
}
*/

void EFC_imgMIRROR( LPEXIMAGE pIMG, LPEXMOVE rMov, sint32 nMIRROR )
{	
	switch( nMIRROR ) {
	case EN_MIRROR_NONE :
		rMov->moveX = pIMG->nMoveX;
		rMov->moveY = pIMG->nMoveY;
		break;
	case EN_MIRROR_HORIZON :
		rMov->moveX = (sint16)(pIMG->nW - pIMG->nMoveX);
		rMov->moveY = pIMG->nMoveY;
		break;
	case EN_MIRROR_VERTICAL :
		rMov->moveX = pIMG->nMoveX;
		rMov->moveY = (sint16)(pIMG->nH - pIMG->nMoveY);
		break;
	case EN_MIRROR_BOTH :
		rMov->moveX = (sint16)(pIMG->nW - pIMG->nMoveX);
		rMov->moveY = (sint16)(pIMG->nH - pIMG->nMoveY);
		break;
	}
}
