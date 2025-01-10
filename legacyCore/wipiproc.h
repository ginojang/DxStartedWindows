
#ifndef __WIPIPROC_H__
#define __WIPIPROC_H__

#include "MCerror.h"
#include "MCtypes.h"
#include "wipiexchange.h"

//#include "natives.h"


//#define EX_SCREEN_WIDTH		240
         //#define EX_SCREEN_HEIGHT1	320
//#define EX_SCREEN_HEIGHT2	400

#define EX_SCREEN_WIDTH	240
#define EX_SCREEN_HEIGHT2	400

#define EX_SCREEN_BPP		16		//?��??��쑵��?
void handleCletEvent(int type, int param1, int param2);
void startClet(int argc, char *argv[]);
void pauseClet(void);
void resumeClet(void);
void destroyClet(void);
void paintClet(int x, int y, int w, int h);

#define EX_GRP_GET_FRAME_BUFFER_POINTER(a) ((M_Int32*)EX_GETDPTR(((struct _EX_GrpFrameBuffer*)EX_GETDPTR(a))->frameBufID))
#define EX_GRP_GET_FRAME_BUFFER_WIDTH(a) ((struct _EX_GrpFrameBuffer *)EX_GETDPTR(a))->w
#define EX_GRP_GET_FRAME_BUFFER_HEIGHT(a) ((struct _EX_GrpFrameBuffer *)EX_GETDPTR(a))->h
#define _EX_GRP_GET_FRAME_BUFFER_BPL(a) (((struct _EX_GrpFrameBuffer *)EX_GETDPTR(a)))->bpl
#define EX_GRP_GET_FRAME_BUFFER_BPP(a) ((struct _EX_GrpFrameBuffer *)EX_GETDPTR(a))->bpp
#define EX_GRP_GET_FRAME_BUFFER_ID(a) (((struct _EX_GrpFrameBuffer *)EX_GETDPTR(a)))->frameBufID

#define EX_GETDPTR(indirectPtr)	(*(void**)indirectPtr)

typedef M_Int32 (*EX_GrpPixelOpProc)(M_Int32 srcpxl,
									 M_Int32 orgpxl, M_Int32 param1);

struct _EX_GrpContext{
	//M_Int32 m_mask;
	//M_Int32 m_clip[4];
	M_Int32 m_fgpxl;
	//M_Int32 m_bgpxl;
	M_Int32 m_transpxl;
	//M_Int32 m_alpha;
	//M_Int32 m_offsetX;
	//M_Int32 m_offsetY;
	EX_GrpPixelOpProc m_pfnPixelOp;
	M_Int32 m_param1;
	//M_Int32 m_reserved;
	//M_Int32 m_font;
	//M_Int32 m_style;
};

typedef struct _EX_GrpContext EX_GrpContext;

struct _EX_GrpFrameBuffer{
	M_Int32 w;
	M_Int32 h;
	M_Int32 bpl;
	M_Int32 bpp;
	M_Int32 frameBufID;
};

typedef M_Int32 EX_GrpFrameBuffer;

struct _EX_GrpImage{
	EX_GrpFrameBuffer img;
	//EX_GrpFrameBuffer mask;
	//signed long			loopcount;
	//signed long			delay;
	//signed long			animated;
	//unsigned long		bufID;
	//signed long			offset;
	//signed long			current;
	//signed long			len;
};
typedef M_Int32 EX_GrpImage;

#define MC_GRP_CONTEXT_CLIP_IDX             0
#define MC_GRP_CONTEXT_FG_PIXEL_IDX         1
//#define MC_GRP_CONTEXT_BG_PIXEL_IDX         2
#define MC_GRP_CONTEXT_TRANS_PIXEL_IDX      3
//#define MC_GRP_CONTEXT_ALPHA_IDX            4
#define MC_GRP_CONTEXT_PIXELOP_IDX          5
#define MC_GRP_CONTEXT_PIXEL_PARAM1_IDX     6
//#define MC_GRP_CONTEXT_FONT_IDX             7
//#define MC_GRP_CONTEXT_STYLE_IDX            8
//#define MC_GRP_CONTEXT_XOR_MODE_IDX         9
//#define MC_GRP_CONTEXT_OFFSET_IDX			10
//#define OEMC_GRP_CONTEXT_OUTLINE_IDX			11




#define MC_GRP_IMAGE_DONE        1
//#define MC_GRP_FRAME_DONE				0

//#define MC_GRP_IS_ANIMATED          1
//#define MC_GRP_ANIMATE_DELAY        2
//#define MC_GRP_LOOP_COUNT           3
#define MC_GRP_IMAGE_WIDTH			4
#define MC_GRP_IMAGE_HEIGHT			5
//#define MC_GRP_IMAGE_BPP			6
//#ifdef FEATURE_WIPI20
//#define MC_GRP_CURRENT_FRAME        7
//#endif
//#define MC_GRP_IS_REPEATED          8



#define MC_FILE_OPEN_RDONLY		0x1
#define MC_FILE_OPEN_WRONLY		0x2
#define MC_FILE_OPEN_WRTRUNC	0x4
#define MC_FILE_OPEN_RDWR		0x8
//#define MC_FILE_OPEN_RESERVE	0x10

#define MC_DIR_PRIVATE_ACCESS		1
#define MC_DIR_SHARED_ACCESS		2
#define MC_DIR_SYSTEM_ACCESS		3

#define				MH_FILE_IS_DIR              0x01
#define				MH_FILE_IS_RESERVED         0x02

#define MC_FILE_IS_DIR 			MH_FILE_IS_DIR
#define MC_FILE_IS_RESERVED 	MH_FILE_IS_RESERVED

#define				MC_FILE_SEEK_SET			0
#define				MC_FILE_SEEK_CUR			1
#define				MC_FILE_SEEK_END			2

//struct _MG_Program;

typedef enum _MH_Event {
	MH_EXIT_EVENT = 1,
	MH_KEY_PRESSEVENT,
	MH_KEY_RELEASEEVENT,
	MH_KEY_REPEATEVENT,
	MH_TIMER_EVENT,
	MH_SMS_EVENT,
	MH_CALL_EVENT,
	MH_ANN_EVENT,
	MH_NETWORK_EVENT,
	MH_SERIAL_EVENT,
	MH_MEDIA_EVENT,
	MH_FOREG_EVENT,
	MH_BACKG_EVENT,
	MH_MSGBOX_EVENT,
	MH_IODEVICE_EVENT,
	MH_WCDMA_NETWORK_EVENT,
	MH_SMSPUSH_EVENT,
	MH_SIP_EVENT,
	MH_GPS_EVENT,
	MH_REMOTE_KEY_PRESSEVENT,
	MH_REMOTE_KEY_RELEASEEVENT,
	MH_REMOTE_KEY_REPEATEVENT,
	MH_POINTER_PRESSEVENT,
	MH_POINTER_RELEASEEVENT,
	MH_POINTER_MOVEEVENT,
	MH_POINTER_DBLCLKEVENT,
	MH_POINTER_REPEATEVENT,
	MH_LCD_EVENT,
	MH_EXTMEM_EVENT,
	MH_WCDMA_CDMA_EVENT,
	MH_FONTCHANGE_EVENT,
    MH_OEMAPP_STARTEVENT,
    MH_OEMAPP_DESTROYEVENT,
	MH_TIMEZONE_CHANGE_EVENT
} MH_Event;

#define MV_SENDMSG_EVENT			0x4000
#define MV_BACKG_EVENT				MV_SENDMSG_EVENT + 1
#define MV_FOREG_EVENT				MV_BACKG_EVENT + 1
#define MV_EIF_EVENT				MV_FOREG_EVENT + 1
#define MV_OEMMSG_EVENT		    	MV_EIF_EVENT + 1
#define MV_PLTBACKG_EVENT			MV_OEMMSG_EVENT + 1
#define MV_PLTFOREG_EVENT			MV_PLTBACKG_EVENT + 1
#define MV_AGENT_CHANGE_EVENT       MV_PLTFOREG_EVENT + 1
#define MV_WM_FOCUS_EVENT			MV_AGENT_CHANGE_EVENT + 1
#define MV_WM_LOSTFOCUS_EVENT		MV_WM_FOCUS_EVENT + 1
#define MV_USER_EVENT				0x5000

typedef enum MH_KeyCode{
    MH_KEY_0	    	= '0',
	MH_KEY_1    		= '1',
	MH_KEY_2    		= '2',
	MH_KEY_3    		= '3',
	MH_KEY_4	    	= '4',
	MH_KEY_5		    = '5',
	MH_KEY_6    		= '6',
	MH_KEY_7 	    	= '7',
	MH_KEY_8		    = '8',
	MH_KEY_9		    = '9',
	MH_KEY_ASTERISK	    = '*',
	MH_KEY_POUND	    = '#',
	MH_KEY_UP	    	= -1,
	MH_KEY_DOWN 		= -2,
	MH_KEY_LEFT	    	= -3,
	MH_KEY_RIGHT    	= -4,
	MH_KEY_SELECT   	= -5,
	MH_KEY_SOFT1    	= -6,
	MH_KEY_SOFT2    	= -7,
	MH_KEY_SOFT3    	= -8,
	MH_KEY_SEND	    	= -10,
	MH_KEY_END	    	= -11,
	MH_KEY_POWER    	= -12,
	MH_KEY_SIDE_UP	    = -13,
	MH_KEY_SIDE_DOWN	= -14,
	MH_KEY_SIDE_SEL 	= -15,
	MH_KEY_CLEAR		= -16,
	MH_KEY_FLIPDOWN 	= -17,
	MH_KEY_FLIPUP		= -18,
	MH_KEY_WHEELDOWN	= -19,
	MH_KEY_WHEELUP		= -20,
	MH_KEY_LCDDOWN	    = -21,
	MH_KEY_LCDUP		= -22,
	MH_KEY_IMGCALL		= -23,		// added by merlin, 2005.11.23
	MH_KEY_VU			= -24,
	MH_KEY_MA			= -25,
	MH_KEY_INVALID  	= 0
} MH_KeyCode;

#define MC_KEY_0					MH_KEY_0
#define MC_KEY_1					MH_KEY_1
#define MC_KEY_2					MH_KEY_2
#define MC_KEY_3					MH_KEY_3
#define MC_KEY_4					MH_KEY_4
#define MC_KEY_5					MH_KEY_5
#define MC_KEY_6					MH_KEY_6
#define MC_KEY_7					MH_KEY_7
#define MC_KEY_8					MH_KEY_8
#define MC_KEY_9					MH_KEY_9
#define MC_KEY_ASTERISK				MH_KEY_ASTERISK
#define MC_KEY_POUND				MH_KEY_POUND
#define MC_KEY_UP					MH_KEY_UP
#define MC_KEY_DOWN					MH_KEY_DOWN
#define MC_KEY_LEFT					MH_KEY_LEFT
#define MC_KEY_RIGHT				MH_KEY_RIGHT
#define MC_KEY_SELECT				MH_KEY_SELECT
#define MC_KEY_SOFT1				MH_KEY_SOFT1
#define MC_KEY_SOFT2				MH_KEY_SOFT2
#define MC_KEY_SOFT3				MH_KEY_SOFT3
#define MC_KEY_SEND					MH_KEY_SEND
#define MC_KEY_END					MH_KEY_END
#define MC_KEY_POWER				MH_KEY_POWER
#define MC_KEY_SIDE_UP				MH_KEY_SIDE_UP
#define MC_KEY_SIDE_DOWN			MH_KEY_SIDE_DOWN
#define MC_KEY_SIDE_SEL				MH_KEY_SIDE_SEL
#define MC_KEY_CLEAR				MH_KEY_CLEAR
#define MC_KEY_FLIPDOWN				MH_KEY_FLIPDOWN
#define MC_KEY_FLIPUP				MH_KEY_FLIPUP
#define MC_KEY_WHEELDOWN			MH_KEY_WHEELDOWN
#define MC_KEY_WHEELUP				MH_KEY_WHEELUP
#define MC_KEY_LCDDOWN			    MH_KEY_LCDDOWN
#define MC_KEY_LCDUP				MH_KEY_LCDUP
#define MC_KEY_IMGCALL				MH_KEY_IMGCALL
#define MC_KEY_VU					MH_KEY_VU
#define MC_KEY_MA					MH_KEY_MA
#define MC_KEY_INVALID				MH_KEY_INVALID

#define MV_KEY_PRESS_EVENT			MH_KEY_PRESSEVENT
#define MV_KEY_RELEASE_EVENT		MH_KEY_RELEASEEVENT
#define MV_KEY_REPEAT_EVENT			MH_KEY_REPEATEVENT
#define MV_ANN_EVENT				MH_ANN_EVENT

//SKT
#define	MV_POINTER_PRESS_EVENT		MH_POINTER_PRESSEVENT
#define	MV_POINTER_RELEASE_EVENT	MH_POINTER_RELEASEEVENT
#define	MV_POINTER_MOVE_EVENT		MH_POINTER_MOVEEVENT
#define	MV_POINTER_DBLCLK_EVENT		MH_POINTER_DBLCLKEVENT
#define	MV_POINTER_REPEAT_EVENT		MH_POINTER_REPEATEVENT

//KTF
#define MV_POINT_PRESS_EVENT		19
#define MV_POINT_RELEASE_EVENT		20
#define MV_POINT_DRAG_EVENT		21

typedef struct EX_PointerEvent {
	M_Int32	x;
	M_Int32	y;
	M_Int32	key;
} EX_PointerEvent;

typedef struct _MTimer {
	//int type;			// C or Java timer
	//struct _MTimer* alarmNext;
	//void* alarmWakupFunc;
	M_Uint64 alarmTime;
	//pthread_t timeThread;

} MTimer;

typedef struct _EXTimer {
	MTimer timer;
	void* parm;
	//	struct _MG_Program* prg;
	void* cb;			// timer callback func
} EXTimer;

typedef void (*TIMERCB)(EXTimer *tm, void* parm);
//typedef void (*MBPROC)(M_Int32 btnID, void* param);

typedef enum MC_BackLight {

	MC_LIGHT_ON = 0,

	MC_LIGHT_OFF,

	MC_LIGHT_ALWAYS_ON,

	MC_LIGHT_DEFAULT,

	MC_LIGHT_ALWAYS_OFF
} MC_BackLight;

#define MC_AF_INET 2				//Internet ???��
#define MC_SOCKET_STREAM 1		// TCP STREAM
#define MC_SOCKET_DGRAM 2			// UDP

typedef struct _fileInfo {
	M_Int32				attrib;
	M_Uint32			creationTime;
	M_Uint32			size;
} MH_FileInfo;

typedef MH_FileInfo		EX_FileInfo;

#define MC_MDA_STATUS_OEM_ERROR              (-2)
#define MC_MDA_STATUS_ERROR                  (-1)
#define MC_MDA_STATUS_END_OF_DATA            1
#define MC_MDA_STATUS_START                  2
#define MC_MDA_STATUS_STARTED                2
#define MC_MDA_STATUS_STOP                   3
#define MC_MDA_STATUS_STOPP                  3
#define MC_MDA_STATUS_STOPPED                3
#define MC_MDA_STATUS_PAUSE                  4
#define MC_MDA_STATUS_PAUSEED                4
#define MC_MDA_STATUS_RESUME                 5
#define MC_MDA_STATUS_RESUMEED               5
#define MC_MDA_STATUS_RECORD                 6
#define MC_MDA_STATUS_RECORDED               6
#define MC_MDA_STATUS_FULL_OF_DATA           7
#define MC_MDA_STATUS_TICK_UPDATE            8
#define MC_MDA_STATUS_RECORD_LENGTH_FULL     9
#define MC_MDA_STATUS_END_OF_MEDIA           10
#define MC_MDA_STATUS_STOPPED_AT_TIME        11

/**
 * 癰귥눖��燁삳똾�믤�醫듼봺揶쏉옙?�곗뺘?怨몄뵥 ?�꾨탣�귐�?�곷� ?��쎗???��?�롫뮉 ?怨몃땾
 * <P>
 * 揶쏅�? 0
 */
#define MC_MDA_VOLCATE_GENERAL	0


/**
 * 癰귥눖��燁삳똾�믤�醫듼봺揶쏉옙?��쉐???�源�?諭�벉 ?��쎗???��?�롫뮉 ?怨몃땾
 * <P>
 * 揶쏅�? 1
 */
#define MC_MDA_VOLCATE_VOICE 	1


/**
 * 癰귥눖��燁삳똾�믤�醫듼봺揶쏉옙筌△뫗��甕�?��쎗???��?�롫뮉 ?怨몃땾
 * <P>
 * 揶쏅�? 2
 */
#define MC_MDA_VOLCATE_RING	2


/**
 * 癰귥눖��燁삳똾�믤�醫듼봺揶쏉옙???�쇱벉?�깆뱽 ?��?�롫뮉 ?怨몃땾
 * <P>
 * 揶쏅�? 3
 */
#define MC_MDA_VOLCATE_KEYTONE	3


/**
 * 癰귥눖��燁삳똾�믤�醫듼봺揶쏉옙SMS message ?袁⑷컩 野껋럡����쎗???��?�롫뮉 ?怨몃땾
 * <P>
 * 揶쏅�? 4
 */
#define MC_MDA_VOLCATE_MESSAGE	4


/**
 * 癰귥눖��燁삳똾�믤�醫듼봺揶쏉옙?��뿺 野껋럡����쎗???��?�롫뮉 ?怨몃땾
 * <P>
 * 揶쏅�? 5
 */
#define MC_MDA_VOLCATE_ALARM	5


/**
 癰귥눖��燁삳똾�믤�醫듼봺揶쏉옙揶쏄낯伊�野껋럡����쎗???��?�롫뮉 ?怨몃땾
 <P>
 揶쏅�? 6
 */
#define MC_MDA_VOLCATE_ALERT	6


/**
 * 癰귥눖��燁삳똾�믤�醫듼봺揶쏉옙筌롳옙怨�?遺용선 ?關��?��쎗???��?�롫뮉 ?怨몃땾
 * <P>
 * 揶쏅�? 7
 */
#define MC_MDA_VOLCATE_MMEDIA	7


/**
 * 癰귥눖��燁삳똾�믤�醫듼봺揶쏉옙野껊슣��?��쎗???��?�롫뮉 ?怨몃땾
 * <P>
 * 揶쏅�? 8
 */
#define MC_MDA_VOLCATE_GAME	8

typedef struct __EX_MdaClip {
	int aaa;
	M_Int32 size;
	M_Int32 frameSize;
	M_Boolean repeat;

	M_Byte *mdaBuff;

}EX_MdaClip;

typedef struct __MC_MdaFileInfo 
{
	M_Char szBuff[256];
	M_Int32 param1;
	M_Int32 param2;
} EX_MdaFileInfo;

typedef void (*MEDIACB)(EX_MdaClip* clip, int status);



M_Uint32 EX_knlAlloc ( M_Int32 size );
void EX_knlFree ( M_Uint32 mID );

//M_Int32 EX_knlGetResource ( M_Char* resourceName, M_Uint32* buf, M_Int32* bufSize );
M_Int32 EX_knlGetResourceID ( M_Char* resourceName, M_Int32* size );
M_Int32 EX_knlGetResource ( M_Int32 resourceID, M_Uint32 ibuf, M_Int32 bufSize );
void EX_knlDefTimer ( EXTimer *tm, TIMERCB cb );
M_Int32 EX_knlSetTimer ( EXTimer *tm, M_Int64 timeout, void* parm );
void EX_knlUnsetTimer ( EXTimer* tm );
M_Int32 EX_knlPrintk ( M_Char* format, ... );
void EX_knlSprintk ( M_Char* buf, M_Char* format, ... );
M_Int32 EX_knlGetSystemProperty ( M_Char* id, M_Char* rtnBuf, M_Int32 bufSize );
M_Int64 EX_knlCurrentTime();
M_Int32 EX_knlGetTotalMemory ();
M_Int32 EX_knlGetFreeMemory ();
M_Int32 EX_knlExit ( M_Int32 exitCode );

EX_GrpFrameBuffer EX_grpGetScreenFrameBuffer ( M_Int32 s );
void EX_grpInitContext( EX_GrpContext *pgc );
void EX_grpSetContext( EX_GrpContext *pgc, M_Int32 index, void *pv );
EX_GrpFrameBuffer EX_grpCreateOffScreenFrameBuffer ( M_Int32 w, M_Int32 h );
M_Int32 EX_grpCreateImage ( EX_GrpImage *newImg, M_Int32 bufID, M_Int32 off, M_Int32 len );
void EX_grpDrawImage ( EX_GrpFrameBuffer dst, M_Int32 dx, M_Int32 dy, M_Int32 w, M_Int32 h, EX_GrpImage src, M_Int32 sx, M_Int32 sy, EX_GrpContext *pgc );
void EX_grpDestroyOffScreenFrameBuffer ( EX_GrpFrameBuffer fb );


M_Int32 EX_grpGetImageProperty( EX_GrpImage img, M_Int32 index );
void EX_grpFlushLcd ( M_Int32 i, EX_GrpFrameBuffer frm, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h );
void EX_grpFillRect ( EX_GrpFrameBuffer dst, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h, EX_GrpContext *pgc );
void EX_grpDrawArc ( EX_GrpFrameBuffer dst, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h, M_Int32 s, M_Int32 e, EX_GrpContext *pgc );
void EX_grpFillArc ( EX_GrpFrameBuffer dst, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h, M_Int32 s, M_Int32 e, EX_GrpContext *pgc );
void EX_grpCopyArea ( EX_GrpFrameBuffer dst, M_Int32 dx, M_Int32 dy, M_Int32 w, M_Int32 h, M_Int32 x, M_Int32 y, EX_GrpContext *pgc );
void EX_grpCopyFrameBuffer ( EX_GrpFrameBuffer dst, M_Int32 dx, M_Int32 dy, M_Int32 w, M_Int32 h, EX_GrpFrameBuffer src, M_Int32 sx, M_Int32 sy, EX_GrpContext *pgc );
void EX_grpDrawLine ( EX_GrpFrameBuffer dst, M_Int32 x1, M_Int32 y1, M_Int32 x2, M_Int32 y2, EX_GrpContext *pgc );
void EX_grpDrawRect ( EX_GrpFrameBuffer dst, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h, EX_GrpContext *pgc );
M_Int32 EX_grpGetPixelFromRGB ( M_Int32 r, M_Int32 g, M_Int32 b );
M_Int32 EX_grpGetRGBFromPixel ( M_Int32 pixel, M_Int32 *r, M_Int32 *g, M_Int32 *b );
void EX_grpPutPixel ( EX_GrpFrameBuffer dst, M_Int32 x, M_Int32 y, EX_GrpContext *pgc );
void EX_grpDestroyImage ( EX_GrpImage img );
EX_GrpFrameBuffer EX_grpGetImageFrameBuffer ( EX_GrpImage img );

typedef void (*NETCONNECTCB)(M_Int32 error, void *param);
typedef void (*NETSOCKCONNECTCB)(M_Int32 fd,M_Int32 error, void *param);
typedef void (*NETSOCKREADCB)(M_Int32 fd,M_Int32 error,void *param);
typedef void (*NETSOCKWRITECB)(M_Int32 fd,M_Int32 error,void *param);

M_Int32 EX_netConnect ( NETCONNECTCB cb, void *param );
M_Int32 EX_netSocket ( M_Int32 domain, M_Int32 type );
M_Int32 EX_netSocketConnect ( M_Int32 fd, M_Int32 addr, M_Int16 port, NETSOCKCONNECTCB cb, void *param );
M_Int32 EX_netSocketWrite ( M_Int32 fd, M_Byte *buf, M_Int32 len );
M_Int32 EX_netSetWriteCB ( M_Int32 fd, NETSOCKWRITECB cb, void *param );
M_Int32 EX_netSocketRead ( M_Int32 fd, M_Byte* buf, M_Int32 len );
M_Int32 EX_netSetReadCB ( M_Int32 fd, NETSOCKREADCB cb, void *param );
M_Int32 EX_netSocketClose ( M_Int32 fd );
void EX_netClose ( void );

M_Int32 EX_utilHtonl ( M_Int32 val );
M_Int16 EX_utilHtons ( M_Int16 val );
//M_Int32 EX_utilInetAddrInt ( M_Byte *addr );
M_Int32 EX_utilInetAddrInt ( M_Char *addr );


M_Int32 EX_fsOpen ( M_Char* name, M_Int32 flag, M_Int32 aMode );
M_Int32 EX_fsRead ( M_Int32 fd, M_Byte* buf, M_Int32 len );
M_Int32 EX_fsWrite ( M_Int32 fd, M_Byte* buf, M_Int32 len );
M_Int32 EX_fsSeek ( M_Int32 fd, M_Int32 pos, M_Int32 wh );
M_Int32 EX_fsTell ( M_Int32 fd );
M_Int32 EX_fsRemove ( M_Char* name, M_Int32 aMode );
M_Int32 EX_fsClose ( M_Int32 fd );
M_Int32 EX_fsFileAttribute ( M_Char* name, EX_FileInfo* fa, M_Int32 aMode );
M_Int32 EX_fsAvailable ( void );


M_Int32 EX_phnCallPlace ( M_Byte* phonenumber );

M_Boolean EX_WIPIinit();
void EX_WIPIDestroy();

EX_MdaClip* EX_mdaClipCreate ( M_Char* mType, M_Int32 bufSize, MEDIACB cb );
M_Int32 EX_mdaClipPutData ( EX_MdaClip* clip, M_Byte* buf, M_Int32 size );
M_Int32 EX_mdaPlay ( EX_MdaClip* clip, M_Boolean repeat );
M_Int32 EX_mdaClipFree ( EX_MdaClip* clip );
M_Int32 EX_mdaStop ( EX_MdaClip* clip );
M_Int32 EX_mdaVibrator ( M_Int32 on_off, M_Int32 timeout );
M_Int32 EX_mdaClipClearData ( EX_MdaClip* clip );
M_Int32 EX_mdaSetDefaultVolume ( M_Int32 cateID, M_Int32 vol );
void EX_mdaSetVolume ( M_Int32 value );
M_Int32 EX_mdaClipSetVolume ( EX_MdaClip* clip, M_Int32 level );
M_Int32 EX_mdaClipGetVolume( MC_MdaClip* clip );
M_Int32 EX_mdaGetVolume( void );
M_Int32 EX_mdaClipAllocPlayer( MC_MdaClip* clip, M_Char* param );
M_Int32 EX_mdaClipFreePlayer( MC_MdaClip* clip );



#endif //__WIPIPROC_H__
