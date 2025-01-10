#include "LegacyCore.h"
#include "Inflater3.h"

#define MAXBITS 16

EXINFLATER xInfl;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
static const unsigned char order[19] = /* permutation of code lengths */
	{16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
static const unsigned char codeext[6] = /* permutation of code lengths */
	{2, 3, 3, 3, 7,11};

static const unsigned short cplens [31]= { // Copy lengths for literal codes 257..285
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0
};
static const unsigned char cplext[31] = { // Extra bits for literal codes 257..285
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 112, 112  // 112==invalid
};
static const unsigned short cpdist[30] = { // Copy offsets for distance codes 0..29
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577
  };
static const unsigned char cpdext[30] = { // Extra bits for distance codes
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
        12, 12, 13, 13};

static const unsigned char mask[9] = {0,1,3,7,15,31,63,127,255};
//////////////////////////////////////////////////////////////////////

void Inflater_bs_move( sint32 n )
{
	xInfl.inOffset += n;
}

sint32 Inflater_bs_read( sint32 n )
{
	ubyte *pData1;
	sint32 bitOffset, valid, middle, rest, retVal;
	sint32 i;

	pData1 = (ubyte *)EFC_memGET( xInfl.pDataIn );
	pData1 += (xInfl.inOffset >> 3);

	bitOffset = (xInfl.inOffset % 8);
	valid = 8 - bitOffset;

	xInfl.inOffset += n;

	if( n <= valid ) {
		return (((*pData1) >> bitOffset) & mask[n]);
	}

	retVal = (((*pData1) >> bitOffset) & mask[valid]);
	pData1++;

	n -= valid;
	rest = (n % 8);

	middle = n - rest;
	if( middle > 0 ) {
		for( i=valid; i < (valid + middle); i+=8, ++pData1 ) {
			retVal |= (((*pData1) & 0xFF) << i);
		}
	}

	if( rest > 0 ) {
		retVal |= (((*pData1) & mask[rest]) << (valid + middle));
	}

	return retVal;
}

void Inflater_code( LPEXBUFF pLens, LPEXBUFF pTable, int maxCodeLen )
{
	sint32 j, offset=0, code;
	uint16 sym;
	ubyte *pData1;
	uint16 *pData2;
	sint32 nCount;
	ubyte val;

	pData1 =(ubyte *)EFC_memGET( pLens );
	pData2 = (uint16 *)EFC_memGET( pTable );

	while( offset < (pLens->nSize) ) {
		code = Inflater_bs_read( maxCodeLen );
		Inflater_bs_move( (pData2[(code << 1)] - maxCodeLen) );
		sym = pData2[((code << 1) + 1)];

		if( sym < 16 ) {
			pData1[offset++] = (ubyte)sym;
		} else {
			sym -= 16;

			nCount = Inflater_bs_read( codeext[(sym << 1)] ) + codeext[((sym << 1) + 1)];
			if( sym == 0 ) {
				val = pData1[(offset - 1)];
			} else {
				val = 0;
			}

			for( j=0; j<nCount; j++ ) {
				pData1[offset++] = val;
			}
		}
	}
}

LPEXBUFF Inflater_makeTable( LPEXBUFF pLens, sint32* maxLen )
{
	LPEXBUFF pTable;
	ubyte *pData1;
	uint16 *pData2;

	sint32 i, j, min, max, code = 0;
	sint32 arrLen;
	sint32 count[MAXBITS], base[MAXBITS];
	sint32 t, t2, ind, nCount;

	MEMSET( count, 0, sizeof(count) );
	MEMSET( base, 0, sizeof(base) );

	pData1 = (ubyte *)EFC_memGET( pLens );
	min = pData1[0];
	max = pData1[0];

	for( i=1; i<(pLens->nSize); i++ ) {
		if( pData1[i] < min ) {
			min = pData1[i];
		}

		if( pData1[i] > max ) {
			max = pData1[i];
		}
	}

	(*maxLen) = max;
	arrLen = (max - min + 1);

	for( i=0; i<(pLens->nSize); i++ ) {
		count[(pData1[i] - min)]++;
	}

	if( min == 0 ) {
		count[0] = 0;
	}

	for( i=1; i<arrLen; i++ ) {
		code = ((code + count[i - 1]) << 1);
		base[i] = code;
	}

	pTable = EFC_memALLOC( ((1 << (max + 1)) * sizeof(short)) );
	if( pTable == NULL ) {
		return NULL;
	}

	pData1 = EFC_memGET( pLens );
	pData2 = (uint16 *)EFC_memGET( pTable );

	for( i=0; i<(pLens->nSize); i++ ) {
		if( pData1[i] == 0 ) {
			continue;
		}

		t = base[pData1[i] - min]++;

		t2 = 0;
		nCount = pData1[i];
		for( j=0; j<nCount; j++ ) {
			t2 |= (((t >> j) & 0x1) << (pData1[i] - 1 - j));
		}

		nCount = (1 << (max - pData1[i]));
		for( j=0; j<nCount; j++ ) {
			ind = ((j << pData1[i]) + t2);
			pData2[(ind << 1)] = (uint16)pData1[i];
			pData2[((ind << 1) + 1)] = (uint16)i;
		}
	}

	return pTable;
}

void Inflater_Last( LPEXBUFF pTableL, sint32 maxLitLen, LPEXBUFF pTableD, sint32 maxDistLen )
{
	sint32 code, index, length, dist, srcOffset;
	uint16 val;
	uint16 *pDataL, *pDataD;
	ubyte *pData1;

	pData1 = (ubyte *)EFC_memGET( xInfl.pDataOut );

	pDataL = (uint16 *)EFC_memGET( pTableL );
	pDataD = (uint16 *)EFC_memGET( pTableD );

	while( xInfl.outOffset < xInfl.pDataOut->nSize ) {
		code = Inflater_bs_read( maxLitLen );
		Inflater_bs_move( pDataL[(code << 1)] - maxLitLen );
		val = pDataL[((code << 1) + 1)];

		if( val == 256 ) {
			break;
		} else if( val < 256 ) {
			pData1[xInfl.outOffset++] = (ubyte)val;
		} else {
			index = val - 256 - 1;
			length = cplens[index] + Inflater_bs_read( cplext[index] );
			code = Inflater_bs_read( maxDistLen );
			Inflater_bs_move( pDataD[(code << 1)] - maxDistLen );
			val = pDataD[((code << 1) + 1)];
			dist = cpdist[val] + Inflater_bs_read( cpdext[val] );

			srcOffset = xInfl.outOffset - dist;
			while( length > 0 ) {
				if( length < dist ) {
					MEMCPY( pData1 + xInfl.outOffset, pData1 + srcOffset, length );
					xInfl.outOffset += length;
					length = 0;
				} else {
					MEMCPY( pData1 + xInfl.outOffset, pData1 + srcOffset, dist );
					xInfl.outOffset += dist;
					length -= dist;
				}
			}
		}
	}
}

void Inflater_fixed( void )
{
	LPEXBUFF pTableL, pTableD;
	LPEXBUFF pLens, pDists;
	ubyte *pData1, *pData2;
	sint32 maxLitLen, maxDistLen;

	pLens = EFC_memALLOC( 288 );
	pData1 = EFC_memGET( pLens );

	MEMSET( pData1, 8, 144 );
	MEMSET( pData1 + 144, 9, 112 );
	MEMSET( pData1 + 256, 7, 24 );
	MEMSET( pData1 + 280, 8, 8 );

	pTableL = Inflater_makeTable( pLens, &maxLitLen );
	EFC_memFREE( pLens );

	pDists = EFC_memALLOC( 30 );
	pData2 = EFC_memGET( pDists );

	MEMSET( pData2, 5, 30 );

	pTableD = Inflater_makeTable( pDists, &maxDistLen );
	EFC_memFREE( pDists );

	Inflater_Last( pTableL, maxLitLen, pTableD, maxDistLen );

	EFC_memFREE( pTableL );
	EFC_memFREE( pTableD );
}

void Inflater_dyn( void )
{
	EXBUFF xCodeLens;
	LPEXBUFF pTable, pTableL, pTableD;
	LPEXBUFF pLens, pDists;
	sint32 i, nLit, nDist, nCode, n, maxCodeLen;
	ubyte codeLens[19];
	sint32 maxLitLen, maxDistLen;

	nLit = Inflater_bs_read( 5 ) + 257;
	nDist = Inflater_bs_read( 5 ) + 1;
	nCode = Inflater_bs_read( 4 ) + 4;

	MEMSET( codeLens, 0, sizeof(codeLens) );
	for( i=0; i<nCode; i++ ) {
		n = Inflater_bs_read( 3 );
		codeLens[order[i]] = (ubyte)n;
	}

	MEMSET( &xCodeLens, 0, sizeof(EXBUFF) );
	xCodeLens.pByte = (ubyte *)codeLens;
	xCodeLens.nSize = 19;
	pTable = Inflater_makeTable( &xCodeLens, &maxCodeLen );

	pLens = EFC_memALLOC( nLit );
	Inflater_code( pLens, pTable, maxCodeLen );
	pTableL = Inflater_makeTable( pLens, &maxLitLen );
	EFC_memFREE( pLens );

	pDists = EFC_memALLOC( nDist );
	Inflater_code( pDists, pTable, maxCodeLen );
	EFC_memFREE( pTable );

	pTableD = Inflater_makeTable( pDists, &maxDistLen );
	EFC_memFREE( pDists );

	Inflater_Last( pTableL, maxLitLen, pTableD, maxDistLen );

	EFC_memFREE( pTableL );
	EFC_memFREE( pTableD );
}

void Inflater_inflate( LPEXBUFF pDataIn, LPEXBUFF pDataOut )
{
	sint32 isLast, comp;

	MEMSET( &xInfl, 0, sizeof(EXINFLATER) );

	xInfl.pDataIn = pDataIn;
	xInfl.inOffset = 64;

	xInfl.pDataOut = pDataOut;
	xInfl.outOffset = 0;

	Inflater_bs_move( 16 ); // skip zlib header

	do {
		isLast = Inflater_bs_read( 1 );
		comp = Inflater_bs_read( 2 );

		switch( comp ) {
		case 1 : // fixed
			Inflater_fixed();
			break;
		case 2 : // dynamic
			Inflater_dyn();
			break;
		}
	} while( !isLast );
}
