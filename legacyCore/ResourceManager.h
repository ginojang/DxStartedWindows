#pragma once


#define EXBUFFERS_NUM	(60000)
#define MAX_ASSET_FOLDERS (20)


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();


public:
	EXBUFF* LoadFromAssets(const char* pszName);

private:
	int numFolders;
	char AssetFolders[MAX_ASSET_FOLDERS][128];

// Buffer
public:
	EXBUFF* AllocBuffer(int size);
	ubyte* GetBufferBits(EXBUFF* buffer);
	EXBUFF* AllocWithBuffer(EXBUFF* srcBuffer);
	void ReleaseBuffer(EXBUFF* buffer);
	void ReleaseAllBuffers();

private:
	EXBUFF* FindBuffer();
	int counterBuffer;
	EXBUFF exBuffers[EXBUFFERS_NUM];


};


extern ResourceManager g_resourceManager;