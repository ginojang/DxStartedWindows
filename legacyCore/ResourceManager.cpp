#include "EXdefault.h"

#include "EXtype.h"
#include "ResourceManager.h"


ResourceManager g_resourceManager;


ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}


EXBUFF* ResourceManager::LoadFromAssets(const char* pszName)
{

	return NULL;
}


//
M_Uint32 EX_knlAlloc(M_Int32 size)
{

	return 0;
}
void EX_knlFree(M_Uint32 mID)
{

}
M_Int32 EX_knlGetTotalMemory()
{
	return 0;
}
M_Int32 EX_knlGetFreeMemory()
{
	return 0;
}


M_Int32 EX_fsOpen(M_Char* name, M_Int32 flag, M_Int32 aMode)
{
	return 0;
}
M_Int32 EX_fsRead(M_Int32 fd, M_Byte* buf, M_Int32 len)
{
	return 0;
}
M_Int32 EX_fsWrite(M_Int32 fd, M_Byte* buf, M_Int32 len)
{
	return 0;
}

M_Int32 EX_fsRemove(M_Char* name, M_Int32 aMode)
{
	return 0;
}

M_Int32 EX_fsClose(M_Int32 fd)
{
	return 0;
}

M_Int32 EX_fsFileAttribute(M_Char* name, EX_FileInfo* fa, M_Int32 aMode)
{
	return 0;
}
M_Int32 EX_fsAvailable(void)
{
	return 0;
}


M_Int32 EX_mdaClipFree(EX_MdaClip* clip)
{
	return 0;
}
M_Int32 EX_mdaStop(EX_MdaClip* clip)
{
	return 0;
}

void EX_mdaSetVolume(M_Int32 value)
{

}
M_Int32 EX_mdaClipSetVolume(EX_MdaClip* clip, M_Int32 level)
{
	return 0;
}



///////////////


EX_GrpFrameBuffer EX_grpCreateOffScreenFrameBuffer(M_Int32 w, M_Int32 h)
{
	EX_GrpFrameBuffer ret = 0;

	return ret;
}
void EX_grpDestroyOffScreenFrameBuffer(EX_GrpFrameBuffer fb)
{

}

EX_GrpFrameBuffer EX_grpGetScreenFrameBuffer(M_Int32 s)
{
	return 0;
}
void EX_grpInitContext(EX_GrpContext* pgc)
{

}


void EX_grpFlushLcd(M_Int32 i, EX_GrpFrameBuffer frm, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h)
{

}




///////////////////
//
M_Int64 EX_knlCurrentTime()
{
	return 0;
}


M_Int32 EX_knlExit(M_Int32 exitCode)
{
	return 0;
}




