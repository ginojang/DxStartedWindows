#include "legacyCore.h"


#include "ResourceManager.h"




ResourceManager g_resourceManager;


ResourceManager::ResourceManager()
{
	counterBuffer = 10;

	for (int i = 0; i < EXBUFFERS_NUM; i++)
		exBuffers->nBuffID = 0;
}

ResourceManager::~ResourceManager()
{

}


//////////////////////////////////////////////////////////////////////////
///  ���� �κ�
///
// public
EXBUFF* ResourceManager::LoadFromAssets(const char* pszName)
{

	return NULL;

}




//////////////////////////////////////////////////////////////////////////
///  ����.
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
/// ���� ���� �ҽ� ������ �ϱ� ���� - ���� ȣ�� - �Լ� -    >>> �۾� ������ LegacyCore.cpp �� �̵� ����
M_Int64 MC_knlCurrentTime()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ���� ���� �ҽ� ������ �ϱ� ���� - ���� ȣ�� - �Լ� -  >>> �۾� ������ LegacyCore.cpp �� �̵� ����
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

