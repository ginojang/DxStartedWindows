#include "EXdefault.h"
#include "EFCfs.h"


#include "EFCmem.h"
#include "EFCknl.h"

#include "Inflater3.h"
#include "ResourceManager.h"

#define CRC_XOR 0x97001326


void EFC_fsInitialize( void )
{
}

void EFC_fsFinalize( void )
{
}

sint32 EFC_fsSIZE( schar *pszFile )
{
	MC_FileInfo fa;
	sint32 nRET;

	MEMSET( &fa, 0, sizeof(MC_FileInfo) );

	nRET = MC_fsFileAttribute( pszFile, &fa, MC_DIR_PRIVATE_ACCESS );
	if( nRET < 0 ) {
		return -1;
	}

	return (sint32)fa.size;
}

ubool EFC_fsIsSPACE( ulong nSize )
{
	ulong nAVAIL;
#ifdef AEE_SIMULATOR
	nAVAIL = 0x7FFFFFFF;
#else
	nAVAIL = MC_fsAvailable();
#endif
	if( nAVAIL < nSize ) {
		return FALSE;
	}

	return TRUE;
}

sint32 EFC_fsREAD( sint32 nFD, ubyte *pByte, sint32 nSize )
{
	return MC_fsRead( nFD, pByte, nSize );
}

sint32 EFC_fsWRITE( sint32 nFD, ubyte *pByte, sint32 nSize )
{
	return MC_fsWrite( nFD, pByte, nSize );
}

LPEXBUFF EFC_fsLoadINFLATE( LPEXBUFF pAlloc )
{
	LPEXBUFF pUnZip;
	EXFILE xFile;
	sint32 nZip, nSize;

	MEMSET( &xFile, 0, sizeof(EXFILE) );
	xFile.pBuff = pAlloc;

	EFC_fsSetPOS( &xFile, 8 );
	nZip = EFC_fsReadUint16( &xFile );

	//LOGI("NZIP: %d", nZip);

	if( nZip == 0x00009C78 ) {
		EFC_fsSetPOS( &xFile, 4 ); // 압축된 파일 사이즈
		nSize = EFC_fsReadSint32( &xFile ); // 풀었을 때 파일 사이즈

		pUnZip = EFC_memALLOC( nSize );
		Inflater_inflate( pAlloc, pUnZip );
		EFC_memFREE( pAlloc );

		return pUnZip;
	}

	return pAlloc;
}

sint32 EFC_fsCREATE( schar *pszFile, uint32 nSize )
{
	LPEXBUFF pALLOC;
	sint32 nFD, nPROC;

	nFD = MC_fsOpen( pszFile, MC_FILE_OPEN_RDONLY, MC_DIR_PRIVATE_ACCESS );
	switch( nFD ) {
	case M_E_NOENT : // MC_FILE_OPEN_RDONLY 로 열 경우 파일이 없음
		if( EFC_fsIsSPACE( nSize ) == FALSE ) {
			return M_E_NOSPACE;
		}

		nFD = MC_fsOpen( pszFile, MC_FILE_OPEN_WRONLY, MC_DIR_PRIVATE_ACCESS );
		if( nFD < 0 ) {
			return nFD;
		}

		// 메모리 할당
		pALLOC = EFC_memALLOC( nSize );
		if( pALLOC == NULL ) {
			nPROC = M_E_ERROR;
		} else {
			nPROC = EFC_fsWRITE( nFD, pALLOC->pByte, pALLOC->nSize );
			EFC_memFREE( pALLOC );
		}

		MC_fsClose( nFD );
		// 파일을 nProc 사이즈만큼 생성하였습니다
		return nPROC;
	case M_E_ERROR : // 기타 이유로 실패
	case M_E_BADFILENAME : // 파일이름 형식이 잘못됨
	case M_E_LONGNAME : // 파일이름의 길이가 최대 길이를 초과할 경우
	case M_E_INVALID : // flag 가 잘못됨
	case M_E_NOSPACE : // 파일 시스템의 남은 공간이 없음
	case M_E_ACCESS : // 파일을 접근할 수 없음
		return nFD;
	default :
		MC_fsClose( nFD );
		break;
	}

	// 파일이 존재합니다
	return 0;
}
/*
sint32 EFC_fsGetCRC( ubyte *pByte, sint32 nSize )
{
	EXFILE xFile;
	EXBUFF xByte;
	sint32 i, nCRC = 0, nCOUNT;

	MEMSET( &xFile, 0, sizeof(EXFILE) );
	MEMSET( &xByte, 0, sizeof(EXBUFF) );

	xByte.pByte = pByte;
	xByte.nSize = nSize;
	xFile.pBuff = &xByte;

	nCOUNT = (nSize >> 2);
	for( i=(nCOUNT - 1); i>=0; i-- ) {
		nCRC += (EFC_fsReadUint32( &xFile ) & 0x0FFFFFFF);
		nCRC &= 0x0FFFFFFF;
	}

	return (nCRC ^ CRC_XOR);
}
*/
void EFC_fsSECURITY( ubyte *pByte, sint32 nSize )
{
	uint32 i, nCNT32 = 0, nCNT16 = 0, nCNT8 = 0;
	uint32 *pDATA32;
	uint16 *pDATA16;
	uint8 *pDATA8;

	nCNT32 = nSize / 4;
	nCNT16 = (nSize - (nCNT32 * 4)) / 2;
	nCNT8  = (nSize - (nCNT32 * 4)) % 2;

	pDATA32 = (uint32 *)pByte;
	pDATA16 = (uint16 *)&pByte[(nCNT32 * 4)];
	pDATA8 = (uint8 *)&pByte[(nCNT32 * 4) + (nCNT16 * 2)];

	for( i=0; i<nCNT32; i++ ) {
		pDATA32[i] = (pDATA32[i] ^ CRC_XOR);
	}

	for( i=0; i<nCNT16; i++ ) {
		pDATA16[i] = (uint16)(pDATA16[i] ^ CRC_XOR);
	}

	for( i=0; i<nCNT8; i++ ) {
		pDATA8[i] = (uint8)(pDATA8[i] ^ CRC_XOR);
	}
}

ubool EFC_fsLoadFILE( schar *pszFile, ubyte *pByte, sint32 nSize, ubool bCRC )
{
	EXBUFF xByte;
	sint32 nFD, nCRC = 0;

	nFD = MC_fsOpen( pszFile, MC_FILE_OPEN_RDONLY, MC_DIR_PRIVATE_ACCESS );
	if( nFD < 0 ) {
		return FALSE;
	}

	EFC_fsREAD( nFD, pByte, nSize );
	if( bCRC == TRUE ) {
		EFC_fsREAD( nFD, (ubyte *)&nCRC, 4 );
	}

	MC_fsClose( nFD );

	if( bCRC == TRUE ) {
		EFC_fsSECURITY( pByte, nSize );

		MEMSET( &xByte, 0, sizeof(xByte) );
		xByte.pByte = pByte;

		if( nCRC != EFC_knlGetCRC( &xByte, 0, nSize ) ) {
			MEMSET( pByte, 0, nSize );
			return FALSE;
		}
	}

	return TRUE;
}

ubool EFC_fsSaveFILE( schar *pszFile, ubyte *pByte, sint32 nSize, ubool bCRC )
{
	EXBUFF xByte;
	sint32 nFD, nCRC = 0;

	if( bCRC == TRUE ) {
		MEMSET( &xByte, 0, sizeof(xByte) );
		xByte.pByte = pByte;

		nCRC = EFC_knlGetCRC( &xByte, 0, nSize );
		EFC_fsSECURITY( pByte, nSize );
	}

	// 파일 저장
	nFD = MC_fsOpen( pszFile, MC_FILE_OPEN_RDWR, MC_DIR_PRIVATE_ACCESS );
	if( nFD < 0 ) {
		return FALSE;
	}

	EFC_fsWRITE( nFD, pByte, nSize );
	if( bCRC == TRUE ) {
		EFC_fsWRITE( nFD, (ubyte *)&nCRC, 4 );
	}

	MC_fsClose( nFD );

	return TRUE;
}

sint32 EFC_fsRemove(schar* pszFile, sint32 aMode)
{
	return MC_fsRemove((char*)pszFile, aMode);
}

LPEXBUFF EFC_fsLoadBUFF( schar *pszFile )
{
	LPEXBUFF pALLOC;
	sint32 nSIZE;

	nSIZE = EFC_fsSIZE( pszFile );
	if( nSIZE < 0 ) {
		return NULL;
	}

	// 파일 사이즈만큼 메모리 할당
	pALLOC = EFC_memALLOC( nSIZE );
	if( pALLOC == NULL ) {
		return NULL;
	}

	if( EFC_fsLoadFILE( pszFile, pALLOC->pByte, pALLOC->nSize, FALSE ) == FALSE ) {
		EFC_memFREE( pALLOC );
		return NULL;
	}

	return pALLOC;
}


// GINO CHECK.
/*
LPEXBUFF EFC_fsLoadRES( schar *pszRes )
{
	LPEXBUFF pZIPFile;
	sint32 nSIZE = 0, nRET;
	schar szRESFileName[1024];
	
	SPRINTF( szRESFileName, "%s", pszRes );

	// GINO CHECK..  다행히 이 함수에서만 사용된다..
	nSIZE = g_MXResMan.GetResourceSize( szRESFileName );
	//nResID = MC_knlGetResourceID( szRES, &nSIZE );

	if( nSIZE <= 0 ) 
	{
		return NULL;
	}
	
	pZIPFile = EFC_memALLOC( nSIZE );
	if( pZIPFile == NULL )
	{
		//assert_msg(pALLOC, "EFC_fsLoadRES : 메모리 부족", nSIZE);
		return NULL;
	}

	nRET = g_MXResMan.GetResource( szRESFileName, pZIPFile );

	if(nRET <= 0)
	{
		//LOGE("");
		return NULL;
	}
	return EFC_fsLoadINFLATE(pZIPFile);
}
*/

//////////////////////////////////////////////////////////
// STREAM
ubool EFC_fsIsAVAIL( LPEXFILE pFile, sint32 nBytes )
{
	if( pFile == NULL ) {
		return FALSE;
	}

	if( EFC_memGET( pFile->pBuff ) == NULL ) {
		return FALSE;
	}

	if( pFile->pBuff->nSize < (pFile->nPos + nBytes) ) {
		return FALSE;
	}

	return TRUE;
}

void EFC_fsSetPOS( LPEXFILE pFile, sint32 nPos )
{
	if( pFile == NULL ) {
		return;
	}

	pFile->nPos = nPos;
}

void EFC_fsSKIP( LPEXFILE pFile, sint32 nSkip )
{
	if( pFile == NULL ) {
		return;
	}

	pFile->nPos += nSkip;
}

sint8 EFC_fsReadSint8( LPEXFILE pFile )
{
	sint8 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 1 ) == FALSE) ) {
		return 0;
	}

	nRet = (sint8)pFile->pBuff->pByte[pFile->nPos++];
	return nRet;
}

uint8 EFC_fsReadUint8( LPEXFILE pFile )
{
	uint8 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 1 ) == FALSE) ) {
		return 0;
	}

	nRet = (uint8)pFile->pBuff->pByte[pFile->nPos++];
	return nRet;
}

sint16 EFC_fsReadSint16( LPEXFILE pFile )
{
	sint16 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 2 ) == FALSE) ) {
		return 0;
	}

	nRet |= (sint16)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((sint16)pFile->pBuff->pByte[pFile->nPos++] << 8);
	return nRet;
}

uint16 EFC_fsReadUint16( LPEXFILE pFile )
{
	uint16 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 2 ) == FALSE) ) {
		return 0;
	}

	nRet |= (uint16)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((uint16)pFile->pBuff->pByte[pFile->nPos++] << 8);
	return nRet;
}

sint32 EFC_fsReadSint32( LPEXFILE pFile )
{
	sint32 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 4 ) == FALSE) ) {
		return 0;
	}

	nRet |= (sint32)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 24);
	return nRet;
}

sint64 EFC_fsReadSint64( LPEXFILE pFile )
{
	sint64 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 8 ) == FALSE) ) {
		return 0;
	}

	nRet |= (sint64)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((sint64)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((sint64)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((sint64)pFile->pBuff->pByte[pFile->nPos++] << 24);
	nRet |= ((sint64)pFile->pBuff->pByte[pFile->nPos++] << 32);
	nRet |= ((sint64)pFile->pBuff->pByte[pFile->nPos++] << 40);
	nRet |= ((sint64)pFile->pBuff->pByte[pFile->nPos++] << 48);
	nRet |= ((sint64)pFile->pBuff->pByte[pFile->nPos++] << 56);
	return nRet;
}

uint32 EFC_fsReadUint32( LPEXFILE pFile )
{
	uint32 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 4 ) == FALSE) ) {
		return 0;
	}

	nRet |= (uint32)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 24);
	return nRet;
}


uint64 EFC_fsReadUint64( LPEXFILE pFile )
{
	uint64 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 8 ) == FALSE) ) {
		return 0;
	}

	nRet |= (uint64)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((uint64)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((uint64)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((uint64)pFile->pBuff->pByte[pFile->nPos++] << 24);
	nRet |= ((uint64)pFile->pBuff->pByte[pFile->nPos++] << 32);
	nRet |= ((uint64)pFile->pBuff->pByte[pFile->nPos++] << 40);
	nRet |= ((uint64)pFile->pBuff->pByte[pFile->nPos++] << 48);
	nRet |= ((uint64)pFile->pBuff->pByte[pFile->nPos++] << 56);

	return nRet;
}

sint16 EFC_fsReadSint16J( LPEXFILE pFile )
{
	sint16 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 2 ) == FALSE) ) {
		return 0;
	}

	nRet |= ((sint16)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((sint16)pFile->pBuff->pByte[pFile->nPos++] << 0);
	return nRet;
}

uint16 EFC_fsReadUint16J( LPEXFILE pFile )
{
	uint16 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 2 ) == FALSE) ) {
		return 0;
	}

	nRet |= ((uint16)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((uint16)pFile->pBuff->pByte[pFile->nPos++] << 0);
	return nRet;
}

sint32 EFC_fsReadSint32J( LPEXFILE pFile )
{
	sint32 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 4 ) == FALSE) ) {
		return 0;
	}

	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 24);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 0);
	return nRet;
}

uint32 EFC_fsReadUint32J( LPEXFILE pFile )
{
	uint32 nRet = 0;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 4 ) == FALSE) ) {
		return 0;
	}

	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 24);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 0);
	return nRet;
}

void EFC_fsReadStringJ( LPEXFILE pFile, schar *pDest )
{
	sint32 nSize;
	nSize = EFC_fsReadSint32J( pFile );
	EFC_fsReadBuff( pFile, (ubyte *)pDest, nSize );
}

sint32 EFC_fsReadBuff( LPEXFILE pFile, ubyte *pDest, sint32 nSize )
{
	MEMSET( pDest, 0, nSize );

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, nSize ) == FALSE) ) {
		return M_E_ERROR;
	}

	MEMCPY( pDest, (ubyte *)&pFile->pBuff->pByte[pFile->nPos], nSize );
	pFile->nPos += nSize;

	return nSize;
}

LPEXBUFF EFC_fsReadMallocBuff( LPEXFILE pFile, sint32 nSize )
{
	LPEXBUFF pALLOC;

	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, nSize ) == FALSE) ) {
		return NULL;
	}

	pALLOC = EFC_memALLOC( nSize );
	if( pALLOC == NULL ) {
		return NULL;
	}

	MEMCPY( pALLOC->pByte, (ubyte *)&pFile->pBuff->pByte[pFile->nPos], nSize );
	pFile->nPos += nSize;

	return pALLOC;
}

sint32 EFC_fsReadData( LPEXFILE pFile, LPEXFILE pSRC, sint32 nSize )
{
	if( (pFile == NULL) || (pSRC == NULL) || (EFC_fsIsAVAIL( pFile, nSize ) == FALSE) || (EFC_fsIsAVAIL( pSRC, nSize ) == FALSE) ) {
		return M_E_ERROR;
	}

	if( (EFC_memGET( pFile->pBuff ) == NULL) || (EFC_memGET( pSRC->pBuff ) == NULL) ) {
		return M_E_ERROR;
	}

	MEMCPY( &pSRC->pBuff->pByte[pSRC->nPos], &pFile->pBuff->pByte[pFile->nPos], nSize );

	pSRC->nPos += nSize;
	pFile->nPos += nSize;

	return nSize;
}

void EFC_fsWriteUint8( LPEXFILE pFile, uint8 nNumber )
{
	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 1 ) == FALSE) ) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)nNumber;
}

void EFC_fsWriteUint16( LPEXFILE pFile, uint16 nNumber )
{
	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 2 ) == FALSE) ) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 0) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 8) & 0xFF);
}

void EFC_fsWriteUint32( LPEXFILE pFile, uint32 nNumber )
{
	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 4 ) == FALSE) ) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 0) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 8) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 16) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 24) & 0xFF);
}

void EFC_fsWriteUint64( LPEXFILE pFile, uint64 nNumber )
{
	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 8 ) == FALSE) ) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 0) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 8) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 16) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 24) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 32) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 40) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 48) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 56) & 0xFF);
}

void EFC_fsWriteUint16J( LPEXFILE pFile, uint16 nNumber )
{
	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 2 ) == FALSE) ) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 8) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 0) & 0xFF);
}

void EFC_fsWriteUint32J( LPEXFILE pFile, uint32 nNumber )
{
	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, 4 ) == FALSE) ) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 24) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 16) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 8) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 0) & 0xFF);
}

void EFC_fsWriteBuff( LPEXFILE pFile, ubyte *pDest, sint32 nSize )
{
	if( (pFile == NULL) || (EFC_fsIsAVAIL( pFile, nSize ) == FALSE) ) {
		return;
	}

	MEMCPY( (ubyte *)&pFile->pBuff->pByte[pFile->nPos], pDest, nSize );
	pFile->nPos += nSize;
}

void EFC_fsWriteStringJ( LPEXFILE pFile, schar *pDest )
{
	sint32 nSize;
	nSize = STRLEN( pDest );
	EFC_fsWriteUint32J( pFile, (uint32)nSize );
	EFC_fsWriteBuff( pFile, (ubyte *)pDest, nSize );
}


LPEXBUFF EFC_fsLoadPKG( schar *pszPkg, schar *pszRes )
{
	schar szRES[64];	
	EXFILE xFile;

	MEMSET( &xFile, 0, sizeof(EXFILE) );

	SPRINTF( szRES, "%s", pszPkg );

	xFile.pBuff = g_resourceManager.LoadFromAssets( szRES );

	while (xFile.nPos < xFile.pBuff->nSize)
	{
		schar szName[32] = {0, };
		uint32 nLen = 0;

		MEMCPY(szName, xFile.pBuff->pByte + xFile.nPos, 32);

		FILE_SKIP( &xFile, 32 );

		nLen = READ_UINT32( &xFile );

		if(STRCMP(szName , pszRes) == 0)
		{
			LPEXBUFF pResALLOC = EFC_memALLOC( nLen );

			if( pResALLOC == NULL )
			{
				//assert_msg(pResALLOC, "EFC_fsLoadPKG : 메모리 부족", nLen);
				return NULL;
			}

			EFC_memGET(xFile.pBuff);
			MEMCPY(pResALLOC->pByte, xFile.pBuff->pByte+xFile.nPos, nLen);
 
			EFC_memFREE( xFile.pBuff );

			return EFC_fsLoadINFLATE(pResALLOC);
		}

		FILE_SKIP( &xFile, nLen );
	}

	//assert_msg(0, "EFC_fsLoadPKG : 파일 찾기 실패 \n%s", pszRes);
	return NULL;
}

LPEXBUFF EFC_fsSerchPKG( LPEXBUFF pPkg, schar *pszRes )
{
	schar szRES[64];	
	EXFILE xFile;

	MEMSET( &xFile, 0, sizeof(EXFILE) );

	xFile.pBuff = pPkg;

	EFC_memGET(xFile.pBuff);

	while (xFile.nPos < xFile.pBuff->nSize)
	{
		schar szName[32] = {0, };
		uint32 nLen = 0;

		MEMCPY(szName, xFile.pBuff->pByte + xFile.nPos, 32);

		FILE_SKIP( &xFile, 32 );

		nLen = READ_UINT32( &xFile );

		if(STRCMP(szName , pszRes) == 0)
		{
			LPEXBUFF pResALLOC = EFC_memALLOC( nLen );

			if( pResALLOC == NULL )
			{
				//assert_msg(pResALLOC, "EFC_fsSerchPKG : 메모리 부족", nLen);
				return NULL;
			}

			EFC_memGET(xFile.pBuff);
			MEMCPY(pResALLOC->pByte, xFile.pBuff->pByte+xFile.nPos, nLen);

			return EFC_fsLoadINFLATE(pResALLOC);
		}

		FILE_SKIP( &xFile, nLen );
	}

	//assert_msg(0, "EFC_fsSerchPKG : 파일 찾기 실패 \n%s", pszRes);
	return NULL;
}
///////////////////////////////////////////////////////////////////////////
#if defined( DOWNLOAD_RES )
void EFC_fsLoadLIST( void )
{
	FileData.pLIST = EFC_fsLoadBUFF( "res.lst" );
}

void EFC_fsFreeLIST( void )
{
	EFC_memFREE( FileData.pLIST );
	FileData.pLIST = NULL;
}

sint32 EFC_fsGetLIST( schar *pszFile )
{
	schar *pPrev, *pNext;

	if( STRLEN( pszFile ) < 1 ) {
		return -1;
	}

	if( EFC_memGET( FileData.pLIST ) == NULL ) {
		return -1;
	}

	SPRINTF( FileData.szFile, "%s:", pszFile );

	pPrev = STRSTR( (schar *)FileData.pLIST->pByte, FileData.szFile );
	if( pPrev == NULL ) {
		return -1;
	}

	pPrev += STRLEN( FileData.szFile );
	pNext = STRSTR( pPrev, "\n" );
	if( pNext == NULL ) {
		return -1;
	}

	MEMSET( FileData.szPos, 0, sizeof(FileData.szPos) );
	MEMCPY( FileData.szPos, pPrev, (pNext - pPrev) );

	return (sint32)ATOI( FileData.szPos );
}

ubool EFC_fsLoadDATA( void )
{
	FileData.nFD = MC_fsOpen( "res.fdt", MC_FILE_OPEN_RDONLY, MC_DIR_PRIVATE_ACCESS );
	if( FileData.nFD < 0 ) {
		return FALSE;
	}

	return TRUE;
}

void EFC_fsFreeDATA( void )
{
	if( FileData.nFD >= 0 ) {
		MC_fsClose( FileData.nFD );
		FileData.nFD = -1;
	}
}

LPEXBUFF EFC_fsGetDATA( schar *pszFile )
{
	LPEXBUFF pALLOC = NULL;
	sint32 nPOS, nPROC, nSIZE = 0;

	if( FileData.nFD < 0 ) {
		return NULL;
	}

	nPOS = EFC_fsGetLIST( pszFile );
	if( nPOS < 0 ) {
		return NULL;
	}

	nPROC = MC_fsSeek( FileData.nFD, nPOS, MC_FILE_SEEK_SET );
	if( nPROC < 0 ) {
		return NULL;
	}

	if( EFC_fsREAD( FileData.nFD, (ubyte *)&nSIZE, 4 ) != 4 ) {
		return NULL;
	}

	pALLOC = EFC_memALLOC( nSIZE );
	if( pALLOC == NULL ) {
		return NULL;
	}

	if( EFC_fsREAD( FileData.nFD, pALLOC->pByte, pALLOC->nSize ) != pALLOC->nSize ) {
		EFC_memFREE( pALLOC );
		return NULL;
	}

	return EFC_fsLoadINFLATE( pALLOC );
}
#endif
