#ifndef __MCTYPES_H_
#define __MCTYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/* WIPI Specification */

typedef		unsigned	char			M_Boolean;
typedef		unsigned	long			M_Uint32;	
typedef		unsigned	short			M_Uint16;
typedef		unsigned	char			M_Uint8;

typedef		signed		long			M_Int32;	
typedef		signed		short			M_Int16;
typedef		signed		char			M_Int8;

typedef		char						M_Char;		
typedef		unsigned	char			M_Byte; 

#if defined(_VCDLL) || defined(_WIN32_WCE) || defined(__WINS__)
typedef		signed		__int64			M_Int64; 
typedef		unsigned	__int64			M_Uint64; 
#else
typedef		signed		long long		M_Int64; 
typedef		unsigned	long long		M_Uint64; 
#endif

typedef		unsigned	short			M_UCode;
typedef		signed		long			M_Sint32;   
typedef		signed		short			M_Sint15;

typedef		M_Uint32					M_Addr;		

#ifdef WUNICODE
typedef		M_UCode						M_TChar;	
#else
typedef		M_Char						M_TChar;	
#endif

#ifdef 		NULL
#undef		NULL
#endif

#ifdef		TRUE
#undef		TRUE
#endif

#ifdef		FALSE
#undef		FALSE
#endif

#define 	NULL		0
#define 	TRUE		1
#define 	FALSE		0

#define		inline		__inline					

/* OEM-Specfic Definition */

typedef					M_Uint64		ulong64;

#if defined(_VCDLL) || defined(_WIN32_WCE) || defined(__WINS__)
typedef		signed		__int64			long64;
#else
typedef		signed		long long		long64;
#endif

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* __MCTYPES_H_ */
