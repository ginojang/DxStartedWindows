#include "LegacyCore.h"
#include "EFCthr.h"

#include "EFCpixel.h"


EXTHREADDATA ThreadData;

void EFC_thrInitialize( void )
{
	MEMSET( &ThreadData, 0, sizeof(EXTHREADDATA) );

	EFC_thrSPEED( MAX_SPEED - 1 );
}

void EFC_thrFinalize( void )
{
	EFC_thrSTOP();
}

//void EFC_thrRUN( MCTimer *tm, void *param )
/*{

	if( ThreadData.bCONTINUE == FALSE ) {
		ThreadData.nDIFF = 0;
		MC_knlUnsetTimer( &ThreadData.hTimer );
		return;
	}

	if (ThreadData.bDRAW == FALSE)
	{
		ThreadData.nNOW = MC_knlCurrentTime();
		MC_knlSetTimer( &ThreadData.hTimer, MAX_CALC( 1, (ThreadData.nMILLIS - (ThreadData.nNOW - ThreadData.nCURR)) ), 0 );
		return;
	}

	ThreadData.nNOW = MC_knlCurrentTime();
	ThreadData.nDIFF = (sint32)(ThreadData.nNOW - ThreadData.nCURR);

	ThreadData.nCURR = ThreadData.nNOW;
	ThreadData.bDRAW = FALSE;
	//EFC_mainDRAW();


	ThreadData.nNOW = MC_knlCurrentTime();
	MC_knlSetTimer( &ThreadData.hTimer, MAX_CALC( 1, (ThreadData.nMILLIS - (ThreadData.nNOW - ThreadData.nCURR)) ), 0 );
	ThreadData.bDRAW = TRUE;
	
}*/

void EFC_thrSLEEP( sint32 nMILLIS )
{
	ThreadData.nMILLIS = (uint32)MAX_CALC( 1, nMILLIS );
}

void EFC_thrSPEED( sint32 nSPEED )
{
	ThreadData.nSPEED = nSPEED%(MAX_SPEED+1);

#if defined(ANDROID) && !defined(LGT_ANDROID)
	EFC_thrSLEEP( 200 + (MAX_SPEED - ThreadData.nSPEED - 1)*200 );
#else
	EFC_thrSLEEP( 50 + (MAX_SPEED - ThreadData.nSPEED - 1)*30 );
#endif
}

void EFC_thrPLAY( void )
{
	//EFC_thrSLEEP( ThreadData.nMILLIS );

	//MC_knlDefTimer( &ThreadData.hTimer, EFC_thrRUN );

	//ThreadData.nDIFF = ThreadData.nMILLIS;
	//ThreadData.nCURR = MC_knlCurrentTime();
	//ThreadData.bCONTINUE = TRUE;
	//ThreadData.bDRAW = TRUE;


	//EFC_thrRUN( NULL, NULL );
}

void EFC_thrSTOP( void )
{
	//ThreadData.bCONTINUE = FALSE;
	//ThreadData.nDIFF = 0;

	//MC_knlUnsetTimer( &ThreadData.hTimer );
}
