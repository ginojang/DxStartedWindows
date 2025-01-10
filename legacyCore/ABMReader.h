#ifndef __ABMREADER_HEADER__
#define __ABMREADER_HEADER__

	////////////////////////////////////////////////////////////////////////////////////////
	// BITMAP
	#define EXBITMAPFILEHEADERSIZE 14
	#define EXBITMAPINFOHEADERSIZE 40

	#define COLORS 256

	typedef struct tagEXBITMAPFILEHEADER {
		uint16			bfType;
		uint32			bfSize;
		uint16			bfReserved1;
		uint16			bfReserved2;
		uint32			bfOffBits;
	} EXBITMAPFILEHEADER, *LPEXBITMAPFILEHEADER; // 2+4+2+2+4 = 14

	typedef struct tagEXBITMAPINFOHEADER {
		uint32			biSize;
		slong			biWidth;
		slong			biHeight;
		uint16			biPlanes;
		uint16			biBitCount;
		uint32			biCompression;
		uint32			biSizeImage;
		slong			biXPelsPerMeter;
		slong			biYPelsPerMeter;
		uint32			biClrUsed;
		uint32			biClrImportant;
	} EXBITMAPINFOHEADER, *LPEXBITMAPINFOHEADER; // 4+4+4 + 2+2 + 4+4 + 4+4 + 4+4 = 40
	////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////
	enum {
		EN_TYPE_ABM,
		EN_TYPE_BBM
	};

	// BMP READER
	typedef struct tagTBMPHEADER {
		sint16				width;
		sint16				height;
		ubyte				col;
	} TBMPHEADER, *LPTBMPHEADER;

	typedef struct tagTBMPREADER {
		LPEXBUFF			pBuff;
		sint32				nTYPE;
		sint32				refX, refY;
		sint32				nPAL;		
		sint32				nPREV;
#if defined(PIXEL_MODE)
		uint8				nPalLen;
		uint8				nPalCnt;
#else
		sint32				nPalLen;
#endif
	} TBMPREADER, *LPTBMPREADER;
	////////////////////////////////////////////////////////////////////////////////////////

	extern void ABMReader_Load( LPEXFILE pFile, LPTBMPREADER pTBmp );
	extern void ABMReader_Free( LPTBMPREADER pTBmp );

	extern ubool ABMReader_skip( LPEXFILE pFile, LPTBMPREADER pTBmp );
	extern ubool ABMReader_next( LPEXFILE pFile, LPEXIMAGE pIMG, LPTBMPREADER pTBmp);
	extern ubool ABMReader_real( LPEXFILE pFile, LPEXIMAGE pIMG, LPTBMPREADER pTBmp);
	extern void ABMReader_imgSetLandScape(LPEXIMAGE pIMG);

	extern void KBitmap_MirrorPixel( ubyte *pByte, sint32 nX1, sint32 nY1, sint32 nX2, sint32 nY2, sint32 nBPL, sint32 nBPP );

#endif
