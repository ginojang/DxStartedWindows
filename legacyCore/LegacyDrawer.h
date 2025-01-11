#pragma once


class LegacyDrawer
{
public:
	LegacyDrawer();
	~LegacyDrawer();

public:
	schar* GetFrameBuffer(sint32 hIndex);
	sint32 GetFrameWidth(sint32 hIndex);
	sint32 GetFrameHeight(sint32 hIndex);
	uint8  GetFrameBpp(sint32 hIndex);

	sint32 GetBplWithWidth(sint32 width, sint32 bpp);

};


extern LegacyDrawer g_legacyDrawer;

//#define MC_GRP_GET_FRAME_BUFFER_POINTER(a) (MC_GETDPTR(a)->frameBufID)
//#define MC_GRP_GET_FRAME_BUFFER_WIDTH(a) (MC_GETDPTR(a)->w)
//#define MC_GRP_GET_FRAME_BUFFER_HEIGHT(a) (MC_GETDPTR(a)->h)
