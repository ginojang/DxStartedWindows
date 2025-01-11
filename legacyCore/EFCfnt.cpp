#include "LegacyCore.h"
#include "LegacyDrawer.h"
#include "BinaryParser.h"

#include "EFCgrp.h"
#include "EFCfnt.h"
#include "EFCpixel.h"
#include "EFCknl.h"

#include "ResourceManager.h"

EXFONTDATA FontData;

void EFC_fntInitialize( void )
{
	EXFILE xFile;

	MEMSET( &FontData, 0, sizeof(EXFONTDATA) );

	FontData.pDATA = g_resourceManager.LoadFromAssets( "gul08.fnt" );

	if( EFC_memGET( FontData.pDATA ) == NULL ) 
	{
		return;
	}

	FontData.pPAL = EFC_memALLOC( (16 * 3) );
	if( EFC_memGET( FontData.pPAL ) == NULL ) 
	{
		return;
	}

	MEMSET( &xFile, 0, sizeof(xFile) );
	xFile.pBuff = FontData.pDATA;
	if( EFC_memGET( xFile.pBuff ) == NULL ) 
	{
		return;
	}
	
	FontData.rtEN.nX = READ_UINT8( &xFile );
	FontData.rtEN.nY = READ_UINT8( &xFile );
	FontData.rtEN.nW = READ_UINT8( &xFile );
	FontData.rtEN.nH = READ_UINT8( &xFile );
	FontData.nEnW = FontData.rtEN.nW;

	FontData.rtHAN.nX = READ_UINT8( &xFile );
	FontData.rtHAN.nY = READ_UINT8( &xFile );
	FontData.rtHAN.nW = READ_UINT8( &xFile );
	FontData.rtHAN.nH = READ_UINT8( &xFile );
	FontData.nHanW = FontData.rtHAN.nW;

	if( FontData.rtEN.nY > FontData.rtHAN.nY ) {
		FontData.rtEN.nY = (sint16)(FontData.rtEN.nY - FontData.rtHAN.nY);
		FontData.rtHAN.nY = 0;
	} else if( FontData.rtEN.nY < FontData.rtHAN.nY ) {
		FontData.rtHAN.nY = (sint16)(FontData.rtHAN.nY - FontData.rtEN.nY);
		FontData.rtEN.nY = 0;
	} else {
		FontData.rtEN.nY = 0;
		FontData.rtHAN.nY = 0;
	}
	

	FontData.nW = 400;
	FontData.nH = (sint16)MAX_CALC( (FontData.rtEN.nY + FontData.rtEN.nH), (FontData.rtHAN.nY + FontData.rtHAN.nH) );

	FontData.nNumEn = (127 - 32); // 숫자, 영어, 특수 기호 개수
	FontData.nEnBytes = (sint16)WIDTH_HEIGHT_BIT_BYTES( FontData.rtEN.nW, FontData.rtEN.nH );
	FontData.nHanBytes = (sint16)WIDTH_HEIGHT_BIT_BYTES( FontData.rtHAN.nW, FontData.rtHAN.nH );

	FontData.hBack = MC_grpCreateOffScreenFrameBuffer( FontData.nW + 2, FontData.nH + 2 );
	
	//MemData.bLOCK = TRUE;

	EFC_fntSetRGB( 1, RGB(0,255,255) );//비취색
	EFC_fntSetRGB( 2, RGB(255,153,204) );//분홍색
	EFC_fntSetRGB( 3, RGB(255,0,0) );//빨강
	EFC_fntSetRGB( 4, RGB(255,92,11) );//주황
	EFC_fntSetRGB( 5, RGB(150,150,150) );//회색
	EFC_fntSetRGB( 6, RGB(228,120,14) );//주황
	EFC_fntSetRGB( 7, RGB(78,223,27) );//녹색
	EFC_fntSetRGB( 8, RGB(255,216,32) );//노랑

	EFC_fntSetBOLD( EN_BOLD_NONE );

}

void EFC_fntFinalize( void )
{
	if( FontData.hBack != 0 ) {
		MC_grpDestroyOffScreenFrameBuffer( FontData.hBack );
		//MemData.bLOCK = TRUE;
	}

	EFC_memFREE( FontData.pPAL );
	EFC_memFREE( FontData.pDATA );

	MEMSET( &FontData, 0, sizeof(EXFONTDATA) );
}

void EFC_fntSetBOLD( sint32 nBOLD )
{
	FontData.nBOLD = (uint8)nBOLD;
}

void EFC_fntSetRGB( sint32 nIDX, sint32 nRGB )
{
	if( (nIDX < 0) || (nIDX > 15) || (EFC_memGET( FontData.pPAL ) == NULL) ) {
		return;
	}

	// 팔레트 데이터 위치
	FontData.pPAL->pByte[nIDX * 3 + 0] = (ubyte)RGB_R( nRGB );
	FontData.pPAL->pByte[nIDX * 3 + 1] = (ubyte)RGB_G( nRGB );
	FontData.pPAL->pByte[nIDX * 3 + 2] = (ubyte)RGB_B( nRGB );
}

sint32 EFC_fntGetRGB( sint32 nIDX )
{
	if( (nIDX < 0) || (nIDX > 15) || (EFC_memGET( FontData.pPAL ) == NULL) ) {
		return 0;
	}

	return RGB( FontData.pPAL->pByte[nIDX * 3 + 0], FontData.pPAL->pByte[nIDX * 3 + 1], FontData.pPAL->pByte[nIDX * 3 + 2] );
}

EXSPACE EFC_fntGetWidthPos( LPEXBUFF pBuff, sint32 nWidth, sint32 *pPos )
{
	EXSPACE xSpace;
	sint32 nW = 0;
	sint32 i;
	schar *pStr;
	ubool bPrevSpace = FALSE;

	MEMSET( &xSpace, 0, sizeof(xSpace) );

	pStr = (schar *)EFC_memGET( pBuff );
	if( pStr == NULL ) {
		return xSpace;
	}

	i = (*pPos);

	while( (pStr[i] == '#') && (pStr[i+1] == 'P') ) {
		i += 5;
	}

	while( pStr[i] != 0 ) {
		if( (pStr[i] == '#') && (pStr[i+1] == 'C') ) {
			i += 3;
			continue;
		}

		if( (pStr[i] == ' ') && (nW == 0) ) {
			i++;
			continue;
		}

		if( ((pStr[i] == '\r') && (pStr[i+1] == '\n')) ||
				(pStr[i] == '\n') ||
				((pStr[i] == '\\') && (pStr[i+1] == 'n')) ) {

			if( (pStr[i] == '\r') || (pStr[i] == '\\') ) {
				i++;
			}
			i++;
			xSpace.bEnter = TRUE;
			break;
		}

		if( nWidth > 0 ) {
			if( (pStr[i] & 0x00000080) == 0x00000080 ) {
				if( (nW + FontData.rtHAN.nW) > nWidth ) {
					break;
				}
			} else {
				if( (nW + FontData.rtEN.nW) > nWidth ) {
					break;
				}
			}
		}

		bPrevSpace = FALSE;
		if( (pStr[i] & 0x00000080) == 0x00000080 ) { // korean
			nW += FontData.nHanW;
			i += 2;
		} else {
			if( pStr[i] == ' ' ) {
				xSpace.nCount++;
				bPrevSpace = TRUE;
			}
			nW += FontData.nEnW;
			i++;
		}
	}

	if( xSpace.bEnter == FALSE ) {
		if( bPrevSpace == TRUE ) {
			xSpace.bLastSpace = TRUE;
		}
	}

	if( pStr[i] == 0 ) {
		(*pPos) = -1;
		xSpace.bEnter = TRUE;
	} else {
		(*pPos) = i;
	}

	xSpace.nWidth = (sint16)(nW - (FontData.nEnW - FontData.rtEN.nW));
	if( xSpace.nCount > 0 ) {
		if( xSpace.bLastSpace == TRUE ) {
			xSpace.nSize = (sint16)(nWidth - xSpace.nWidth + FontData.nEnW);
			xSpace.nCount--;
		} else {
			xSpace.nSize = (sint16)(nWidth - xSpace.nWidth);
		}
	}

	return xSpace;
}

sint32 EFC_fntGetWIDTH( LPEXBUFF pBuff )
{
	EXSPACE xSpace;
	sint32 nPos = 0;
	xSpace = EFC_fntGetWidthPos( pBuff, -1, &nPos );
	return (sint32)xSpace.nWidth;
}

////추가/////////////////////////////////
void EFC_fntInitFillRect(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	sint32 nWIDTH, nBPL, nBPW;
	
	nWIDTH = g_legacyDrawer.GetFrameWidth( hBack );
	nBPL = g_legacyDrawer.GetBplWithWidth( nWIDTH, PixelData.nBPP );
	nBPW = nBPL / (PixelData.nBPP >> 3);

	{
		uint16 *pDATA, nCOLOR;

		nCOLOR = (uint16)PixelData.nTRANS;
		pDATA = (uint16 *)g_legacyDrawer.GetFrameBuffer( hBack );
		{
			sint32 nSX1, nEX1, nSY1, nEY1;
			for( nSY1=(nX + nBPW * nY), nEY1=(nSY1 + nBPW * nH); nSY1<nEY1; nSY1+=nBPW ) {
				for( nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++ ) {
					pDATA[nSX1] = nCOLOR;
				}
			}
		}
	}
}

void EFC_fntInitBUFF( void )
{
	EFC_fntInitFillRect( FontData.hBack, 0, 0, FontData.nW + 2, FontData.nH + 2 );
}

void EFC_fntSetBUFF( sint32 nOFFSET, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nRGB )
{
	sint32 nPos, i, nBPL, nBPW, nWH, nNextX, nNextY;

	if( (nW < 1) || (EFC_memGET( FontData.pDATA ) == NULL) ) {
		return;
	}

	nOFFSET += 8;

	nBPL = g_legacyDrawer.GetBplWithWidth( (FontData.nW + 2), PixelData.nBPP );
	nBPW = nBPL / (PixelData.nBPP >> 3);
	nWH = nW * nH;

	{
		uint16 *pDATA;
		uint16 nCOLOR;

		pDATA = (uint16 *)g_legacyDrawer.GetFrameBuffer( FontData.hBack );
		nCOLOR = RGB32RGB16( RGB_R(nRGB), RGB_G(nRGB), RGB_B(nRGB) );

		switch( FontData.nBOLD ) {
		case EN_BOLD_NONE :
			for( i=(nWH - 1); i>=0; i-- ) {
				if( ((FontData.pDATA->pByte[nOFFSET + (i >> 3)] >> (i & 0x00000007)) & 0x00000001) != 0 ) {
					nPos = nX + (i % nW);
					if( (nPos >= 0) && (nPos < FontData.nW) ) {
						nNextY = (i / nW);
						nNextX = nPos + nY;
						pDATA[((nNextY + 1) * nBPW + (nNextX + 1))] = nCOLOR;
					}
				}	
			}
			break;
		case EN_BOLD_ALL :
			for( i=(nWH - 1); i>=0; i-- ) {
				if( ((FontData.pDATA->pByte[nOFFSET + (i >> 3)] >> (i & 0x00000007)) & 0x00000001) != 0 ) {
					nPos = nX + (i % nW);
					if( (nPos >= 0) && (nPos < FontData.nW) ) {
						nNextY = (i / nW);
						nNextX = nPos + nY;
						pDATA[((nNextY    ) * nBPW + (nNextX    ))] = nCOLOR;
						pDATA[((nNextY    ) * nBPW + (nNextX + 1))] = nCOLOR;
						pDATA[((nNextY    ) * nBPW + (nNextX + 2))] = nCOLOR;
						pDATA[((nNextY + 1) * nBPW + (nNextX    ))] = nCOLOR;
						pDATA[((nNextY + 1) * nBPW + (nNextX + 1))] = nCOLOR;
						pDATA[((nNextY + 1) * nBPW + (nNextX + 2))] = nCOLOR;
						pDATA[((nNextY + 2) * nBPW + (nNextX    ))] = nCOLOR;
						pDATA[((nNextY + 2) * nBPW + (nNextX + 1))] = nCOLOR;
						pDATA[((nNextY + 2) * nBPW + (nNextX + 2))] = nCOLOR;
					}
				}	
			}
			break;
		case EN_BOLD_WIDTH :
			for( i=(nWH - 1); i>=0; i-- ) {
				if( ((FontData.pDATA->pByte[nOFFSET + (i >> 3)] >> (i & 0x00000007)) & 0x00000001) != 0 ) {
					nPos = nX + (i % nW);
					if( (nPos >= 0) && (nPos < FontData.nW) ) {
						nNextY = (i / nW);
						nNextX = nPos + nY;
						pDATA[((nNextY + 1) * nBPW + (nNextX + 1))] = nCOLOR;
						pDATA[((nNextY + 1) * nBPW + (nNextX + 2))] = nCOLOR;
					}
				}	
			}
			break;
		}
	}
}

void EFC_fntDrawBUFF( sint32 nX, sint32 nY, sint32 nW, sint32 nANCHOR )
{
	EXRECT rt;
	EFC_grpGetANCHOR( &rt, nX, nY, nW, FontData.nH, nANCHOR );
	switch( FontData.nBOLD ) {
	case EN_BOLD_NONE:
		EFC_grpCopyFRAME( rt.nX, rt.nY, rt.nW, rt.nH, FontData.hBack, 1, 1, TRUE );
		break;
	case EN_BOLD_ALL :
		EFC_grpCopyFRAME( rt.nX - 1, rt.nY - 1, rt.nW + 2, rt.nH + 2, FontData.hBack, 0, 0, TRUE );
		break;
	case EN_BOLD_WIDTH :
		EFC_grpCopyFRAME( rt.nX, rt.nY, rt.nW + 1, rt.nH, FontData.hBack, 1, 1, TRUE );
		break;
	}
}

sint32 EFC_fntGetNUMBER( schar szCHAR )
{
	sint32 nCOLOR = 0;

	if( (szCHAR >= '0') && (szCHAR <= '9') ) {
		nCOLOR = (szCHAR - '0');
	} else if( (szCHAR >= 'a') && (szCHAR <= 'f') ) {
		nCOLOR = (szCHAR - 'a' + 10);
	} else if( (szCHAR >= 'A') && (szCHAR <= 'F') ) {
		nCOLOR = (szCHAR - 'A' + 10);
	}

	return nCOLOR;
}

sint32 EFC_fntStringPALETTE( LPEXBUFF pBuff )
{
	sint32 i = 0;
	sint32 nPAL = 0, nR, nG, nB;

	if( EFC_memGET( pBuff ) == NULL ) {
		return i;
	}

	while( (pBuff->pByte[i] == '#') && (pBuff->pByte[i + 1] == 'P') ) {
		nR = EFC_fntGetNUMBER( pBuff->pByte[i+2] );
		nR <<= 4;

		nG = EFC_fntGetNUMBER( pBuff->pByte[i+3] );
		nG <<= 4;

		nB = EFC_fntGetNUMBER( pBuff->pByte[i+4] );
		nB <<= 4;

		EFC_fntSetRGB( nPAL++, RGB(nR, nG, nB) );

		i += 5;
	}

	return i;
}

sint32 EFC_fntSetNumber(char *pSTR, sint32 nNUMB)
{
	sprintf(pSTR, "%d", nNUMB);

	/*
	sint32 digit = Util_DigitSet(nNUMB);
	sint32 i = 0, temp = 0;
	sint32 nPOS = 0;
	for( i =digit-1;i>=0;i-- )
	{
		temp = ((nNUMB % Util_Pow(i+1))) / Util_Pow(i);
		pSTR[nPOS] = (char)('0'+temp);
		nPOS++;
	}*/
	return strlen(pSTR);
}

void EFC_fntSetNumberStr(const char *pStr, char *desStr, sint32 *pNumber)
{//char에 숫자배열을 셋팅
	sint32 i = 0;
	sint32 nPOS = 0;
	sint32 nNumPos = 0;
	while( pStr[i] != 0 ) {
		if( (pStr[i] == '%') && (pStr[i + 1] == 'd') ) {//정수적용 
			nPOS+=EFC_fntSetNumber(&desStr[nPOS], pNumber[nNumPos]);
			nNumPos++;
			i += 2;
		} else if( (pStr[i] == '%') && (pStr[i + 1] == 'f') ) {//실수처럼 변환
			nPOS+=EFC_fntSetNumber(&desStr[nPOS], pNumber[nNumPos]/10);
			if(pNumber[nNumPos]%10!=0)
			{
				desStr[nPOS] = '.';
				nPOS++;
				nPOS+=EFC_fntSetNumber(&desStr[nPOS], pNumber[nNumPos]%10);
			}
			nNumPos++;
			i += 2;
		} 
#if _CHECK
		else if( (pStr[i] == '%') && (pStr[i + 1] == 'c') ) {//프레임정보를 시간정보처럼 변환
			nPOS+=EFC_fntSetNumber(&desStr[nPOS], pNumber[nNumPos]/TIMECNT);
			if(pNumber[nNumPos]%TIMECNT!=0)
			{
				desStr[nPOS] = '.';
				nPOS++;
				nPOS+=EFC_fntSetNumber(&desStr[nPOS], 5);
			}
			nNumPos++;
			i += 2;
		} 
#endif
		else {
			desStr[nPOS] = pStr[i];
			i++;
			nPOS++;
		}
	}
}

void EFC_fntDrawText( sint32 nX, sint32 nY, sint32 nW, sint32 nLineH, LPEXBUFF pBuff, sint32 nPos, sint32 *pTyping, sint32 nLines, sint32 anchor )
{
	sint32 i = 0, nWidth = 0, nIndex = 0, nOffset, nLine = 1, nStart;
	sint32 nRGB = 0;
	schar *pStr;

	EXSPACE xSpace;
	sint32 nSize;

	EFC_fntInitBUFF();

	pStr = (schar *)EFC_memGET( pBuff );
	if( pStr == NULL ) {
		return;
	}

	if( nPos < 0 ) {
		nPos = 0;
	}

	nStart = nPos;

	i = nPos;
	if( i > 0 ) {
		EFC_fntStringPALETTE( pBuff );
		while( nPos >= 0 ) {
			if( (pStr[nPos] == '#') && (pStr[nPos + 1] == 'C') ) {
				nIndex = EFC_fntGetNUMBER( pStr[nPos+2] );
				break;
			}
			nPos--;
		}
	} else {
		i = EFC_fntStringPALETTE( pBuff );
	}

	// 혼합 정렬을 위한 추가
	nPos = i;
	xSpace = EFC_fntGetWidthPos( pBuff, nW, &nPos );

	nRGB = EFC_fntGetRGB( nIndex );
	while( pStr[i] != 0 ) {
		if( (pStr[i] == '#') && (pStr[i + 1] == 'C') ) {
			nIndex = EFC_fntGetNUMBER( pStr[i+2] );
			nRGB = EFC_fntGetRGB( nIndex );
			i += 3;
			continue;
		}

		if( (pStr[i] == ' ') && (nWidth == 0) ) {
			i++;
			continue;
		}

		if( i > (nStart + (*pTyping)) ) {
			break;
		}

		if( ((pStr[i] == '\r') && (pStr[i + 1] == '\n')) || (pStr[i] == '\n') || ((pStr[i] == '\\') && (pStr[i + 1] == 'n')) ) {
			nLine++;
			if( nLine > nLines ) {
				(*pTyping) = 0x0000FFFF;
				break;
			}

			if( (anchor & EN_MIX) == EN_MIX ) {
			} else if( xSpace.bLastSpace == TRUE ) {
				nWidth -= FontData.nEnW;
			}

			EFC_fntDrawBUFF( nX, nY, nWidth, anchor );

			EFC_fntInitBUFF();
			pStr = (schar *)EFC_memGET( pBuff );

			nWidth = 0;
			nY += nLineH;

			if( (pStr[i] == '\r') || (pStr[i] == '\\') ) {
				i++;
			}
			i++;

			nPos = i;
			xSpace = EFC_fntGetWidthPos( pBuff, nW, &nPos );
			continue;
		}

		if( (nW > 0) && 
			((((pStr[i] & 0x00000080) == 0x00000080) && ((nWidth + FontData.rtHAN.nW) > nW)) ||
			(((pStr[i] & 0x00000080) == 0x00000000) && ((nWidth + FontData.rtEN.nW) > nW))) ) {
			nLine++;
			if( nLine > nLines ) {
				(*pTyping) = 0x0000FFFF;
				break;
			}

			if( (anchor & EN_MIX) == EN_MIX ) {
			} else if( xSpace.bLastSpace == TRUE ) {
				nWidth -= FontData.nEnW;
			}

			EFC_fntDrawBUFF( nX, nY, nWidth, anchor );

			EFC_fntInitBUFF();
			pStr = (schar *)EFC_memGET( pBuff );

			nWidth = 0;
			nY += nLineH;

			nPos = i;
			xSpace = EFC_fntGetWidthPos( pBuff, nW, &nPos );
			continue;
		}

		if( (pStr[i] & 0x00000080) == 0x00000080 ) {
			nOffset = FontData.nNumEn * FontData.nEnBytes + FontData.nHanBytes * (((pStr[i] & 0x000000FF) - 0x000000B0) * 94 + (pStr[i+1] & 0x000000FF) - 0x000000A1);
			EFC_fntSetBUFF( nOffset, nWidth, FontData.rtHAN.nY, FontData.rtHAN.nW, FontData.rtHAN.nH, nRGB );

			nWidth += FontData.nHanW;
			i += 2;
		} else {
			if( pStr[i] == ' ' ) {
				// 혼합 정렬을 위한 추가
				if( (anchor & EN_MIX) == EN_MIX ) {
					if( xSpace.bEnter == FALSE ) {
						if( xSpace.nCount > 0 ) {
							nSize = xSpace.nSize / xSpace.nCount;
							nWidth += nSize;

							xSpace.nSize = (sint16)(xSpace.nSize - nSize);
							xSpace.nCount--;
						}
					}
				}
			} else {
				nOffset = FontData.nEnBytes * ((pStr[i] & 0x000000FF) - 0x00000021);
				EFC_fntSetBUFF( nOffset, nWidth, FontData.rtEN.nY, FontData.rtEN.nW, FontData.rtEN.nH, nRGB );
			}

			nWidth += FontData.nEnW;
			i++;
		}
	}

	if( pStr[i] == 0 ) {
		(*pTyping) = 0x0000FFFF;
	}

	if( (anchor & EN_MIX) == EN_MIX ) {
	} else if( xSpace.bLastSpace == TRUE ) {
		nWidth -= FontData.nEnW;
	}

	EFC_fntDrawBUFF( nX, nY, nWidth, anchor );
}

void EFC_fntSetStringPage( LPEXPAGE pPage )
{
	sint32 nPos = 0;

	pPage->nPages = 1;
	pPage->nLine = 0;

	while( (nPos >= 0) && (pPage->nPages < FNT_PAGES) ) {
		EFC_fntGetWidthPos( pPage->pBuff, pPage->nWidth, &nPos );

		pPage->nLine++;
		if( nPos < 0 ) {
			break;
		}

		if( (pPage->nLines < 0) || (pPage->nLine < pPage->nLines) ) {
			continue;
		}

		pPage->nLine = 0;
		pPage->nPos[pPage->nPages++] = nPos;
	}

	if( pPage->nLines < 1 ) {
		pPage->nLines = (sint8)pPage->nLine;
	}
}

LPEXBUFF EFC_fntReplace( LPEXBUFF pBuff, schar *pOld, schar *pNew )
{
	LPEXBUFF pALLOC;
	schar *pChar, *pPrev, *pDest;
	sint32 nLen, nOldLen, nNewLen, nSize, nCount = 0;

	pChar = (schar *)EFC_memGET( pBuff );
	if( (pChar == NULL) || (pOld == NULL) || (pNew == NULL) ) {
		return NULL;
	}

	nLen = STRLEN( pChar );
	nOldLen = STRLEN( pOld );
	nNewLen = STRLEN( pNew );

	while( pChar != NULL ) {
		pChar = (schar *)STRSTR( (schar *)pChar, pOld );
		if( pChar == NULL ) {
			break;
		}
		pChar += nOldLen;
		nCount++;
	}

	pALLOC = EFC_memALLOC( nLen + (nNewLen - nOldLen) * nCount + 1 );
	if( EFC_memGET( pALLOC ) == NULL ) {
		return NULL;
	}

	pDest = (schar *)EFC_memGET( pALLOC );
	pChar = (schar *)EFC_memGET( pBuff );
	pPrev = pChar;

	while( pChar != NULL ) {
		pChar = (schar *)STRSTR( (schar *)pPrev, pOld );
		if( pChar == NULL ) {
			break;
		}

		nSize = pChar - pPrev;
		STRNCPY( pDest, pPrev, nSize );
		pDest += nSize;

		nSize = nNewLen;
		STRNCPY( pDest, pNew, nSize );
		pDest += nSize;

		pChar += nOldLen;
		pPrev = pChar;
	}

	STRCPY( pDest, pPrev );
	return pALLOC;
}

void EFC_fntChange( LPEXBUFF pBuff, schar szOld, schar szNew )
{
	schar *pChar, *pPrev;
	schar szBuff[2];

	szBuff[0] = szOld;
	szBuff[1] = 0;

	pChar = (schar *)EFC_memGET( pBuff );
	if( pChar == NULL ) {
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

EXPAGE EFC_fntGetStringPage( LPEXBUFF pBuff, sint32 nWidth, sint32 nLineH, sint32 nLines )
{
	EXPAGE xPage;

	MEMSET( &xPage, 0, sizeof(EXPAGE) );

	if( EFC_memGET( pBuff ) == NULL )
	{
		return xPage;
	}

	xPage.pBuff = pBuff;

	xPage.nWidth = (sint16)nWidth;
	xPage.nLineH = (sint16)nLineH;
	xPage.nLines = (sint8)nLines;
	xPage.nTyping = 0;

	if (xPage.nWidth > 400)
	{
		xPage.nWidth = 400;
	}

	EFC_fntSetStringPage( &xPage );

	return xPage;
}

void EFC_fntDrawStringPage( sint32 nX, sint32 nY, LPEXPAGE pPage, sint32 nANCHOR )
{
	sint32 nPage;
	sint32 nCNT = 0x0000FFFF;

	if( (pPage == NULL) || (pPage->nPages < 1) ) {
		return;
	}

	nPage = pPage->nPage;
	if( nPage < 0 ) {
		nPage = 0;
	}

	if( nPage > (pPage->nPages - 1) ) {
		nPage = (pPage->nPages - 1);
	}

	if( nPage < (pPage->nPages - 1) ) {
		nCNT = (pPage->nPos[nPage + 1] - pPage->nPos[nPage] - 1);
	}

	EFC_fntDrawText( nX, nY, pPage->nWidth, FontData.nH + pPage->nLineH, pPage->pBuff, pPage->nPos[nPage], &nCNT, pPage->nLines, nANCHOR );
}

void EFC_fntDrawStringTyping( sint32 nX, sint32 nY, LPEXPAGE pPage, sint32 nANCHOR )
{
	sint32 nPage;

	if( (pPage == NULL) || (pPage->nPages < 1) ) {
		return;
	}

	nPage = pPage->nPage;
	if( nPage < 0 ) {
		nPage = 0;
	}

	if( nPage > (pPage->nPages - 1) ) {
		nPage = (pPage->nPages - 1);
	}

	EFC_fntDrawText( nX, nY, pPage->nWidth, FontData.nH + pPage->nLineH, pPage->pBuff, pPage->nPos[nPage], &pPage->nTyping, pPage->nLines, nANCHOR );
}

void EFC_fntDrawStringEx( sint32 nX, sint32 nY, sint32 nW, sint32 nLineH, LPEXBUFF pBuff, sint32 nANCHOR )
{
	sint32 nCNT = 0x0000FFFF;
	EFC_fntDrawText( nX, nY, nW, FontData.nH + nLineH, pBuff, 0, &nCNT, 255, nANCHOR );
}

void EFC_fntDrawString( sint32 nX, sint32 nY, LPEXBUFF pBuff, sint32 nANCHOR )
{
	sint32 nCNT = 0x0000FFFF;
	EFC_fntDrawText( nX, nY, -1, FontData.nH, pBuff, 0, &nCNT, 255, nANCHOR );
}

void EFC_fntDrawStringBold( sint32 nX, sint32 nY, schar *pSTR, sint32 nANCHOR, RGBVAL nRGB1, RGBVAL nRGB2 )
{
	EXBUFF xByte;

	MEMSET( &xByte, 0, sizeof(xByte) );
	xByte.pByte = (ubyte *)pSTR;

	if( nRGB1 != RGB_NONE ) {
		EFC_fntSetBOLD( EN_BOLD_ALL );
		EFC_fntSetRGB( 0, nRGB1 );
		EFC_fntDrawString( nX, nY, &xByte, nANCHOR );
	}

	EFC_fntSetBOLD( EN_BOLD_NONE );
	if( nRGB2 != RGB_NONE ) {
		EFC_fntSetRGB( 0, nRGB2 );
		EFC_fntDrawString( nX, nY, &xByte, nANCHOR );
	}
}

void EFC_fntTRIM( schar *pSTR )
{
	sint32 i, nLEN, nFIND = -1;
	if( pSTR != NULL ) {
		nLEN = STRLEN( pSTR );
		for( i=(nLEN - 1); i>=0; i-- ) {
			if( pSTR[i] == ' ' ) {
				pSTR[i] = 0;
			} else {
				break;
			}
		}

		nLEN = STRLEN( pSTR );
		for( i=0; i<nLEN; i++ ) {
			if( pSTR[i] != ' ' ) {
				nFIND = i;
				break;
			}
		}

		if( nFIND > 0 ) {
			SPRINTF( pSTR, pSTR + nFIND, nLEN - nFIND );
		}
	}
}

void EFC_fntStringPageLine( LPEXPAGE pPage )
{
	sint32 nPos = 0, nLine = 0;

	pPage->nPages = 1;
	while( nPos >= 0 ) {
		EFC_fntGetWidthPos( pPage->pBuff, pPage->nWidth, &nPos );

		nLine++;
		if( nPos < 0 ) {
			break;
		}

		if( pPage->nLines < 0 ) {
			continue;
		}

		if( nLine < pPage->nLines ) {
			continue;
		}

		nLine = 0;
		pPage->nPos[pPage->nPages++] = nPos;
	}

	pPage->nLines = (sint8)nLine;
}
