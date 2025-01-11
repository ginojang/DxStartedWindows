#ifndef __EFC_IMAGE_HEADER__
#define __EFC_IMAGE_HEADER__


	enum {
		EN_MIRROR_NONE = 0,
		EN_MIRROR_HORIZON,
		EN_MIRROR_VERTICAL,
		EN_MIRROR_BOTH,
		EN_MIRROR_LAST
	};

	extern void EFC_imgLoadARRAY( LPEXIMAGE pIMGs, schar *pStr, sint32 *pIDXs, sint32 nCount, ubool bORDER, ubool bMIRROR );
	extern void EFC_imgLoadCOUNT( LPEXIMAGE pIMGs, schar *pStr, sint32 nIDX, sint32 nCount );
	extern void EFC_imgLoadIMAGE( LPEXIMAGE pIMGs, schar *pStr, sint32 nIDX );

	extern void EFC_imgFREE( LPEXIMAGE pIMG );
	extern void EFC_imgFreeARRAY( LPEXIMAGE pIMGs, sint32 nCount );
	extern void EFC_imgMIRROR( LPEXIMAGE pIMG, LPEXMOVE pMov, sint32 nMIRROR );

#endif
