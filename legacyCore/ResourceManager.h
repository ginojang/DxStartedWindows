#pragma once

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();


	EXBUFF* LoadFromAssets(const char* pszName);

};


extern ResourceManager g_resourceManager;