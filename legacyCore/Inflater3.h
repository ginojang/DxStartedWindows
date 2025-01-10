#ifndef __INFLATER3_HEADER__
#define __INFLATER3_HEADER__

	typedef struct tagEXINFLATER {
		LPEXBUFF pDataIn;
		sint32 inOffset;

		LPEXBUFF pDataOut;
		sint32 outOffset;
	} EXINFLATER, *LPEXINFLATER;

	extern EXINFLATER xInfl;

	extern void Inflater_inflate( LPEXBUFF pDataIn, LPEXBUFF pDataOut );

#endif
