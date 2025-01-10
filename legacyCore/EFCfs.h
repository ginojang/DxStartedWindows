#ifndef __EFC_FILE_HEADER__
#define __EFC_FILE_HEADER__



	/////////////////////////////////////////////////////////////////////////////////
	// DEFINE POSITION
	#define FILE_POS(fp,pos) (fp)->nPos=(pos);
	#define FILE_SKIP(fp,skip) (fp)->nPos+=(skip);

	#define CONVERT_INT(fp, a)	(fp[a]|(fp[a+1]<<8)|(fp[a+2]<<16)|(fp[a+3]<<24));

	// DEFINE READ
	#define READ_INT32(fp) ((fp)->pBuff->pByte[(fp)->nPos] | ((fp)->pBuff->pByte[(fp)->nPos + 1] << 8) | ((fp)->pBuff->pByte[(fp)->nPos + 2] << 16) | ((fp)->pBuff->pByte[(fp)->nPos + 3] << 24)); (fp)->nPos+=4;
	#define READ_INT16(fp) ((fp)->pBuff->pByte[(fp)->nPos] | ((fp)->pBuff->pByte[(fp)->nPos + 1] << 8)); (fp)->nPos+=2;
	#define READ_INT8(fp) ((fp)->pBuff->pByte[(fp)->nPos]); (fp)->nPos+=1;

	#define READ_SINT32(fp) (sint32)READ_INT32(fp)
	#define READ_UINT32(fp) (uint32)READ_INT32(fp)
	#define READ_SINT16(fp) (sint16)READ_INT16(fp)
	#define READ_UINT16(fp) (uint16)READ_INT16(fp)
	#define READ_SINT8(fp) (sint8)READ_INT8(fp)
	#define READ_UINT8(fp) (uint8)READ_INT8(fp)

	#define READ_INT32J(fp) ((fp)->pBuff->pByte[(fp)->nPos + 3] | ((fp)->pBuff->pByte[(fp)->nPos + 2] << 8) | ((fp)->pBuff->pByte[(fp)->nPos + 1] << 16) | ((fp)->pBuff->pByte[(fp)->nPos] << 24)); (fp)->nPos+=4;
	#define READ_INT16J(fp) ((fp)->pBuff->pByte[(fp)->nPos + 1] | ((fp)->pBuff->pByte[(fp)->nPos] << 8)); (fp)->nPos+=2;

	#define READ_SINT32J(fp) (sint32)READ_INT32J(fp)
	#define READ_UINT32J(fp) (uint32)READ_INT32J(fp)
	#define READ_SINT16J(fp) (sint16)READ_INT16J(fp)
	#define READ_UINT16J(fp) (uint16)READ_INT16J(fp)

	// DEFINE WRITE
	#define WRITE_UINT32(fp,s) (fp)->pBuff->pByte[(fp)->nPos]=(ubyte)((s) & 0xFF); (fp)->pBuff->pByte[(fp)->nPos + 1]=(ubyte)(((s) >> 8) & 0xFF); (fp)->pBuff->pByte[(fp)->nPos + 2]=(ubyte)(((s) >> 16) & 0xFF); (fp)->pBuff->pByte[(fp)->nPos + 3]=(ubyte)(((s) >> 24) & 0xFF); (fp)->nPos+=4;
	#define WRITE_UINT16(fp,s) (fp)->pBuff->pByte[(fp)->nPos]=(ubyte)((s) & 0xFF); (fp)->pBuff->pByte[(fp)->nPos + 1]=(ubyte)(((s) >> 8) & 0xFF); (fp)->nPos+=2;

	#define WRITE_UINT8(fp,s) (fp)->pBuff->pByte[(fp)->nPos]=(ubyte)(s); (fp)->nPos+=1;

	#define WRITE_UINT32J(fp,s) (fp)->pBuff->pByte[(fp)->nPos + 3]=(ubyte)((s) & 0xFF); (fp)->pBuff->pByte[(fp)->nPos + 2]=(ubyte)(((s) >> 8) & 0xFF); (fp)->pBuff->pByte[(fp)->nPos + 1]=(ubyte)(((s) >> 16) & 0xFF); (fp)->pBuff->pByte[(fp)->nPos]=(ubyte)(((s) >> 24) & 0xFF); (fp)->nPos+=4;
	#define WRITE_UINT16J(fp,s) (fp)->pBuff->pByte[(fp)->nPos + 1]=(ubyte)((s) & 0xFF); (fp)->pBuff->pByte[(fp)->nPos]=(ubyte)(((s) >> 8) & 0xFF); (fp)->nPos+=2;

	/////////////////////////////////////////////////////////////////////////////////
	
	extern sint32 EFC_fsCREATE( schar *pszFile, uint32 nSize );
	extern void EFC_fsSECURITY( ubyte *pByte, sint32 nSize );
	extern ubool EFC_fsLoadFILE( schar *pszFile, ubyte *pByte, sint32 nSize, ubool bCRC );
	extern ubool EFC_fsSaveFILE( schar *pszFile, ubyte *pByte, sint32 nSize, ubool bCRC );
	extern sint32 EFC_fsRemove(schar* pszFile, sint32 aMode);
	extern LPEXBUFF EFC_fsLoadBUFF( schar *pszFile );
	
	extern ubool EFC_fsIsAVAIL( LPEXFILE pFile, sint32 nBytes );
	extern void EFC_fsSetPOS( LPEXFILE pFile, sint32 nPos );
	extern void EFC_fsSKIP( LPEXFILE pFile, sint32 nSkip );

	extern sint8 EFC_fsReadSint8( LPEXFILE pFile );
	extern uint8 EFC_fsReadUint8( LPEXFILE pFile );
	extern sint16 EFC_fsReadSint16( LPEXFILE pFile );
	extern uint16 EFC_fsReadUint16( LPEXFILE pFile );
	extern sint32 EFC_fsReadSint32( LPEXFILE pFile );
	extern uint32 EFC_fsReadUint32( LPEXFILE pFile );
	extern uint64 EFC_fsReadUint64( LPEXFILE pFile );

	extern sint16 EFC_fsReadSint16J( LPEXFILE pFile );
	extern uint16 EFC_fsReadUint16J( LPEXFILE pFile );
	extern sint32 EFC_fsReadSint32J( LPEXFILE pFile );
	extern uint32 EFC_fsReadUint32J( LPEXFILE pFile );

	extern void EFC_fsReadStringJ( LPEXFILE pFile, schar *pDest );
	extern sint32 EFC_fsReadBuff( LPEXFILE pFile, ubyte *pDest, sint32 nSize );
	extern LPEXBUFF EFC_fsReadMallocBuff( LPEXFILE pFile, sint32 nSize );
	extern sint32 EFC_fsReadData( LPEXFILE pFile, LPEXFILE pSRC, sint32 nSize );

	extern void EFC_fsWriteUint8( LPEXFILE pFile, uint8 nNumber );
	extern void EFC_fsWriteUint16( LPEXFILE pFile, uint16 nNumber );
	extern void EFC_fsWriteUint32( LPEXFILE pFile, uint32 nNumber );
	extern void EFC_fsWriteUint64( LPEXFILE pFile, uint64 nNumber );

	extern void EFC_fsWriteUint16J( LPEXFILE pFile, uint16 nNumber );
	extern void EFC_fsWriteUint32J( LPEXFILE pFile, uint32 nNumber );

	extern void EFC_fsWriteBuff( LPEXFILE pFile, ubyte *pDest, sint32 nSize );
	extern void EFC_fsWriteStringJ( LPEXFILE pFile, schar *pDest );
	extern LPEXBUFF EFC_fsLoadPKG( schar *pszPkg, schar *pszRes );
	extern LPEXBUFF EFC_fsSerchPKG( LPEXBUFF pPkg, schar *pszRes );

#endif
