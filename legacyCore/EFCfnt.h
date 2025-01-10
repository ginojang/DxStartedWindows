#ifndef __EFC_FONT_HEADER__
#define __EFC_FONT_HEADER__

	#define FNT_PAGES 50

	enum {
		EN_BOLD_NONE = 0,
		EN_BOLD_ALL,
		EN_BOLD_WIDTH,
		EN_BOLD_LAST
	};

	typedef struct tagEXSPACE {
		sint16			nWidth;
		sint16			nSize;
		sint16			nCount;
		ubool			bEnter;
		ubool			bLastSpace;
	} EXSPACE, *LPEXSPACE;

	typedef struct tagEXPAGE {
		sint32			nPos[FNT_PAGES];
		LPEXBUFF		pBuff;
		sint16			nWidth;
		sint16			nLineH;
		sint8			nLines;
		sint8			nLine;
		ubyte			nPage;
		ubyte			nPages;
		sint32			nTyping;
	} EXPAGE, *LPEXPAGE;

	typedef struct tagEXFONTDATA {
		MC_GrpFrameBuffer		hBack;
		LPEXBUFF				pPAL;
		LPEXBUFF				pDATA;	// 폰트 완성형 데이터 버퍼
		uint8					nBOLD;

		sint16					nW;	// 이미지 가로 사이즈
		sint16					nH;

		// FONT DATA
		sint16					nEnW;
		sint16					nHanW;
		sint16					nNumEn;

		sint16					nEnBytes;
		sint16					nHanBytes;

		EXRECT					rtEN;
		EXRECT					rtHAN;
	} EXFONTDATA, *LPEXFONTDATA;

	extern EXFONTDATA FontData;

	extern void EFC_fntInitialize( void );
	extern void EFC_fntFinalize( void );

	extern void EFC_fntSetBOLD( sint32 nBOLD );
	extern void EFC_fntSetRGB( sint32 nIDX, sint32 nRGB );
	extern sint32 EFC_fntGetRGB( sint32 nIDX );

	extern EXSPACE EFC_fntGetWidthPos( LPEXBUFF pBuff, sint32 nWidth, sint32 *pPos );
	extern sint32 EFC_fntGetWIDTH( LPEXBUFF pBuff );

	extern LPEXBUFF EFC_fntReplace( LPEXBUFF pBuff, schar *pOld, schar *pNew );
	extern void EFC_fntChange( LPEXBUFF pBuff, schar szOld, schar szNew );
	extern void EFC_fntSetNumberStr(const char *pStr, char *desStr, sint32 *pNumber);

	extern EXPAGE EFC_fntGetStringPage( LPEXBUFF pBuff, sint32 nWidth, sint32 nLineH, sint32 nLines );
	extern void EFC_fntDrawStringPage( sint32 nX, sint32 nY, LPEXPAGE pPage, sint32 nANCHOR );
	extern void EFC_fntDrawStringTyping( sint32 nX, sint32 nY, LPEXPAGE pPage, sint32 nANCHOR );
	extern void EFC_fntDrawText( sint32 nX, sint32 nY, sint32 nW, sint32 nLineH, LPEXBUFF pBuff, sint32 nPos, sint32 *pTyping, sint32 nLines, sint32 anchor );

	extern void EFC_fntDrawStringEx( sint32 nX, sint32 nY, sint32 nW, sint32 nLineH, LPEXBUFF pBuff, sint32 nANCHOR );
	extern void EFC_fntDrawString( sint32 nX, sint32 nY, LPEXBUFF pBuff, sint32 nANCHOR );
	extern void EFC_fntDrawStringBold( sint32 nX, sint32 nY, schar *pSTR, sint32 nANCHOR, RGBVAL nRGB1, RGBVAL nRGB2 );

	extern void EFC_fntStringPageLine( LPEXPAGE pPage );
	extern void EFC_fntTRIM( schar *pSTR );

#endif
