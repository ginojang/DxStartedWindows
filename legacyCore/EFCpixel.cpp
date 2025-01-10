#include "LegacyCore.h"
#include "LegacyCore.h"

#include "EFCpixel.h"
#include "EFCgrp.h"
#include "EFCimg.h"
#include "EFCfs.h"

#if defined( KT_WIPI )
	WIPIC_SystemAPIs* g_SystemAPIs;
	WIPIC_AnnAPI* WIPIC_annInterface;
#endif

PIXELDATA PixelData;
PIXELPROC PixelProc[EN_MODE_LAST];

sint32 midX;
sint32 midY;
sint32 stdX;
sint32 stdY;
sint32 menuX;
sint32 menuY;
sint32 LCD_W;
sint32 LCD_H;

const uint8 ALPHA_ETC_VALUE[18] =
{
	0, 0, // 32,
	1, 0, // 48
	1, 1, // 64
	2, 0, // 80
	2, 1, // 96
	2, 2, // 128
	3, 0, // 144
	3, 1, // 160
	3, 2, // 192
};

// EN_MODE_ONE
uint16 EFC_pixel_ONE_Proc( sint32 nBACK, sint32 nSRC )
{
	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	return PixelData.nEFF;
}
// EN_MODE_AND
uint16 EFC_pixel_AND_Proc( sint32 nBACK, sint32 nSRC )
{
	return (uint16)(nBACK & nSRC);
}
// EN_MODE_OR
uint16 EFC_pixel_OR_Proc( sint32 nBACK, sint32 nSRC )
{
	return (uint16)(nBACK | nSRC);
}
// EN_MODE_XOR
uint16 EFC_pixel_XOR_Proc( sint32 nBACK, sint32 nSRC )
{
	return (uint16)(nBACK ^ nSRC);
}

// EN_MODE_BRIGHT
uint16 EFC_pixel_BRIGHT16_Proc( sint32 nBACK, sint32 nSRC )
{
	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	return (uint16)BRIGHT16( nSRC, PixelData.nRGB2[0], PixelData.nRGB2[1], PixelData.nRGB2[2] );

}

// EN_MODE_DARK
uint16 EFC_pixel_DARK16_Proc( sint32 nBACK, sint32 nSRC )
{
	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	return (uint16)DARK16( nSRC, PixelData.nRGB2[0], PixelData.nRGB2[1], PixelData.nRGB2[2] );

}

// EN_MODE_ALPHA
uint16 EFC_pixel_ALPHA16_Proc( sint32 nBACK, sint32 nSRC )
{
	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	return (uint16)ALPHA16( nBACK, nSRC, PixelData.nEFF );

}

uint16 EFC_pixel_ALPHA16_112_Proc( sint32 nBACK, sint32 nSRC )
{

	return (uint16)ALPHA16_112( nBACK, nSRC );
}

uint16 EFC_pixel_ALPHA16_128_Proc( sint32 nBACK, sint32 nSRC )
{

	return (uint16)ALPHA16_128( nBACK, nSRC );
}

uint16 EFC_pixel_ALPHA16_ETC_Proc( sint32 nBACK, sint32 nSRC )
{
	return (uint16)ALPHA16_ETC( nBACK, nSRC, ALPHA_ETC_VALUE[PixelData.nEFF<<1], ALPHA_ETC_VALUE[(PixelData.nEFF<<1)+1] );
}
// EN_MODE_GRAY
uint16 EFC_pixel_GRAY16_Proc( sint32 nBACK, sint32 nSRC )
{
	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	PixelData.nCALC = GRAY16_C( nSRC );
	return (uint16)(GRAY16( PixelData.nCALC ) & PixelData.nEFF);

}

// EN_MODE_GRAYGRADE
uint16 EFC_pixel_GRAY16GRADE_Proc( sint32 nBACK, sint32 nSRC )
{
	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	PixelData.nCALC = GRAY16_C( nSRC );
	return (uint16)GRAY16GRADE( nSRC, PixelData.nCALC, PixelData.nEFF );

}

// EN_MODE_MULTIPLY
uint16 EFC_pixel_MULTIPLY16_Proc( sint32 nBACK, sint32 nSRC )
{
	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	return (uint16)MULTIPLY16( nBACK, nSRC );

}

// EN_MODE_SCREEN
uint16 EFC_pixel_SCREEN16_Proc( sint32 nBACK, sint32 nSRC )
{
	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	return (uint16)SCREEN16( nBACK, nSRC );

}

// EN_MODE_LINEAR_DODGE
uint16 EFC_pixel_LINEAR_DODGE16_Proc( sint32 nBACK, sint32 nSRC )
{

	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	return (uint16)LINEAR_DODGE16( nBACK, nSRC );

}

// EN_MODE_LIGHTEN
uint16 EFC_pixel_LIGHTEN16_Proc( sint32 nBACK, sint32 nSRC )
{

	if( PixelData.nTRANS == nSRC ) {
		return (uint16)nBACK;
	}
	return (uint16)LIGHTEN16( nBACK, nSRC );

}

// EN_MODE_SCREENGRAY
uint16 EFC_pixel_SCREENGRAY16_Proc( sint32 nBACK, sint32 nSRC )
{
	PixelData.nCALC = GRAY16_C( nBACK );
	return (uint16)(GRAY16( PixelData.nCALC ));
}

// EN_MODE_SCREENCYAN
uint16 EFC_pixel_SCREENSEPIA16_Proc( sint32 nBACK, sint32 nSRC )
{
	PixelData.nCALC = SEPIA_CALC(RGB16_R(nBACK)<<1,RGB16_G(nBACK),RGB16_B(nBACK)<<1);
	return (uint16)(SEPIA16(PixelData.nCALC));	
}

void EFC_pxlInitialize( void )
{
	MEMSET( PixelProc, 0, sizeof(PixelProc) );
	MEMSET( &PixelData, 0, sizeof(PixelData) );


	PixelData.hReal = MC_grpGetScreenFrameBuffer( 0 );	
	PixelData.rtREAL.nW = (sint16)EFC_GRP_GET_FRAME_BUFFER_WIDTH( PixelData.hReal );
	PixelData.rtREAL.nH = (sint16)(EFC_GRP_GET_FRAME_BUFFER_HEIGHT( PixelData.hReal ) - PixelData.nANNUN);
	PixelData.nBPP = (uint8)EFC_GRP_GET_FRAME_BUFFER_BPP( PixelData.hReal );

	PixelData.isDoubleScreen = FALSE;

	if(PixelData.rtREAL.nH == 800)
	{
		PixelData.isDoubleScreen = TRUE;	
	}
	
	//MemData.bLOCK = TRUE;
	PixelData.nTRANS = RGB32RGB16( 255, 0, 255 );
	PixelData.nCOLOR = RGB32RGB16( 255, 255, 255 );
	PixelData.nScreenOrientation = -1;

	MC_grpInitContext( &PixelData.hGC );


	// PIXEL PROC
	//PixelProc[EN_MODE_NORMAL].proc                    = EFC_pixel_NORMALEX_Proc;
	PixelProc[EN_MODE_ONE].proc                       = EFC_pixel_ONE_Proc;
	PixelProc[EN_MODE_AND].proc                       = EFC_pixel_AND_Proc;
	PixelProc[EN_MODE_OR].proc                        = EFC_pixel_OR_Proc;
	PixelProc[EN_MODE_XOR].proc                       = EFC_pixel_XOR_Proc;

	//if( PixelData.nBPP == 16 ) 
	{
		PixelProc[EN_MODE_BRIGHT].proc                = EFC_pixel_BRIGHT16_Proc;
		PixelProc[EN_MODE_DARK].proc                  = EFC_pixel_DARK16_Proc;
		PixelProc[EN_MODE_ALPHA].proc                 = EFC_pixel_ALPHA16_Proc;

		PixelProc[EN_MODE_GRAY].proc                  = EFC_pixel_GRAY16_Proc;
		PixelProc[EN_MODE_GRAYGRADE].proc             = EFC_pixel_GRAY16GRADE_Proc;

		PixelProc[EN_MODE_MULTIPLY].proc              = EFC_pixel_MULTIPLY16_Proc;
		PixelProc[EN_MODE_SCREEN].proc                = EFC_pixel_SCREEN16_Proc;
		PixelProc[EN_MODE_LINEAR_DODGE].proc          = EFC_pixel_LINEAR_DODGE16_Proc;
		PixelProc[EN_MODE_LIGHTEN].proc               = EFC_pixel_LIGHTEN16_Proc;

		PixelProc[EN_MODE_ALPHA_128].proc             = EFC_pixel_ALPHA16_128_Proc;
		PixelProc[EN_MODE_ALPHA_ETC].proc			  = EFC_pixel_ALPHA16_ETC_Proc;
		PixelProc[EN_MODE_ALPHA_112].proc             = EFC_pixel_ALPHA16_112_Proc;

		PixelProc[EN_MODE_SCREENGRAY].proc			  = EFC_pixel_SCREENGRAY16_Proc;
		PixelProc[EN_MODE_SCREENSEPIA].proc           = EFC_pixel_SCREENSEPIA16_Proc;		
	}
}

void EFC_pxlFinalize( void )
{
	if( PixelData.hScreen != 0 ) {
		MC_grpDestroyOffScreenFrameBuffer( PixelData.hScreen );

		if (PixelData.hLandScapeScreen)
		{
			MC_grpDestroyOffScreenFrameBuffer( PixelData.hLandScapeScreen );
		}

		PixelData.hScreen = 0;
		//MemData.bLOCK = TRUE;
	}

}

void EFC_pxlRectPROC( uint16 *pDATA, sint32 nBPW, sint32 nX, sint32 nY, sint32 nW, sint32 nH, uint16 nCOLOR )
{
	sint32 nSX1, nEX1, nSY1, nEY1;

	if( PixelData.nMODE > EN_MODE_NORMAL ) {
		for( nSY1=(nX + nBPW * nY), nEY1=(nSY1 + nBPW * nH); nSY1<nEY1; nSY1+=nBPW ) {
			for( nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++ ) {
				pDATA[nSX1] = PixelProc[PixelData.nMODE].proc( pDATA[nSX1], nCOLOR );
			}
		}
	} else {
		for( nSY1=(nX + nBPW * nY), nEY1=(nSY1 + nBPW * nH); nSY1<nEY1; nSY1+=nBPW ) {
			for( nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++ ) {
				pDATA[nSX1] = nCOLOR;
			}
		}
	}
}

void EFC_pxlCopyArea( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSX, sint32 nSY )
{
	uint16 *pDATA;
	sint32 nSX1 = 0, nEX1 = 0, nSY1 = 0, nEY1 = 0, nSX2 = 0, nSY2 = 0;
	sint32 nWIDTH, nBPL1, nBPW1;

	nWIDTH = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hBack );
	nBPL1 = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH, PixelData.nBPP );
	nBPW1 = nBPL1 / (PixelData.nBPP >> 3);
	pDATA = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hBack );

	if( (nX == nSX) && (nY == nSY) ) {
		return;
	}

	if( nY < nSY ) {
		if( nX < nSX ) {
			for( nSY1=(nX + nWIDTH * nY), nEY1=(nSY1 + nWIDTH * nH), nSY2=(nSX + nWIDTH * nSY); nSY1<nEY1; nSY1+=nWIDTH, nSY2+=nWIDTH ) {
				for( nSX2=nSY2, nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++, nSX2++ ) {
					pDATA[nSX1] = pDATA[nSX2];
				}
			}
		} else {
			for( nSY1=(nX + nWIDTH * nY), nEY1=(nSY1 + nWIDTH * nH), nSY2=(nSX + nWIDTH * nSY); nSY1<nEY1; nSY1+=nWIDTH, nSY2+=nWIDTH ) {
				for( nSX2=(nSY2 + nW - 1), nSX1=(nSY1 + nW - 1), nEX1=(nSY1 - 1); nSX1>nEX1; nSX1--, nSX2-- ) {
					pDATA[nSX1] = pDATA[nSX2];
				}
			}
		}
	} else {
		if( nX < nSX ) {
			for( nSY1=(nX + nWIDTH * (nY + nH - 1)), nEY1=(nSY1 - (nWIDTH * nH)), nSY2=(nSX + nWIDTH * (nSY + nH - 1)); nSY1>nEY1; nSY1-=nWIDTH, nSY2-=nWIDTH ) {
				for( nSX2=nSY2, nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++, nSX2++ ) {
					pDATA[nSX1] = pDATA[nSX2];
				}
			}
		} else {
			for( nSY1=(nX + nWIDTH * (nY + nH - 1)), nEY1=(nSY1 - (nWIDTH * nH)), nSY2=(nSX + nWIDTH * (nSY + nH - 1)); nSY1>nEY1; nSY1-=nWIDTH, nSY2-=nWIDTH ) {
				for( nSX2=(nSY2 + nW - 1), nSX1=(nSY1 + nW - 1), nEX1=(nSY1 - 1); nSX1>nEX1; nSX1--, nSX2-- ) {
					pDATA[nSX1] = pDATA[nSX2];
				}
			}
		}
	}

}

void EFC_pxlSetFRAME( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, MC_GrpFrameBuffer hSRC, sint32 nSx, sint32 nSy, ubool bTRANS )
{
	sint32 nWIDTH1, nBPL1, nBPW1, nBPL2, nBPW2, nWIDTH2;
	sint32 nSx1, nEx1, nSy1, nEy1, nSx2, nSy2 = 0, nMX2 = 1;

	nWIDTH1 = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hBack );
	nBPL1 = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH1, PixelData.nBPP );
	nBPW1 = nBPL1 / (PixelData.nBPP >> 3);

	nWIDTH2 = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hSRC );
	nBPL2 = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH2, PixelData.nBPP );
	nBPW2 = nBPL2 / (PixelData.nBPP >> 3);

	nSy2 = (nSx + nBPW2 * nSy);

	{
		uint16 *pDATA1, *pDATA2;

		pDATA1 = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hBack );
		pDATA2 = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hSRC );

		if( PixelData.nMODE > EN_MODE_NORMAL ) {
			if( bTRANS == TRUE ) {
				for( nSy1=(nX + nBPW1 * nY), nEy1=(nSy1 + nBPW1 * nH); nSy1<nEy1; nSy1+=nBPW1, nSy2+=nBPW2 ) {
					for( nSx2=nSy2, nSx1=nSy1, nEx1=(nSy1 + nW); nSx1<nEx1; nSx1++, nSx2+=nMX2 ) {
						if( PixelData.nTRANS != pDATA2[nSx2] ) {
							pDATA1[nSx1] = PixelProc[PixelData.nMODE].proc( pDATA1[nSx1], pDATA2[nSx2] );
						}
					}
				}
			} else {
				for( nSy1=(nX + nBPW1 * nY), nEy1=(nSy1 + nBPW1 * nH); nSy1<nEy1; nSy1+=nBPW1, nSy2+=nBPW2 ) {
					for( nSx2=nSy2, nSx1=nSy1, nEx1=(nSy1 + nW); nSx1<nEx1; nSx1++, nSx2+=nMX2 ) {
						pDATA1[nSx1] = PixelProc[PixelData.nMODE].proc( pDATA1[nSx1], pDATA2[nSx2] );
					}
				}
			}
		} else {
			if( bTRANS == TRUE ) {
				for( nSy1=(nX + nBPW1 * nY), nEy1=(nSy1 + nBPW1 * nH); nSy1<nEy1; nSy1+=nBPW1, nSy2+=nBPW2 ) {
					for( nSx2=nSy2, nSx1=nSy1, nEx1=(nSy1 + nW); nSx1<nEx1; nSx1++, nSx2+=nMX2 ) {
						if( PixelData.nTRANS != pDATA2[nSx2] ) {
							pDATA1[nSx1] = pDATA2[nSx2];
						}
					}
				}
			} else {
				for( nSy1=(nX + nBPW1 * nY), nEy1=(nSy1 + nBPW1 * nH); nSy1<nEy1; nSy1+=nBPW1, nSy2+=nBPW2 ) {
					for( nSx2=nSy2, nSx1=nSy1, nEx1=(nSy1 + nW); nSx1<nEx1; nSx1++, nSx2+=nMX2 ) {
						pDATA1[nSx1] = pDATA2[nSx2];
					}
				}
			}
		}
	}
}

void EFC_pxlCopyFrameBuffer( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, MC_GrpFrameBuffer hSRC, sint32 nSx, sint32 nSy, ubool bTRANS )
{
	if( hBack == hSRC ) {
		EFC_pxlCopyArea( hBack, nX, nY, nW, nH, nSx, nSy );
	} else {
		EFC_pxlSetFRAME( hBack, nX, nY, nW, nH, hSRC, nSx, nSy, bTRANS );
	}
}

void EFC_pxlCopyFrameScale( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, MC_GrpFrameBuffer hSRC, sint32 nSX, sint32 nSY, sint32 nEx, sint32 nEy, ubool bTRANS )
{
	sint32 nWIDTH1, nBPL1, nBPW1, nBPL2, nBPW2, nWIDTH2, nHEIGHT2;
	sint32 i, j, nDX1, nDY1, nSY1, nSX1, nSX2, nGW, nGH, nPOS;

	uint16 *pDATA1, *pDATA2;

	nWIDTH1 = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hBack );
	nBPL1 = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH1, PixelData.nBPP );
	nBPW1 = nBPL1 / (PixelData.nBPP >> 3);

	nWIDTH2 = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hSRC );
	nBPL2 = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH2, PixelData.nBPP );
	nBPW2 = nBPL2 / (PixelData.nBPP >> 3);

	nHEIGHT2 = EFC_GRP_GET_FRAME_BUFFER_HEIGHT( hSRC );

	nSX = MIN_CALC( MAX_CALC( 0, nSX ), (nWIDTH2 - 1) );
	nSY = MIN_CALC( MAX_CALC( 0, nSY ), (nHEIGHT2 - 1) );
	nEx = MIN_CALC( MAX_CALC( 0, nEx ), (nWIDTH2 - 1) );
	nEy = MIN_CALC( MAX_CALC( 0, nEy ), (nHEIGHT2 - 1) );

	nGW = ((nEx - nSX + 1) << 12) / nW;
	nGH = ((nEy - nSY + 1) << 12) / nH;

	pDATA1 = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hBack );
	pDATA2 = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hSRC );

	if( PixelData.nMODE != NULL ) {
		if(bTRANS) 
		{
			for( i=0, nDY1=(nY * nWIDTH1 + nX), nSY1=0; i<nH; i++, nDY1+=nWIDTH1, nSY1+=nGH ) {
				for( j=0, nDX1=nDY1, nSX1=((nSY + (nSY1 >> 12)) * nWIDTH2 + nSX), nSX2=0; j<nW; j++, nDX1++, nSX2+=nGW ) {
					nPOS = (nSX1 + (nSX2 >> 12));
					if( PixelData.nTRANS != pDATA2[nPOS] ) {
						pDATA1[nDX1] = PixelProc[PixelData.nMODE].proc( pDATA1[nDX1], pDATA2[nPOS] );
					}
				}
			}
		} else {
			for( i=0, nDY1=(nY * nWIDTH1 + nX), nSY1=0; i<nH; i++, nDY1+=nWIDTH1, nSY1+=nGH ) {
				for( j=0, nDX1=nDY1, nSX1=((nSY + (nSY1 >> 12)) * nWIDTH2 + nSX), nSX2=0; j<nW; j++, nDX1++, nSX2+=nGW ) {
					nPOS = (nSX1 + (nSX2 >> 12));
					pDATA1[nDX1] = PixelProc[PixelData.nMODE].proc( pDATA1[nDX1], pDATA2[nPOS] );
				}
			}
		}		
	} else if(bTRANS) {
		for( i=0, nDY1=(nY * nWIDTH1 + nX), nSY1=0; i<nH; i++, nDY1+=nWIDTH1, nSY1+=nGH ) {
			for( j=0, nDX1=nDY1, nSX1=((nSY + (nSY1 >> 12)) * nWIDTH2 + nSX), nSX2=0; j<nW; j++, nDX1++, nSX2+=nGW ) {
				nPOS = (nSX1 + (nSX2 >> 12));
				if( PixelData.nTRANS != pDATA2[nPOS] ) {
					pDATA1[nDX1] = pDATA2[nPOS];
				}
			}
		}
	}
	else {
		for( i=0, nDY1=(nY * nWIDTH1 + nX), nSY1=0; i<nH; i++, nDY1+=nWIDTH1, nSY1+=nGH ) {
			for( j=0, nDX1=nDY1, nSX1=((nSY + (nSY1 >> 12)) * nWIDTH2+ nSX), nSX2=0; j<nW; j++, nDX1++, nSX2+=nGW ) {
				nPOS = (nSX1 + (nSX2 >> 12));
				pDATA1[nDX1] = pDATA2[nPOS];
			}
		}
	}
}

void EFC_pxlSetPixel( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY )
{
	sint32 nWIDTH, nBPL, nBPW;

	nWIDTH = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hBack );
	nBPL = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH, PixelData.nBPP );
	nBPW = nBPL / (PixelData.nBPP >> 3);

	{
		uint16 *pDATA, nCOLOR;

		nCOLOR = (uint16)PixelData.nCOLOR;
		pDATA = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hBack );

		EFC_pxlRectPROC( pDATA, nBPW, nX, nY, 1, 1, nCOLOR );
	} 
}

void EFC_pxlPutPixel( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY )
{
	EFC_pxlSetPixel( hBack, nX, nY );
}

void EFC_pxlSetRect( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	sint32 nWIDTH, nBPL, nBPW;

	nWIDTH = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hBack );
	nBPL = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH, PixelData.nBPP );
	nBPW = nBPL / (PixelData.nBPP >> 3);

	{
		uint16 *pDATA, nCOLOR;

		nCOLOR = (uint16)PixelData.nCOLOR;
		pDATA = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hBack );

		EFC_pxlRectPROC( pDATA, nBPW, nX, nY, nW, nH, nCOLOR );
	}
}

void EFC_pxlFillRect( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH )
{
	EFC_pxlSetRect( hBack, nX, nY, nW, nH );
}
/*
void EFC_pxlDrawShadow( sint32 nX, sint32 nY, LPEXIMAGE pIMG, sint32 dir, sint32 nMIR )
{
	EXMOVE rMov;
	EXREGION rgn;

	sint32 revX = 0, revY = 0, revW = 0, revH = 0;

	EFC_imgMIRROR(pIMG, &rMov, nMIR);

	EFC_rgnSetREGION( &rgn, nX - rMov.moveX, nY - rMov.moveY, pIMG->nW, pIMG->nH, 0, 0 );
	if( EFC_rgnRealREGION( &rgn, nMIR ) == FALSE ) {
		return;
	}

	{
		MC_GrpFrameBuffer hSRC;
		hSRC =  MC_grpCreateOffScreenFrameBuffer( rgn.nW, rgn.nH );
		if( hSRC == 0 ) {
			return;
		}
		revX = (dir-2)%2*7;
		revY = (dir-1)%2*7;

		EFC_pxlDrawBuff( (*PixelData.pFrame), rgn.nX, rgn.nY, rgn.nW, rgn.nH, pIMG, rgn.nSx, rgn.nSy, nMIR, 0);

		EFC_grpReset();

		EFC_grpCopyBUFF(rgn.nX+revX, rgn.nY+revY, rgn.nW+revW, rgn.nH+revH,hSRC, 0,0,rgn.nW, rgn.nH, TRUE);
		MC_grpDestroyOffScreenFrameBuffer( hSRC );
	}
}
*/
void EFC_pxlDrawArc( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, MC_GrpFrameBuffer hSRC )
{
	sint32 nWIDTH, nBPL, nBPW1, nBPW2;
	sint32 nSX1, nEX1, nSY1, nEY1, nSX2, nSY2 = 0;
	uint16 *pDATA, *pDATA2;

	nWIDTH = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hBack );
	nBPL = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH, PixelData.nBPP );
	nBPW1 = nBPL / (PixelData.nBPP >> 3);

	nWIDTH = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hSRC );
	nBPL = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH, PixelData.nBPP );
	nBPW2 = nBPL / (PixelData.nBPP >> 3);

	pDATA = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hBack );
	pDATA2 = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hSRC );

	for( nSY1=(nX + nBPW1 * nY), nEY1=(nSY1 + nBPW1 * nH); nSY1<nEY1; nSY1+=nBPW1, nSY2+=nBPW2) {
		for( nSX2=nSY2, nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++, nSX2++) {
			if(pDATA2[nSX2]!=PixelData.nTRANS)
			{
				pDATA2[nSX2] = pDATA[nSX1];
			}
		}
	}
}
void EFC_pxlDrawBuff( MC_GrpFrameBuffer hBack, sint32 nX, sint32 nY, sint32 nW, sint32 nH, LPEXIMAGE pIMG, sint32 nSX, sint32 nSY, sint32 nMIR, sint32 nPalette)
{
	sint32 nWIDTH, nBPL, nBPW1, nBPW2;
	sint32 nSX1, nEX1, nSY1, nEY1, nSX2, nSY2 = 0, nMX1 = 2, nMX2 = 1;

	nWIDTH = EFC_GRP_GET_FRAME_BUFFER_WIDTH( hBack );
	nBPL = EFC_GRP_GET_FRAME_BUFFER_BPL( nWIDTH, PixelData.nBPP );
	nBPW1 = nBPL / (PixelData.nBPP >> 3);
	nBPW2 =  WIDTH_BYTES(pIMG->nW);

	{
		uint16 *pDATA;
		ubyte *pDATA2 = EFC_memGET( pIMG->pBuff );
		sint32 nPIXEL = 0;
		uint32 nCOLOR;
		sint32 nPALPOS = MIN_CALC(nPalette, pIMG->nPalCnt-1) * ( pIMG->nPalLen << 2 );
		sint32 nTRANS = CONVERT_INT(pDATA2, 0);
		pDATA = (uint16 *)EFC_GRP_GET_FRAME_BUFFER_POINTER( hBack );
		
		switch( nMIR ) {
		case EN_MIRROR_NONE :
			nSY2 = (pIMG->nDATPOS + nSX + nBPW2 * (pIMG->nH - nSY - 1));
			break;
		case EN_MIRROR_HORIZON :
			if(nX <= 0) {
				nSX = 0;
			} else {
				nSX = pIMG->nW-nW;
			}
			nMX1 = -nMX1;
			nMX2 = -nMX2;
			nSY2 = (pIMG->nDATPOS + nSX + nBPW2 * (pIMG->nH - nSY - 1)) + nW - 1;
			break;
		case EN_MIRROR_VERTICAL :
			if(nY <= 0) {
				nSY = 0;
			} else {                                                                                                                                                                                                                                                                                                                                                                                                                                                          
				nSY = pIMG->nH-nH;
			}
			nSY2 = (pIMG->nDATPOS + nSX + nBPW2 * (pIMG->nH - nSY - nH));
			nBPW2 = -nBPW2;
			break;
		case EN_MIRROR_BOTH :
			if(nX <= 0) {
				nSX = 0;
			} else {
				nSX = pIMG->nW-nW;
			}
			if(nY <= 0) {
				nSY = 0;
			} else {
				nSY = pIMG->nH-nH;
			}
			nMX1 = -nMX1;
			nMX2 = -nMX2;
			nSY2 = (pIMG->nDATPOS + nSX + nBPW2 * (pIMG->nH - nSY - nH)) + nW - 1;
			nBPW2 = -nBPW2;
			break;
		}

		if( PixelData.nMODE != NULL ) {
			if(nTRANS==PixelData.nTRANS) {
				for( nSY1=(nX + nBPW1 * nY), nEY1=(nSY1 + nBPW1 * nH); nSY1<nEY1; nSY1+=nBPW1, nSY2-=nBPW2 ) {
					for( nSX2=nSY2, nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++, nSX2+=nMX2 ) {
						nPIXEL = (pDATA2[nSX2]<<2);
						if(nPIXEL>0)
						{
							nCOLOR = CONVERT_INT(pDATA2, nPIXEL+nPALPOS);
							pDATA[nSX1] = PixelProc[PixelData.nMODE].proc( pDATA[nSX1], nCOLOR );
						}
					}
				}
			} else {
				for( nSY1=(nX + nBPW1 * nY), nEY1=(nSY1 + nBPW1 * nH); nSY1<nEY1; nSY1+=nBPW1, nSY2-=nBPW2 ) {
					for( nSX2=nSY2, nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++, nSX2+=nMX2 ) {
						nPIXEL = (pDATA2[nSX2]<<2)+nPALPOS;
						nCOLOR = CONVERT_INT(pDATA2, nPIXEL);
						pDATA[nSX1] = PixelProc[PixelData.nMODE].proc( pDATA[nSX1], nCOLOR );
					}
				}
			}
		} else if(nTRANS==PixelData.nTRANS) {
			for( nSY1=(nX + nBPW1 * nY), nEY1=(nSY1 + nBPW1 * nH); nSY1<nEY1; nSY1+=nBPW1, nSY2-=nBPW2 ) {
				for( nSX2=nSY2, nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++, nSX2+=nMX2 ) {
					nPIXEL = (pDATA2[nSX2]<<2);
					if(nPIXEL>0)
					{
						nCOLOR = CONVERT_INT(pDATA2, nPIXEL+nPALPOS);
						pDATA[nSX1] = (uint16)nCOLOR;
					}
				}
			}
		} else {
			for( nSY1=(nX + nBPW1 * nY), nEY1=(nSY1 + nBPW1 * nH); nSY1<nEY1; nSY1+=nBPW1, nSY2-=nBPW2 ) {
				for( nSX2=nSY2, nSX1=nSY1, nEX1=(nSY1 + nW); nSX1<nEX1; nSX1++, nSX2+=nMX2 ) {
					nPIXEL = (pDATA2[nSX2]<<2)+nPALPOS;
					nCOLOR = CONVERT_INT(pDATA2, nPIXEL);
					pDATA[nSX1] = (uint16)nCOLOR;
				}
			}
		}
	}
}

