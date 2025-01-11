#pragma once


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wtypes.h>

#define MAX_IMG_PALLETE	20


#define MC_GRP_CONTEXT_CLIP_IDX             0
#define MC_GRP_CONTEXT_FG_PIXEL_IDX         1
#define MC_GRP_CONTEXT_TRANS_PIXEL_IDX      3
#define MC_GRP_CONTEXT_PIXELOP_IDX          5
#define MC_GRP_CONTEXT_PIXEL_PARAM1_IDX     6

#define MC_GRP_IMAGE_DONE	1
#define MC_GRP_IMAGE_WIDTH	4
#define MC_GRP_IMAGE_HEIGHT	5


#define	MC_FILE_SEEK_SET	0
#define	MC_FILE_SEEK_CUR	1
#define	MC_FILE_SEEK_END	2




////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef		unsigned	char			M_Boolean;
typedef		unsigned	int				M_Uint32;
typedef		unsigned	short			M_Uint16;
typedef		unsigned	char			M_Uint8;

typedef		signed		int				M_Int32;
typedef		signed		short			M_Int16;
typedef		signed		char			M_Int8;

typedef		char						M_Char;
typedef		unsigned	char			M_Byte;

typedef		signed		long long		M_Int64;
typedef		unsigned	long long		M_Uint64;

typedef		unsigned	short			M_UCode;
typedef		signed		int				M_Sint32;
typedef		signed		short			M_Sint15;

typedef		M_Uint32					M_Addr;
typedef		M_Char						M_TChar;

typedef					M_Uint64		ulong64;
typedef		signed		long long		long64;


typedef  M_Boolean	ubool;
typedef  M_Byte		ubyte;
typedef  M_Char		schar;
typedef  M_UCode	wchar;
typedef  M_Int8		sint8;
typedef  M_Uint8	uint8;
typedef  M_Int16	sint16;
typedef  M_Uint16	uint16;
typedef  M_Int32	sint32;
typedef  M_Uint32	uint32;
typedef  signed long	slong;
typedef  unsigned long	ulong;
typedef  M_Int64		sint64;
typedef  M_Uint64		uint64;


typedef enum _ToryKeyType
{
	KEY_NOTHING = 0,
	KEY_NULL = 0,

	KEY_NUM0 = 10, 
	KEY_NUM1 = 11,
	KEY_NUM2 = 12,
	KEY_NUM3 = 13,
	KEY_NUM4 = 14,
	KEY_NUM5 = 15,
	KEY_NUM6 = 16,
	KEY_NUM7 = 17,
	KEY_NUM8 = 18,
	KEY_NUM9 = 19,
	KEY_STAR = 20,
	KEY_POUND = 21,

	KEY_POWER = 22,
	KEY_QUIT = 23,
	KEY_SEND = 24,
	KEY_CLR = 25,

	KEY_UP = 30,
	KEY_DOWN = 31,
	KEY_LEFT = 32,
	KEY_RIGHT = 33,
	KEY_CENTER = 34,

	KEY_SOFT1 = 40,
	KEY_SOFT2 = 41,
	KEY_SOFT3 = 42,
	KEY_LAST
} ToryKeyType;


// BUFF
typedef struct tagEXBUFF 
{
	uint32				nBuffID;
	sint32				nSize;
	ubyte* pByte;
} EXBUFF, * LPEXBUFF;


// FILE
typedef struct tagEXFILE 
{
	LPEXBUFF			pBuff;
	uint32				nPos;
} EXFILE, * LPEXFILE;



typedef struct EX_PointerEvent
{
	M_Int32	x;
	M_Int32	y;
	M_Int32	key;
} EX_PointerEvent;

typedef struct _MTimer
{
	M_Uint64 alarmTime;

} MTimer;

typedef struct _EXTimer
{
	MTimer timer;
	void* parm;
	void* cb;			// timer callback func
} EXTimer;


typedef struct tagEXRECT {
	sint16		nX, nY;
	sint16		nW, nH;
} EXRECT, * LPEXRECT;

typedef struct tagEXPOINT {
	sint16		nX, nY;
} EXPOINT, * LPEXPOINT;

typedef struct tagEXREGION {
	sint16		nX, nY;
	sint16		nW, nH;
	sint16		nSx, nSy;
} EXREGION, * LPEXREGION;


/////////////////////////////////////////////////////////////////////////////////////////////////
// Graphicis
// 
/////////////////////////////////////////////////////////
// CALC
#define CONVERT_INT(fp, a)	(fp[a]|(fp[a+1]<<8)|(fp[a+2]<<16)|(fp[a+3]<<24));
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



//

typedef struct tagPIXELDATA
{
	sint32	hReal;
	sint32	hScreen;
	sint32  hFrame;

	//MC_GrpContext		hGC;

	/////////////////////////////////////
	// PIXEL


	//uint8				nMODE;
	//uint16				nEFF;
	//sint32				nCALC;
	/////////////////////////////////////

	uint8				nBPP;
	EXRECT				rtMAIN; // 화면 RECT
	EXRECT				rtREAL; // 화면 RECT

	ubool				bFULL;

	EXPOINT				ptTRANS;	// 이동 좌표 POINT
	EXRECT				rtCLIP;		// 클리핑 RECT

} PIXELDATA, * LPPIXELDATA;



/////////////////////////////////////////////////////////////////////////////////////////////////
// IMAGE
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

enum {
	EN_MIRROR_NONE = 0,
	EN_MIRROR_HORIZON,
	EN_MIRROR_VERTICAL,
	EN_MIRROR_BOTH,
	EN_MIRROR_LAST
};

typedef struct tagEXIMAGE 
{
	LPEXBUFF			pBuff;

	sint16				nW;
	sint16				nH;
	sint8				nMoveX;
	sint8				nMoveY;

	uint8				nPalette;
	ubyte				nMIRROR;
	uint8				nPalLen;
	uint8				nPalCnt;

	uint32				nDATPOS;

} EXIMAGE, * LPEXIMAGE;

typedef struct tagEXMOVE
{
	sint16		moveX, moveY;

} EXMOVE, * LPEXMOVE;


#define SPRINTF	sprintf 
#define MEMSET	memset
#define MALLOC	MC_knlCalloc
#define STRLEN	strlen
#define FREE	MC_knlFree
#define MEMCPY	memcpy
#define STRCPY	strcpy
#define STRNCPY	strncpy
#define STRSTR	strstr
#define STRCMP	strcmp
#define ATOI	atoi
#define RGBVAL uint32
#define GETCOLOR MC_grpGetPixelFromRGB


////////////////////////////////////////////////////////////////////////
//
extern M_Int64 MC_knlCurrentTime();
extern LPEXBUFF EFC_fsLoadBUFF(schar* pszFile);



////////////////////////////////////////////////////////////////////////
//
extern ubyte* EFC_memGET(LPEXBUFF pBuff);
extern ubool EFC_memSET(LPEXBUFF pBuff, sint32 nSize);
extern LPEXBUFF EFC_memCOPY(LPEXBUFF pBuff);
extern LPEXBUFF EFC_memALLOC(sint32 nSize);
extern void EFC_memFREE(LPEXBUFF pBuff);

extern void EFC_fxtCHANGE(LPEXBUFF pBuff, schar szOld, schar szNew);





////////////////////////////////////////////////////////////////////////
//
extern void DebugLog(const wchar_t* format, ...);