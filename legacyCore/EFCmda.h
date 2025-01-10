#ifndef __EFC_MEDIA_HEADER__
#define __EFC_MEDIA_HEADER__

	#define DF_MDA_COUNT 42

	#define VIBRATE_1 60
//	#define VIBRATE_2 200
//	#define VIBRATE_3 500
	#define VIBRATE_4 200
//	#define VIBRATE_5 2000

	typedef struct tagEXPLAY {
		sint32				nID;
		ubool				bREPEAT;
		sint8				nPLAY;
		sint8				nDEPTH;
	} EXPLAY, *LPEXPLAY;

	typedef struct tagEXMDADATA {
		MC_MdaClip*			pCLIP;

		EXPLAY				xPlay;
		EXPLAY				xResv;
		EXPLAY				xBack;

		sint32				nDIFFs;

		sint32				nSTATUS;
		sint64				actSndTime;
		ubool				bVIBRATE;
		ubool				bSHOWHELMET;
		ubool				bSCREENORIENTATION;

		//추가
		sint8				nQUALITY;
		sint8				nSPEED;
		sint8				nITEMLOOT;
		sint8				nSOUNDKIND;//"효과음", "배경음", "배경음/효과음"

		sint8				nVOLUME;
		sint8				nVOLUMEs;

		LPEXBUFF            pDATAs[DF_MDA_COUNT];

		ubool				bKEYTONE;
		ubool				nVIBRATE;
		sint32				nVIBRATEPOWER;
		ubool				bSAVEUPLOADCHECK;
	} EXMDADATA, *LPEXMDADATA;

	extern EXMDADATA MdaData;

	extern void EFC_mdaInitialize( sint32 nBufSize );
	extern void EFC_mdaFinalize( void );

	extern void EFC_mdaFREE( sint32 nID );
	extern void EFC_mdaFreeALL( void );

	extern void EFC_mdaPlayBACK( sint32 nID );
	extern void EFC_mdaPlayCONTINUE( sint32 nID );
	extern void EFC_mdaPlayREPEAT( sint32 nID );
	extern void EFC_mdaPlayEFFECT( sint32 nID );
	extern void EFC_mdaPlayDATA( sint32 nID );
	extern void EFC_mdaPlayTest( sint32 nID );
	extern void EFC_mdaPlayREAL( void );
	extern void EFC_mdaStopREAL( void );

	extern void EFC_mdaStopALL( void );
	extern void EFC_mdaStopWAIT( void );

	extern void EFC_mdaPAUSE( void );
	extern void EFC_mdaRESUME( void );

	extern void EFC_mdaRUN( void );

	extern void EFC_mdaVIBRATE( sint32 nTimeOut );
	extern ubool EFC_mdaVOLUME( sint32 nSTATUS );

#endif
