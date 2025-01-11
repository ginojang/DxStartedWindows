#pragma once


#define EXBUFFERS_NUM	(60000)
#define MAX_ASSET_FOLDERS (20)


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();


// asset file
public:
	EXBUFF* LoadFromAssets(const char* pszName);

private:
	LPEXBUFF EFC_fsLoadINFLATE(LPEXBUFF pAlloc);

private:
	int numFolders;
	char AssetFolders[MAX_ASSET_FOLDERS][128];



// Buffer
public:
	EXBUFF* AllocBuffer(int size);
	ubyte* GetBufferBits(EXBUFF* buffer);
	EXBUFF* AllocWithBuffer(EXBUFF* srcBuffer);
	void ReleaseBuffer(EXBUFF* buffer);
	void ReleaseAll();

private:
	EXBUFF* FindBuffer();
	int counterBuffer;
	EXBUFF exBuffers[EXBUFFERS_NUM];
};
extern ResourceManager g_resourceManager;



extern void EFC_imgLoadARRAY(LPEXIMAGE pIMGs, schar* pStr, sint32* pIDXs, sint32 nCount, ubool bORDER, ubool bMIRROR);
extern void EFC_imgLoadCOUNT(LPEXIMAGE pIMGs, schar* pStr, sint32 nIDX, sint32 nCount);
extern void EFC_imgLoadIMAGE(LPEXIMAGE pIMGs, schar* pStr, sint32 nIDX);

extern void EFC_imgFREE(LPEXIMAGE pIMG);
extern void EFC_imgFreeARRAY(LPEXIMAGE pIMGs, sint32 nCount);
extern void EFC_imgMIRROR(LPEXIMAGE pIMG, LPEXMOVE pMov, sint32 nMIRROR);



#ifdef NATIVE_STAND_ALONE
typedef struct tagEXINFLATER 
{
	LPEXBUFF pDataIn;
	sint32 inOffset;

	LPEXBUFF pDataOut;
	sint32 outOffset;
} EXINFLATER, * LPEXINFLATER;

extern EXINFLATER xInfl;

extern void Inflater_inflate(LPEXBUFF pDataIn, LPEXBUFF pDataOut);

#endif