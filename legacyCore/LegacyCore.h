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




/////////////////////////////////////////////////////////////////////////////////////////////////
// Graphicis

#define MC_GRP_GET_FRAME_BUFFER_POINTER(a) ((M_Int32*)MC_GETDPTR(((struct _EX_GrpFrameBuffer*)MC_GETDPTR(a))->frameBufID))
#define MC_GRP_GET_FRAME_BUFFER_WIDTH(a) ((struct _EX_GrpFrameBuffer *)MC_GETDPTR(a))->w
#define MC_GRP_GET_FRAME_BUFFER_HEIGHT(a) ((struct _EX_GrpFrameBuffer *)MC_GETDPTR(a))->h
#define MC_GRP_GET_FRAME_BUFFER_BPL(a) (((struct _EX_GrpFrameBuffer *)MC_GETDPTR(a)))->bpl
#define MC_GRP_GET_FRAME_BUFFER_BPP(a) ((struct _EX_GrpFrameBuffer *)MC_GETDPTR(a))->bpp
#define MC_GRP_GET_FRAME_BUFFER_ID(a) (((struct _EX_GrpFrameBuffer *)MC_GETDPTR(a)))->frameBufID

#define MC_GETDPTR(indirectPtr)	(*(void**)indirectPtr)


#define EFC_GRP_GET_IMAGE_BUFFER(a) (((struct _MC_GrpImage *)MC_GETDPTR(a))->img)
#define EFC_GRP_GET_MASK_BUFFER(a) (((struct _MC_GrpImage *)MC_GETDPTR(a))->mask)

#define EFC_GRP_GET_FRAME_BUFFER_BPP(a) MC_GRP_GET_FRAME_BUFFER_BPP(a)
#define EFC_GRP_GET_FRAME_BUFFER_BPL(w,bpp) (((w) * (bpp) + 7) >> 3)

#define EFC_GRP_GET_FRAME_BUFFER_WIDTH MC_GRP_GET_FRAME_BUFFER_WIDTH
#define EFC_GRP_GET_FRAME_BUFFER_HEIGHT MC_GRP_GET_FRAME_BUFFER_HEIGHT
#define EFC_GRP_GET_FRAME_BUFFER_POINTER MC_GRP_GET_FRAME_BUFFER_POINTER

#define GETDATA	MC_GETDPTR


typedef M_Int32(*MC_GrpPixelOpProc)(M_Int32 srcpxl, M_Int32 orgpxl, M_Int32 param1);

typedef struct _MC_GrpContext
{
	M_Int32 m_fgpxl;
	M_Int32 m_transpxl;
	MC_GrpPixelOpProc m_pfnPixelOp;
	M_Int32 m_param1;

} MC_GrpContext;


struct _EX_GrpFrameBuffer
{
	M_Int32 w;
	M_Int32 h;
	M_Int32 bpl;
	M_Int32 bpp;
	M_Int32 frameBufID;
};
typedef M_Int32 MC_GrpFrameBuffer;


struct _MC_GrpImage
{
	MC_GrpFrameBuffer img;
};
typedef M_Int32 MC_GrpImage;


extern MC_GrpFrameBuffer MC_grpCreateOffScreenFrameBuffer(M_Int32 w, M_Int32 h);
extern void MC_grpDestroyOffScreenFrameBuffer(MC_GrpFrameBuffer fb);
extern MC_GrpFrameBuffer MC_grpGetScreenFrameBuffer(M_Int32 s);
extern void MC_grpInitContext(MC_GrpContext* pgc);
extern void MC_grpFlushLcd(M_Int32 i, MC_GrpFrameBuffer frm, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h);


/////////////////////////////////////////////////////////////////////////////////////////////////
// IMAGE
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