#ifndef __EXTEST__
#define __EXTEST__
															
															
#define MC_GRP_GET_FRAME_BUFFER_POINTER										EX_GRP_GET_FRAME_BUFFER_POINTER
#define MC_GRP_GET_FRAME_BUFFER_WIDTH										EX_GRP_GET_FRAME_BUFFER_WIDTH
#define MC_GRP_GET_FRAME_BUFFER_HEIGHT										EX_GRP_GET_FRAME_BUFFER_HEIGHT
#define MC_GRP_GET_FRAME_BUFFER_BPL											EX_GRP_GET_FRAME_BUFFER_BPL
#define MC_GRP_GET_FRAME_BUFFER_BPP											EX_GRP_GET_FRAME_BUFFER_BPP
#define MC_GRP_GET_FRAME_BUFFER_ID											EX_GRP_GET_FRAME_BUFFER_ID

#define MC_GETDPTR															EX_GETDPTR
																																
#define MC_GrpPixelOpProc													EX_GrpPixelOpProc

#define MC_GrpContext														EX_GrpContext

#define MC_GrpFrameBuffer													EX_GrpFrameBuffer

#define MC_GrpImage															EX_GrpImage
#define _MC_GrpImage														_EX_GrpImage

#define MC_PointerEvent														EX_PointerEvent

#define MCTimer																EXTimer
#define MC_FileInfo															EX_FileInfo
#define MC_MdaClip															EX_MdaClip

#define MC_GrpFrameBuffer													EX_GrpFrameBuffer

// GINO CHECK
//#define MC_knlGetResourceID													EX_knlGetResourceID
//#define MC_knlGetResource													EX_knlGetResource
#define MC_knlDefTimer														EX_knlDefTimer
#define MC_knlSetTimer														EX_knlSetTimer
#define MC_knlUnsetTimer													EX_knlUnsetTimer
#define MC_knlPrintk														EX_knlPrintk
#define MC_knlSprintk														EX_knlSprintk
#define MC_knlGetSystemProperty												EX_knlGetSystemProperty
#define MC_knlCurrentTime													EX_knlCurrentTime
#define MC_knlGetTotalMemory												EX_knlGetTotalMemory
#define MC_knlGetFreeMemory													EX_knlGetFreeMemory
#define MC_knlExit															EX_knlExit

#define MC_grpGetScreenFrameBuffer											EX_grpGetScreenFrameBuffer
#define MC_grpInitContext													EX_grpInitContext
#define MC_grpSetContext													EX_grpSetContext
#define MC_grpCreateImage													EX_grpCreateImage
#define MC_grpDrawImage														EX_grpDrawImage
#define MC_grpCreateOffScreenFrameBuffer									EX_grpCreateOffScreenFrameBuffer
#define MC_grpDestroyOffScreenFrameBuffer									EX_grpDestroyOffScreenFrameBuffer

#define MC_grpGetImageProperty												EX_grpGetImageProperty
#define MC_grpFlushLcd														EX_grpFlushLcd
#define MC_grpFillRect														EX_grpFillRect
#define MC_grpCopyFrameBuffer												EX_grpCopyFrameBuffer
#define MC_grpDrawLine														EX_grpDrawLine
#define MC_grpDrawRect														EX_grpDrawRect
#define MC_grpGetPixelFromRGB												EX_grpGetPixelFromRGB
#define MC_grpGetRGBFromPixel												EX_grpGetRGBFromPixel
#define MC_grpPutPixel														EX_grpPutPixel
#define MC_grpDestroyImage													EX_grpDestroyImage
#define MC_grpGetImageFrameBuffer											EX_grpGetImageFrameBuffer

#define MC_netConnect														EX_netConnect
#define MC_netSocket														EX_netSocket
#define MC_netSocketConnect													EX_netSocketConnect
#define MC_netSocketWrite													EX_netSocketWrite
#define MC_netSetWriteCB													EX_netSetWriteCB
#define MC_netSocketRead													EX_netSocketRead
#define MC_netSetReadCB														EX_netSetReadCB
#define MC_netSocketClose													EX_netSocketClose
#define MC_netClose															EX_netClose

#define MC_utilHtonl														EX_utilHtonl
#define MC_utilHtons														EX_utilHtons
#define MC_utilInetAddrInt													EX_utilInetAddrInt

#define MC_fsOpen															EX_fsOpen
#define MC_fsRead															EX_fsRead
#define MC_fsWrite															EX_fsWrite
#define MC_fsSeek															EX_fsSeek
#define MC_fsTell															EX_fsTell
#define MC_fsRemove															EX_fsRemove
#define MC_fsClose															EX_fsClose
#define MC_fsFileAttribute													EX_fsFileAttribute
#define MC_fsAvailable														EX_fsAvailable

#define MC_phnCallPlace														EX_phnCallPlace

#define MC_grpGetPixelFromRGB												EX_grpGetPixelFromRGB

#define MC_mdaClipCreate													EX_mdaClipCreate
#define MC_mdaClipPutData													EX_mdaClipPutData
#define MC_mdaPlay															EX_mdaPlay
#define MC_mdaClipFree														EX_mdaClipFree
#define MC_mdaStop															EX_mdaStop
#define MC_mdaVibrator														EX_mdaVibrator
#define MC_mdaClipClearData													EX_mdaClipClearData
#define MC_mdaSetDefaultVolume												EX_mdaSetDefaultVolume
#define MC_mdaSetVolume														EX_mdaSetVolume
#define MC_mdGetVolume														EX_mdaGetVolume
#define MC_mdaClipSetVolume													EX_mdaClipSetVolume
#define MC_mdaClipGetVolume												EX_mdaClipGetVolume
#define MC_mdaClipAllocPlayer												EX_mdaClipAllocPlayer
#define MC_mdaClipFreePlayer													EX_mdaClipFreePlayer


#endif

