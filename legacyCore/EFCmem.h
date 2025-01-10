#ifndef __EFC_MEMORY_HEADER__
#define __EFC_MEMORY_HEADER__

	#define DF_MEM_COUNT 30000

	typedef struct tagEXMEMDATA 
	{
		EXBUFF				xBuffs[DF_MEM_COUNT];

	} EXMEMDATA, *LPEXMEMDATA;


	extern EXMEMDATA MemData;

	extern void EFC_memInitialize( void );
	extern void EFC_memFinalize( void );

	extern void EFC_memReset( void );

	extern void EFC_memCheck( void );
	extern LPEXBUFF EFC_memEMPTY( void );
	extern ubyte *EFC_memGET( LPEXBUFF pBuff );

	extern ubool EFC_memSET( LPEXBUFF pBuff, sint32 nSize );
	extern LPEXBUFF EFC_memCOPY( LPEXBUFF pBuff );

	extern LPEXBUFF EFC_memALLOC( sint32 nSize );
	extern void EFC_memFREE( LPEXBUFF pBuff );
	extern void EFC_memFreeALL( void );

	extern void EFC_fxtCHANGE( LPEXBUFF pBuff, schar szOld, schar szNew );

#endif
