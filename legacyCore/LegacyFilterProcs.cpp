#include "LegacyCore.h"

#include "ResourceManager.h"
#include "LegacyDrawer.h"
#include "LegacyFilterProcs.h"



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




const uint8 ALPHA_ETC_VALUE[18] =
{
	0, 0, // 32,
	1, 0, // 48
	1, 1, // 64
	2, 0, // 80
	2, 1, // 96
	2, 2, // 128
	3, 0, // 144
	3, 1, // 160
	3, 2, // 192
};

uint16 EFC_pixel_ONE_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)	return (uint16)nBACK;
	return PixelProcData.nEFF;
}
uint16 EFC_pixel_AND_Proc(sint32 nBACK, sint32 nSRC)
{
	return (uint16)(nBACK & nSRC);
}
uint16 EFC_pixel_OR_Proc(sint32 nBACK, sint32 nSRC)
{
	return (uint16)(nBACK | nSRC);
}
uint16 EFC_pixel_XOR_Proc(sint32 nBACK, sint32 nSRC)
{
	return (uint16)(nBACK ^ nSRC);
}
uint16 EFC_pixel_BRIGHT16_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)
		return (uint16)nBACK;
	return (uint16)BRIGHT16(nSRC, PixelProcData.nRGB2[0], PixelProcData.nRGB2[1], PixelProcData.nRGB2[2]);
}
uint16 EFC_pixel_DARK16_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)
		return (uint16)nBACK;
	return (uint16)DARK16(nSRC, PixelProcData.nRGB2[0], PixelProcData.nRGB2[1], PixelProcData.nRGB2[2]);
}
uint16 EFC_pixel_ALPHA16_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)
		return (uint16)nBACK;
	return (uint16)ALPHA16(nBACK, nSRC, PixelProcData.nEFF);
}
uint16 EFC_pixel_ALPHA16_112_Proc(sint32 nBACK, sint32 nSRC)
{
	return (uint16)ALPHA16_112(nBACK, nSRC);
}
uint16 EFC_pixel_ALPHA16_128_Proc(sint32 nBACK, sint32 nSRC)
{
	return (uint16)ALPHA16_128(nBACK, nSRC);
}
uint16 EFC_pixel_ALPHA16_ETC_Proc(sint32 nBACK, sint32 nSRC)
{
	return (uint16)ALPHA16_ETC(nBACK, nSRC, ALPHA_ETC_VALUE[PixelProcData.nEFF << 1], ALPHA_ETC_VALUE[(PixelProcData.nEFF << 1) + 1]);
}
uint16 EFC_pixel_GRAY16_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)
		return (uint16)nBACK;
	PixelProcData.nCALC = GRAY16_C(nSRC);
	return (uint16)(GRAY16(PixelProcData.nCALC) & PixelProcData.nEFF);
}
uint16 EFC_pixel_GRAY16GRADE_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)
		return (uint16)nBACK;
	PixelProcData.nCALC = GRAY16_C(nSRC);
	return (uint16)GRAY16GRADE(nSRC, PixelProcData.nCALC, PixelProcData.nEFF);
}
uint16 EFC_pixel_MULTIPLY16_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)
		return (uint16)nBACK;
	return (uint16)MULTIPLY16(nBACK, nSRC);
}
uint16 EFC_pixel_SCREEN16_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)
		return (uint16)nBACK;
	return (uint16)SCREEN16(nBACK, nSRC);
}
uint16 EFC_pixel_LINEAR_DODGE16_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)
		return (uint16)nBACK;
	return (uint16)LINEAR_DODGE16(nBACK, nSRC);
}
uint16 EFC_pixel_LIGHTEN16_Proc(sint32 nBACK, sint32 nSRC)
{
	if (PixelProcData.nTRANS == nSRC)
		return (uint16)nBACK;
	return (uint16)LIGHTEN16(nBACK, nSRC);
}
uint16 EFC_pixel_SCREENGRAY16_Proc(sint32 nBACK, sint32 nSRC)
{
	PixelProcData.nCALC = GRAY16_C(nBACK);
	return (uint16)(GRAY16(PixelProcData.nCALC));
}
uint16 EFC_pixel_SCREENSEPIA16_Proc(sint32 nBACK, sint32 nSRC)
{
	PixelProcData.nCALC = SEPIA_CALC(RGB16_R(nBACK) << 1, RGB16_G(nBACK), RGB16_B(nBACK) << 1);
	return (uint16)(SEPIA16(PixelProcData.nCALC));
}


PIXELPROC PixelProc[EN_MODE_LAST];
FilterProcData PixelProcData;
PIXELContext PixelContext;


void InitFilterPixelProcs()
{
	MEMSET(&PixelProcData, 0, sizeof(PixelProcData));
	MEMSET(PixelProc, 0, sizeof(PixelProc));
	MEMSET(&PixelContext, 0, sizeof(PixelContext));


	PixelProc[EN_MODE_ONE].proc = EFC_pixel_ONE_Proc;
	PixelProc[EN_MODE_AND].proc = EFC_pixel_AND_Proc;
	PixelProc[EN_MODE_OR].proc = EFC_pixel_OR_Proc;
	PixelProc[EN_MODE_XOR].proc = EFC_pixel_XOR_Proc;

	PixelProc[EN_MODE_BRIGHT].proc = EFC_pixel_BRIGHT16_Proc;
	PixelProc[EN_MODE_DARK].proc = EFC_pixel_DARK16_Proc;
	PixelProc[EN_MODE_ALPHA].proc = EFC_pixel_ALPHA16_Proc;

	PixelProc[EN_MODE_GRAY].proc = EFC_pixel_GRAY16_Proc;
	PixelProc[EN_MODE_GRAYGRADE].proc = EFC_pixel_GRAY16GRADE_Proc;

	PixelProc[EN_MODE_MULTIPLY].proc = EFC_pixel_MULTIPLY16_Proc;
	PixelProc[EN_MODE_SCREEN].proc = EFC_pixel_SCREEN16_Proc;
	PixelProc[EN_MODE_LINEAR_DODGE].proc = EFC_pixel_LINEAR_DODGE16_Proc;
	PixelProc[EN_MODE_LIGHTEN].proc = EFC_pixel_LIGHTEN16_Proc;

	PixelProc[EN_MODE_ALPHA_128].proc = EFC_pixel_ALPHA16_128_Proc;
	PixelProc[EN_MODE_ALPHA_ETC].proc = EFC_pixel_ALPHA16_ETC_Proc;
	PixelProc[EN_MODE_ALPHA_112].proc = EFC_pixel_ALPHA16_112_Proc;

	PixelProc[EN_MODE_SCREENGRAY].proc = EFC_pixel_SCREENGRAY16_Proc;
	PixelProc[EN_MODE_SCREENSEPIA].proc = EFC_pixel_SCREENSEPIA16_Proc;

	PixelProcData.nTRANS = RGB32RGB16(255, 0, 255);
	PixelProcData.nCOLOR = RGB32RGB16(255, 255, 255);


	// 아래 초기화
	//PixelContext PixelContext

}