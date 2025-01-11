#pragma once


typedef struct _MC_GrpFrameBuffer
{
	M_Int32 w;
	M_Int32 h;
	M_Int32 bpl;
	M_Int32 bpp;
	M_Int32 frameBufID;

	LPEXBUFF buffer;

} MC_GrpFrameBuffer;

#define MAX_FRAME_BUFFER	(12)


class LegacyDrawer
{
public:
	LegacyDrawer();
	~LegacyDrawer();

	void Init(uint32* pScreenBuffer, int width, int height);
	void ReleaseAll();


public:
	sint32 GetMainScreenIndex();
	schar* GetFrameBuffer(sint32 hIndex);
	sint32 GetFrameWidth(sint32 hIndex);
	sint32 GetFrameHeight(sint32 hIndex);
	uint8  GetFrameBpp(sint32 hIndex);
	sint32 GetBplWithWidth_PlusNumber7(sint32 width, sint32 bpp);

	sint32 CreateOffScreenBuffer(sint32 width, sint32 bpp);
	void ReleaseOffScreenBuffer(sint32 hIndex);

	void Flush();

private:
	int	mainScreenWidth, mainScreenHeight;
	int mainScreenBpp, mainScreenBpl;
	uint32* mainScreenBuffer;
	int mainScreenIndex, frameBufferCounter;

	MC_GrpFrameBuffer* pGrpFrameBuffer[MAX_FRAME_BUFFER];


public:
	sint32 midX, midY, stdX, stdY, menuX, menuY;
	sint32 LCD_W, LCD_H;

};

extern LegacyDrawer g_legacyDrawer;
extern PIXELDATA PixelData;

extern void EFC_grpGetANCHOR(LPEXRECT pRect, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nANCHOR);
extern ubool EFC_grpIsMAIN(sint32 hFrame);
extern sint32 EFC_grpSetFRAME(sint32 hFrame);
extern void EFC_grpCopyAREA(sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy);
extern void EFC_grpCopyFRAME(sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hFrame, sint32 nSx, sint32 nSy, ubool bTRANS);
extern void EFC_grpCopyBUFF(sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hFrame, sint32 nSx, sint32 nSy, sint32 nEx, sint32 nEy, ubool bTRANS);
extern void EFC_grpSetCLIP(sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_grpSetTRANS(sint32 nX, sint32 nY);
extern void EFC_grpSetCOLOR(sint32 nCOLOR);
extern void EFC_grpSetRGB(sint32 nRGB);
extern void EFC_grpInitMODE(void);
extern void EFC_grpSetMODE(sint32 nMODE, sint32 nEFF);
extern void EFC_grpReset(void);

// DRAW
extern void EFC_grpDrawPIXEL(sint32 nX, sint32 nY);
extern void EFC_grpDrawLINE(sint32 nX1, sint32 nY1, sint32 nX2, sint32 nY2);
extern void EFC_grpDrawRECT(sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_grpFillRECT(sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_grpDrawPOLYGON(sint32 nX, sint32 nY, LPEXPOINT pPTs, sint32 nCOUNT);
extern void EFC_grpFillPOLYGON(sint32 nX, sint32 nY, LPEXPOINT pPTs, sint32 nCOUNT);
extern void EFC_grpDrawRoundRECT(sint32 nTYPE, sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_grpFillRoundRECT(sint32 nTYPE, sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_grpFillSCREEN(void);
extern void EFC_grpFillGradientV(sint32 nX, sint32 nY, sint32 nW, sint32 nH, RGBVAL nRGB_T, RGBVAL nRGB_B);
extern void EFC_grpFillGradientH(sint32 nX, sint32 nY, sint32 nW, sint32 nH, RGBVAL nRGB_L, RGBVAL nRGB_R);
extern void EFC_grpFillOpacityV(sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_grpFillOpacityVEX(sint32 nX, sint32 nY, sint32 nW, sint32 nH, RGBVAL nRGB_T, RGBVAL nRGB_B);
extern void EFC_grpDrawARC(sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_grpDrawARC2(sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nLINES);
extern void EFC_grpFillARC(sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_grpFillSemiARC(sint32 nX, sint32 nY, sint32 nW, sint32 nH, ubool bUP);
extern void EFC_grpDrawBEZIER(sint32 nX1, sint32 nY1, sint32 nX2, sint32 nY2, sint32 nX3, sint32 nY3, sint32 nX4, sint32 nY4, sint32 nCOUNT);
extern void EFC_grpDrawClipIMAGE(sint32 nX, sint32 nY, sint32 nW, sint32 nH, LPEXIMAGE pIMG, sint32 nSx, sint32 nSy, sint32 nPal);
extern void EFC_grpDrawIMAGE(sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nANCHOR);
extern void EFC_grpDrawSPRITE(sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nANCHOR);
extern void EFC_grpDrawSpritePal(sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nPal);
extern void EFC_grpFillBLUR(sint32 hFrame);
extern void EFC_grpDrawMirror(sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nMIR, sint32 nPalette);


extern ubool EFC_rgnIsInRECT(LPEXRECT pRect, sint32 nX, sint32 nY);
extern void EFC_rgnSetPOINT(LPEXPOINT pPT, sint32 nX, sint32 nY);
extern void EFC_rgnRealPOINT(LPEXPOINT pPT, sint32 nX, sint32 nY);
extern ubool EFC_rgnSetRECT(LPEXRECT pRect, sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern ubool EFC_rgnInterRECT(LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2);
extern ubool EFC_rgnUnionRECT(LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2);
extern ubool EFC_rgnPlusRECT(LPEXRECT pRect, LPEXRECT pRect1, sint32 nX, sint32 nY);
extern ubool EFC_rgnRealRECT(LPEXRECT pRect, LPEXRECT pRect1);
extern ubool EFC_rgnSetREGION(LPEXREGION pRgn, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy);
extern ubool EFC_rgnInterREGION(LPEXREGION pRgn, LPEXRECT pRect, LPEXREGION pRegion);
extern ubool EFC_rgnRealREGION(LPEXREGION pRgn, sint32 nMIR);
extern ubool EFC_rgnGetRECT(LPEXRECT pRect, LPEXREGION pRegion);
extern sint32 EFC_mathiSqrt(uint32 nX);
extern sint32 EFC_mathCircleX(sint32 nX, sint32 nW, sint32 nH);
extern sint32 EFC_mathCircleY(sint32 nY, sint32 nW, sint32 nH);
extern sint32 EFC_mathSIN(sint32 nANGLE);
extern sint32 EFC_mathCOS(sint32 nANGLE);
extern void EFC_mathMOVE(LPEXPOINT ptMOVE, LPEXPOINT ptCURR, LPEXPOINT ptNEXT, LPEXPOINT ptDIRECT, sint32 nMOVE, uint8 nRATE1, uint16 nRATE2);

extern void EFC_pxlCopyArea(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy);
extern void EFC_pxlCopyFrameBuffer(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC, sint32 nSx, sint32 nSy, ubool bTRANS);
extern void EFC_pxlCopyFrameScale(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC, sint32 nSX, sint32 nSY, sint32 nEx, sint32 nEy, ubool bTRANS);
extern void EFC_pxlPutPixel(sint32 hBack, sint32 nX, sint32 nY);
extern void EFC_pxlFillRect(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_pxlDrawBuff(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, LPEXIMAGE pIMG, sint32 nSX, sint32 nSY, sint32 nMIR, sint32 nPalette);
extern void EFC_pxlDrawArc(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC);
