#ifndef __EFC_PIXEL_HEADER__
#define __EFC_PIXEL_HEADER__

#include "EFCknl.h"

	#define CHANGE_COLORS 4

	enum {
		EN_MODE_NORMAL = 0,
		EN_MODE_IMAGE,
		EN_MODE_AND,
		EN_MODE_OR,
		EN_MODE_MULTIPLY,
		EN_MODE_SCREEN,
		EN_MODE_LIGHTEN,
		EN_MODE_LINEAR_DODGE,

		EN_MODE_ALPHA,
		EN_MODE_BRIGHT,
		EN_MODE_DARK,		

		EN_MODE_GRAY,
		EN_MODE_ONE,

		EN_MODE_GRAYGRADE,

		EN_MODE_ALPHA_112,
		EN_MODE_ALPHA_128,
		EN_MODE_ALPHA_ETC,
		EN_MODE_XOR,

		EN_MODE_SCREENGRAY,
		EN_MODE_SCREENSEPIA,

		EN_MODE_LAST
	};

	typedef struct tagPIXELPROC {
		uint16 (*proc)( sint32 nBACK, sint32 nSRC );
	} PIXELPROC, *LPPIXELPROC;

	typedef struct tagPIXELDATA {
		sint32	hReal;
		sint32	hScreen;
		sint32	hLandScapeScreen;
		sint32  hFrame;

		MC_GrpContext		hGC;

		/////////////////////////////////////
		// PIXEL
		sint32				nRGB1[CHANGE_COLORS];
		sint32				nRGB2[CHANGE_COLORS];

		uint8				nMODE;
		uint16				nEFF;
		sint32				nCALC;
		/////////////////////////////////////

		uint8				nBPP;
		sint8				nANNUN; // ANNUNCIATOR
		EXRECT				rtMAIN; // 화면 RECT
		EXRECT				rtREAL; // 화면 RECT

		sint32				nTRANS;

		ubool				bFULL;
		sint32				nCOLOR;

		EXPOINT				ptTRANS; // 이동 좌표 POINT
		EXRECT				rtCLIP; // 클리핑 RECT

		sint8				nLOCK; // 모드 설정을 막는다
		sint8				nRESV;

		sint8				nScreenOrientation;
		ubool				isDoubleScreen;
	} PIXELDATA, *LPPIXELDATA;

	extern PIXELDATA PixelData;
	extern PIXELPROC PixelProc[EN_MODE_LAST];

	//extern uint16 EFC_pixel_CONVERT_Proc( sint32 srcpxl, sint32 orgpxl, sint32 param1 );

	extern void EFC_pxlInitialize( void );
	extern void EFC_pxlFinalize( void );

	extern void EFC_pxlCopyArea( sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy );
	extern void EFC_pxlCopyFrameBuffer(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC, sint32 nSx, sint32 nSy, ubool bTRANS );
	extern void EFC_pxlCopyFrameScale(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC, sint32 nSX, sint32 nSY, sint32 nEx, sint32 nEy, ubool bTRANS );
	extern void EFC_pxlPutPixel(sint32 hBack, sint32 nX, sint32 nY );
	extern void EFC_pxlFillRect(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	//extern void EFC_pxlDrawImage( sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, MC_GrpImage imgSRC, sint32 nSx, sint32 nSy, MC_GrpContext *pGC );
	extern void EFC_pxlDrawBuff(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, LPEXIMAGE pIMG, sint32 nSX, sint32 nSY, sint32 nMIR, sint32 nPalette);

	extern void EFC_pxlDrawShadow( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 dir, sint32 nMIR );
	extern void EFC_pxlDrawArc(sint32 hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 hSRC );

	/////////////////////////////////////////////////////////
	// 아래의 모든 계산에 들어가는 N SHIFT는 2^N - 1 값으로
	// 나누는 것이 맞지만 속도를 위해 SHIFT를 사용하였습니다.
	// 범위는 0 - 255입니다
	/////////////////////////////////////////////////////////
/*
	union {
		uint16 nRGB;
		struct {
			unsigned Blue:5;
			unsigned Green:6;
			unsigned Red:5;
			unsigned Reserved:1;
		} rgb;
	} RGB565;
*/
	extern sint32 midX;
	extern sint32 midY;
	extern sint32 stdX;
	extern sint32 stdY;
	extern sint32 menuX;
	extern sint32 menuY;
	extern sint32 LCD_W;
	extern sint32 LCD_H;

	/////////////////////////////////////////////////////////
	// CALC

	#define RGB32RGB16(r,g,b) ((uint16)((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3)))

	#define ABS32(a) (((a) < 0) ? -(a) : (a))

	#define MIN_CALC(a,b) (((a) < (b)) ? (a) : (b))
	#define MAX_CALC(a,b) (((a) > (b)) ? (a) : (b))
	#define RANGE_CALC(a,min,max) ((a) < (min) ? (min) : ((a) > (max) ?(max):(a)))  //MIN_CALC(MAX_CALC((a),(min)), (max))

	#define WIDTH_BYTES(w) ((((w) + 3) >> 2) << 2)
	#define WIDTH_HEIGHT_BIT_BYTES(w,h) (((w) * (h) + 7) >> 3)

	#define BIT_SET1(a,n) ((a) | (0x1 << (n)))
	#define BIT_SET0(a,n) ((a) ^ ~(0x1 << (n)))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// RGB32
	#define RGB8_MAX 0x000000FF

#ifdef RGB
#undef RGB
#endif
	#define RGB(r,g,b) ( ((r) << 16) | ((g) << 8) | (b) )

	#define RGB_R(c) ( ((c) >> 16) & RGB8_MAX )
	#define RGB_G(c) ( ((c) >> 8) & RGB8_MAX )
	#define RGB_B(c) ( (c) & RGB8_MAX )

	#define RGB_NONE (0xffffffffl)
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// RGB16
	#define RGB5_MAX 0x0000001F
	#define RGB6_MAX 0x0000003F

	#define R16_MAX 0x0000F800
	#define G16_MAX 0x000007E0
	#define B16_MAX 0x0000001F

	#define RGB16_R(c) ( ((c) >> 11) & RGB5_MAX )
	#define RGB16_G(c) ( ((c) >> 5) & RGB6_MAX )
	#define RGB16_B(c) ( (c) & RGB5_MAX )

	#define RGB16(r,g,b) ( ((r) << 11) | ((g) << 5) | (b) )

	#define COLOR16_R(c) ( (c) & R16_MAX )
	#define COLOR16_G(c) ( (c) & G16_MAX )
	#define COLOR16_B(c) ( (c) & B16_MAX )

	#define COLOR16(r,g,b) ( (r) | (g) | (b) )
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	#define GRADE_CALC(b,s,grade) ((b) + ((((s) - (b)) * (grade)) >> 8))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// ALPHA : background, sprite, alpha
	#define ALPHA_CALC(b,s,alpha) GRADE_CALC(b,s,alpha)
	//#define ALPHA_CALC(b,s,alpha) (((b) + ((((s) - (b)) << (4)) >> 8)))

	#define ALPHA16_R(b,s,alpha) (ALPHA_CALC(RGB16_R((b)), RGB16_R((s)), (alpha)))
	#define ALPHA16_G(b,s,alpha) (ALPHA_CALC(RGB16_G((b)), RGB16_G((s)), (alpha)))
	#define ALPHA16_B(b,s,alpha) (ALPHA_CALC(RGB16_B((b)), RGB16_B((s)), (alpha)))

	//(nBACK+ (((nBACK - nSRC) * PixelData.nEFF + 0x8010)>>5))&0xf81f; - //
	#define ALPHA16(b,s,alpha) (RGB16(ALPHA16_R((b),(s),(alpha)), ALPHA16_G((b),(s),(alpha)), ALPHA16_B((b),(s),(alpha))))

	#define ALPHA_CALC2(b,s,p1,p2) ((b) + (((((s) - (b))<<(p1))+(((s) - (b))<<(p2)))>>4))
	#define ALPHA16_R2(b,s,p1,p2) (ALPHA_CALC2(RGB16_R((b)), RGB16_R((s)),(p1),(p2)))
	#define ALPHA16_G2(b,s,p1,p2) (ALPHA_CALC2(RGB16_G((b)), RGB16_G((s)),(p1),(p2)))
	#define ALPHA16_B2(b,s,p1,p2) (ALPHA_CALC2(RGB16_B((b)), RGB16_B((s)),(p1),(p2)))
	#define ALPHA16_ETC(b,s,p1,p2) (RGB16(ALPHA16_R2((b),(s),(p1),(p2)), ALPHA16_G2((b),(s),(p1),(p2)), ALPHA16_B2((b),(s),(p1),(p2))))

	#define ALPHA_CALC3(b,s) ((b) + (((((s) - (b))<<(3))-((s) - (b)))>>4))
	#define ALPHA16_R3(b,s) (ALPHA_CALC3(RGB16_R((b)), RGB16_R((s))))
	#define ALPHA16_G3(b,s) (ALPHA_CALC3(RGB16_G((b)), RGB16_G((s))))
	#define ALPHA16_B3(b,s) (ALPHA_CALC3(RGB16_B((b)), RGB16_B((s))))
	#define ALPHA16_112(b,s) (RGB16(ALPHA16_R3((b),(s)), ALPHA16_G3((b),(s)), ALPHA16_B3((b),(s))))

	#define ALPHA16_128(b,s) ((((b) >> 1) & 0x00007BEF) + (((s) >> 1) & 0x00007BEF))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// BRIGHT : background, sprite, bright
	#define BRIGHT_CALC(s,max,bright) GRADE_CALC(s,max,bright)

	#define BRIGHT16_R(s,bright) (BRIGHT_CALC(RGB16_R((s)), RGB5_MAX, (bright)))
	#define BRIGHT16_G(s,bright) (BRIGHT_CALC(RGB16_G((s)), RGB6_MAX, (bright)))
	#define BRIGHT16_B(s,bright) (BRIGHT_CALC(RGB16_B((s)), RGB5_MAX, (bright)))
	#define BRIGHT16(s,r,g,b) (RGB16(BRIGHT16_R((s),(r)), BRIGHT16_G((s),(g)), BRIGHT16_B((s),(b))))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// DARK : background, sprite, dark
	#define DARK_CALC(s,dark) GRADE_CALC(s,0,dark)

	#define DARK16_R(s,dark) (DARK_CALC(RGB16_R((s)), (dark)))
	#define DARK16_G(s,dark) (DARK_CALC(RGB16_G((s)), (dark)))
	#define DARK16_B(s,dark) (DARK_CALC(RGB16_B((s)), (dark)))
	#define DARK16(s,r,g,b) (RGB16(DARK16_R((s),(r)), DARK16_G((s),(g)), DARK16_B((s),(b))))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// GRAY : 16비트는 0x0000003F로 통일
	#define GRAY_CALC(r,g,b) ((((r) << 6) + ((g) << 7) + ((g) << 5) + ((b) << 5)) >> 8)
	#define GRAY16_C(s) (GRAY_CALC((RGB16_R((s)) << 1), RGB16_G((s)), (RGB16_B((s)) << 1)))

	#define GRAY16(c) (RGB16(((c) >> 1), (c), ((c) >> 1)))
	// GRAY_GRADE
	#define GRAY16GRADE_R(s,c,grade) (GRADE_CALC(RGB16_R((s)), ((c) >> 1), (grade)))
	#define GRAY16GRADE_G(s,c,grade) (GRADE_CALC(RGB16_G((s)), (c), (grade)))
	#define GRAY16GRADE_B(s,c,grade) (GRADE_CALC(RGB16_B((s)), ((c) >> 1), (grade)))
	#define GRAY16GRADE(s,c,grade) (RGB16(GRAY16GRADE_R((s),(c),(grade)), GRAY16GRADE_G((s),(c),(grade)), GRAY16GRADE_B((s),(c),(grade))))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// MULTIPLY
	#define MULTIPLY_CALC(b,s,shift) (((b) * (s)) >> (shift))

	#define MULTIPLY16_R(b,s) (MULTIPLY_CALC(RGB16_R((b)), RGB16_R((s)), 5))
	#define MULTIPLY16_G(b,s) (MULTIPLY_CALC(RGB16_G((b)), RGB16_G((s)), 6))
	#define MULTIPLY16_B(b,s) (MULTIPLY_CALC(RGB16_B((b)), RGB16_B((s)), 5))
	#define MULTIPLY16(b,s) (RGB16(MULTIPLY16_R((b),(s)), MULTIPLY16_G((b),(s)), MULTIPLY16_B((b),(s))))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// SCREEN
	#define SCREEN_CALC(b,s,max,shift) ((max) - ((((max) - (b)) * ((max) - (s))) >> (shift)))

	#define SCREEN16_R(b,s) (SCREEN_CALC(RGB16_R((b)), RGB16_R((s)), RGB5_MAX, 5))
	#define SCREEN16_G(b,s) (SCREEN_CALC(RGB16_G((b)), RGB16_G((s)), RGB6_MAX, 6))
	#define SCREEN16_B(b,s) (SCREEN_CALC(RGB16_B((b)), RGB16_B((s)), RGB5_MAX, 5))
	#define SCREEN16(b,s) (RGB16(SCREEN16_R((b),(s)), SCREEN16_G((b),(s)), SCREEN16_B((b),(s))))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// LINEAR_DODGE
	#define LINEAR_DODGE_CALC(b,s,max) (MIN_CALC( ((b) + (s)), (max) ))

	#define LINEAR_DODGE16_R(b,s) (LINEAR_DODGE_CALC(COLOR16_R(b), COLOR16_R(s), R16_MAX))
	#define LINEAR_DODGE16_G(b,s) (LINEAR_DODGE_CALC(COLOR16_G(b), COLOR16_G(s), G16_MAX))
	#define LINEAR_DODGE16_B(b,s) (LINEAR_DODGE_CALC(COLOR16_B(b), COLOR16_B(s), B16_MAX))
	#define LINEAR_DODGE16(b,s) (COLOR16(LINEAR_DODGE16_R((b),(s)), LINEAR_DODGE16_G((b),(s)), LINEAR_DODGE16_B((b),(s))))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// LIGHTEN
	#define LIGHTEN_CALC(b,s) (MAX_CALC( (b), (s) ))

	#define LIGHTEN16_R(b,s) (LIGHTEN_CALC(COLOR16_R((b)), COLOR16_R((s))))
	#define LIGHTEN16_G(b,s) (LIGHTEN_CALC(COLOR16_G((b)), COLOR16_G((s))))
	#define LIGHTEN16_B(b,s) (LIGHTEN_CALC(COLOR16_B((b)), COLOR16_B((s))))
	#define LIGHTEN16(b,s) (COLOR16(LIGHTEN16_R((b),(s)), LIGHTEN16_G((b),(s)), LIGHTEN16_B((b),(s))))
	/////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////
	// SEPIA
	#define SEPIA_CALC(r,g,b) (uint8)((((r)<<9)+((g)<<10)+((b)<<7))>>9)
	#define SEPIA16_R(t) MIN_CALC((t)+49,255)
	#define SEPIA16_G(t) MAX_CALC((t)-14,0)
	#define SEPIA16_B(t) MAX_CALC((t)-56,0)
//#define SEPIA_CALC(r,g,b) (uint8)((((r)<<8)+((g)<<10)+((b)<<8))>>10)
//#define SEPIA16_R(t) MAX_CALC((t)-10,0)
//#define SEPIA16_G(t) MIN_CALC((t)+10,255)
//#define SEPIA16_B(t) MIN_CALC((t)+56,255)
	#define SEPIA16(t) RGB32RGB16(SEPIA16_R(t),SEPIA16_G(t),SEPIA16_B(t))
	/////////////////////////////////////////////////////////

	//// ETC //

	///////////////////////////////////////////////////////////
	//// DARKEN : background, sprite
	//#define DARKEN_CALC(b,s) (MIN_CALC( b, s ))

	//#define DARKEN16_R(b,s) (DARKEN_CALC(RGB16_R(b), RGB16_R(s)))
	//#define DARKEN16_G(b,s) (DARKEN_CALC(RGB16_G(b), RGB16_G(s)))
	//#define DARKEN16_B(b,s) (DARKEN_CALC(RGB16_B(b), RGB16_B(s)))
	//#define DARKEN16(b,s) (RGB16(DARKEN16_R(b,s), DARKEN16_G(b,s), DARKEN16_B(b,s)))
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	//// COLOR BURN : background, sprite
	//#define COLOR_BURN_CALC(b,s,max) (max - ((max - b) / MAX_CALC(1, s)))

	//#define COLOR_BURN16_R(b,s) (COLOR_BURN_CALC(RGB16_R(b), RGB16_R(s), R16_MAX))
	//#define COLOR_BURN16_G(b,s) (COLOR_BURN_CALC(RGB16_G(b), RGB16_G(s), G16_MAX))
	//#define COLOR_BURN16_B(b,s) (COLOR_BURN_CALC(RGB16_B(b), RGB16_B(s), B16_MAX))
	//#define COLOR_BURN16(b,s) (RGB16(COLOR_BURN16_R(b,s), COLOR_BURN16_G(b,s), COLOR_BURN16_B(b,s)))
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	//// COLOR DODGE : background, sprite
	//#define COLOR_DODGE_CALC(b,s,max) (b / MAX_CALC(1, (max - s)))

	//#define COLOR_DODGE16_R(b,s) (COLOR_DODGE_CALC(RGB16_R(b), RGB16_R(s), R16_MAX))
	//#define COLOR_DODGE16_G(b,s) (COLOR_DODGE_CALC(RGB16_G(b), RGB16_G(s), G16_MAX))
	//#define COLOR_DODGE16_B(b,s) (COLOR_DODGE_CALC(RGB16_B(b), RGB16_B(s), B16_MAX))
	//#define COLOR_DODGE16(b,s) (RGB16(COLOR_DODGE16_R(b,s), COLOR_DODGE16_G(b,s), COLOR_DODGE16_B(b,s)))
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	//// DEFFERENCE : background, sprite
	//#define DEFFERENCE_CALC(b,s) (ABS32( (b - s) ))

	//#define DEFFERENCE16_R(b,s) (DEFFERENCE_CALC(RGB16_R(b), RGB16_R(s)))
	//#define DEFFERENCE16_G(b,s) (DEFFERENCE_CALC(RGB16_G(b), RGB16_G(s)))
	//#define DEFFERENCE16_B(b,s) (DEFFERENCE_CALC(RGB16_B(b), RGB16_B(s)))
	//#define DEFFERENCE16(b,s) (RGB16(DEFFERENCE16_R(b,s), DEFFERENCE16_G(b,s), DEFFERENCE16_B(b,s)))
	///////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////
	//// EXCLUSION : background, sprite
	//#define EXCLUSION_CALC(b,s,c) ((c >> 1) - ((2 * (b - (c >> 1)) * (s - (c >> 1))) / c) )

	//#define EXCLUSION16_R(b,s) (EXCLUSION_CALC(RGB16_R(b), RGB16_R(s), R16_MAX))
	//#define EXCLUSION16_G(b,s) (EXCLUSION_CALC(RGB16_G(b), RGB16_G(s), G16_MAX))
	//#define EXCLUSION16_B(b,s) (EXCLUSION_CALC(RGB16_B(b), RGB16_B(s), B16_MAX))
	//#define EXCLUSION16(b,s) (RGB16(EXCLUSION16_R(b,s), EXCLUSION16_G(b,s), EXCLUSION16_B(b,s)))
	///////////////////////////////////////////////////////////

#endif
