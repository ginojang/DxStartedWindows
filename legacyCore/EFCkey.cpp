#include "EXdefault.h"
#include "EFCkey.h"

#include "EFCthr.h"


// EFCkey.c
EXKEYDATA KeyData;

void EFC_keyInitialize( void )
{
	EFC_keyReset();
}

void EFC_keyFinalize( void )
{
	EFC_keyReset();
}

sint32 EFC_keyCOUNT( void )
{
	return KeyData.nCOUNT;
}

sint32 EFC_keyGET( void )
{
	sint32 nKEY = KEY_NULL;

	if( KeyData.nCOUNT > 0 ) {
		nKEY = KeyData.nKEYS[KeyData.nPOS];
		KeyData.nPOS = (KeyData.nPOS + 1) % KEY_BUFFS;
		KeyData.nCOUNT--;
	}

	if( nKEY != 0 ) {
		KeyData.nCURR = nKEY;
		KeyData.nDIFF = 100;
	}

	return nKEY;
}

sint32 EFC_keyGetDASH( sint32 nKEY, ubool bMODIFY )
{
	sint32 nPREV, nZERO, nCURR;
	sint32 nDIFF;

	nPREV = ((KeyData.nPOS + KEY_BUFFS - 3) % KEY_BUFFS);
	nZERO = ((KeyData.nPOS + KEY_BUFFS - 2) % KEY_BUFFS);
	nCURR = ((KeyData.nPOS + KEY_BUFFS - 1) % KEY_BUFFS);

	nDIFF = (sint32)(ThreadData.nCURR - KeyData.nTIMES[nPREV]);
	if( (nDIFF > 500) || (KeyData.nKEYS[nZERO] != 0) ) {
		return KEY_NULL;
	}

	if( bMODIFY == TRUE ) {
		if( (nKEY == EFC_keyMODIFY( KeyData.nKEYS[nPREV] )) && (nKEY == EFC_keyMODIFY( KeyData.nKEYS[nCURR] )) ) {
			return nKEY;
		}
	} else {
		if( (nKEY == KeyData.nKEYS[nPREV]) && (nKEY == KeyData.nKEYS[nCURR]) ) {
			return nKEY;
		}
	}

	return KEY_NULL;
}

sint32 EFC_keyLastTIMES( void )
{
	return (sint32)(ThreadData.nCURR - KeyData.nTIMES[ ((KeyData.nPOS + KeyData.nCOUNT) % KEY_BUFFS) ]);
}

sint32 EFC_keyTIMES( void )
{
	return (sint32)(ThreadData.nCURR - KeyData.nTIMES[ ((KeyData.nPOS + KeyData.nCOUNT - 1) % KEY_BUFFS) ]);
}

ubool EFC_keyIsREPEAT( void )
{
	if( (KeyData.nCOUNT > 0) || (KeyData.nKEY == KEY_NULL) || (KeyData.nKEY == 0) || (EFC_keyTIMES() < KeyData.nREPEAT) ) {
		return FALSE;
	}

	return TRUE;
}

sint32 EFC_keyLAST( void )
{
	return KeyData.nKEY;
}

void EFC_keySetREPEAT( sint32 nTIMES )
{
	KeyData.nREPEAT = nTIMES;
}

sint32 EFC_keyTransform(sint32 nKEY)
{
	//if(EFC_grpGetOrientationMode() == SCREEN_ORIENTATION_LANDSCAPE)
	{
		switch( nKEY ) 
		{
		case KEY_LEFT :
			nKEY = KEY_DOWN;
			break;
		case KEY_RIGHT :
			nKEY = KEY_UP;
			break;
		case KEY_UP :
			nKEY = KEY_LEFT;
			break;
		case KEY_DOWN :
			nKEY = KEY_RIGHT;
			break;
		}
	}

	return nKEY;
}

void EFC_keyADD( sint32 nKEY )
{
	sint32 nPOS;

	if( KeyData.nCOUNT == KEY_BUFFS ) {
		KeyData.nPOS = (KeyData.nPOS + 1) % KEY_BUFFS;
		KeyData.nCOUNT--;
	}

	nPOS = (KeyData.nPOS + KeyData.nCOUNT) % KEY_BUFFS;
	KeyData.nCOUNT++;

	KeyData.nKEY = nKEY;
	KeyData.nKEYS[nPOS] = nKEY;
	KeyData.nTIMES[nPOS] = ThreadData.nCURR;
}

void EFC_keyReset( void )
{
	MEMSET( &KeyData, 0, sizeof(EXKEYDATA) );
	KeyData.nKEY = KEY_NULL;
}

sint32 EFC_keyMODIFY( sint32 nKEY )
{
	switch( nKEY ) {
	case KEY_LEFT :
	case KEY_NUM4 :
		nKEY = KEY_LEFT;
		break;
	case KEY_RIGHT :
	case KEY_NUM6 :
		nKEY = KEY_RIGHT;
		break;
	case KEY_UP :
	case KEY_NUM2 :
		nKEY = KEY_UP;
		break;
	case KEY_DOWN :
	case KEY_NUM8 :
		nKEY = KEY_DOWN;
		break;
	case KEY_CENTER :
	case KEY_NUM5 :
		nKEY = KEY_CENTER;
		break;
	}

	return nKEY;
}

sint32 EFC_keyPREV( sint32 nPREV )
{
	sint32 nPOS;
	nPOS = (KeyData.nPOS + KeyData.nCOUNT - 1 + KEY_BUFFS + nPREV) % KEY_BUFFS;
	return KeyData.nKEYS[nPOS];
}

sint32 EFC_keySTART( sint32 nCURR, sint32 nNEXT, sint32 nMINUS )
{
	if( nCURR > nNEXT ) {
		nCURR = nNEXT;
	}
	if( nCURR < (nNEXT - nMINUS) ) {
		nCURR = (nNEXT - nMINUS);
	}
	return nCURR;
}

sint32 EFC_keyPOS( sint32 nCURR, sint32 nMAX, sint32 nPLUS )
{
	nCURR = ((nCURR + nMAX + nPLUS) % nMAX);
	return nCURR;
}
