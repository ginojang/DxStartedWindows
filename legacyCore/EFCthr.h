#ifndef __EFC_THREAD_HEADER__
#define __EFC_THREAD_HEADER__

	#define MAX_SPEED 3

	typedef struct tagEXTHREADDATA {
		MCTimer				hTimer;	// ≈∏¿Ã∏”

		sint32				nSPEED;
		sint32				nMILLIS;

		sint64				nNOW;
		sint64				nCURR;
		sint32				nDIFF;
		ubool				bDRAW;
		ubool				bCONTINUE;

#if defined( USE_DEBUG_MESSAGE )
		sint32				nDIFFs;
		sint32				nDIFFCnt;
		schar				cDebugBuff[256];
		schar				cDebugBuff2[256];
#endif
	} EXTHREADDATA, *LPEXTHREADDATA;

	extern EXTHREADDATA ThreadData;

	extern void EFC_thrInitialize( void );
	extern void EFC_thrFinalize( void );

	extern void EFC_thrSLEEP( sint32 nMILLIS );
	extern void EFC_thrSPEED( sint32 nSPEED );

	extern void EFC_thrPLAY( void );
	extern void EFC_thrSTOP( void );

#endif
