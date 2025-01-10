#ifndef __EXTYPE_HEADER__
#define __EXTYPE_HEADER__


#define MAX_IMG_PALLETE	20

// NUMBER TYPE
	// boolean
	#ifndef ubool
		#define ubool M_Boolean
	#endif

	// byte
	#ifndef ubyte
		#define ubyte M_Byte
	#endif

	// charater
	#ifndef schar
		#define schar M_Char
	#endif

	#ifndef wchar
		#define wchar M_UCode
	#endif

	// signed number
	#ifndef sint8
		#define sint8 M_Int8
	#endif

	// unsigned number
	#ifndef uint8
		#define uint8 M_Uint8
	#endif

	#ifndef sint16
		#define sint16 M_Int16
	#endif

	#ifndef uint16
		#define uint16 M_Uint16
	#endif

	#ifndef sint32
		#define sint32 M_Int32
	#endif

	#ifndef uint32
		#define uint32 M_Uint32
	#endif

	#ifndef slong
		#define slong signed long
	#endif

	#ifndef ulong
		#define ulong unsigned long
	#endif

	#ifndef sint64
		#define sint64 M_Int64
	#endif

	#ifndef uint64
		#define uint64 M_Uint64
	#endif

	typedef enum _ToryKeyType {
		KEY_NOTHING = 0,

		KEY_NUM0 = MC_KEY_0,
		KEY_NUM1 = MC_KEY_1,
		KEY_NUM2 = MC_KEY_2,
		KEY_NUM3 = MC_KEY_3,
		KEY_NUM4 = MC_KEY_4,
		KEY_NUM5 = MC_KEY_5,
		KEY_NUM6 = MC_KEY_6,
		KEY_NUM7 = MC_KEY_7,
		KEY_NUM8 = MC_KEY_8,
		KEY_NUM9 = MC_KEY_9,
		KEY_STAR = MC_KEY_ASTERISK,
		KEY_POUND = MC_KEY_POUND,

		KEY_POWER = MC_KEY_POWER,
		KEY_QUIT = MC_KEY_END,
		KEY_SEND = MC_KEY_SEND,
		KEY_CLR = MC_KEY_CLEAR,

		KEY_UP = MC_KEY_UP,
		KEY_DOWN = MC_KEY_DOWN,
		KEY_LEFT = MC_KEY_LEFT,
		KEY_RIGHT = MC_KEY_RIGHT,
		KEY_CENTER = MC_KEY_SELECT,

		KEY_SOFT1 = MC_KEY_SOFT1,
		KEY_SOFT2 = MC_KEY_SOFT2,

		KEY_SOFT3 = MC_KEY_SOFT3,

		KEY_VOLUME_UP = MC_KEY_SIDE_UP,
		KEY_VOLUME_DOWN = MC_KEY_SIDE_DOWN,

		KEY_INVALID = MC_KEY_INVALID, // 0
		KEY_NULL = 0x00007FFF,

		KEY_LAST
	} ToryKeyType;

	// BUFF
	typedef struct tagEXBUFF {
		uint32				nBuffID;
		sint32				nSize;
		ubyte*				pByte;
	} EXBUFF, *LPEXBUFF;

	// FILE
	typedef struct tagEXFILE {
		LPEXBUFF			pBuff;
		uint32				nPos;
	} EXFILE, *LPEXFILE;

	// IMAGE
	typedef struct tagEXIMAGE {
		LPEXBUFF			pBuff;
				
		sint16				nW;
		sint16				nH;		
	#if defined(REF2BYTE_MODE)
		sint16				nMoveX;
		sint16				nMoveY;
	#else
		sint8				nMoveX;
		sint8				nMoveY;
		//sint8				nReserved1;
		//sint8				nReserved2;
	#endif

		uint8				nPalette;
		ubyte				nMIRROR;	
		uint8				nPalLen;
		uint8				nPalCnt;

		uint32				nDATPOS;

	} EXIMAGE, *LPEXIMAGE;

	

#if defined( LGT_WIPI )
	#define EFC_GRP_GET_IMAGE_BUFFER(a) MC_grpGetImageFrameBuffer(a)
	#define EFC_GRP_GET_MASK_BUFFER(a) 0

	#define EFC_GRP_GET_FRAME_BUFFER_BPP(a) MC_GRP_GET_FRAME_BUFFER_BPP(a)
	#define EFC_GRP_GET_FRAME_BUFFER_BPL(w,bpp) (((w) * (bpp) + 7) >> 3)

	#define EFC_GRP_GET_FRAME_BUFFER_WIDTH MC_GRP_GET_FRAME_BUFFER_WIDTH
	#define EFC_GRP_GET_FRAME_BUFFER_HEIGHT MC_GRP_GET_FRAME_BUFFER_HEIGHT
	#define EFC_GRP_GET_FRAME_BUFFER_POINTER MC_GRP_GET_FRAME_BUFFER_POINTER

	#define m_pfnPixelOp pfnPixelOp
	#define m_fgpxl ForeColor
#else
	#define EFC_GRP_GET_IMAGE_BUFFER(a) (((struct _MC_GrpImage *)MC_GETDPTR(a))->img)
	#define EFC_GRP_GET_MASK_BUFFER(a) (((struct _MC_GrpImage *)MC_GETDPTR(a))->mask)

	#define EFC_GRP_GET_FRAME_BUFFER_BPP(a) MC_GRP_GET_FRAME_BUFFER_BPP(a)
//	#define EFC_GRP_GET_FRAME_BUFFER_BPL(a,w,bpp) MC_GRP_GET_FRAME_BUFFER_BPL(a)
	#define EFC_GRP_GET_FRAME_BUFFER_BPL(w,bpp) (((w) * (bpp) + 7) >> 3)

	#define EFC_GRP_GET_FRAME_BUFFER_WIDTH MC_GRP_GET_FRAME_BUFFER_WIDTH
	#define EFC_GRP_GET_FRAME_BUFFER_HEIGHT MC_GRP_GET_FRAME_BUFFER_HEIGHT
	#define EFC_GRP_GET_FRAME_BUFFER_POINTER MC_GRP_GET_FRAME_BUFFER_POINTER
#endif

	// TYPE DEFINE
	#define GETDATA	MC_GETDPTR
	#define SPRINTF		sprintf // LGT에서 %D, %X 사용 불가
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
	// SKT TOUCH
	#ifndef MV_POINTER_PRESS_EVENT
	#define MV_POINTER_PRESS_EVENT 23
	#endif

	#ifndef MV_POINTER_RELEASE_EVENT
	#define MV_POINTER_RELEASE_EVENT 24
	#endif

	#ifndef MV_POINTER_MOVE_EVENT
	#define MV_POINTER_MOVE_EVENT 25
	#endif

	// KTF TOUCH
	#ifndef MV_POINT_PRESS_EVENT
	#define MV_POINT_PRESS_EVENT 19
	#endif

	#ifndef MV_POINT_RELEASE_EVENT
	#define MV_POINT_RELEASE_EVENT 20
	#endif

	#ifndef MV_POINT_DRAG_EVENT
	#define MV_POINT_DRAG_EVENT 21
	#endif

	// LGT TOUCH
	#ifndef LGTV_POINTER_EVENT
	#define LGTV_POINTER_EVENT 1800
	#endif

	enum {
		ENSONY_POINTER_DOWN = 0,
		ENSONY_POINTER_UP = 1,
		ENSONY_POINTER_MOVE = 2,
		ENSONY_POINTER_REPEAT = 3
	};

	// SKT MA 실행중
	#ifndef MV_WM_FOCUS_EVENT 
	#define MV_WM_FOCUS_EVENT			0x4008
	#endif

	#ifndef MV_WM_LOSTFOCUS_EVENT 
	#define MV_WM_LOSTFOCUS_EVENT		0x4009
	#endif

	////////////////////////////////////////////////////////////////////////

#endif
