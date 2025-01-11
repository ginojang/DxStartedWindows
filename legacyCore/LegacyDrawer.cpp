#include "LegacyCore.h"
#include "ResourceManager.h"
#include "LegacyDrawer.h"
#include "LegacyFilterProcs.h"

#include "EFCfnt.h"
#include "EFCimg.h"


LegacyDrawer g_legacyDrawer;

PIXELDATA PixelData;



LegacyDrawer::LegacyDrawer()
{
	mainScreenWidth = 0;
	mainScreenHeight = 0;
	mainScreenBpp = 0;
	mainScreenBpl = 0;
	mainScreenBuffer = NULL;

	mainScreenIndex = 0;
	frameBufferCounter = mainScreenIndex + 1;

	int i;
	for (i = 0; i < MAX_FRAME_BUFFER; i++)
		pGrpFrameBuffer[i] = NULL;


	midX = 0;
	midY = 0;
	stdX = 0;
	stdY = 0;
	menuX = 0;
	menuY = 0;
	LCD_W = 0;
	LCD_H = 0;
}

LegacyDrawer::~LegacyDrawer()
{

}

void LegacyDrawer::Init(uint32* pScreenBuffer, int width, int height)
{
	mainScreenWidth = width;
	mainScreenHeight = height;
	mainScreenBpp = 32;
	mainScreenBpl = (width * (mainScreenBpp >> 3));	
	mainScreenBuffer = pScreenBuffer;

	//
	MEMSET(&PixelData, 0, sizeof(PixelData));

	PixelData.hReal = GetMainScreenIndex();
	PixelData.rtREAL.nW = GetFrameWidth(PixelData.hReal);
	PixelData.rtREAL.nH = GetFrameHeight(PixelData.hReal);
	PixelData.nBPP = GetFrameBpp(PixelData.hReal);

	//
	InitFilterPixelProcs();

	//
	EFC_grpSetFRAME(PixelData.hScreen);
	//

	if (PixelData.rtREAL.nW == 800 && PixelData.rtREAL.nH == 480)
	{
		PixelData.rtMAIN.nW = (sint16)PixelData.rtREAL.nW / 2;
		PixelData.rtMAIN.nH = (sint16)PixelData.rtREAL.nH / 2;
	}
	else
	{
		DebugLog(L"현재 - 800 * 480 만 지원합니다.>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	}

	PixelData.hScreen = CreateOffScreenBuffer(PixelData.rtMAIN.nW, PixelData.rtMAIN.nH);
	PixelData.nBPP = GetFrameBpp(PixelData.hScreen);

	PixelData.rtCLIP.nW = PixelData.rtMAIN.nW;
	PixelData.rtCLIP.nH = PixelData.rtMAIN.nH;

	LCD_W = PixelData.rtMAIN.nW;
	LCD_H = PixelData.rtMAIN.nH;

	stdX = ((LCD_W) >> 1);
	stdY = ((LCD_H) >> 1);
	midX = (LCD_W >> 1);
	midY = (LCD_H >> 1);

	menuX = 0;
	menuY = (LCD_H - 240) / 2 + 20;

	//
	EFC_fntInitialize();
}

void LegacyDrawer::ReleaseAll()
{
	EFC_fntFinalize();

	for (int i = 0; i < MAX_FRAME_BUFFER; i++)
	{
		if (pGrpFrameBuffer[i] != NULL)
		{
			delete pGrpFrameBuffer[i];
		}
	}
}

int LegacyDrawer::GetMainScreenIndex()
{
	return mainScreenIndex;
}

schar* LegacyDrawer::GetFrameBuffer(sint32 hIndex)
{
	if (hIndex == mainScreenIndex)
	{
		// 외부에서 메인 버퍼 가져가면 안됨..
		return (schar*)mainScreenBuffer;
	}

	if (hIndex < 0 || hIndex >= MAX_FRAME_BUFFER)
		return NULL;

	if (pGrpFrameBuffer[hIndex])
		pGrpFrameBuffer[hIndex]->buffer;
	
	return NULL;
}

sint32 LegacyDrawer::GetFrameWidth(sint32 hIndex)
{
	if (hIndex == mainScreenIndex)
	{
		return mainScreenWidth;
	}

	if (hIndex < 0 || hIndex >= MAX_FRAME_BUFFER)
		return 0;

	if (pGrpFrameBuffer[hIndex])
		pGrpFrameBuffer[hIndex]->w;

	return 0;
}

sint32 LegacyDrawer::GetFrameHeight(sint32 hIndex)
{
	if (hIndex == mainScreenIndex)
	{
		return mainScreenHeight;
	}

	if (hIndex < 0 || hIndex >= MAX_FRAME_BUFFER)
		return 0;

	if (pGrpFrameBuffer[hIndex])
		pGrpFrameBuffer[hIndex]->h;

	return 0;
}

uint8 LegacyDrawer::GetFrameBpp(sint32 hIndex)
{
	if (hIndex == mainScreenIndex)
	{
		return mainScreenBpp;
	}

	if (hIndex < 0 || hIndex >= MAX_FRAME_BUFFER)
		return 0;

	if (pGrpFrameBuffer[hIndex])
		pGrpFrameBuffer[hIndex]->bpp;

	return 0;
}

sint32 LegacyDrawer::GetBplWithWidth_PlusNumber7(sint32 width, sint32 bpp)
{
	// 짝수 맞출려고 +7 하는듯
	//return (((width) * (bpp) + 7) >> 3);
	return (width * (bpp >> 3));
}

sint32 LegacyDrawer::CreateOffScreenBuffer(sint32 width, sint32 height)
{
	if (width % 2 != 0)
		width++;

	// 0번은 메인 스크린.
	for (int i = 1; i < MAX_FRAME_BUFFER; i++)
	{
		if (pGrpFrameBuffer[i] == NULL)
		{
			MC_GrpFrameBuffer *mcBuffer = new MC_GrpFrameBuffer();
			mcBuffer->w = width;
			mcBuffer->h = height;
			mcBuffer->bpp = 16;
			mcBuffer->bpl = (width * (mcBuffer->bpp >> 3));
			mcBuffer->buffer = g_resourceManager.AllocBuffer((width * 2) * height);

			pGrpFrameBuffer[i] = mcBuffer;
			return i;
		}
	}
	
	// 버퍼 FULL.......
	return 0;
}

void LegacyDrawer::ReleaseOffScreenBuffer(sint32 hIndex)
{
	if (hIndex == mainScreenIndex)
	{
		return;
	}

	if (hIndex < 0 || hIndex >= MAX_FRAME_BUFFER)
		return;

	if (pGrpFrameBuffer[hIndex])
	{
		g_resourceManager.ReleaseBuffer(pGrpFrameBuffer[hIndex]->buffer);
		delete pGrpFrameBuffer[hIndex];
		pGrpFrameBuffer[hIndex] = NULL;
	}

	return;
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

void MC_grpFlushLcd(M_Int32 i, M_Int32 mcBuffer, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h)
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 - 






/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 - 

void EFC_pxlRectPROC(uint16* pDATA, sint32 nBPW, sint32 nX, sint32 nY, sint32 nW, sint32 nH, uint16 nCOLOR)
{
	sint32 nSX1, nEX1, nSY1, nEY1;

	if (PixelProcData.nMODE > EN_MODE_NORMAL) {
		for (nSY1 = (nX + nBPW * nY), nEY1 = (nSY1 + nBPW * nH); nSY1 < nEY1; nSY1 += nBPW) {
			for (nSX1 = nSY1, nEX1 = (nSY1 + nW); nSX1 < nEX1; nSX1++) {
				pDATA[nSX1] = PixelProc[PixelProcData.nMODE].proc(pDATA[nSX1], nCOLOR);
			}
		}
	}
	else {
		for (nSY1 = (nX + nBPW * nY), nEY1 = (nSY1 + nBPW * nH); nSY1 < nEY1; nSY1 += nBPW) {
			for (nSX1 = nSY1, nEX1 = (nSY1 + nW); nSX1 < nEX1; nSX1++) {
				pDATA[nSX1] = nCOLOR;
			}
		}
	}
}

void EFC_pxlCopyArea(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSX, sint32 nSY)
{
	uint16* pDATA;
	sint32 nSX1 = 0, nEX1 = 0, nSY1 = 0, nEY1 = 0, nSX2 = 0, nSY2 = 0;
	sint32 nWIDTH, nBPL1, nBPW1;

	nWIDTH = g_legacyDrawer.GetFrameWidth(hBack);
	nBPL1 = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH, PixelData.nBPP);
	nBPW1 = nBPL1 / (PixelData.nBPP >> 3);
	pDATA = (uint16*)g_legacyDrawer.GetFrameBuffer(hBack);

	if ((nX == nSX) && (nY == nSY)) {
		return;
	}

	if (nY < nSY) {
		if (nX < nSX) {
			for (nSY1 = (nX + nWIDTH * nY), nEY1 = (nSY1 + nWIDTH * nH), nSY2 = (nSX + nWIDTH * nSY); nSY1 < nEY1; nSY1 += nWIDTH, nSY2 += nWIDTH) {
				for (nSX2 = nSY2, nSX1 = nSY1, nEX1 = (nSY1 + nW); nSX1 < nEX1; nSX1++, nSX2++) {
					pDATA[nSX1] = pDATA[nSX2];
				}
			}
		}
		else {
			for (nSY1 = (nX + nWIDTH * nY), nEY1 = (nSY1 + nWIDTH * nH), nSY2 = (nSX + nWIDTH * nSY); nSY1 < nEY1; nSY1 += nWIDTH, nSY2 += nWIDTH) {
				for (nSX2 = (nSY2 + nW - 1), nSX1 = (nSY1 + nW - 1), nEX1 = (nSY1 - 1); nSX1 > nEX1; nSX1--, nSX2--) {
					pDATA[nSX1] = pDATA[nSX2];
				}
			}
		}
	}
	else {
		if (nX < nSX) {
			for (nSY1 = (nX + nWIDTH * (nY + nH - 1)), nEY1 = (nSY1 - (nWIDTH * nH)), nSY2 = (nSX + nWIDTH * (nSY + nH - 1)); nSY1 > nEY1; nSY1 -= nWIDTH, nSY2 -= nWIDTH) {
				for (nSX2 = nSY2, nSX1 = nSY1, nEX1 = (nSY1 + nW); nSX1 < nEX1; nSX1++, nSX2++) {
					pDATA[nSX1] = pDATA[nSX2];
				}
			}
		}
		else {
			for (nSY1 = (nX + nWIDTH * (nY + nH - 1)), nEY1 = (nSY1 - (nWIDTH * nH)), nSY2 = (nSX + nWIDTH * (nSY + nH - 1)); nSY1 > nEY1; nSY1 -= nWIDTH, nSY2 -= nWIDTH) {
				for (nSX2 = (nSY2 + nW - 1), nSX1 = (nSY1 + nW - 1), nEX1 = (nSY1 - 1); nSX1 > nEX1; nSX1--, nSX2--) {
					pDATA[nSX1] = pDATA[nSX2];
				}
			}
		}
	}

}

void EFC_pxlSetFRAME(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC, sint32 nSx, sint32 nSy, ubool bTRANS)
{
	sint32 nWIDTH1, nBPL1, nBPW1, nBPL2, nBPW2, nWIDTH2;
	sint32 nSx1, nEx1, nSy1, nEy1, nSx2, nSy2 = 0, nMX2 = 1;

	nWIDTH1 = g_legacyDrawer.GetFrameWidth(hBack);
	nBPL1 = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH1, PixelData.nBPP);
	nBPW1 = nBPL1 / (PixelData.nBPP >> 3);

	nWIDTH2 = g_legacyDrawer.GetFrameWidth(hSRC);
	nBPL2 = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH2, PixelData.nBPP);
	nBPW2 = nBPL2 / (PixelData.nBPP >> 3);

	nSy2 = (nSx + nBPW2 * nSy);

	{
		uint16* pDATA1, * pDATA2;

		pDATA1 = (uint16*)g_legacyDrawer.GetFrameBuffer(hBack);
		pDATA2 = (uint16*)g_legacyDrawer.GetFrameBuffer(hSRC);

		if (PixelProcData.nMODE > EN_MODE_NORMAL) {
			if (bTRANS == TRUE) {
				for (nSy1 = (nX + nBPW1 * nY), nEy1 = (nSy1 + nBPW1 * nH); nSy1 < nEy1; nSy1 += nBPW1, nSy2 += nBPW2) {
					for (nSx2 = nSy2, nSx1 = nSy1, nEx1 = (nSy1 + nW); nSx1 < nEx1; nSx1++, nSx2 += nMX2) {
						if (PixelProcData.nTRANS != pDATA2[nSx2]) {
							pDATA1[nSx1] = PixelProc[PixelProcData.nMODE].proc(pDATA1[nSx1], pDATA2[nSx2]);
						}
					}
				}
			}
			else {
				for (nSy1 = (nX + nBPW1 * nY), nEy1 = (nSy1 + nBPW1 * nH); nSy1 < nEy1; nSy1 += nBPW1, nSy2 += nBPW2) {
					for (nSx2 = nSy2, nSx1 = nSy1, nEx1 = (nSy1 + nW); nSx1 < nEx1; nSx1++, nSx2 += nMX2) {
						pDATA1[nSx1] = PixelProc[PixelProcData.nMODE].proc(pDATA1[nSx1], pDATA2[nSx2]);
					}
				}
			}
		}
		else {
			if (bTRANS == TRUE) {
				for (nSy1 = (nX + nBPW1 * nY), nEy1 = (nSy1 + nBPW1 * nH); nSy1 < nEy1; nSy1 += nBPW1, nSy2 += nBPW2) {
					for (nSx2 = nSy2, nSx1 = nSy1, nEx1 = (nSy1 + nW); nSx1 < nEx1; nSx1++, nSx2 += nMX2) {
						if (PixelProcData.nTRANS != pDATA2[nSx2]) {
							pDATA1[nSx1] = pDATA2[nSx2];
						}
					}
				}
			}
			else {
				for (nSy1 = (nX + nBPW1 * nY), nEy1 = (nSy1 + nBPW1 * nH); nSy1 < nEy1; nSy1 += nBPW1, nSy2 += nBPW2) {
					for (nSx2 = nSy2, nSx1 = nSy1, nEx1 = (nSy1 + nW); nSx1 < nEx1; nSx1++, nSx2 += nMX2) {
						pDATA1[nSx1] = pDATA2[nSx2];
					}
				}
			}
		}
	}
}

void EFC_pxlCopyFrameBuffer(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC, sint32 nSx, sint32 nSy, ubool bTRANS)
{
	if (hBack == hSRC) {
		EFC_pxlCopyArea(hBack, nX, nY, nW, nH, nSx, nSy);
	}
	else {
		EFC_pxlSetFRAME(hBack, nX, nY, nW, nH, hSRC, nSx, nSy, bTRANS);
	}
}

void EFC_pxlCopyFrameScale(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC, sint32 nSX, sint32 nSY, sint32 nEx, sint32 nEy, ubool bTRANS)
{
	sint32 nWIDTH1, nBPL1, nBPW1, nBPL2, nBPW2, nWIDTH2, nHEIGHT2;
	sint32 i, j, nDX1, nDY1, nSY1, nSX1, nSX2, nGW, nGH, nPOS;

	uint16* pDATA1, * pDATA2;

	nWIDTH1 = g_legacyDrawer.GetFrameWidth(hBack);
	nBPL1 = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH1, PixelData.nBPP);
	nBPW1 = nBPL1 / (PixelData.nBPP >> 3);

	nWIDTH2 = g_legacyDrawer.GetFrameWidth(hSRC);
	nBPL2 = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH2, PixelData.nBPP);
	nBPW2 = nBPL2 / (PixelData.nBPP >> 3);

	nHEIGHT2 = g_legacyDrawer.GetFrameHeight(hSRC);

	nSX = MIN_CALC(MAX_CALC(0, nSX), (nWIDTH2 - 1));
	nSY = MIN_CALC(MAX_CALC(0, nSY), (nHEIGHT2 - 1));
	nEx = MIN_CALC(MAX_CALC(0, nEx), (nWIDTH2 - 1));
	nEy = MIN_CALC(MAX_CALC(0, nEy), (nHEIGHT2 - 1));

	nGW = ((nEx - nSX + 1) << 12) / nW;
	nGH = ((nEy - nSY + 1) << 12) / nH;

	pDATA1 = (uint16*)g_legacyDrawer.GetFrameBuffer(hBack);
	pDATA2 = (uint16*)g_legacyDrawer.GetFrameBuffer(hSRC);

	if (PixelProcData.nMODE != NULL) {
		if (bTRANS)
		{
			for (i = 0, nDY1 = (nY * nWIDTH1 + nX), nSY1 = 0; i < nH; i++, nDY1 += nWIDTH1, nSY1 += nGH) {
				for (j = 0, nDX1 = nDY1, nSX1 = ((nSY + (nSY1 >> 12)) * nWIDTH2 + nSX), nSX2 = 0; j < nW; j++, nDX1++, nSX2 += nGW) {
					nPOS = (nSX1 + (nSX2 >> 12));
					if (PixelProcData.nTRANS != pDATA2[nPOS]) {
						pDATA1[nDX1] = PixelProc[PixelProcData.nMODE].proc(pDATA1[nDX1], pDATA2[nPOS]);
					}
				}
			}
		}
		else {
			for (i = 0, nDY1 = (nY * nWIDTH1 + nX), nSY1 = 0; i < nH; i++, nDY1 += nWIDTH1, nSY1 += nGH) {
				for (j = 0, nDX1 = nDY1, nSX1 = ((nSY + (nSY1 >> 12)) * nWIDTH2 + nSX), nSX2 = 0; j < nW; j++, nDX1++, nSX2 += nGW) {
					nPOS = (nSX1 + (nSX2 >> 12));
					pDATA1[nDX1] = PixelProc[PixelProcData.nMODE].proc(pDATA1[nDX1], pDATA2[nPOS]);
				}
			}
		}
	}
	else if (bTRANS) {
		for (i = 0, nDY1 = (nY * nWIDTH1 + nX), nSY1 = 0; i < nH; i++, nDY1 += nWIDTH1, nSY1 += nGH) {
			for (j = 0, nDX1 = nDY1, nSX1 = ((nSY + (nSY1 >> 12)) * nWIDTH2 + nSX), nSX2 = 0; j < nW; j++, nDX1++, nSX2 += nGW) {
				nPOS = (nSX1 + (nSX2 >> 12));
				if (PixelProcData.nTRANS != pDATA2[nPOS]) {
					pDATA1[nDX1] = pDATA2[nPOS];
				}
			}
		}
	}
	else {
		for (i = 0, nDY1 = (nY * nWIDTH1 + nX), nSY1 = 0; i < nH; i++, nDY1 += nWIDTH1, nSY1 += nGH) {
			for (j = 0, nDX1 = nDY1, nSX1 = ((nSY + (nSY1 >> 12)) * nWIDTH2 + nSX), nSX2 = 0; j < nW; j++, nDX1++, nSX2 += nGW) {
				nPOS = (nSX1 + (nSX2 >> 12));
				pDATA1[nDX1] = pDATA2[nPOS];
			}
		}
	}
}

void EFC_pxlSetPixel(sint32 hBack, sint32 nX, sint32 nY)
{
	sint32 nWIDTH, nBPL, nBPW;

	nWIDTH = g_legacyDrawer.GetFrameWidth(hBack);
	nBPL = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH, PixelData.nBPP);
	nBPW = nBPL / (PixelData.nBPP >> 3);

	{
		uint16* pDATA, nCOLOR;

		nCOLOR = (uint16)PixelProcData.nCOLOR;
		pDATA = (uint16*)g_legacyDrawer.GetFrameBuffer(hBack);

		EFC_pxlRectPROC(pDATA, nBPW, nX, nY, 1, 1, nCOLOR);
	}
}

void EFC_pxlPutPixel(sint32 hBack, sint32 nX, sint32 nY)
{
	EFC_pxlSetPixel(hBack, nX, nY);
}

void EFC_pxlSetRect(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH)
{
	sint32 nWIDTH, nBPL, nBPW;

	nWIDTH = g_legacyDrawer.GetFrameWidth(hBack);
	nBPL = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH, PixelData.nBPP);
	nBPW = nBPL / (PixelData.nBPP >> 3);

	{
		uint16* pDATA, nCOLOR;

		nCOLOR = (uint16)PixelProcData.nCOLOR;
		pDATA = (uint16*)g_legacyDrawer.GetFrameBuffer(hBack);

		EFC_pxlRectPROC(pDATA, nBPW, nX, nY, nW, nH, nCOLOR);
	}
}

void EFC_pxlFillRect(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH)
{
	EFC_pxlSetRect(hBack, nX, nY, nW, nH);
}
/*
void EFC_pxlDrawShadow( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 dir, sint32 nMIR )
{
	EXMOVE rMov;
	EXREGION rgn;

	sint32 revX = 0, revY = 0, revW = 0, revH = 0;

	EFC_imgMIRROR(pIMG, &rMov, nMIR);

	EFC_rgnSetREGION( &rgn, nX - rMov.moveX, nY - rMov.moveY, pIMG->nW, pIMG->nH, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, nMIR ) == FALSE ) {
		return;
	}

	{
		MC_GrpFrameBuffer hSRC;
		hSRC =  MC_grpCreateOffScreenFrameBuffer( rgn.nW, rgn.nH );
		if( hSRC == 0 ) {
			return;
		}
		revX = (dir-2)%2*7;
		revY = (dir-1)%2*7;

		EFC_pxlDrawBuff( (*PixelData.pFrame), rgn.nX, rgn.nY, rgn.nW, rgn.nH, pIMG, rgn.nSx, rgn.nSy, nMIR, 0);

		EFC_grpReset();

		EFC_grpCopyBUFF(rgn.nX+revX, rgn.nY+revY, rgn.nW+revW, rgn.nH+revH,hSRC, 0,0,rgn.nW, rgn.nH, TRUE);
		MC_grpDestroyOffScreenFrameBuffer( hSRC );
	}
}
*/
void EFC_pxlDrawArc(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC)
{
	sint32 nWIDTH, nBPL, nBPW1, nBPW2;
	sint32 nSX1, nEX1, nSY1, nEY1, nSX2, nSY2 = 0;
	uint16* pDATA, * pDATA2;

	nWIDTH = g_legacyDrawer.GetFrameWidth(hBack);
	nBPL = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH, PixelData.nBPP);
	nBPW1 = nBPL / (PixelData.nBPP >> 3);

	nWIDTH = g_legacyDrawer.GetFrameWidth(hSRC);
	nBPL = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH, PixelData.nBPP);
	nBPW2 = nBPL / (PixelData.nBPP >> 3);

	pDATA = (uint16*)g_legacyDrawer.GetFrameBuffer(hBack);
	pDATA2 = (uint16*)g_legacyDrawer.GetFrameBuffer(hSRC);

	for (nSY1 = (nX + nBPW1 * nY), nEY1 = (nSY1 + nBPW1 * nH); nSY1 < nEY1; nSY1 += nBPW1, nSY2 += nBPW2) {
		for (nSX2 = nSY2, nSX1 = nSY1, nEX1 = (nSY1 + nW); nSX1 < nEX1; nSX1++, nSX2++) {
			if (pDATA2[nSX2] != PixelProcData.nTRANS)
			{
				pDATA2[nSX2] = pDATA[nSX1];
			}
		}
	}
}
void EFC_pxlDrawBuff(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, LPEXIMAGE pIMG, sint32 nSX, sint32 nSY, sint32 nMIR, sint32 nPalette)
{
	sint32 nWIDTH, nBPL, nBPW1, nBPW2;
	sint32 nSX1, nEX1, nSY1, nEY1, nSX2, nSY2 = 0, nMX1 = 2, nMX2 = 1;

	nWIDTH = g_legacyDrawer.GetFrameWidth(hBack);
	nBPL = g_legacyDrawer.GetBplWithWidth_PlusNumber7(nWIDTH, PixelData.nBPP);
	nBPW1 = nBPL / (PixelData.nBPP >> 3);
	nBPW2 = WIDTH_BYTES(pIMG->nW);

	{
		uint16* pDATA;
		ubyte* pDATA2 = EFC_memGET(pIMG->pBuff);
		sint32 nPIXEL = 0;
		uint32 nCOLOR;
		sint32 nPALPOS = MIN_CALC(nPalette, pIMG->nPalCnt - 1) * (pIMG->nPalLen << 2);
		sint32 nTRANS = CONVERT_INT(pDATA2, 0);
		pDATA = (uint16*)g_legacyDrawer.GetFrameBuffer(hBack);

		switch (nMIR) {
		case EN_MIRROR_NONE:
			nSY2 = (pIMG->nDATPOS + nSX + nBPW2 * (pIMG->nH - nSY - 1));
			break;
		case EN_MIRROR_HORIZON:
			if (nX <= 0) {
				nSX = 0;
			}
			else {
				nSX = pIMG->nW - nW;
			}
			nMX1 = -nMX1;
			nMX2 = -nMX2;
			nSY2 = (pIMG->nDATPOS + nSX + nBPW2 * (pIMG->nH - nSY - 1)) + nW - 1;
			break;
		case EN_MIRROR_VERTICAL:
			if (nY <= 0) {
				nSY = 0;
			}
			else {
				nSY = pIMG->nH - nH;
			}
			nSY2 = (pIMG->nDATPOS + nSX + nBPW2 * (pIMG->nH - nSY - nH));
			nBPW2 = -nBPW2;
			break;
		case EN_MIRROR_BOTH:
			if (nX <= 0) {
				nSX = 0;
			}
			else {
				nSX = pIMG->nW - nW;
			}
			if (nY <= 0) {
				nSY = 0;
			}
			else {
				nSY = pIMG->nH - nH;
			}
			nMX1 = -nMX1;
			nMX2 = -nMX2;
			nSY2 = (pIMG->nDATPOS + nSX + nBPW2 * (pIMG->nH - nSY - nH)) + nW - 1;
			nBPW2 = -nBPW2;
			break;
		}

		if (PixelProcData.nMODE != NULL) {
			if (nTRANS == PixelProcData.nTRANS) {
				for (nSY1 = (nX + nBPW1 * nY), nEY1 = (nSY1 + nBPW1 * nH); nSY1 < nEY1; nSY1 += nBPW1, nSY2 -= nBPW2) {
					for (nSX2 = nSY2, nSX1 = nSY1, nEX1 = (nSY1 + nW); nSX1 < nEX1; nSX1++, nSX2 += nMX2) {
						nPIXEL = (pDATA2[nSX2] << 2);
						if (nPIXEL > 0)
						{
							nCOLOR = CONVERT_INT(pDATA2, nPIXEL + nPALPOS);
							pDATA[nSX1] = PixelProc[PixelProcData.nMODE].proc(pDATA[nSX1], nCOLOR);
						}
					}
				}
			}
			else {
				for (nSY1 = (nX + nBPW1 * nY), nEY1 = (nSY1 + nBPW1 * nH); nSY1 < nEY1; nSY1 += nBPW1, nSY2 -= nBPW2) {
					for (nSX2 = nSY2, nSX1 = nSY1, nEX1 = (nSY1 + nW); nSX1 < nEX1; nSX1++, nSX2 += nMX2) {
						nPIXEL = (pDATA2[nSX2] << 2) + nPALPOS;
						nCOLOR = CONVERT_INT(pDATA2, nPIXEL);
						pDATA[nSX1] = PixelProc[PixelProcData.nMODE].proc(pDATA[nSX1], nCOLOR);
					}
				}
			}
		}
		else if (nTRANS == PixelProcData.nTRANS) {
			for (nSY1 = (nX + nBPW1 * nY), nEY1 = (nSY1 + nBPW1 * nH); nSY1 < nEY1; nSY1 += nBPW1, nSY2 -= nBPW2) {
				for (nSX2 = nSY2, nSX1 = nSY1, nEX1 = (nSY1 + nW); nSX1 < nEX1; nSX1++, nSX2 += nMX2) {
					nPIXEL = (pDATA2[nSX2] << 2);
					if (nPIXEL > 0)
					{
						nCOLOR = CONVERT_INT(pDATA2, nPIXEL + nPALPOS);
						pDATA[nSX1] = (uint16)nCOLOR;
					}
				}
			}
		}
		else {
			for (nSY1 = (nX + nBPW1 * nY), nEY1 = (nSY1 + nBPW1 * nH); nSY1 < nEY1; nSY1 += nBPW1, nSY2 -= nBPW2) {
				for (nSX2 = nSY2, nSX1 = nSY1, nEX1 = (nSY1 + nW); nSX1 < nEX1; nSX1++, nSX2 += nMX2) {
					nPIXEL = (pDATA2[nSX2] << 2) + nPALPOS;
					nCOLOR = CONVERT_INT(pDATA2, nPIXEL);
					pDATA[nSX1] = (uint16)nCOLOR;
				}
			}
		}
	}
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 - 

uint16 g_nCOS[180] = {
0x1000, 0x0FFD, 0x0FF6, 0x0FE9, 0x0FD8, 0x0FC1, 0x0FA6, 0x0F86, 0x0F61, 0x0F37,
0x0F08, 0x0ED5, 0x0E9D, 0x0E61, 0x0E20, 0x0DDB, 0x0D91, 0x0D43, 0x0CF1, 0x0C9B,
0x0C41, 0x0BE3, 0x0B82, 0x0B1D, 0x0AB4, 0x0A48, 0x09D9, 0x0967, 0x08F2, 0x087A,
0x0800, 0x0782, 0x0703, 0x0681, 0x05FE, 0x0578, 0x04F1, 0x0469, 0x03DE, 0x0353,
0x02C7, 0x023A, 0x01AC, 0x011D, 0x008E, 0x0000, 0xFF72, 0xFEE3, 0xFE54, 0xFDC6,
0xFD39, 0xFCAD, 0xFC22, 0xFB97, 0xFB0F, 0xFA88, 0xFA02, 0xF97F, 0xF8FD, 0xF87E,
0xF801, 0xF786, 0xF70E, 0xF699, 0xF627, 0xF5B8, 0xF54C, 0xF4E3, 0xF47E, 0xF41D,
0xF3BF, 0xF365, 0xF30F, 0xF2BD, 0xF26F, 0xF225, 0xF1E0, 0xF19F, 0xF163, 0xF12B,
0xF0F8, 0xF0C9, 0xF09F, 0xF07A, 0xF05A, 0xF03F, 0xF028, 0xF017, 0xF00A, 0xF003,
0xF000, 0xF003, 0xF00A, 0xF017, 0xF028, 0xF03F, 0xF05A, 0xF07A, 0xF09F, 0xF0C9,
0xF0F8, 0xF12B, 0xF163, 0xF19F, 0xF1E0, 0xF225, 0xF26F, 0xF2BD, 0xF30F, 0xF365,
0xF3BF, 0xF41D, 0xF47E, 0xF4E3, 0xF54C, 0xF5B8, 0xF627, 0xF699, 0xF70E, 0xF786,
0xF800, 0xF87E, 0xF8FD, 0xF97F, 0xFA02, 0xFA88, 0xFB0F, 0xFB97, 0xFC22, 0xFCAD,
0xFD39, 0xFDC6, 0xFE54, 0xFEE3, 0xFF72, 0x0000, 0x008E, 0x011D, 0x01AC, 0x023A,
0x02C7, 0x0353, 0x03DE, 0x0469, 0x04F1, 0x0578, 0x05FE, 0x0681, 0x0703, 0x0782,
0x07FF, 0x087A, 0x08F2, 0x0967, 0x09D9, 0x0A48, 0x0AB4, 0x0B1D, 0x0B82, 0x0BE3,
0x0C41, 0x0C9B, 0x0CF1, 0x0D43, 0x0D91, 0x0DDB, 0x0E20, 0x0E61, 0x0E9D, 0x0ED5,
0x0F08, 0x0F37, 0x0F61, 0x0F86, 0x0FA6, 0x0FC1, 0x0FD8, 0x0FE9, 0x0FF6, 0x0FFD };



ubool EFC_rgnIsInRECT(LPEXRECT pRect, sint32 nX, sint32 nY)
{
	sint32 nL, nT, nR, nB;

	nL = pRect->nX;
	nT = pRect->nY;
	nR = pRect->nX + pRect->nW - 1;
	nB = pRect->nY + pRect->nH - 1;

	if ((nX < nL) || (nX > nR) || (nY < nT) || (nY > nB)) {
		return FALSE;
	}

	return TRUE;
}

void EFC_rgnSetPOINT(LPEXPOINT pPT, sint32 nX, sint32 nY)
{
	pPT->nX = (sint16)nX;
	pPT->nY = (sint16)nY;
}

void EFC_rgnRealPOINT(LPEXPOINT pPT, sint32 nX, sint32 nY)
{
	if (EFC_grpIsMAIN(PixelData.hFrame) == TRUE) {
		EFC_rgnSetPOINT(pPT,
			nX + PixelData.ptTRANS.nX,
			nY + PixelData.ptTRANS.nY);
	}
	else {
		EFC_rgnSetPOINT(pPT, nX, nY);
	}
}

ubool EFC_rgnSetRECT(LPEXRECT pRect, sint32 nX, sint32 nY, sint32 nW, sint32 nH)
{
	if ((nW > 0) && (nH > 0)) {
		pRect->nX = (sint16)nX;
		pRect->nY = (sint16)nY;
		pRect->nW = (sint16)nW;
		pRect->nH = (sint16)nH;
		return TRUE;
	}

	MEMSET(pRect, 0, sizeof(EXRECT));
	return FALSE;
}

ubool EFC_rgnInterRECT(LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2)
{
	sint32 n1L, n1T, n1R, n1B;
	sint32 n2L, n2T, n2R, n2B;

	n1L = pRect1->nX;
	n1T = pRect1->nY;
	n1R = pRect1->nX + pRect1->nW - 1;
	n1B = pRect1->nY + pRect1->nH - 1;

	n2L = pRect2->nX;
	n2T = pRect2->nY;
	n2R = pRect2->nX + pRect2->nW - 1;
	n2B = pRect2->nY + pRect2->nH - 1;

	// CALC
	n1L = MAX_CALC(n1L, n2L);
	n1T = MAX_CALC(n1T, n2T);
	n1R = MIN_CALC(n1R, n2R);
	n1B = MIN_CALC(n1B, n2B);

	return EFC_rgnSetRECT(pRect, n1L, n1T, n1R - n1L + 1, n1B - n1T + 1);
}

ubool EFC_rgnUnionRECT(LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2)
{
	sint32 n1L, n1T, n1R, n1B;
	sint32 n2L, n2T, n2R, n2B;

	if ((pRect1->nW < 1) || (pRect1->nH < 1)) {
		return EFC_rgnSetRECT(pRect, pRect2->nX, pRect2->nY, pRect2->nW, pRect2->nH);
	}

	if ((pRect2->nW < 1) && (pRect2->nH < 1)) {
		return EFC_rgnSetRECT(pRect, pRect1->nX, pRect1->nY, pRect1->nW, pRect1->nH);
	}

	n1L = pRect1->nX;
	n1T = pRect1->nY;
	n1R = pRect1->nX + pRect1->nW - 1;
	n1B = pRect1->nY + pRect1->nH - 1;

	n2L = pRect2->nX;
	n2T = pRect2->nY;
	n2R = pRect2->nX + pRect2->nW - 1;
	n2B = pRect2->nY + pRect2->nH - 1;

	// CALC
	n1L = MIN_CALC(n1L, n2L);
	n1T = MIN_CALC(n1T, n2T);
	n1R = MAX_CALC(n1R, n2R);
	n1B = MAX_CALC(n1B, n2B);

	return EFC_rgnSetRECT(pRect, n1L, n1T, n1R - n1L + 1, n1B - n1T + 1);
}

ubool EFC_rgnPlusRECT(LPEXRECT pRect, LPEXRECT pRect1, sint32 nX, sint32 nY)
{
	sint32 n1L, n1T, n1R, n1B;

	n1L = pRect1->nX;
	n1T = pRect1->nY;
	n1R = pRect1->nX + pRect1->nW - 1;
	n1B = pRect1->nY + pRect1->nH - 1;

	// CALC
	n1L = MIN_CALC(n1L, nX);
	n1T = MIN_CALC(n1T, nY);
	n1R = MAX_CALC(n1R, nX);
	n1B = MAX_CALC(n1B, nY);

	return EFC_rgnSetRECT(pRect, n1L, n1T, n1R - n1L + 1, n1B - n1T + 1);
}

ubool EFC_rgnRealRECT(LPEXRECT pRect, LPEXRECT pRect1)
{
	EXREGION rgn;
	EFC_rgnSetREGION(&rgn, pRect1->nX, pRect1->nY, pRect1->nW, pRect1->nH, 0, 0);
	EFC_rgnRealREGION(&rgn, EN_MIRROR_NONE);
	return EFC_rgnGetRECT(pRect, &rgn);
}

ubool EFC_rgnSetREGION(LPEXREGION pRgn, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy)
{
	if ((nW > 0) && (nH > 0)) {
		pRgn->nX = (sint16)nX;
		pRgn->nY = (sint16)nY;
		pRgn->nW = (sint16)nW;
		pRgn->nH = (sint16)nH;
		pRgn->nSx = (sint16)nSx;
		pRgn->nSy = (sint16)nSy;
		return TRUE;
	}

	MEMSET(pRgn, 0, sizeof(EXREGION));
	return FALSE;
}

ubool EFC_rgnInterREGION(LPEXREGION pRgn, LPEXRECT pRect, LPEXREGION pRegion)
{
	sint32 nBackL, nBackT, nBackR, nBackB;
	sint32 nL, nT, nR, nB, nSx, nSy;

	// 드로잉 영역
	nBackL = pRect->nX;
	nBackT = pRect->nY;
	nBackR = pRect->nX + pRect->nW - 1;
	nBackB = pRect->nY + pRect->nH - 1;

	// 이미지 영역
	nL = pRegion->nX;
	nT = pRegion->nY;
	nR = pRegion->nX + pRegion->nW - 1;
	nB = pRegion->nY + pRegion->nH - 1;
	nSx = pRegion->nSx;
	nSy = pRegion->nSy;

	// CALC
	nSx = MAX_CALC((nSx + nBackL - nL), nSx);
	nL = MAX_CALC(nL, nBackL);
	nSy = MAX_CALC((nSy + nBackT - nT), nSy);
	nT = MAX_CALC(nT, nBackT);
	nR = MIN_CALC(nR, nBackR);
	nB = MIN_CALC(nB, nBackB);

	return EFC_rgnSetREGION(pRgn, nL, nT, nR - nL + 1, nB - nT + 1, nSx, nSy);
}

void EFC_rgnSetTRANS(LPEXPOINT pPT, sint32 nX, sint32 nY)
{
	if (EFC_grpIsMAIN(PixelData.hFrame) == TRUE) {
		EFC_rgnSetPOINT(pPT, nX + PixelData.ptTRANS.nX, nY + PixelData.ptTRANS.nY);
	}
	else {
		EFC_rgnSetPOINT(pPT, nX, nY);
	}
}

void EFC_rgnSetANNUN(LPEXPOINT pPT, sint32 nX, sint32 nY)
{
	EFC_rgnSetPOINT(pPT, nX, nY);
}

//ubool EFC_rgnRealREGION( LPEXREGION pRgn )
//{
//	EXPOINT pt;
//	EXRECT rt;
//
//	EFC_rgnSetRECT( &rt, 0, 0, 0x00007FFF, 0x00007FFF );
//	EFC_rgnInterRECT( &rt, &PixelData.rtCLIP, &rt );
//
//	EFC_rgnSetTRANS( &pt, 0, 0 );
//	EFC_rgnSetREGION( pRgn, pRgn->nX + pt.nX, pRgn->nY + pt.nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy );
//	EFC_rgnInterREGION( pRgn, &rt, pRgn );
//
//	EFC_rgnSetANNUN( &pt, 0, 0 );
//	return EFC_rgnSetREGION( pRgn, pRgn->nX + pt.nX, pRgn->nY + pt.nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy );
//}

ubool EFC_rgnRealREGION(LPEXREGION pRgn, sint32 nMIRROR)
{
	EXREGION rgn;

	pRgn->nX = (sint16)(pRgn->nX + PixelData.ptTRANS.nX);
	pRgn->nY = (sint16)(pRgn->nY + PixelData.ptTRANS.nY);

	switch (nMIRROR) {
	case EN_MIRROR_NONE:
		EFC_rgnInterREGION(pRgn, &PixelData.rtCLIP, pRgn);
		break;
	case EN_MIRROR_HORIZON:
		EFC_rgnSetREGION(&rgn, pRgn->nX, pRgn->nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy);
		EFC_rgnInterREGION(pRgn, &PixelData.rtCLIP, pRgn);
		if ((rgn.nX + rgn.nW) < (PixelData.rtCLIP.nX + PixelData.rtCLIP.nW)) {
			pRgn->nSx = rgn.nSx;
		}
		else {
			pRgn->nSx = (sint16)MAX_CALC(0, ((rgn.nX + rgn.nW) - (PixelData.rtCLIP.nX + PixelData.rtCLIP.nW)));
		}
		break;
	case EN_MIRROR_VERTICAL:
		EFC_rgnSetREGION(&rgn, pRgn->nX, pRgn->nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy);
		EFC_rgnInterREGION(pRgn, &PixelData.rtCLIP, pRgn);
		if ((rgn.nY + rgn.nH) < (PixelData.rtCLIP.nY + PixelData.rtCLIP.nH)) {
			pRgn->nSy = rgn.nSy;
		}
		else {
			pRgn->nSy = (sint16)MAX_CALC(0, ((rgn.nY + rgn.nH) - (PixelData.rtCLIP.nY + PixelData.rtCLIP.nH)));
		}
		break;
	case EN_MIRROR_BOTH:
		EFC_rgnSetREGION(&rgn, pRgn->nX, pRgn->nY, pRgn->nW, pRgn->nH, pRgn->nSx, pRgn->nSy);
		EFC_rgnInterREGION(pRgn, &PixelData.rtCLIP, pRgn);
		if ((rgn.nX + rgn.nW) < (PixelData.rtCLIP.nX + PixelData.rtCLIP.nW)) {
			pRgn->nSx = rgn.nSx;
		}
		else {
			pRgn->nSx = (sint16)MAX_CALC(0, ((rgn.nX + rgn.nW) - (PixelData.rtCLIP.nX + PixelData.rtCLIP.nW)));
		}
		if ((rgn.nY + rgn.nH) < (PixelData.rtCLIP.nY + PixelData.rtCLIP.nH)) {
			pRgn->nSy = rgn.nSy;
		}
		else {
			pRgn->nSy = (sint16)MAX_CALC(0, ((rgn.nY + rgn.nH) - (PixelData.rtCLIP.nY + PixelData.rtCLIP.nH)));
		}
		break;
	}

	if ((pRgn->nW < 1) || (pRgn->nH < 1)) {
		return FALSE;
	}

	return TRUE;
}

ubool EFC_rgnGetRECT(LPEXRECT pRect, LPEXREGION pRegion)
{
	return EFC_rgnSetRECT(pRect, pRegion->nX, pRegion->nY, pRegion->nW, pRegion->nH);
}

sint32 EFC_mathiSqrt(uint32 nX)
{
	uint32 m = 0x40000000;
	uint32 r = 0, nr;

	do {
		nr = m + r;
		r >>= 1;

		if (nr <= nX) {
			nX -= nr;
			r += m;
		}

		m >>= 2;
	} while (m != 0);

	return ((sint32)r);
}
// 타원 방정식
// x^2 / a^2 + y^2 / b^2 = 1
sint32 EFC_mathCircleX(sint32 nX, sint32 nW, sint32 nH)
{
	return EFC_mathiSqrt(nH * nH * (nW * nW - nX * nX) / (nW * nW));

	//	return EFC_mathiSqrt( (((nW * nH) - (nH * nX)) * ((nW * nH) + (nH * nX))) / (nW * nW) );
}

sint32 EFC_mathCircleY(sint32 nY, sint32 nW, sint32 nH)
{
	return EFC_mathiSqrt(nW * nW * (nH * nH - nY * nY) / (nH * nH));

	//	return EFC_mathiSqrt( ((nW * nH - nW * nY) * (nW * nH + nW * nY)) / (nH * nH) );
}

sint32 EFC_mathSIN(sint32 nANGLE)
{
	nANGLE += 270;
	while (nANGLE < 0) {
		nANGLE += 360;
	}
	nANGLE %= 360;

	return (sint16)g_nCOS[(nANGLE >> 1)];
}

sint32 EFC_mathCOS(sint32 nANGLE)
{
	while (nANGLE < 0) {
		nANGLE += 360;
	}
	nANGLE %= 360;

	return (sint16)g_nCOS[(nANGLE >> 1)];
}

void EFC_mathNORMAL(LPEXPOINT pt, sint32 nDIST)
{
	sint64 nCALC;
	sint32 nX, nY;

	if (pt->nX == 0) {
		nX = 0;
		nY = nDIST;
	}
	else if (pt->nY == 0) {
		nX = nDIST;
		nY = 0;
	}
	else {
		nCALC = ((sint64)nDIST * nDIST * pt->nX * pt->nX) / (((sint64)pt->nX * pt->nX) + ((sint64)pt->nY * pt->nY));
		nX = EFC_mathiSqrt((sint32)nCALC);

		nCALC = ((sint64)nDIST * nDIST) - (nX * nX);
		nY = EFC_mathiSqrt((sint32)nCALC);
	}

	if (pt->nX < 0) {
		pt->nX = (sint16)-nX;
	}
	else {
		pt->nX = (sint16)nX;
	}

	if (pt->nY < 0) {
		pt->nY = (sint16)-nY;
	}
	else {
		pt->nY = (sint16)nY;
	}
}

void EFC_mathMOVE(LPEXPOINT ptMOVE, LPEXPOINT ptCURR, LPEXPOINT ptNEXT, LPEXPOINT ptDIRECT, sint32 nMOVE, uint8 nRATE1, uint16 nRATE2)
{
	EXPOINT ptCALC;

	EFC_rgnSetPOINT(&ptCALC, (sint32)ptNEXT->nX - (sint32)ptCURR->nX, (sint32)ptNEXT->nY - (sint32)ptCURR->nY);

	EFC_mathNORMAL(&ptCALC, (sint32)nRATE1 * 100);
	EFC_mathNORMAL(ptDIRECT, (sint32)nRATE2 * 100);

	EFC_rgnSetPOINT(ptDIRECT, (sint32)ptCALC.nX + (sint32)ptDIRECT->nX, (sint32)ptCALC.nY + (sint32)ptDIRECT->nY);

	EFC_rgnSetPOINT(ptMOVE, ptDIRECT->nX, ptDIRECT->nY);
	EFC_mathNORMAL(ptMOVE, nMOVE);
}
