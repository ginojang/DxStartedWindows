#include "LegacyCore.h"
#include "ResourceManager.h"
#include "LoopCore.h"
#include "EFCmain.h"


LoopCore g_loopCore;


LoopCore::LoopCore()
{
}

LoopCore::~LoopCore()
{
}


void LoopCore::Start(uint32* pBuffer, int width, int height)
{
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
	g_resourceManager.ReleaseAllBuffers();
}