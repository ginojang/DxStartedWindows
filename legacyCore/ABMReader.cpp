#include "LegacyCore.h"
#include "BinaryParser.h"
#include "ABMReader.h"



sint32 KBitmap_GetSize( sint32 nW, sint32 nH )
{
	return (WIDTH_BYTES( nW ) * nH);
}

#if !defined(PIXEL_MODE)
void KBitmap_GetFileHeader( LPEXBITMAPFILEHEADER pHeader, LPEXBUFF pBuff )
{
	EXFILE xFile;

	MEMSET( &xFile, 0, sizeof(EXFILE) );
	xFile.pBuff = pBuff;

	MEMSET( pHeader, 0, sizeof(EXBITMAPFILEHEADER) );

	pHeader->bfType      = EFC_fsReadUint16( &xFile );
	pHeader->bfSize      = EFC_fsReadUint32( &xFile );
	pHeader->bfReserved1 = EFC_fsReadUint16( &xFile );
	pHeader->bfReserved2 = EFC_fsReadUint16( &xFile );
	pHeader->bfOffBits   = EFC_fsReadUint32( &xFile );
}

void KBitmap_SetFileHeader( LPEXBITMAPFILEHEADER pHeader, LPEXBUFF pBuff )
{
	EXFILE xFile;

	MEMSET( &xFile, 0, sizeof(EXFILE) );
	xFile.pBuff = pBuff;

	EFC_fsWriteUint16( &xFile, pHeader->bfType );
	EFC_fsWriteUint32( &xFile, pHeader->bfSize );
	EFC_fsWriteUint16( &xFile, pHeader->bfReserved1 );
	EFC_fsWriteUint16( &xFile, pHeader->bfReserved2 );
	EFC_fsWriteUint32( &xFile, pHeader->bfOffBits );
}

void KBitmap_GetInfoHeader( LPEXBITMAPINFOHEADER pHeader, LPEXBUFF pBuff )
{
	EXFILE xFile;

	MEMSET( &xFile, 0, sizeof(EXFILE) );
	xFile.pBuff = pBuff;

	EFC_fsSetPOS( &xFile, EXBITMAPFILEHEADERSIZE );

	MEMSET( pHeader, 0, sizeof(EXBITMAPINFOHEADER) );

	pHeader->biSize          = EFC_fsReadUint32( &xFile );
	pHeader->biWidth         = EFC_fsReadSint32( &xFile );
	pHeader->biHeight        = EFC_fsReadSint32( &xFile );
	pHeader->biPlanes        = EFC_fsReadUint16( &xFile );
	pHeader->biBitCount      = EFC_fsReadUint16( &xFile );
	pHeader->biCompression   = EFC_fsReadUint32( &xFile );
	pHeader->biSizeImage     = EFC_fsReadUint32( &xFile );
	pHeader->biXPelsPerMeter = EFC_fsReadSint32( &xFile );
	pHeader->biYPelsPerMeter = EFC_fsReadSint32( &xFile );
	pHeader->biClrUsed       = EFC_fsReadUint32( &xFile );
	pHeader->biClrImportant  = EFC_fsReadUint32( &xFile );
}

void KBitmap_SetInfoHeader( LPEXBITMAPINFOHEADER pHeader, LPEXBUFF pBuff )
{
	EXFILE xFile;

	MEMSET( &xFile, 0, sizeof(EXFILE) );
	xFile.pBuff = pBuff;

	EFC_fsSetPOS( &xFile, EXBITMAPFILEHEADERSIZE );

	EFC_fsWriteUint32( &xFile, pHeader->biSize );
	EFC_fsWriteUint32( &xFile, pHeader->biWidth );
	EFC_fsWriteUint32( &xFile, pHeader->biHeight );
	EFC_fsWriteUint16( &xFile, pHeader->biPlanes );
	EFC_fsWriteUint16( &xFile, pHeader->biBitCount );
	EFC_fsWriteUint32( &xFile, pHeader->biCompression );
	EFC_fsWriteUint32( &xFile, pHeader->biSizeImage );
	EFC_fsWriteUint32( &xFile, pHeader->biXPelsPerMeter );
	EFC_fsWriteUint32( &xFile, pHeader->biYPelsPerMeter );
	EFC_fsWriteUint32( &xFile, pHeader->biClrUsed );
	EFC_fsWriteUint32( &xFile, pHeader->biClrImportant );
}

LPEXBUFF KBitmap_Load( LPEXBUFF pBuff, sint32 nW, sint32 nH )
{
	EXBITMAPFILEHEADER bfHeader;
	EXBITMAPINFOHEADER biHeader;
	uint32 nHEADER, nDATA, nSIZE;

	nHEADER = EXBITMAPFILEHEADERSIZE + EXBITMAPINFOHEADERSIZE + (COLORS << 2);
	nDATA = KBitmap_GetSize( nW, nH );
	nSIZE = nHEADER + nDATA;

	if( pBuff == NULL ) {
		pBuff = EFC_memALLOC( nSIZE );
	}

	if( (pBuff->nSize < 1) || ((uint32)pBuff->nSize != nSIZE) ) {
		EFC_memFREE( pBuff );
		pBuff = EFC_memALLOC( nSIZE );
	}

	// BITMAPFILEHEADER
	MEMSET( &bfHeader, 0, sizeof(EXBITMAPFILEHEADER) );
	bfHeader.bfType      = 0x4d42;
	bfHeader.bfSize      = nSIZE;
	bfHeader.bfReserved1 = 1;
	bfHeader.bfReserved2 = 0;
	bfHeader.bfOffBits   = nHEADER;
	KBitmap_SetFileHeader( &bfHeader, pBuff );

	// BITMAPINFOHEADER
	MEMSET( &biHeader, 0, sizeof(EXBITMAPINFOHEADER) );
	biHeader.biSize          = EXBITMAPINFOHEADERSIZE;
	biHeader.biWidth         = nW;
	biHeader.biHeight        = nH;
	biHeader.biPlanes        = 1;
	biHeader.biBitCount      = 8;
	biHeader.biCompression   = 0;
	biHeader.biSizeImage     = nDATA;
	biHeader.biXPelsPerMeter = 0;
	biHeader.biYPelsPerMeter = 0;
	biHeader.biClrUsed       = 256;
	biHeader.biClrImportant  = 256;
	KBitmap_SetInfoHeader( &biHeader, pBuff );

	return pBuff;
}

void KBitmap_MirrorPixel( ubyte *pByte, sint32 nX1, sint32 nY1, sint32 nX2, sint32 nY2, sint32 nBPL, sint32 nBPP )
{
	sint32 nNext1, nNext2;
	sint32 nBytePerPixel;

	nNext1 = (nY1 * nBPL) + ((nX1 * nBPP) >> 3);
	nNext2 = (nY2 * nBPL) + ((nX2 * nBPP) >> 3);

	nBytePerPixel = (nBPP >> 3);
	if( nBytePerPixel < 1 ) { // 비트 연산
		ubyte nBit1, nBit2;
		ubyte nByte1, nByte2;

		nBit1 = (ubyte)(0x01 << (nX1 & 0x00000007));
		nBit2 = (ubyte)(0x01 << (nX2 & 0x00000007));

		nByte1 = (ubyte)(pByte[nNext1] & nBit1);
		nByte2 = (ubyte)(pByte[nNext2] & nBit2);

		if( nByte2 == 0 ) {
			pByte[nNext1] &= ~nBit1;
		} else {
			pByte[nNext1] |= nBit1;
		}

		if( nByte1 == 0 ) {
			pByte[nNext2] &= ~nBit2;
		} else {
			pByte[nNext2] |= nBit2;
		}
	} else { // 바이트 연산
		sint32 nTemp = 0;
		MEMCPY( &nTemp, &pByte[nNext1], nBytePerPixel );
		MEMCPY( &pByte[nNext1], &pByte[nNext2], nBytePerPixel );
		MEMCPY( &pByte[nNext2], &nTemp, nBytePerPixel );
	}
}

void KBitmap_MirrorH( LPEXBUFF pBuff )
{
	EXBITMAPFILEHEADER bfHeader;
	EXBITMAPINFOHEADER biHeader;

	sint32 i, j;
	sint32 nBPL, nBPP;
	sint32 nW, nH, nW3, nH3;

	if( EFC_memGET( pBuff ) == NULL ) {
		return;
	}

	KBitmap_GetFileHeader( &bfHeader, pBuff );
	KBitmap_GetInfoHeader( &biHeader, pBuff );

	nW = biHeader.biWidth;
	nH = biHeader.biHeight;

	nBPP = biHeader.biBitCount;
	nBPL = WIDTH_BYTES( nW ) * (nBPP >> 3);

	nW3 = (nW >> 1);
	nH3 = nH;

	for( j=(nH3 - 1); j>=0; j-- ) {
		for( i=(nW3 - 1); i>=0; i-- ) {
			KBitmap_MirrorPixel( pBuff->pByte + bfHeader.bfOffBits, i, j, (nW - 1 - i), j, nBPL, nBPP );
		}
	}
}

void KBitmap_MirrorV( LPEXBUFF pBuff )
{
	EXBITMAPFILEHEADER bfHeader;
	EXBITMAPINFOHEADER biHeader;

	sint32 i, j;
	sint32 nBPL, nBPP;
	sint32 nW, nH, nW3, nH3;

	if( EFC_memGET( pBuff ) == NULL ) {
		return;
	}

	KBitmap_GetFileHeader( &bfHeader, pBuff );
	KBitmap_GetInfoHeader( &biHeader, pBuff );

	nW = biHeader.biWidth;
	nH = biHeader.biHeight;

	nBPP = biHeader.biBitCount;
	nBPL = WIDTH_BYTES( nW ) * (nBPP >> 3);

	nW3 = nW;
	nH3 = (nH >> 1);

	for( j=(nH3 - 1); j>=0; j-- ) {
		for( i=(nW3 - 1); i>=0; i-- ) {
			KBitmap_MirrorPixel( pBuff->pByte + bfHeader.bfOffBits, i, j, i, (nH - 1 - j), nBPL, nBPP );
		}
	}
}

void KBitmap_MirrorHV( LPEXBUFF pBuff )
{
	EXBITMAPFILEHEADER bfHeader;
	EXBITMAPINFOHEADER biHeader;

	sint32 i, j;
	sint32 nBPL, nBPP;
	sint32 nW, nH, nW3, nH3;

	if( EFC_memGET( pBuff ) == NULL ) {
		return;
	}

	KBitmap_GetFileHeader( &bfHeader, pBuff );
	KBitmap_GetInfoHeader( &biHeader, pBuff );

	nW = biHeader.biWidth;
	nH = biHeader.biHeight;

	nBPP = biHeader.biBitCount;
	nBPL = WIDTH_BYTES( nW ) * (nBPP >> 3);

	nW3 = (nW >> 1);
	nH3 = (nH >> 1);

	for( j=(nH3 - 1); j>=0; j-- ) {
		for( i=(nW3 - 1); i>=0; i-- ) {
			KBitmap_MirrorPixel( pBuff->pByte + bfHeader.bfOffBits, (nW - 1 - i), j, i, (nH - 1 - j), nBPL, nBPP );
			KBitmap_MirrorPixel( pBuff->pByte + bfHeader.bfOffBits, i, j, (nW - 1 - i), (nH - 1 - j), nBPL, nBPP );
		}
	}

	if( (nW & 0x00000001) == 0x00000001 ) {
		for( j=(nH3 - 1); j>=0; j-- ) {
			KBitmap_MirrorPixel( pBuff->pByte + bfHeader.bfOffBits, nW3, j, nW3, (nH - 1 - j), nBPL, nBPP );
		}
	}

	if( (nH & 0x00000001) == 0x00000001 ) {
		for( i=(nW3 - 1); i>=0; i-- ) {
			KBitmap_MirrorPixel( pBuff->pByte + bfHeader.bfOffBits, i, nH3, (nW - 1 - i), nH3, nBPL, nBPP );
		}
	}
}

#endif

ubool KBitmap_SetPalette( LPEXFILE pFile, LPTBMPREADER pTBmp )
{
	EXFILE xFile;
	sint32 nPOS;
	ubool bFIND = FALSE;
	ubyte nR, nG, nB, nTemp;
	uint32 i,j,nC;

	if( EFC_memGET( pTBmp->pBuff ) == NULL ) {
		return FALSE;
	}

	nPOS = pFile->nPos;
	EFC_fsSetPOS( pFile, pTBmp->nPAL );

	MEMSET( &xFile, 0, sizeof(EXFILE) );
	xFile.pBuff = pTBmp->pBuff;
#if defined(PIXEL_MODE)
	EFC_fsSetPOS( &xFile, 0 );
	EFC_fsReadData( pFile, &xFile, (pTBmp->nPalLen << 2)*pTBmp->nPalCnt );

	EFC_fsSetPOS( &xFile, 0 );
	nR = EFC_fsReadUint8( &xFile );
	nG = EFC_fsReadUint8( &xFile );
	nB = EFC_fsReadUint8( &xFile );
	if( (nR == 255) && (nG == 0) && (nB == 255) ) {
		bFIND = TRUE;
	}

	EFC_fsSetPOS( &xFile, 0 );
	for( i=0; i<pTBmp->nPalCnt; i++ ) {
		for( j=0; j<pTBmp->nPalLen; j++ ) {
			nB = READ_UINT8( &xFile );
			nG = READ_UINT8( &xFile );
			nR = READ_UINT8( &xFile );
			nTemp = READ_UINT8( &xFile );
			nC = RGB32RGB16( nR, nG, nB );

			FILE_SKIP( &xFile, -4 );
			WRITE_UINT32( &xFile, nC );
		}
	}

	EFC_fsSetPOS( pFile, nPOS );
#else
	EFC_fsSetPOS( &xFile, EXBITMAPFILEHEADERSIZE + EXBITMAPINFOHEADERSIZE );
	EFC_fsReadData( pFile, &xFile, pTBmp->nPalLen );

	EFC_fsSetPOS( &xFile, EXBITMAPFILEHEADERSIZE + EXBITMAPINFOHEADERSIZE );
	nR = EFC_fsReadUint8( &xFile );
	nG = EFC_fsReadUint8( &xFile );
	nB = EFC_fsReadUint8( &xFile );
	if( (nR == 255) && (nG == 0) && (nB == 255) ) {
		bFIND = TRUE;
	}

	if( bFIND == FALSE ) {
		EXBITMAPFILEHEADER bfHeader;
		KBitmap_GetFileHeader( &bfHeader, pTBmp->pBuff );
		bfHeader.bfReserved1 = 0;
		KBitmap_SetFileHeader( &bfHeader, pTBmp->pBuff );
	}

	EFC_fsSetPOS( pFile, nPOS );
#endif

	return bFIND;
}

void ABMReader_Load( LPEXFILE pFile, LPTBMPREADER pTBmp )
{
	sint32 nSize;

	nSize = EFC_fsReadUint16( pFile );
	if( nSize == 0 ) {
		pTBmp->nTYPE = EN_TYPE_BBM;
		nSize = EFC_fsReadUint16( pFile );
	} else {
		pTBmp->nTYPE = EN_TYPE_ABM;
	}
}

void ABMReader_Free( LPTBMPREADER pTBmp )
{
	EFC_memFREE( pTBmp->pBuff );

	MEMSET( pTBmp, 0, sizeof(TBMPREADER) );
}

sint16 ABMReader_header( LPEXFILE pFile, LPTBMPHEADER pHeader )
{
	sint16 nNORMAL;

	nNORMAL = EFC_fsReadSint16( pFile );
	if( nNORMAL == 0 ) {
		EFC_fsReadBuff( pFile, (ubyte *)pHeader, 5 );
	} else {
		pHeader->width = nNORMAL;
		pHeader->height = EFC_fsReadSint16( pFile );
		pHeader->col = EFC_fsReadUint8( pFile );
	}

	return nNORMAL;
}

void ABMReader_palette( LPEXFILE pFile, LPTBMPREADER pTBmp, LPTBMPHEADER pHeader, sint16 nNORMAL )
{
	uint8 nCOLs = 0, nPalLen;

	nCOLs = pHeader->col;
	if( nNORMAL == 0 ) {
		uint8 i, nPalCnt;
		nPalCnt = EFC_fsReadUint8( pFile );
		nPalLen = EFC_fsReadUint8( pFile );

		for( i=0; i<nPalCnt; i++ ) {
			if( i == 0 ) {
				pTBmp->nPAL = pFile->nPos;
				pTBmp->nPalLen = nPalLen;
			}
			EFC_fsSKIP( pFile, (nPalLen << 2) );
		}
#if defined(PIXEL_MODE)
		pTBmp->nPalCnt = nPalCnt;
#endif
	} else if( nCOLs != 0 ) {
		nCOLs++;
		nPalLen = nCOLs;
		pTBmp->nPAL = pFile->nPos;
		pTBmp->nPalLen = nCOLs;
		EFC_fsSKIP( pFile, (nPalLen << 2) );
#if defined(PIXEL_MODE)
		pTBmp->nPalCnt = 1;
#endif
	}
}

void ABMReader_data( LPEXFILE pFile, LPEXIMAGE pIMG, LPTBMPREADER pTBmp, LPTBMPHEADER pHeader, ubool bCOPY, sint32 nCALC )
{
	uint32 nHEADER, nDATA;

#if defined(PIXEL_MODE)
	uint32 nSIZE;
	nHEADER = (pTBmp->nPalLen << 2)*pTBmp->nPalCnt;;
	nDATA = KBitmap_GetSize( pHeader->width, pHeader->height );
	nSIZE = nHEADER+nDATA;
	if( bCOPY == TRUE ) {
		if( pTBmp->pBuff == NULL ) {
			pTBmp->pBuff = EFC_memALLOC( nSIZE );
		}
		if( (pTBmp->pBuff->nSize < 1) || ((uint32)pTBmp->pBuff->nSize != nSIZE) ) {
			EFC_memFREE( pTBmp->pBuff );
			pTBmp->pBuff = EFC_memALLOC( nSIZE );
		}

		if( EFC_memGET( pTBmp->pBuff ) != NULL ) {
			if(nCALC==EN_MIRROR_HORIZON) {
				sint32 i,j;
				sint32 nW = WIDTH_BYTES(pHeader->width);
				sint32 nW2 = nW-pHeader->width;
				for(i=pHeader->height-1;i>=0;i--) {
					for(j=nW-1;j>=0;j--) {
						pTBmp->pBuff->pByte[nHEADER+(pHeader->height-1-i)*nW+(j-nW2)] = pFile->pBuff->pByte[pFile->nPos++];
					}
				}
			} 
			else 
			{
				EFC_fsReadBuff( pFile, &pTBmp->pBuff->pByte[nHEADER], nDATA );
			}
			KBitmap_SetPalette( pFile, pTBmp );
			pIMG->nPalLen = pTBmp->nPalLen;
			pIMG->nPalCnt = pTBmp->nPalCnt;
			pIMG->nDATPOS = nHEADER;
			pIMG->pBuff = pTBmp->pBuff;
			pIMG->nW = pHeader->width;
			pIMG->nH = pHeader->height;
			pTBmp->pBuff = NULL;
			return;
		}
	}
#else
	nHEADER = EXBITMAPFILEHEADERSIZE + EXBITMAPINFOHEADERSIZE + (COLORS << 2);
	nDATA = KBitmap_GetSize( pHeader->width, pHeader->height );
	if( bCOPY == TRUE ) {
		pTBmp->pBuff = KBitmap_Load( pTBmp->pBuff, pHeader->width, pHeader->height );
		if( EFC_memGET( pTBmp->pBuff ) != NULL ) {
			EFC_fsReadBuff( pFile, &pTBmp->pBuff->pByte[nHEADER], nDATA );

			// SET PALETTE : 투명색 사용 여부를 리턴
			//pIMG->bTRANS = 
				KBitmap_SetPalette( pFile, pTBmp );

			// COPY BUFF
			pIMG->pBuff = pTBmp->pBuff;
			pTBmp->pBuff = NULL;
			return;
		}
	}
#endif
	
	EFC_fsSKIP( pFile, nDATA );
}


ubool ABMReader_move( LPEXFILE pFile, LPEXIMAGE pIMG, LPTBMPREADER pTBmp, ubool bCOPY, ubool bMIRROR )
{
	TBMPHEADER tbHeader;
	sint32 nCALC = 0;
	sint16 nNORMAL;

	MEMSET( &tbHeader, 0, sizeof(TBMPHEADER) );

	if( EFC_fsIsAVAIL( pFile, 1 ) == FALSE ) {
		return FALSE;
	}

	if( pTBmp->nTYPE == EN_TYPE_BBM ) 
	{
#if defined(REF2BYTE_MODE)
		pTBmp->refX = EFC_fsReadSint16( pFile );
		pTBmp->refY = EFC_fsReadSint16( pFile );
#else
		pTBmp->refX = EFC_fsReadSint8( pFile );
		pTBmp->refY = EFC_fsReadSint8( pFile );
#endif		
	}

	nCALC = EFC_fsReadUint8( pFile );
	if( nCALC == 0xFF ) {
		nCALC = EFC_fsReadUint8( pFile );

		if( bMIRROR == FALSE ) {
			bCOPY = FALSE;
		}

		if( bCOPY == TRUE ) {
			sint32 nCURR;

			nCURR = pFile->nPos;
			EFC_fsSetPOS( pFile, pTBmp->nPREV );

			nNORMAL = ABMReader_header( pFile, &tbHeader );
			ABMReader_palette( pFile, pTBmp, &tbHeader, nNORMAL );
			ABMReader_data( pFile, pIMG, pTBmp, &tbHeader, bCOPY, nCALC );
#if !defined(PIXEL_MODE)
			if( nCALC == EN_MIRROR_HORIZON ) {
				KBitmap_MirrorH( pIMG->pBuff );
			}
#endif
			pIMG->nMIRROR = (ubyte)nCALC;

			EFC_fsSetPOS( pFile, nCURR );
		}

		return TRUE;
	}

	EFC_fsSKIP( pFile, -1 );

	pTBmp->nPREV = pFile->nPos;

	nNORMAL = ABMReader_header( pFile, &tbHeader );
	ABMReader_palette( pFile, pTBmp, &tbHeader, nNORMAL );
	ABMReader_data( pFile, pIMG, pTBmp, &tbHeader, bCOPY, nCALC );

	return TRUE;
}

ubool ABMReader_skip( LPEXFILE pFile, LPTBMPREADER pTBmp )
{
	return ABMReader_move( pFile, NULL, pTBmp, FALSE, TRUE );
}

ubool ABMReader_next( LPEXFILE pFile, LPEXIMAGE pIMG, LPTBMPREADER pTBmp)
{
	return ABMReader_move( pFile, pIMG, pTBmp, TRUE, TRUE );
}

ubool ABMReader_real( LPEXFILE pFile, LPEXIMAGE pIMG, LPTBMPREADER pTBmp )
{
	return ABMReader_move( pFile, pIMG, pTBmp, TRUE, FALSE );
}



void ABMReader_imgSetLandScape(LPEXIMAGE pIMG)
{
	uint32 nDATA, nDATA2;
	sint32 nNewWidth, nNewHeight;
	sint32 nNewMovX, nNewMovY;
	sint32 nByteWidth, nNewByteWidth;
	sint32 nGap;
	LPEXBUFF pBuff;

	nDATA = KBitmap_GetSize(pIMG->nH, pIMG->nW);
	nDATA2 = KBitmap_GetSize(pIMG->nW, pIMG->nH);

	nByteWidth = WIDTH_BYTES( pIMG->nW );
	nNewByteWidth = WIDTH_BYTES( pIMG->nH );

	nGap = ABS32(WIDTH_BYTES( pIMG->nW ) - WIDTH_BYTES( pIMG->nH ));
	//return;

	nNewWidth = pIMG->nH;
	nNewHeight = pIMG->nW;
	nNewMovX = pIMG->nMoveX;
	nNewMovY = pIMG->nMoveY;

	pBuff = EFC_memALLOC(pIMG->nDATPOS + nDATA);

	if( EFC_memGET( pBuff ) != NULL ) 
	{
		sint32 i, j, k;
		uint16 *pTarget, *pSrc;
		
		EFC_memGET( pIMG->pBuff );

		MEMCPY(pBuff->pByte, pIMG->pBuff->pByte, pIMG->nDATPOS);

		pTarget = (uint16*)&pBuff->pByte[pIMG->nDATPOS];
		pSrc = (uint16*)&pIMG->pBuff->pByte[pIMG->nDATPOS];
/*
		for(i = 0; i < nNewHeight; ++i)
		{
			for(j = 0, k = nNewWidth-1; j < nNewWidth; ++j, --k)
			{
				pBuff->pByte[pIMG->nDATPOS + i*nNewByteWidth+j] = pIMG->pBuff->pByte[pIMG->nDATPOS + k*nByteWidth+i];
			}
		}
*/
		/*
		for(i = 0; i < nNewHeight; ++i){
			for(j = nNewWidth-1; j >= 0; --j){
				*pNewPixel = pOriginPixel[j*nNewHeight+i];
				pNewPixel++;
			}
		}
		*/


		for(i = 0, k = nNewHeight-1; i < nNewHeight; ++i, --k)
		{
			for(j = 0; j < nNewWidth; ++j)
			{
				pBuff->pByte[pIMG->nDATPOS + i*nNewByteWidth+j] = pIMG->pBuff->pByte[pIMG->nDATPOS + j*nByteWidth+k];
			}
		}

		EFC_memFREE(pIMG->pBuff);
		EFC_memGET( pBuff );
		pIMG->pBuff = pBuff;

		pIMG->nW = nNewWidth;
		pIMG->nH = nNewHeight;
		pIMG->nMoveX = nNewMovX;
		pIMG->nMoveY = nNewMovY;
	}
	/*
	uint32 nHEADER, nDATA;
	uint32 nSIZE;
	uint32 nNewWidth, nNewHeight;

	nHEADER = (pTBmp->nPalLen << 2)*pTBmp->nPalCnt;
	nDATA = KBitmap_GetSize( pHeader->width, pHeader->height );
	nSIZE = nHEADER+nDATA;

	nNewWidth = pIMG->nH;
	nNewHeight = pIMG->nW;

	pTBmp->pBuff = EFC_memALLOC( nSIZE );

	if( (pTBmp->pBuff->nSize < 1) || ((uint32)pTBmp->pBuff->nSize != nSIZE) ) 
	{
	EFC_memFREE( pTBmp->pBuff );
	pTBmp->pBuff = EFC_memALLOC( nSIZE );
	}

	if( EFC_memGET( pTBmp->pBuff ) != NULL ) 
	{
	if(nCALC==EN_MIRROR_HORIZON) 
	{
	sint32 i,j;
	sint32 nW = WIDTH_BYTES(pHeader->width);
	sint32 nW2 = nW-pHeader->width;

	for(i=pHeader->height-1;i>=0;i--) 
	{
	for(j=nW-1;j>=0;j--) 
	{
	pTBmp->pBuff->pByte[nHEADER+(pHeader->height-1-i)*nW+(j-nW2)] = pFile->pBuff->pByte[pFile->nPos++];
	}
	}
	} 
	else 
	{
	EFC_fsReadBuff( pFile, &pTBmp->pBuff->pByte[nHEADER], nDATA );
	}

	KBitmap_SetPalette( pFile, pTBmp );
	pIMG->nPalLen = pTBmp->nPalLen;
	pIMG->nPalCnt = pTBmp->nPalCnt;
	pIMG->nDATPOS = nHEADER;
	pIMG->pBuff = pTBmp->pBuff;
	pIMG->nW = pHeader->width;
	pIMG->nH = pHeader->height;
	pTBmp->pBuff = NULL;
	return;
	}
	*/
}
