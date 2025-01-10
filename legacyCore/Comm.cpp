#include "EXdefault.h"
#include "Comm.h"


COMM Comm;
COMMSTATUS NetStatus;
OUTPACKET outPacket;
INPACKET inPacket;

//////////////////////////////////////////////////////////////////////
enum {
	RESULT_CONNECT = 1,
	RESULT_SEND,
	RESULT_RECEIVE,
	RESULT_LGT
};

void Comm_netConnectCB( sint32 error, void *param );
void Comm_resultCB( sint16 state, sint32 error, void* param );
void Comm_netSockConnectCB( sint32 fd, sint32 error, void *param );
void Comm_netSockWrite( sint32 fd, sint32 error, void *param );
//////////////////////////////////////////////////////////////////////


void Comm_Initialize( void )
{
	Comm.nSocket = -1;
	Comm.nStatus = COMM_DISCONNECT;
	Comm.curReceiveBytes = 0;

	MEMSET( outPacket.packet, 0, OUTPACKET_MAX_LEN);
	MEMSET( inPacket.packet, 0, INPACKET_MAX_LEN);
}

void Comm_Finalize( void )
{   
	Comm.nStatus = COMM_DISCONNECT;
}

void Comm_SetProcessCB( NETPROCESSCB cb ) 
{
	Comm.netProcessCB = cb;
}

sint32 Comm_Connect(char *ip, uint16 port) 
{
	sint32 connectRv = 0;

	Comm.isDllLoad = FALSE;
	Comm.isReconncect = FALSE;

	Comm.nStatus = COMM_CONNECTING;

	STRCPY(Comm.serverIP, ip);
	Comm.serverPort = port;

	return connectRv;
}


void Comm_netConnectCB( sint32 error, void *param ) 
{
}

void Comm_netSockConnectCB( sint32 fd, sint32 error, void *param ) 
{
}

void Comm_resultCB(sint16 state, sint32 error, void* param)
{
}

void Comm_Init(sint16 command) 
{
	MEMSET( outPacket.packet, 0, OUTPACKET_MAX_LEN);
	MEMSET( inPacket.packet, 0, INPACKET_MAX_LEN);	
}

void Comm_Write2Bytes(sint16 v) 
{
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >>  0) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >>  8) & 0xFF);	
}

void Comm_Write4Bytes(sint32 v) 
{
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >>  0) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >>  8) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >> 16) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >> 24) & 0xFF);
}

void Comm_Write8Bytes(uint64 v) 
{
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >>  0) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >>  8) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >> 16) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >> 24) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >> 32) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >> 40) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >> 48) & 0xFF);
	outPacket.packet[outPacket.curPosition++] = (sint8)((v >> 56) & 0xFF);
}

void Comm_WriteStr(char *c, uint32 len) 
{
	uint32 i;
	uint32 strLen;

	strLen = (uint32)STRLEN(c);
	//RETOUCH_10
	for( i = 0 ; i < strLen && i < len; i++ ) {
		outPacket.packet[outPacket.curPosition++] = (sint8)c[i];
	}
	strLen = (sint32)len;
	for( ; i < strLen ; i++ ) {
		outPacket.packet[outPacket.curPosition++] = (sint8)0;
	}
}

void Comm_Write(sint8 v) 
{
	outPacket.packet[outPacket.curPosition++] = v;
}

void Comm_Flush( void ) 
{
}



void Comm_netSockWrite( sint32 fd, sint32 error, void *param ) 
{
}

void Comm_netSockReadCB( sint32 fd, sint32 error, void *param ) 
{
}

void Comm_Run( void )
{
}

sint8 Comm_Read()
{
	return inPacket.packet[inPacket.curPosition++];
}

sint16 Comm_Read2Bytes()
{
	sint16 res = ((inPacket.packet[inPacket.curPosition+1]&0xff)<<8) | (inPacket.packet[inPacket.curPosition]&0xff);
	inPacket.curPosition += 2;
	return res;
}

sint32 Comm_Read4Bytes()
{
	sint32 res = ((inPacket.packet[inPacket.curPosition+3]&0xff)<<24) | ((inPacket.packet[inPacket.curPosition+2]&0xff)<<16) 
		| ((inPacket.packet[inPacket.curPosition+1]&0xff)<<8) | (inPacket.packet[inPacket.curPosition]&0xff);
	inPacket.curPosition += 4;

	return res;
}

uint64 Comm_Read8Bytes()
{
	uint64 res = 0;

	res|=inPacket.packet[inPacket.curPosition+7];
	res<<=8;
	res|=inPacket.packet[inPacket.curPosition+6];
	res<<=8;
	res|=inPacket.packet[inPacket.curPosition+5];
	res<<=8;
	res|=inPacket.packet[inPacket.curPosition+4];
	res<<=8;
	res|=inPacket.packet[inPacket.curPosition+3];
	res<<=8;
	res|=inPacket.packet[inPacket.curPosition+2];
	res<<=8;
	res|=inPacket.packet[inPacket.curPosition+1];
	res<<=8;
	res|=inPacket.packet[inPacket.curPosition+0];

	inPacket.curPosition += 8;

	return res;
}

void Comm_ReadStr(char* c, uint32 len)
{
	uint32 i;

	for( i=0; i<len; i++ )
	{
		c[i] = inPacket.packet[inPacket.curPosition++];
	}

	//널값 처리
	c[len] = 0x00;
}


void Comm_getResultMsg( sint32 result )
{	

}


void Comm_Link( void ) 
{
}


void Comm_SetCharge(char* pSrt)
{
}
