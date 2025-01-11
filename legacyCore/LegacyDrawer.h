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


extern void MC_grpFlushLcd(M_Int32 i, M_Int32 frm, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h);


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


extern void EFC_pxlInitialize(void);
extern void EFC_pxlFinalize(void);
extern void EFC_pxlCopyArea(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy);
extern void EFC_pxlCopyFrameBuffer(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC, sint32 nSx, sint32 nSy, ubool bTRANS);
extern void EFC_pxlCopyFrameScale(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC, sint32 nSX, sint32 nSY, sint32 nEx, sint32 nEy, ubool bTRANS);
extern void EFC_pxlPutPixel(sint32 hBack, sint32 nX, sint32 nY);
extern void EFC_pxlFillRect(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern void EFC_pxlDrawBuff(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, LPEXIMAGE pIMG, sint32 nSX, sint32 nSY, sint32 nMIR, sint32 nPalette);
extern void EFC_pxlDrawArc(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC);
