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

// public
EXBUFF* ResourceManager::LoadFromAssets(const char* pszName)
{

	return NULL;

}




//////////////////////////////////////////////////////////////////////////
///  ¹öÆÛ.
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




///////////////


MC_GrpFrameBuffer MC_grpCreateOffScreenFrameBuffer(M_Int32 w, M_Int32 h)
{
	MC_GrpFrameBuffer ret = 0;

	return ret;
}
void MC_grpDestroyOffScreenFrameBuffer(MC_GrpFrameBuffer fb)
{

}

MC_GrpFrameBuffer MC_grpGetScreenFrameBuffer(M_Int32 s)
{
	return 0;
}
void MC_grpInitContext(MC_GrpContext* pgc)
{

}


void MC_grpFlushLcd(M_Int32 i, MC_GrpFrameBuffer frm, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h)
{

}




///////////////////
//
M_Int64 MC_knlCurrentTime()
{
	return 0;
}




