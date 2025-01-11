#pragma once

#define CHANGE_COLORS 4


enum {
	EN_MODE_NORMAL = 0,
	EN_MODE_IMAGE,
	EN_MODE_AND,
	EN_MODE_OR,
	EN_MODE_MULTIPLY,
	EN_MODE_SCREEN,
	EN_MODE_LIGHTEN,
	EN_MODE_LINEAR_DODGE,

	EN_MODE_ALPHA,
	EN_MODE_BRIGHT,
	EN_MODE_DARK,

	EN_MODE_GRAY,
	EN_MODE_ONE,

	EN_MODE_GRAYGRADE,

	EN_MODE_ALPHA_112,
	EN_MODE_ALPHA_128,
	EN_MODE_ALPHA_ETC,
	EN_MODE_XOR,

	EN_MODE_SCREENGRAY,
	EN_MODE_SCREENSEPIA,

	EN_MODE_LAST
};

typedef M_Int32(*MC_GrpPixelOpProc)(M_Int32 srcpxl, M_Int32 orgpxl, M_Int32 param1);

typedef struct tagPIXELPROC
{
	uint16(*proc)(sint32 nBACK, sint32 nSRC);

} PIXELPROC, * LPPIXELPROC;

typedef struct _MC_GrpContext
{
	M_Int32 m_fgpxl;
	M_Int32 m_transpxl;
	MC_GrpPixelOpProc m_pfnPixelOp;
	M_Int32 m_param1;

} PIXELContext;

typedef struct _FilterProcData
{
	uint8				nMODE;
	uint16				nEFF;
	sint32				nCALC;
	
	sint32				nTRANS;
	sint32				nCOLOR;

	sint32				nRGB1[CHANGE_COLORS];
	sint32				nRGB2[CHANGE_COLORS];

} FilterProcData;


extern PIXELPROC PixelProc[EN_MODE_LAST];
extern FilterProcData PixelProcData;
extern PIXELContext PixelContext;


extern void InitFilterPixelProcs();
