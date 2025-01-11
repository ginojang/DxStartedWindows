#pragma once



typedef struct _MC_GrpFrameBuffer
{
	M_Int32 w;
	M_Int32 h;
	M_Int32 bpl;
	M_Int32 bpp;
	M_Int32 frameBufID;

	LPEXBUFF buffer;

} MC_GrpFrameBuffer;



class LegacyDrawer
{
public:
	LegacyDrawer();
	~LegacyDrawer();

	void Init(uint32* pScreenBuffer, int width, int height);
	void ReleaseAll();

public:
	schar* GetFrameBuffer(sint32 hIndex);
	sint32 GetFrameWidth(sint32 hIndex);
	sint32 GetFrameHeight(sint32 hIndex);
	uint8  GetFrameBpp(sint32 hIndex);
	sint32 GetBplWithWidth(sint32 width, sint32 bpp);



private:

};

extern LegacyDrawer g_legacyDrawer;






extern M_Int32 MC_grpCreateOffScreenFrameBuffer(M_Int32 w, M_Int32 h);
extern void MC_grpDestroyOffScreenFrameBuffer(M_Int32 fb);
extern M_Int32 MC_grpGetScreenFrameBuffer(M_Int32 s);
extern void MC_grpFlushLcd(M_Int32 i, M_Int32 frm, M_Int32 x, M_Int32 y, M_Int32 w, M_Int32 h);

// 픽셀에 있는 구조체
// 아래도 필요하면 핸들 구조로 변경
extern void MC_grpInitContext(void* pgc);



//
extern ubool EFC_rgnIsInRECT(LPEXRECT pRect, sint32 nX, sint32 nY);

extern void EFC_rgnSetPOINT(LPEXPOINT pPT, sint32 nX, sint32 nY);
extern void EFC_rgnRealPOINT(LPEXPOINT pPT, sint32 nX, sint32 nY);

extern ubool EFC_rgnSetRECT(LPEXRECT pRect, sint32 nX, sint32 nY, sint32 nW, sint32 nH);
extern ubool EFC_rgnInterRECT(LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2);
extern ubool EFC_rgnUnionRECT(LPEXRECT pRect, LPEXRECT pRect1, LPEXRECT pRect2);
extern ubool EFC_rgnPlusRECT(LPEXRECT pRect, LPEXRECT pRect1, sint32 nX, sint32 nY);
extern ubool EFC_rgnRealRECT(LPEXRECT pRect, LPEXRECT pRect1);

extern ubool EFC_rgnSetREGION(LPEXREGION pRgn, sint32 nX, sint32 nY, sint32 nW, sint32 nH, sint32 nSx, sint32 nSy);
extern ubool EFC_rgnInterREGION(LPEXREGION pRgn, LPEXRECT pRect, LPEXREGION pRegion);
extern ubool EFC_rgnRealREGION(LPEXREGION pRgn, sint32 nMIR);
extern ubool EFC_rgnGetRECT(LPEXRECT pRect, LPEXREGION pRegion);

extern sint32 EFC_mathiSqrt(uint32 nX);

extern sint32 EFC_mathCircleX(sint32 nX, sint32 nW, sint32 nH);
extern sint32 EFC_mathCircleY(sint32 nY, sint32 nW, sint32 nH);

extern sint32 EFC_mathSIN(sint32 nANGLE);
extern sint32 EFC_mathCOS(sint32 nANGLE);

extern void EFC_mathMOVE(LPEXPOINT ptMOVE, LPEXPOINT ptCURR, LPEXPOINT ptNEXT, LPEXPOINT ptDIRECT, sint32 nMOVE, uint8 nRATE1, uint16 nRATE2);