#include "LegacyCore.h"

#include "ResourceManager.h"
#include "LegacyDrawer.h"


LegacyDrawer g_legacyDrawer;

LegacyDrawer::LegacyDrawer()
{

}

LegacyDrawer::~LegacyDrawer()
{

}


schar* LegacyDrawer::GetFrameBuffer(sint32 hIndex)
{
	return NULL;
}

sint32 LegacyDrawer::GetFrameWidth(sint32 hIndex)
{
	return 0;
}

sint32 LegacyDrawer::GetFrameHeight(sint32 hIndex)
{
	return 0;
}

uint8 LegacyDrawer::GetFrameBpp(sint32 hIndex)
{
	return 0;
}

sint32 LegacyDrawer::GetBplWithWidth(sint32 width, sint32 bpp)
{
	return (((width) * (bpp)+7) >> 3);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 기존 게임 소스 동작을 하기 위한 - 직접 호출 - 함수 -  >>> 작업 끝나고 LegacyCore.cpp 로 이동 예정
 
M_Int32 MC_grpCreateOffScreenFrameBuffer(M_Int32 w, M_Int32 h)
{
	return 0;
}
void MC_grpDestroyOffScreenFrameBuffer(M_Int32 fb)
{
}

M_Int32 MC_grpGetScreenFrameBuffer(M_Int32 s)
{
	return 0;
}
void MC_grpInitContext(MC_GrpContext* pgc)
{
}

void MC_grpFlushLcd(M_Int32 i, M_Int32 mcBuffer, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h)
{
}


