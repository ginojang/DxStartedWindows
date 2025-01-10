#ifndef __EFC_GRAPHICS_HEADER__
#define __EFC_GRAPHICS_HEADER__

#include "EFCknl.h"

	enum {
		EN_TOP = 0x00000000,
		EN_VCENTER = 0x00000010,
		EN_BOTTOM = 0x00000020,
		EN_UP = 0x00000030,
		EN_DOWN = 0x00000040
	};

	enum {
		EN_LEFT = 0x00000000,
		EN_HCENTER = 0x00000001,
		EN_RIGHT = 0x00000002,
		EN_MIX = 0x00000003
	};

	typedef enum _ScreenOrientation
	{
		SCREEN_ORIENTATION_PORTRAIT = 0,
		SCREEN_ORIENTATION_LANDSCAPE,
		SCREEN_ORIENTATION_KIND
	}ScreenOrientation;

	typedef struct tagEXMOVE {
		sint16		moveX, moveY;
	} EXMOVE, *LPEXMOVE;

	extern void EFC_grpInitialize( void );
	extern void EFC_grpFinalize( void );
	extern void EFC_grpSetScreenOrientation(ScreenOrientation nMode);
	extern sint8 EFC_grpGetOrientationMode(void);
	extern void EFC_grpTransformMousePos(LPEXPOINT pPoint);
	extern void EFC_grpFLUSH( void );

	extern void EFC_grpGetANCHOR( LPEXRECT pRect, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nANCHOR );
	extern ubool EFC_grpIsMAIN( MC_GrpFrameBuffer *pFrame );

	extern MC_GrpFrameBuffer *EFC_grpSetFRAME( MC_GrpFrameBuffer *pFrame );
	extern void EFC_grpCopyAREA( sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy );
	extern void EFC_grpCopyFRAME( sint32 nX, sint32 nY, sint32 nW, sint32 nH, MC_GrpFrameBuffer *pFrame, sint32 nSx, sint32 nSy, ubool bTRANS );
	extern void EFC_grpCopyBUFF( sint32 nX, sint32 nY, sint32 nW, sint32 nH, MC_GrpFrameBuffer pFrame, sint32 nSx, sint32 nSy, sint32 nEx, sint32 nEy, ubool bTRANS );

	extern void EFC_grpSetCLIP( sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	extern void EFC_grpSetLOCK( sint32 nLOCK );
	extern void EFC_grpSetTRANS( sint32 nX, sint32 nY );
	extern void EFC_grpSetCOLOR( sint32 nCOLOR );
	extern void EFC_grpSetRGB( sint32 nRGB );

	extern void EFC_grpInitMODE( void );
	extern void EFC_grpSetMODE( sint32 nMODE, sint32 nEFF );
	extern void EFC_grpReset( void );

	extern void EFC_grpSetGRADE( sint32 nR, sint32 nG, sint32 nB );

	// DRAW
	extern void EFC_grpDrawPIXEL( sint32 nX, sint32 nY );
	extern void EFC_grpDrawLINE( sint32 nX1, sint32 nY1, sint32 nX2, sint32 nY2 );

	extern void EFC_grpDrawRECT( sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	extern void EFC_grpFillRECT( sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	extern void EFC_grpDrawPOLYGON( sint32 nX, sint32 nY, LPEXPOINT pPTs, sint32 nCOUNT );
	extern void EFC_grpFillPOLYGON( sint32 nX, sint32 nY, LPEXPOINT pPTs, sint32 nCOUNT );
	extern void EFC_grpDrawRoundRECT( sint32 nTYPE, sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	extern void EFC_grpFillRoundRECT( sint32 nTYPE, sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	extern void EFC_grpFillSCREEN( void );

	extern void EFC_grpFillGradientV( sint32 nX, sint32 nY, sint32 nW, sint32 nH, RGBVAL nRGB_T, RGBVAL nRGB_B );
	extern void EFC_grpFillGradientH( sint32 nX, sint32 nY, sint32 nW, sint32 nH, RGBVAL nRGB_L, RGBVAL nRGB_R );
	extern void EFC_grpFillOpacityV( sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	extern void EFC_grpFillOpacityVEX( sint32 nX, sint32 nY, sint32 nW, sint32 nH, RGBVAL nRGB_T, RGBVAL nRGB_B );

	extern void EFC_grpDrawARC( sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	extern void EFC_grpDrawARC2( sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nLINES );
	extern void EFC_grpFillARC( sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	extern void EFC_grpFillSemiARC( sint32 nX, sint32 nY, sint32 nW, sint32 nH, ubool bUP );
	extern void EFC_grpDrawBEZIER( sint32 nX1, sint32 nY1, sint32 nX2, sint32 nY2, sint32 nX3, sint32 nY3, sint32 nX4, sint32 nY4, sint32 nCOUNT );

	extern void EFC_grpDrawClipIMAGE( sint32 nX, sint32 nY, sint32 nW, sint32 nH, LPEXIMAGE pIMG, sint32 nSx, sint32 nSy, sint32 nPal );
	extern void EFC_grpDrawIMAGE( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nANCHOR );
	extern void EFC_grpDrawSPRITE( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nANCHOR );
	extern void EFC_grpDrawIMAGEMirror( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nANCHOR, sint32 nMIRROR );
	extern void EFC_grpDrawSpritePal( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nPal );
	//extern void EFC_grpDrawSPRITEMirror( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nANCHOR, sint32 nMIRROR );

	extern void EFC_grpFillGausianBlur( MC_GrpFrameBuffer *pFrame,sint32 sX, sint32 sY, sint32 sW, sint32 sH, uint8 size );
	extern void EFC_grpFillBLUR( MC_GrpFrameBuffer *pFrame );
	extern void EFC_grpDrawMirror( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nMIR, sint32 nPalette );
/*
	extern void EFC_grpSetSCALE( MC_GrpFrameBuffer hINPUT, sint32 nScaleX, sint32 nScaleY, MC_GrpFrameBuffer *pOUTPUT );
	extern void EFC_grpDrawIMAGEEX( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 nANCHOR, sint32 nROTATE, sint32 nSCALE );
*/
#endif
