#include "legacyCore.h"
#include "ResourceManager.h"
#include "BinaryParser.h"
#include "ABMReader.h"

ResourceManager g_resourceManager;


ResourceManager::ResourceManager()
{
	int i;
	counterBuffer = 10;
	for (i = 0; i < EXBUFFERS_NUM; i++)
		exBuffers->nBuffID = 0;

	for (i = 0; i < MAX_ASSET_FOLDERS; i++)
		AssetFolders[i][0] = NULL;

	numFolders = 0;

#ifdef NATIVE_STAND_ALONE
	strcpy(AssetFolders[numFolders++], "../Assets/");
	strcpy(AssetFolders[numFolders++], "../Assets/Font/");

#endif
}

ResourceManager::~ResourceManager()
{

}


//////////////////////////////////////////////////////////////////////////
///  파일 부분
///
LPEXBUFF ResourceManager::EFC_fsLoadINFLATE(LPEXBUFF pAlloc)
{
#ifdef NATIVE_STAND_ALONE
	LPEXBUFF pUnZip;
	EXFILE xFile;
	sint32 nZip, nSize;

	MEMSET(&xFile, 0, sizeof(EXFILE));
	xFile.pBuff = pAlloc;

	EFC_fsSetPOS(&xFile, 8);
	nZip = EFC_fsReadUint16(&xFile);
	
	if (nZip == 0x00009C78) {
		EFC_fsSetPOS(&xFile, 4); // 압축된 파일 사이즈
		nSize = EFC_fsReadSint32(&xFile); // 풀었을 때 파일 사이즈

		pUnZip = EFC_memALLOC(nSize);
		Inflater_inflate(pAlloc, pUnZip);
		EFC_memFREE(pAlloc);

		return pUnZip;
	}

	return pAlloc;
#else

	return NULL;

#endif
}

// public
EXBUFF* ResourceManager::LoadFromAssets(const char* pszName)
{
#ifdef NATIVE_STAND_ALONE
	for (int i = 0; i < numFolders; i++)
	{
		char* dirName = AssetFolders[i];
		char fileName[512];
		sprintf(fileName, "%s%s", dirName, pszName);

		//char dir[512];
		//GetCurrentDirectoryA(512, dir);

		FILE* fp = fopen(fileName, "rb");
		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			int nSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			if (nSize > 0)
			{
				LPEXBUFF pBuf = EFC_memALLOC(nSize);
				fread(pBuf->pByte, 1, nSize, fp);
				fclose(fp);
				return EFC_fsLoadINFLATE(pBuf);
			}
			else
				fclose(fp);
		}
	}
	return NULL;
#else


	return NULL;
#endif
}

//////////////////////////////////////////////////////////////////////////
///  버퍼.
///
EXBUFF* ResourceManager::FindBuffer()
{
	int i;
	for(i=0; i< EXBUFFERS_NUM; i++)
		if (exBuffers[i].nBuffID == 0)
		{
			return &exBuffers[i];
		}
	//
	//
	DebugLog(L" >>>>>>>>>>>>>>>  Fatal Error >>  Out of Memory  >>>>>>>>>>>>>>>>>>>");
	return NULL;
}

EXBUFF* ResourceManager::AllocBuffer(int size)
{
	EXBUFF* pBuffer = FindBuffer();

	pBuffer->nBuffID = counterBuffer++;
	pBuffer->pByte = new ubyte[size];
	pBuffer->nSize = size;

	return pBuffer;
}

ubyte* ResourceManager::GetBufferBits(EXBUFF* buffer)
{
	if (buffer == NULL)
		return NULL;
	
	return buffer->pByte;
}

EXBUFF* ResourceManager::AllocWithBuffer(EXBUFF* srcBuffer)
{
	if (srcBuffer == NULL)
		return NULL;

	EXBUFF* newBuffer = AllocBuffer(srcBuffer->nSize);
	MEMCPY(newBuffer->pByte, srcBuffer->pByte, newBuffer->nSize);

	return newBuffer;
}

void ResourceManager::ReleaseBuffer(EXBUFF* buffer)
{
	buffer->nBuffID = 0;
	delete[] buffer->pByte;
}

void ResourceManager::ReleaseAll()
{
	for (int i = 0; i < EXBUFFERS_NUM; i++)
		if (exBuffers[i].nBuffID != 0)
		{
			ReleaseBuffer(&exBuffers[i]);
		}
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 -    
M_Int64 MC_knlCurrentTime()
{
	return 0;
}

LPEXBUFF EFC_fsLoadBUFF(schar* pszFile)
{
	return g_resourceManager.LoadFromAssets(pszFile);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 -  
ubyte* EFC_memGET(LPEXBUFF pBuff)
{
	return g_resourceManager.GetBufferBits(pBuff);
}
ubool EFC_memSET(LPEXBUFF pBuff, sint32 nSize)
{
	pBuff = EFC_memALLOC(nSize);
	return TRUE;
}
LPEXBUFF EFC_memALLOC(sint32 nSize)
{
	return g_resourceManager.AllocBuffer(nSize);
}
LPEXBUFF EFC_memCOPY(LPEXBUFF pBuff)
{
	return g_resourceManager.AllocWithBuffer(pBuff);
}
void EFC_memFREE(LPEXBUFF pBuff)
{
	g_resourceManager.ReleaseBuffer(pBuff);
}
void EFC_fxtCHANGE(LPEXBUFF pBuff, schar szOld, schar szNew)
{
	schar* pChar, * pPrev;
	schar szBuff[2];

	szBuff[0] = szOld;
	szBuff[1] = 0;

	pChar = (schar*)EFC_memGET(pBuff);
	if (pChar == NULL)
	{
		return;
	}

	pPrev = pChar;
	while (pChar != NULL) {
		pChar = (schar*)STRSTR((schar*)pPrev, szBuff);
		if (pChar == NULL) {
			break;
		}

		pChar[0] = szNew;

		pChar++;
		pPrev = pChar;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 -  

void EFC_imgLoadARRAY(LPEXIMAGE pIMGs, schar* pStr, sint32* pIDXs, sint32 nCount, ubool bORDER, ubool bMIRROR)
{
	TBMPREADER xTBmp;
	EXFILE xFile;
	sint32 i, nIDX = 0, nIMG = 0;
	ubool bBREAK = FALSE;

	MEMSET(&xFile, 0, sizeof(xFile));
	xFile.pBuff = g_resourceManager.LoadFromAssets(pStr);
	if (EFC_memGET(xFile.pBuff) == NULL)
	{
		return;
	}

	MEMSET(&xTBmp, 0, sizeof(xTBmp));
	ABMReader_Load(&xFile, &xTBmp);

	for (i = 0; i < nCount; i++) {
		while (nIDX < pIDXs[i]) {
			if (ABMReader_skip(&xFile, &xTBmp) == FALSE) {
				bBREAK = TRUE;
				break;
			}

			nIDX++;
			if (bORDER == FALSE) {
				nIMG++;
			}
		}

		if (bBREAK == TRUE) {
			break;
		}

		EFC_imgFREE(&pIMGs[nIMG]);

		if (bMIRROR == FALSE) {
			if (ABMReader_real(&xFile, &pIMGs[nIMG], &xTBmp) == FALSE) {
				bBREAK = TRUE;
			}
		}
		else {
			if (ABMReader_next(&xFile, &pIMGs[nIMG], &xTBmp) == FALSE) {
				bBREAK = TRUE;
			}
		}

		if (bBREAK == TRUE)
		{
			break;
		}

		pIMGs[nIMG].nMoveX = xTBmp.refX;
		pIMGs[nIMG].nMoveY = xTBmp.refY;
		pIMGs[nIMG].nPalette = 0;

		nIDX++;
		nIMG++;
	}

	ABMReader_Free(&xTBmp);
	EFC_memFREE(xFile.pBuff);
}

void EFC_imgLoadCOUNT(LPEXIMAGE pIMGs, schar* pStr, sint32 nIDX, sint32 nCount)
{
	sint32 nARRAYs[255];
	sint32 i;

	for (i = 0; i < nCount; i++) {
		nARRAYs[i] = nIDX + i;
	}

	EFC_imgLoadARRAY(pIMGs, pStr, nARRAYs, nCount, TRUE, TRUE);
}

void EFC_imgLoadIMAGE(LPEXIMAGE pIMGs, schar* pStr, sint32 nIDX)
{
	sint32 nARRAYs[1];

	nARRAYs[0] = nIDX;

	EFC_imgLoadARRAY(pIMGs, pStr, nARRAYs, 1, TRUE, TRUE);
}

void EFC_imgFREE(LPEXIMAGE pIMG)
{
	EFC_memFREE(pIMG->pBuff);

	MEMSET(pIMG, 0, sizeof(EXIMAGE));
}

void EFC_imgFreeARRAY(LPEXIMAGE pIMGs, sint32 nCount)
{
	sint32 i;

	for (i = nCount - 1; i >= 0; i--) {
		EFC_imgFREE(&pIMGs[i]);
	}

	MEMSET(pIMGs, 0, (sizeof(EXIMAGE) * nCount));
}
/*

sint32 EFC_imgGetMIRROR( sint32 nCurr, sint32 nNext )
{
	sint32 nCALC[4];

	nCALC[0] = (nNext & 0x00000003);
	nCALC[1] = ((5 - nNext) & 0x00000003);
	nCALC[2] = ((nNext + 2) & 0x00000003);
	nCALC[3] = ((3 - nNext) & 0x00000003);

	return nCALC[nCurr];
}
*/

void EFC_imgMIRROR(LPEXIMAGE pIMG, LPEXMOVE rMov, sint32 nMIRROR)
{
	switch (nMIRROR) {
	case EN_MIRROR_NONE:
		rMov->moveX = pIMG->nMoveX;
		rMov->moveY = pIMG->nMoveY;
		break;
	case EN_MIRROR_HORIZON:
		rMov->moveX = (sint16)(pIMG->nW - pIMG->nMoveX);
		rMov->moveY = pIMG->nMoveY;
		break;
	case EN_MIRROR_VERTICAL:
		rMov->moveX = pIMG->nMoveX;
		rMov->moveY = (sint16)(pIMG->nH - pIMG->nMoveY);
		break;
	case EN_MIRROR_BOTH:
		rMov->moveX = (sint16)(pIMG->nW - pIMG->nMoveX);
		rMov->moveY = (sint16)(pIMG->nH - pIMG->nMoveY);
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 -  
#ifdef NATIVE_STAND_ALONE

void EFC_fsSECURITY(ubyte* pByte, sint32 nSize)
{
	int CRC_XOR = 0x97001326;

	uint32 i, nCNT32 = 0, nCNT16 = 0, nCNT8 = 0;
	uint32* pDATA32;
	uint16* pDATA16;
	uint8* pDATA8;

	nCNT32 = nSize / 4;
	nCNT16 = (nSize - (nCNT32 * 4)) / 2;
	nCNT8 = (nSize - (nCNT32 * 4)) % 2;

	pDATA32 = (uint32*)pByte;
	pDATA16 = (uint16*)&pByte[(nCNT32 * 4)];
	pDATA8 = (uint8*)&pByte[(nCNT32 * 4) + (nCNT16 * 2)];

	for (i = 0; i < nCNT32; i++) {
		pDATA32[i] = (pDATA32[i] ^ CRC_XOR);
	}

	for (i = 0; i < nCNT16; i++) {
		pDATA16[i] = (uint16)(pDATA16[i] ^ CRC_XOR);
	}

	for (i = 0; i < nCNT8; i++) {
		pDATA8[i] = (uint8)(pDATA8[i] ^ CRC_XOR);
	}
}


#define MAXBITS 16
EXINFLATER xInfl;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
static const unsigned char order[19] = /* permutation of code lengths */
{ 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
static const unsigned char codeext[6] = /* permutation of code lengths */
{ 2, 3, 3, 3, 7,11 };

static const unsigned short cplens[31] = { // Copy lengths for literal codes 257..285
		3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
		35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0
};
static const unsigned char cplext[31] = { // Extra bits for literal codes 257..285
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
		3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 112, 112  // 112==invalid
};
static const unsigned short cpdist[30] = { // Copy offsets for distance codes 0..29
		1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
		257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
		8193, 12289, 16385, 24577
};
static const unsigned char cpdext[30] = { // Extra bits for distance codes
		0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
		7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
		12, 12, 13, 13 };

static const unsigned char mask[9] = { 0,1,3,7,15,31,63,127,255 };
//////////////////////////////////////////////////////////////////////

void Inflater_bs_move(sint32 n)
{
	xInfl.inOffset += n;
}

sint32 Inflater_bs_read(sint32 n)
{
	ubyte* pData1;
	sint32 bitOffset, valid, middle, rest, retVal;
	sint32 i;

	pData1 = (ubyte*)EFC_memGET(xInfl.pDataIn);
	pData1 += (xInfl.inOffset >> 3);

	bitOffset = (xInfl.inOffset % 8);
	valid = 8 - bitOffset;

	xInfl.inOffset += n;

	if (n <= valid) {
		return (((*pData1) >> bitOffset) & mask[n]);
	}

	retVal = (((*pData1) >> bitOffset) & mask[valid]);
	pData1++;

	n -= valid;
	rest = (n % 8);

	middle = n - rest;
	if (middle > 0) {
		for (i = valid; i < (valid + middle); i += 8, ++pData1) {
			retVal |= (((*pData1) & 0xFF) << i);
		}
	}

	if (rest > 0) {
		retVal |= (((*pData1) & mask[rest]) << (valid + middle));
	}

	return retVal;
}

void Inflater_code(LPEXBUFF pLens, LPEXBUFF pTable, int maxCodeLen)
{
	sint32 j, offset = 0, code;
	uint16 sym;
	ubyte* pData1;
	uint16* pData2;
	sint32 nCount;
	ubyte val;

	pData1 = (ubyte*)EFC_memGET(pLens);
	pData2 = (uint16*)EFC_memGET(pTable);

	while (offset < (pLens->nSize)) {
		code = Inflater_bs_read(maxCodeLen);
		Inflater_bs_move((pData2[(code << 1)] - maxCodeLen));
		sym = pData2[((code << 1) + 1)];

		if (sym < 16) {
			pData1[offset++] = (ubyte)sym;
		}
		else {
			sym -= 16;

			nCount = Inflater_bs_read(codeext[(sym << 1)]) + codeext[((sym << 1) + 1)];
			if (sym == 0) {
				val = pData1[(offset - 1)];
			}
			else {
				val = 0;
			}

			for (j = 0; j < nCount; j++) {
				pData1[offset++] = val;
			}
		}
	}
}

LPEXBUFF Inflater_makeTable(LPEXBUFF pLens, sint32* maxLen)
{
	LPEXBUFF pTable;
	ubyte* pData1;
	uint16* pData2;

	sint32 i, j, min, max, code = 0;
	sint32 arrLen;
	sint32 count[MAXBITS], base[MAXBITS];
	sint32 t, t2, ind, nCount;

	MEMSET(count, 0, sizeof(count));
	MEMSET(base, 0, sizeof(base));

	pData1 = (ubyte*)EFC_memGET(pLens);
	min = pData1[0];
	max = pData1[0];

	for (i = 1; i < (pLens->nSize); i++) {
		if (pData1[i] < min) {
			min = pData1[i];
		}

		if (pData1[i] > max) {
			max = pData1[i];
		}
	}

	(*maxLen) = max;
	arrLen = (max - min + 1);

	for (i = 0; i < (pLens->nSize); i++) {
		count[(pData1[i] - min)]++;
	}

	if (min == 0) {
		count[0] = 0;
	}

	for (i = 1; i < arrLen; i++) {
		code = ((code + count[i - 1]) << 1);
		base[i] = code;
	}

	pTable = EFC_memALLOC(((1 << (max + 1)) * sizeof(short)));
	if (pTable == NULL) {
		return NULL;
	}

	pData1 = EFC_memGET(pLens);
	pData2 = (uint16*)EFC_memGET(pTable);

	for (i = 0; i < (pLens->nSize); i++) {
		if (pData1[i] == 0) {
			continue;
		}

		t = base[pData1[i] - min]++;

		t2 = 0;
		nCount = pData1[i];
		for (j = 0; j < nCount; j++) {
			t2 |= (((t >> j) & 0x1) << (pData1[i] - 1 - j));
		}

		nCount = (1 << (max - pData1[i]));
		for (j = 0; j < nCount; j++) {
			ind = ((j << pData1[i]) + t2);
			pData2[(ind << 1)] = (uint16)pData1[i];
			pData2[((ind << 1) + 1)] = (uint16)i;
		}
	}

	return pTable;
}

void Inflater_Last(LPEXBUFF pTableL, sint32 maxLitLen, LPEXBUFF pTableD, sint32 maxDistLen)
{
	sint32 code, index, length, dist, srcOffset;
	uint16 val;
	uint16* pDataL, * pDataD;
	ubyte* pData1;

	pData1 = (ubyte*)EFC_memGET(xInfl.pDataOut);

	pDataL = (uint16*)EFC_memGET(pTableL);
	pDataD = (uint16*)EFC_memGET(pTableD);

	while (xInfl.outOffset < xInfl.pDataOut->nSize) {
		code = Inflater_bs_read(maxLitLen);
		Inflater_bs_move(pDataL[(code << 1)] - maxLitLen);
		val = pDataL[((code << 1) + 1)];

		if (val == 256) {
			break;
		}
		else if (val < 256) {
			pData1[xInfl.outOffset++] = (ubyte)val;
		}
		else {
			index = val - 256 - 1;
			length = cplens[index] + Inflater_bs_read(cplext[index]);
			code = Inflater_bs_read(maxDistLen);
			Inflater_bs_move(pDataD[(code << 1)] - maxDistLen);
			val = pDataD[((code << 1) + 1)];
			dist = cpdist[val] + Inflater_bs_read(cpdext[val]);

			srcOffset = xInfl.outOffset - dist;
			while (length > 0) {
				if (length < dist) {
					MEMCPY(pData1 + xInfl.outOffset, pData1 + srcOffset, length);
					xInfl.outOffset += length;
					length = 0;
				}
				else {
					MEMCPY(pData1 + xInfl.outOffset, pData1 + srcOffset, dist);
					xInfl.outOffset += dist;
					length -= dist;
				}
			}
		}
	}
}

void Inflater_fixed(void)
{
	LPEXBUFF pTableL, pTableD;
	LPEXBUFF pLens, pDists;
	ubyte* pData1, * pData2;
	sint32 maxLitLen, maxDistLen;

	pLens = EFC_memALLOC(288);
	pData1 = EFC_memGET(pLens);

	MEMSET(pData1, 8, 144);
	MEMSET(pData1 + 144, 9, 112);
	MEMSET(pData1 + 256, 7, 24);
	MEMSET(pData1 + 280, 8, 8);

	pTableL = Inflater_makeTable(pLens, &maxLitLen);
	EFC_memFREE(pLens);

	pDists = EFC_memALLOC(30);
	pData2 = EFC_memGET(pDists);

	MEMSET(pData2, 5, 30);

	pTableD = Inflater_makeTable(pDists, &maxDistLen);
	EFC_memFREE(pDists);

	Inflater_Last(pTableL, maxLitLen, pTableD, maxDistLen);

	EFC_memFREE(pTableL);
	EFC_memFREE(pTableD);
}

void Inflater_dyn(void)
{
	EXBUFF xCodeLens;
	LPEXBUFF pTable, pTableL, pTableD;
	LPEXBUFF pLens, pDists;
	sint32 i, nLit, nDist, nCode, n, maxCodeLen;
	ubyte codeLens[19];
	sint32 maxLitLen, maxDistLen;

	nLit = Inflater_bs_read(5) + 257;
	nDist = Inflater_bs_read(5) + 1;
	nCode = Inflater_bs_read(4) + 4;

	MEMSET(codeLens, 0, sizeof(codeLens));
	for (i = 0; i < nCode; i++) {
		n = Inflater_bs_read(3);
		codeLens[order[i]] = (ubyte)n;
	}

	MEMSET(&xCodeLens, 0, sizeof(EXBUFF));
	xCodeLens.pByte = (ubyte*)codeLens;
	xCodeLens.nSize = 19;
	pTable = Inflater_makeTable(&xCodeLens, &maxCodeLen);

	pLens = EFC_memALLOC(nLit);
	Inflater_code(pLens, pTable, maxCodeLen);
	pTableL = Inflater_makeTable(pLens, &maxLitLen);
	EFC_memFREE(pLens);

	pDists = EFC_memALLOC(nDist);
	Inflater_code(pDists, pTable, maxCodeLen);
	EFC_memFREE(pTable);

	pTableD = Inflater_makeTable(pDists, &maxDistLen);
	EFC_memFREE(pDists);

	Inflater_Last(pTableL, maxLitLen, pTableD, maxDistLen);

	EFC_memFREE(pTableL);
	EFC_memFREE(pTableD);
}

void Inflater_inflate(LPEXBUFF pDataIn, LPEXBUFF pDataOut)
{
	sint32 isLast, comp;

	MEMSET(&xInfl, 0, sizeof(EXINFLATER));

	xInfl.pDataIn = pDataIn;
	xInfl.inOffset = 64;

	xInfl.pDataOut = pDataOut;
	xInfl.outOffset = 0;

	Inflater_bs_move(16); // skip zlib header

	do {
		isLast = Inflater_bs_read(1);
		comp = Inflater_bs_read(2);

		switch (comp) {
		case 1: // fixed
			Inflater_fixed();
			break;
		case 2: // dynamic
			Inflater_dyn();
			break;
		}
	} while (!isLast);
}

#endif