#ifndef __EXDEFAULT_HEADER__
#define __EXDEFAULT_HEADER__



#define	SKT_ANDROID
#define	USE_TOUCH_PROC
#define LAUNCH_VERSION
#define	RELEASE
//#define	BETA_TEST


#include "wipiproc.h"
#include "wipiexchange.h"

#include "MCtypes.h"
#include "MCerror.h"
#include "Extype.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
/*
#include "../StdAfx.h"
#include "external_src.h"
*/


#if defined(SKT_WIPI)
#ifndef MV_WM_FOCUS_EVENT
#define MV_WM_FOCUS_EVENT			0x4008
#endif

#ifndef MV_WM_LOSTFOCUS_EVENT
#define MV_WM_LOSTFOCUS_EVENT		0x4009
#endif
#elif defined(LGT_WIPI)
#ifndef M_E_SUCCESS
#define M_E_SUCCESS	0
#endif
#endif
#define 	EX_knlPrintk(...) __android_log_print(ANDROID_LOG_VERBOSE, "wook", __VA_ARGS__);
#define		MC_knlCalloc	MC_knlAlloc
#define		DBG(f,s)		__android_log_print(ANDROID_LOG_DEBUG, "wook",f,s)
#endif
