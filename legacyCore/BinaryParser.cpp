#include "LegacyCore.h"
#include "BinaryParser.h"


//
// LPEXFILE -> LPEXBUFF로 수정 할것.

ubool EFC_fsIsAVAIL(LPEXFILE pFile, sint32 nBytes)
{
	if (pFile == NULL) {
		return FALSE;
	}

	if (EFC_memGET(pFile->pBuff) == NULL) {
		return FALSE;
	}

	if (pFile->pBuff->nSize < (pFile->nPos + nBytes)) {
		return FALSE;
	}

	return TRUE;
}

void EFC_fsSetPOS(LPEXFILE pFile, sint32 nPos)
{
	if (pFile == NULL) {
		return;
	}

	pFile->nPos = nPos;
}

void EFC_fsSKIP(LPEXFILE pFile, sint32 nSkip)
{
	if (pFile == NULL) {
		return;
	}

	pFile->nPos += nSkip;
}

sint8 EFC_fsReadSint8(LPEXFILE pFile)
{
	sint8 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 1) == FALSE)) {
		return 0;
	}

	nRet = (sint8)pFile->pBuff->pByte[pFile->nPos++];
	return nRet;
}

uint8 EFC_fsReadUint8(LPEXFILE pFile)
{
	uint8 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 1) == FALSE)) {
		return 0;
	}

	nRet = (uint8)pFile->pBuff->pByte[pFile->nPos++];
	return nRet;
}

sint16 EFC_fsReadSint16(LPEXFILE pFile)
{
	sint16 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 2) == FALSE)) {
		return 0;
	}

	nRet |= (sint16)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((sint16)pFile->pBuff->pByte[pFile->nPos++] << 8);
	return nRet;
}

uint16 EFC_fsReadUint16(LPEXFILE pFile)
{
	uint16 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 2) == FALSE)) {
		return 0;
	}

	nRet |= (uint16)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((uint16)pFile->pBuff->pByte[pFile->nPos++] << 8);
	return nRet;
}

sint32 EFC_fsReadSint32(LPEXFILE pFile)
{
	sint32 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 4) == FALSE)) {
		return 0;
	}

	nRet |= (sint32)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 24);
	return nRet;
}

sint64 EFC_fsReadSint64(LPEXFILE pFile)
{
	sint64 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 8) == FALSE)) {
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

uint32 EFC_fsReadUint32(LPEXFILE pFile)
{
	uint32 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 4) == FALSE)) {
		return 0;
	}

	nRet |= (uint32)pFile->pBuff->pByte[pFile->nPos++];
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 24);
	return nRet;
}


uint64 EFC_fsReadUint64(LPEXFILE pFile)
{
	uint64 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 8) == FALSE)) {
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

sint16 EFC_fsReadSint16J(LPEXFILE pFile)
{
	sint16 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 2) == FALSE)) {
		return 0;
	}

	nRet |= ((sint16)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((sint16)pFile->pBuff->pByte[pFile->nPos++] << 0);
	return nRet;
}

uint16 EFC_fsReadUint16J(LPEXFILE pFile)
{
	uint16 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 2) == FALSE)) {
		return 0;
	}

	nRet |= ((uint16)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((uint16)pFile->pBuff->pByte[pFile->nPos++] << 0);
	return nRet;
}

sint32 EFC_fsReadSint32J(LPEXFILE pFile)
{
	sint32 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 4) == FALSE)) {
		return 0;
	}

	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 24);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((sint32)pFile->pBuff->pByte[pFile->nPos++] << 0);
	return nRet;
}

uint32 EFC_fsReadUint32J(LPEXFILE pFile)
{
	uint32 nRet = 0;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 4) == FALSE)) {
		return 0;
	}

	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 24);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 16);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 8);
	nRet |= ((uint32)pFile->pBuff->pByte[pFile->nPos++] << 0);
	return nRet;
}

void EFC_fsReadStringJ(LPEXFILE pFile, schar* pDest)
{
	sint32 nSize;
	nSize = EFC_fsReadSint32J(pFile);
	EFC_fsReadBuff(pFile, (ubyte*)pDest, nSize);
}

sint32 EFC_fsReadBuff(LPEXFILE pFile, ubyte* pDest, sint32 nSize)
{
	MEMSET(pDest, 0, nSize);

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, nSize) == FALSE)) {
		return -1;
	}

	MEMCPY(pDest, (ubyte*)&pFile->pBuff->pByte[pFile->nPos], nSize);
	pFile->nPos += nSize;

	return nSize;
}

LPEXBUFF EFC_fsReadMallocBuff(LPEXFILE pFile, sint32 nSize)
{
	LPEXBUFF pALLOC;

	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, nSize) == FALSE)) {
		return NULL;
	}

	pALLOC = EFC_memALLOC(nSize);
	if (pALLOC == NULL) {
		return NULL;
	}

	MEMCPY(pALLOC->pByte, (ubyte*)&pFile->pBuff->pByte[pFile->nPos], nSize);
	pFile->nPos += nSize;

	return pALLOC;
}

sint32 EFC_fsReadData(LPEXFILE pFile, LPEXFILE pSRC, sint32 nSize)
{
	if ((pFile == NULL) || (pSRC == NULL) || (EFC_fsIsAVAIL(pFile, nSize) == FALSE) || (EFC_fsIsAVAIL(pSRC, nSize) == FALSE)) {
		return -1;
	}

	if ((EFC_memGET(pFile->pBuff) == NULL) || (EFC_memGET(pSRC->pBuff) == NULL)) {
		return -1;
	}

	MEMCPY(&pSRC->pBuff->pByte[pSRC->nPos], &pFile->pBuff->pByte[pFile->nPos], nSize);

	pSRC->nPos += nSize;
	pFile->nPos += nSize;

	return nSize;
}

void EFC_fsWriteUint8(LPEXFILE pFile, uint8 nNumber)
{
	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 1) == FALSE)) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)nNumber;
}

void EFC_fsWriteUint16(LPEXFILE pFile, uint16 nNumber)
{
	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 2) == FALSE)) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 0) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 8) & 0xFF);
}

void EFC_fsWriteUint32(LPEXFILE pFile, uint32 nNumber)
{
	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 4) == FALSE)) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 0) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 8) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 16) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 24) & 0xFF);
}

void EFC_fsWriteUint64(LPEXFILE pFile, uint64 nNumber)
{
	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 8) == FALSE)) {
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

void EFC_fsWriteUint16J(LPEXFILE pFile, uint16 nNumber)
{
	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 2) == FALSE)) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 8) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 0) & 0xFF);
}

void EFC_fsWriteUint32J(LPEXFILE pFile, uint32 nNumber)
{
	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, 4) == FALSE)) {
		return;
	}

	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 24) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 16) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 8) & 0xFF);
	pFile->pBuff->pByte[pFile->nPos++] = (ubyte)((nNumber >> 0) & 0xFF);
}

void EFC_fsWriteBuff(LPEXFILE pFile, ubyte* pDest, sint32 nSize)
{
	if ((pFile == NULL) || (EFC_fsIsAVAIL(pFile, nSize) == FALSE)) {
		return;
	}

	MEMCPY((ubyte*)&pFile->pBuff->pByte[pFile->nPos], pDest, nSize);
	pFile->nPos += nSize;
}

void EFC_fsWriteStringJ(LPEXFILE pFile, schar* pDest)
{
	sint32 nSize;
	nSize = STRLEN(pDest);
	EFC_fsWriteUint32J(pFile, (uint32)nSize);
	EFC_fsWriteBuff(pFile, (ubyte*)pDest, nSize);
}




#if _CHECK
LPEXBUFF EFC_fsLoadPKG(schar* pszPkg, schar* pszRes)
{
	schar szRES[64];
	EXFILE xFile;

	MEMSET(&xFile, 0, sizeof(EXFILE));

	SPRINTF(szRES, "%s", pszPkg);

	xFile.pBuff = g_resourceManager.LoadFromAssets(szRES);

	while (xFile.nPos < xFile.pBuff->nSize)
	{
		schar szName[32] = { 0, };
		uint32 nLen = 0;

		MEMCPY(szName, xFile.pBuff->pByte + xFile.nPos, 32);

		FILE_SKIP(&xFile, 32);

		nLen = READ_UINT32(&xFile);

		if (STRCMP(szName, pszRes) == 0)
		{
			LPEXBUFF pResALLOC = EFC_memALLOC(nLen);

			if (pResALLOC == NULL)
			{
				//assert_msg(pResALLOC, "EFC_fsLoadPKG : 메모리 부족", nLen);
				return NULL;
			}

			EFC_memGET(xFile.pBuff);
			MEMCPY(pResALLOC->pByte, xFile.pBuff->pByte + xFile.nPos, nLen);

			EFC_memFREE(xFile.pBuff);

			return EFC_fsLoadINFLATE(pResALLOC);
		}

		FILE_SKIP(&xFile, nLen);
	}

	//assert_msg(0, "EFC_fsLoadPKG : 파일 찾기 실패 \n%s", pszRes);
	return NULL;
}

LPEXBUFF EFC_fsSerchPKG(LPEXBUFF pPkg, schar* pszRes)
{
	schar szRES[64];
	EXFILE xFile;

	MEMSET(&xFile, 0, sizeof(EXFILE));

	xFile.pBuff = pPkg;

	EFC_memGET(xFile.pBuff);

	while (xFile.nPos < xFile.pBuff->nSize)
	{
		schar szName[32] = { 0, };
		uint32 nLen = 0;

		MEMCPY(szName, xFile.pBuff->pByte + xFile.nPos, 32);

		FILE_SKIP(&xFile, 32);

		nLen = READ_UINT32(&xFile);

		if (STRCMP(szName, pszRes) == 0)
		{
			LPEXBUFF pResALLOC = EFC_memALLOC(nLen);

			if (pResALLOC == NULL)
			{
				//assert_msg(pResALLOC, "EFC_fsSerchPKG : 메모리 부족", nLen);
				return NULL;
			}

			EFC_memGET(xFile.pBuff);
			MEMCPY(pResALLOC->pByte, xFile.pBuff->pByte + xFile.nPos, nLen);

			return EFC_fsLoadINFLATE(pResALLOC);
		}

		FILE_SKIP(&xFile, nLen);
	}

	//assert_msg(0, "EFC_fsSerchPKG : 파일 찾기 실패 \n%s", pszRes);
	return NULL;
}

#endif