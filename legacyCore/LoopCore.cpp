#include "LegacyCore.h"

#include "ResourceManager.h"
#include "LoopCore.h"
#include "LegacyDrawer.h"

#include "EFCmain.h"


//
LoopCore g_loopCore;


LoopCore::LoopCore()
{
}

LoopCore::~LoopCore()
{
}


void LoopCore::Start(uint32* pBuffer, int width, int height)
{
	g_legacyDrawer.Init(pBuffer, width, height);


	//
	EFC_mainInitialize();
}

void LoopCore::FixedUpdate(float time)
{

}

void LoopCore::Update()
{

}

void LoopCore::Close()
{
	g_legacyDrawer.ReleaseAll();
	g_resourceManager.ReleaseAll();
}


int __g_nRand = 0;
sint32 EFC_knlGetRAND(void)
{
#ifdef NATIVE_STAND_ALONE
	return (sint32)(((__g_nRand = (__g_nRand * 214013L + 2531011L)) >> 16) & 0x00007FFF);
#else
	// 유니티에서 가져온다.

#endif
}

