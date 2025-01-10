#ifndef __EFC_KERNELL_HEADER__
#define __EFC_KERNELL_HEADER__


	typedef struct tagEXRECT {
		sint16		nX, nY;
		sint16		nW, nH;
	} EXRECT, *LPEXRECT;

	typedef struct tagEXPOINT {
		sint16		nX, nY;
	} EXPOINT, *LPEXPOINT;

	typedef struct tagEXREGION {
		sint16		nX, nY;
		sint16		nW, nH;
		sint16		nSx, nSy;
	} EXREGION, *LPEXREGION;

	typedef struct tagEXKNLDATA {
		sint64			nRand;
		ubool			isFirst;
		ubool			bEVENT1;
		ubool			bEVENT2;
		schar			szPhone[32];
		schar			szModel[32];
	} EXKNLDATA, *LPEXKNLDATA;

	extern EXKNLDATA KnlData;

	extern void EFC_knlInitialize( void );
	extern void EFC_knlFinalize( void );

	extern sint32 EFC_knlGetRAND( void );
	extern void EFC_knlEXIT( void );

	extern sint32 EFC_knlTouchEvent( LPEXPOINT pPOINT, sint32 type, sint32 param1, sint32 param2 );
	extern sint32 EFC_knlGetCRC( LPEXBUFF pBuff, sint32 Offset, sint32 Size );

	extern void EFC_miscBackLight( ubool bON );

	extern ubool EFC_rgnIsInRECT( LPEXRECT pRect, sint32 nX, sint32 nY );

	extern void EFC_rgnSetPOINT( LPEXPOINT pPT, sint32 nX, sint32 nY );
	extern void EFC_rgnRealPOINT( LPEXPOINT pPT, sint32 nX, sint32 nY );

	extern ubool EFC_rgnSetRECT( LPEXRECT pRect, sint32 nX, sint32 nY, sint32 nW, sint32 nH );
	extern ubool EFC_rgnInterRECT( LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2 );
	extern ubool EFC_rgnUnionRECT( LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2 );
	extern ubool EFC_rgnPlusRECT( LPEXRECT pRect, LPEXRECT pRect1, sint32 nX, sint32 nY );
	extern ubool EFC_rgnRealRECT( LPEXRECT pRect, LPEXRECT pRect1 );

	extern ubool EFC_rgnSetREGION( LPEXREGION pRgn, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy );
	extern ubool EFC_rgnInterREGION( LPEXREGION pRgn, LPEXRECT pRect, LPEXREGION pRegion );
	extern ubool EFC_rgnRealREGION( LPEXREGION pRgn, sint32 nMIR );
	extern ubool EFC_rgnGetRECT( LPEXRECT pRect, LPEXREGION pRegion );

	extern sint32 EFC_mathiSqrt( uint32 nX );

	extern sint32 EFC_mathCircleX( sint32 nX, sint32 nW, sint32 nH );
	extern sint32 EFC_mathCircleY( sint32 nY, sint32 nW, sint32 nH );

	extern sint32 EFC_mathSIN( sint32 nANGLE );
	extern sint32 EFC_mathCOS( sint32 nANGLE );

	extern void EFC_mathMOVE( LPEXPOINT ptMOVE, LPEXPOINT ptCURR, LPEXPOINT ptNEXT, LPEXPOINT ptDIRECT, sint32 nMOVE, uint8 nRATE1, uint16 nRATE2 );

#endif
