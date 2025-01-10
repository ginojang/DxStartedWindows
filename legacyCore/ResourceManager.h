#pragma once


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();


public:
	EXBUFF* LoadFromAssets(const char* pszName);
	uint32 AllocBuffer(int size);
	void ReleaseBuffer(uint32 index);
};


extern ResourceManager g_resourceManager;