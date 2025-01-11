#include "legacyCore.h"
#include "ResourceManager.h"
#include "BinaryParser.h"

#ifdef NATIVE_STAND_ALONE
#include "Inflater3.h"
#endif


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
#ifdef NATIVE_STAND_ALONE

LPEXBUFF EFC_fsLoadINFLATE(LPEXBUFF pAlloc)
{
	LPEXBUFF pUnZip;
	EXFILE xFile;
	sint32 nZip, nSize;

	MEMSET(&xFile, 0, sizeof(EXFILE));
	xFile.pBuff = pAlloc;

	EFC_fsSetPOS(&xFile, 8);
	nZip = EFC_fsReadUint16(&xFile);

	//LOGI("NZIP: %d", nZip);

	if (nZip == 0x00009C78) {
		EFC_fsSetPOS(&xFile, 4); // 압축된 파일 사이즈
		nSize = EFC_fsReadSint32(&xFile); // 풀었을 때 파일 사이즈

		pUnZip = EFC_memALLOC(nSize);
		Inflater_inflate(pAlloc, pUnZip);
		EFC_memFREE(pAlloc);

		return pUnZip;
	}

	return pAlloc;
}

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
#endif

// public
EXBUFF* ResourceManager::LoadFromAssets(const char* pszName)
{
#ifdef NATIVE_STAND_ALONE
	for (int i = 0; i < numFolders; i++)
	{
		char* dirName = AssetFolders[i];
		char fileName[512];
		sprintf(fileName, "%s%s", dirName, pszName);

		int k = 1;
	}


	return NULL;
#else


	return NULL;
#endif
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

void ResourceManager::ReleaseAllBuffers()
{
	for (int i = 0; i < EXBUFFERS_NUM; i++)
		if (exBuffers[i].nBuffID != 0)
		{
			ReleaseBuffer(&exBuffers[i]);
		}
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 -    >>> 작업 끝나고 LegacyCore.cpp 로 이동 예정
M_Int64 MC_knlCurrentTime()
{
	return 0;
}

LPEXBUFF EFC_fsLoadBUFF(schar* pszFile)
{
	return g_resourceManager.LoadFromAssets(pszFile);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 -  >>> 작업 끝나고 LegacyCore.cpp 로 이동 예정
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

