#ifndef __EFC_MAIN_HEADER__
#define __EFC_MAIN_HEADER__



	#define TOUCH_COUNT 10

	typedef struct tagEXTOUCHKEYDATA {
		sint32 nKEY;
		sint32 nKEYS;
		sint32 nTouchType;
		EXRECT rt;
	} EXTOUCHKEYDATA, *LPEXTOUCHKEYDATA;

	typedef struct tagEXTOUCHKEYLIST {
		sint32 nTOUCHES;
		EXTOUCHKEYDATA xDATA[TOUCH_COUNT];
	} EXTOUCHKEYLIST, *LPEXTOUCHKEYLIST;

	typedef struct tagEXTOUCHDATA {
		sint32 nType;
		EXPOINT	 stPoint;
	} EXTOUCHDATA, *LPEXTOUCHDATA;

	typedef struct tagEXTOUCHLIST {
		sint32 nPOS;
		sint32 nCOUNT;
		EXTOUCHDATA xDATA[TOUCH_COUNT];
	} EXTOUCHLIST, *LPEXTOUCHLIST;

	typedef struct tagEXMAINDATA {
		// APPLICATION DATA
		sint32			nKEY;
		sint32			stateCount;

		ubool			bFINAL;

		ubool			bEND;
		ubool			isDebugDraw;
		sint32			nEXIT;

		EXTOUCHKEYLIST		xTOUCHKEY;
		EXTOUCHLIST		xTOUCH;		
	} EXMAINDATA, *LPEXMAINDATA;

	extern EXMAINDATA MainData;

	extern sint32 midX;
	extern sint32 midY;
	extern sint32 stdX;
	extern sint32 stdY;
	extern sint32 LCD_W;
	extern sint32 LCD_H;
	extern sint32 menuX;
	extern sint32 menuY;

	extern void EFC_mainInitialize( void );
	extern void EFC_mainFinalize( void );
	extern void EFC_mainPAUSE( void );
	extern void EFC_mainRESUME( void );

	extern void EFC_mainTouch(LPEXPOINT pPoint, sint32 nType);
	extern void EFC_mainKeyPRESS( sint32 nKEY );
	extern void EFC_mainKeyRELEASE( void );
	extern ubool EFC_mainKeyTOUCH( LPEXPOINT pPoint, sint32 nType);

	extern void EFC_mainSetNEXT( sint32 nNEXT );

	extern void EFC_TouchReset( void );
	extern void EFC_mainInitTOUCH( void );
	extern void EFC_mainSetTOUCH( sint32 nKEY, sint32 nKEYS, sint32 nX, sint32 nY, sint32 nW, sint32 nH );

	extern void EFC_mainDRAW( void );
	
#if defined( SKT_WIPI )
	extern void EFC_mainURL( schar *pStr );
#endif

#endif
