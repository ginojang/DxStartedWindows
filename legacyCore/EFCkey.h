#ifndef __EFC_KEY_HEADER__
#define __EFC_KEY_HEADER__

	#define KEY_DASH_TIMES 1000
	#define KEY_BUFFS 12

	typedef struct tagEXKEYDATA {
		sint32				nPOS;
		sint32				nCOUNT;

		sint32				nKEY;
		sint32				nCURR;
		sint32				nDIFF;

		sint64				nTIMES[KEY_BUFFS]; // 키 마지막 누른 시간
		sint32				nKEYS[KEY_BUFFS];

		sint32				nREPEAT; // 리피트로 처리할 시간
	} EXKEYDATA, *LPEXKEYDATA;

	extern EXKEYDATA KeyData;

	extern void EFC_keyInitialize( void );
	extern void EFC_keyFinalize( void );

	extern sint32 EFC_keyCOUNT( void );
	extern sint32 EFC_keyGET( void );
	extern sint32 EFC_keyGetDASH( sint32 nKEY, ubool bMODIFY );

	extern sint32 EFC_keyLastTIMES( void );
	extern ubool EFC_keyIsREPEAT( void );
	extern sint32 EFC_keyLAST( void );
	extern void EFC_keySetREPEAT( sint32 nTIMES );

	extern sint32 EFC_keyTransform(sint32 nKEY);
	extern void EFC_keyADD( sint32 nKEY );
	extern void EFC_keyReset( void );

	extern sint32 EFC_keyMODIFY( sint32 nKEY );
	extern sint32 EFC_keyPREV( sint32 nPREV );

	extern sint32 EFC_keySTART( sint32 nCURR, sint32 nNEXT, sint32 nMINUS );
	extern sint32 EFC_keyPOS( sint32 nCURR, sint32 nMAX, sint32 nPLUS );

#endif
