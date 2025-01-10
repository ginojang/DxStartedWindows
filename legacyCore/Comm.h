#ifndef __COMM_HEADER__
#define __COMM_HEADER__


	enum {
		COMM_DISCONNECT = 0,		//
		COMM_CONNECTING,			// 연결중
		COMM_CONNECT,				// 연결
		COMM_CONNECT_BEFORE,				// 연결 직전
		COMM_CLOSE,				// 연결종료
		COMM_CONNECT_END		//연결 중간에 서버와 끊긴 경우..
	};

	enum{
		EN_CASH_100 = 0,
		EN_CASH_1000,
		EN_CASH_2000,
		EN_CASH_3900,

		EN_CASH_LAST
	};

	enum {
		EN_NET_DISCONNECT = 0,		// 초기값

		EN_NET_CONNECTING,			// 연결중
		EN_NET_CONNECT,				// 연결
		EN_NET_REQUEST,

		EN_NET_LOAD,				// 완료
		EN_NET_FREESPACE,			// 파일 공간 부족
		EN_NET_CERTIFY,				// 인증 요청
		EN_NET_OVERPRICE,			// 구매 한도 초과

		EN_NET_BILLCOMM_NOTI,		// 긴급 공지
		EN_NET_ERROR,				// 데이터가 문제가 있다 - 재요청할 것인가?
		EN_NET_CONTENT,				// 실패 원인

		EN_NET_PRICE,				// 이미 정식 등록되었음
		EN_NET_CLOSE				// 연결 종료
	};

	enum {
		NET_ERROR_NONE = 0,
		NET_ERROR_CONNECT,
		NET_ERROR_NETCONNECTCB,		// NETCONNECTCB FAIL
		NET_ERROR_SOCKET,			// 소켓 생성 실패
		NET_ERROR_BILLSOCKET,		// 연결에 문제가 있습니다
		NET_ERROR_NETSOCKCONNECTCB,	// NETSOCKCONNECTCB FAIL

		NET_ERROR_NETSETREADCB,
		NET_ERROR_NETSOCKREADCB,
		NET_ERROR_NETWOULDBLOCK,
		NET_ERROR_NETNOPACKET,
		NET_ERROR_READHEADER,
		NET_ERROR_TIMEOVER,

		NET_ERROR_NETSETWRITECB,
		NET_ERROR_NETSOCKETWRITE,

		NET_ERROR_DISCONNECT,
		NET_ERROR_BILLCOMM_NOTI,
#if defined(LGT_WIPI)
		NET_ERROR_LGT_DSLOCK,
		NET_ERROR_LGT_ROAMINGAREA,
		NET_ERROR_LGT_AIRPLANE,
#endif

		NET_ERROR_LAST
	};

	enum {
		PT_NULL = 0,

		PT_LOG_IN,
		PT_LOG_OUT,

		PT_REGIST_WINNING_MENT,

		PT_DIRECT_BATTLE_AUTO,
		PT_CHANNEL_LIST,
		PT_CHANNEL_USER_LIST,
		PT_CHANNEL_USER_INFO,

 		PT_BATTLE_REQUEST,
		PT_BATTLE_REQUEST_DIRECT,
		PT_BATTLE_REQUEST_WAIT,
		PT_BATTLE_REQUEST_TRANS,
		PT_BATTLE_REQUEST_REPLY,
		PT_BATTLE_REQUEST_RESULT, 
		PT_BATTLE_REQUEST_RESULT_OK,

		PT_TRADE_SELL_LIST,
		PT_TRADE_SELL_REGIST,
		PT_TRADE_SELL_REMOVE,
		PT_TRADE_SELL_REMOVE_OK,

		PT_TRADE_BUY_LIST,
		PT_TRADE_BUY_INFO
	};

	enum {
		NET_RECV_DATA = 0,
		NET_RECV_NOTI
	};

	//엔소니 프로토콜
	enum{
		ERROR_SUCCESS = 0x0000,

		ERROR_UNDEFINED = 0x8001,				//할당되지 않은 에러코드
		ERROR_ON_SERVER,						//서버 에러
		ERROR_ILLEGAL_HEADER_DATA,				//유효하지 않은 헤더 데이터
		ERROR_ILLEGAL_DATA,						//잘못된 데이터
		ERROR_NOT_REGIST_DATA_TYPE,				//등록되지 않은 데이터 타입

		ERROR_LGT_STAFF = 0x8101,				//LGT 임직원폰 구매 에러
		ERROR_LGT_UPGRADE,						//LGT 구형 폰 펌웨어 업그레이드 메시지
		
		ERROR_CLIENT_VER_NOT_MATCH = 0x8201,	//클라이언트 버전이 맞지 않음
		ERROR_CLIENT_NEED_UPGRADE,				//클라이언트 업데이트 필요
		ERROR_CLINET_NO_INFO,					//클라이언트 정보 없음
		ERROR_NOTICE_NOT_REGISTERED,			//등록된 공지사항 없음
		ERROR_ON_SMS_SERVER,					//SMS 인증 서버 에러(추가됨:2009-03-30)

		ERROR_ON_FILE_SAVE = 0x8251,			//파일 저장 에러
		ERROR_ON_FILE_GET,						//파일 가져오기 에러
		ERROR_ON_FILE_CRC,						//파일 CRC 코드 에러
		ERROR_ON_FILE_NAME,						//파일 이름 에러(20자 제한 또는 특수문자 포함)

		ERROR_ON_SHOP_ACTION = 0x8301,			//아이템 구매 작업중 에러
		ERROR_ON_REGIST_SHOP_LOG,				//구매 정보 저장중 에러
		OVERFLOW_SHOPPING_LIMIT,				//한달내 구매 가능 금액 초과
		ITEM_CODE_ERROR,						//잘못된 아이템 코드
		ALREADY_CERTIFIED,						//이미 유료전환을 결재한 경우
		ERROR_ON_QUERY_KOIN,					//KOIN 조회 에러
		ERROR_ON_USE_KOIN,						//KOIN 사용 에러
		KHUB_SESSION_TIMEOVER,					//KHUB 처리 시간 초과로 인한 처리 실패
		ERROR_ON_KHUB,							//KHUB 사용 에러
		NOT_EXIST_KHUB_KEY = 0x830A,			//KHUB 인증키가 없음

		ERROR_GIFT_ALREADY_RECEIVED = 0x8401,	//이미 받았거나 삭제된 선물
		ERROR_PHONE_NUMBER_UNAVAILABLE,			//전화번호가 유효하지 않음
		ERROR_GIFT_NOT_REGISTERD,				//등록된 선물이 아님
		ERROR_ON_REGIST_GIFT = 0x8405,			//선물 등록중 에러
		ERROR_GIFT_MAX_COUNT_OVER,				//선물 최대 개수 초과(30개)
		ERROR_ON_GIFT_ACTION,					//선물 관련 작업중 에러
		ERROR_NOT_REGISTERED_RECEIVER,			//받는 사람이 등록되지 않은 경우
		ERROR_CANNOT_SEND_TO_SELF,				//본인에게 선물을 보낼 수 없음
		
		ERROR_ON_RANK_ACTION = 0x8501,			//랭킹 등록 작업중 에러
		NO_RANK_DATA							//랭킹데이터가 없음
	};

	#define PT_INIT				0
	#define PT_TIME_SYNC		1
	#define PT_MY_INFO			2
	#define PT_READY_OK			3
	#define PT_ALLPLAYERS_STATE	4
	#define PT_PLAYER_MOVE		5
	#define PT_PLAYER_ATTACK	6
	#define PT_MONSTER_INFO		7
	#define PT_MONSTER_MOVE		8
	#define PT_MONSTER_ATTACK	9
	#define PT_PLAYER_OUT		10
	#define PT_PING				99

	#define CS_HEADER_SIZE		13
	#define SC_HEADER_SIZE		7

	#define OUTPACKET_MAX_LEN	2048
	#define INPACKET_MAX_LEN	2048//300

	

#ifndef M_E_BILLCOMM_NOTI	// -1001
	#define M_E_BILLCOMM_NOTI	-1001
#endif

#ifdef TEST_SERVER 
	#define SERVER_IP		"180.131.21.66"
	#define SERVER_PORT		5018
	//#define SERVER_PORT		5019	//디버깅용
	#define	GAME_ID				741 //드하3
#else		
	#define SERVER_IP		"218.50.3.88"
	#define SERVER_PORT		5018
	#define	GAME_ID				141 //드하3
#endif

#if defined(SKT_WIPI) || defined(SKT_ANDROID)

	#ifdef GXG_CHARGE_TEST
		#define GXG_GW_SERVER_IP		"112.172.129.23"		///< GXG 게이트웨이 접속 주소
		#define GXG_GW_SERVER_PORT		8004				///< GXG 게이트웨이 접속 포트
		#define GXG_USE_DOMAINCON			FALSE	
	#else
		#define GXG_GW_SERVER_IP		"ngw.gxg.com"		///< GXG 게이트웨이 접속 주소
		#define GXG_GW_SERVER_PORT		7300				///< GXG 게이트웨이 접속 포트
		#define GXG_USE_DOMAINCON			TRUE	
	#endif

	#ifdef DSP_TEST_SERVER
		#define	DSP_SERVER_IP	"112.172.129.26"
		#define	DSP_SERVER_PORT	8001
		#define	DSP_USE_DOMAINCON				FALSE

	#else
		#define	DSP_SERVER_IP	"ngph.gxg.com"
		#define	DSP_SERVER_PORT	7110
		#define	DSP_USE_DOMAINCON				TRUE
	#endif
#endif

#if defined(SKT_WIPI)
	#define _TEST_MIN		"01049114372"
	//#define _TEST_MIN		"01094276915"
	#define	_MCID		"0095650598"

	#if defined(USE_TOUCH_PROC)
		#define CHARGE_PID100		"9000540292"
		#define CHARGE_PID1000		"9000540293"
		#define CHARGE_PID2000		"9000540294"
		#define CHARGE_PID3900		"9000540295"
	#else
		#define CHARGE_PID100		"9000540288"
		#define CHARGE_PID1000		"9000540289"
		#define CHARGE_PID2000		"9000540290"
		#define CHARGE_PID3900		"9000540291"
	#endif
#elif defined(SKT_ANDROID)
	#define _TEST_MIN		"01033333821"
	#define	_MCID			"0095650598"

	#define CHARGE_PID100		"0900234035"
	#define CHARGE_PID1000		"0900234036"
	#define CHARGE_PID2000		"0900234037"
	#define CHARGE_PID3900		"0900234038"
#elif defined(KT_ANDROID)
	#define _TEST_MIN		"01099712149"
	#define	_CERTIFYKEY		"cDFn0Ofhn1"
	#define	_MCID			"8100BB5A"

	#define CHARGE_PID100		"141010100"
	#define CHARGE_PID1000		"141011000"
	#define CHARGE_PID2000		"141012000"
	#define CHARGE_PID3900		"141013900"
#elif defined(KT_WIPI)
	#define	_MCID		"01042D84"
	#define	_CERTIFYKEY		"cDFn0Ofhn1"
	#define _TEST_MIN		"01099712149"

	#define CHARGE_PID100		"00646FL02"
	#define CHARGE_PID1000		"00646FL03"
	#define CHARGE_PID2000		"00646FL04"
	#define CHARGE_PID3900		"00646FL05"
	
	#define CHARGE_WINGSID100		"I00646008051"
	#define CHARGE_WINGSID1000		"I00646008052"
	#define CHARGE_WINGSID2000		"I00646008053"
	#define CHARGE_WINGSID3900		"I00646008054"
#elif defined(LGT_ANDROID)
	#define _TEST_MIN		"01022852946"
	#define ARM_ID	"A900104686"

	#define CHARGE_PID100		"000385FD001"
	#define CHARGE_PID1000		"000385FD002"
	#define CHARGE_PID2000		"000385FD003"
	#define CHARGE_PID3900		"000385FD004"
#elif defined(LGT_WIPI)
	#define _TEST_MIN		"01033333821"

	#define CHARGE_PID100		"00036CFD001"
	#define CHARGE_PID1000		"00036CFD002"
	#define CHARGE_PID2000		"00036CFD003"
	#define CHARGE_PID3900		"00036CFD004"
#endif







	typedef struct tagOUTPACKET {
		sint32 curPosition;
		sint16 dataType;
		sint8 packet[OUTPACKET_MAX_LEN];//EXFILE packet;//
	} OUTPACKET, *LPOUTPACKET;

	typedef struct tagINPACKET {
		sint32 curPosition;
		sint32 packetLen;
		sint16 dataType;
		ubyte packet[INPACKET_MAX_LEN];//EXFILE packet;
		//sint8 packet[INPACKET_MAX_LEN];//EXFILE packet;
	} INPACKET, *LPINPACKET;

	typedef void (*NETPROCESSCB)(LPINPACKET inPacket, sint32 error);

	typedef struct tagEXIODATA {
		LPEXBUFF		pAlloc;
		sint32			nOffSet;
		sint32			nTotal;
		sint32			nRemain;
		sint32			nPos;
		ubool			bIO;
	} EXIODATA, *LPEXIODATA;

	typedef struct tagCOMMSTATUS {
		sint32			nCOMMAND;

		LPEXBUFF		pData; // SEND DATA

		EXIODATA		xSend;
		EXIODATA		xRecv;

		schar			szNoti[256];

		schar			szPhone[32];
		schar			szModel[32];
	} COMMSTATUS, *LPCOMMSTATUS;

	typedef struct _BILLCOM_HEADER
	{
		sint32		nHeaderCode;					//4
		schar		cEvendID[12];					//12	
		schar		cMIN[16];							//16
		sint32		nSignalMangementFlag;		//4	
		schar		cReserved[12];					//12
		sint32		sDataLength;						//4
		schar		cServiceType[4];					//4
	}BILLCOM_HEADER;

	typedef struct _FASTGW
	{
		void		(*FastAuthUser)();			// 정상 사용자 
		void		(*FastFailUser)();			// 잘못된 사용자 

		void		(*FastRelaySetAppID)(M_Byte* str ,M_Uint32 len);
		M_Int32		(*FastRelayTCPConnect)(M_Uint32 addr,M_Uint16 port,M_Boolean mode);
		M_Int32		(*FastRelayTCPWrite)(M_Int32 fd,M_Byte *buf,M_Int32 len);
		M_Int32		(*FastRelayTCPRead)(M_Int32 fd,M_Byte *buf,M_Int32 len);
		void		(*FastRelayTCPClose)(M_Int32 fd);
		M_Int32		(*FastRelayTCPChk)();

		// UDP 함수 
		M_Int32		(*FastRelayUDPOpen)(M_Uint32 addr, M_Uint16 port, M_Boolean mode);
		M_Int32		(*FastRelayUDPSend)(M_Int32 fd,M_Byte *buf,M_Uint32 len);
		M_Int32		(*FastRelayUDPRecv)(M_Int32 fd,M_Byte *buf,M_Uint32 len);
		void		(*FastRelayUDPClose)(M_Int32 fd);

		M_Int32		(*FastGetSocket)(M_Boolean bUDPConnect); 
	}
	FASTGWRelay; 

	typedef struct tagCOMM {
		char			serverIP[16];
		uint32			serverPort;		
		sint32			nSocket;
		ubyte			nStatus;
		ubool			isDllLoad;
		ubool			isReconncect;
		sint32			curSendBytes;
		sint32			outBufferLen;
		ubyte			outBuffer[OUTPACKET_MAX_LEN+13];
		//sint8			outBuffer[OUTPACKET_MAX_LEN];
		sint32			nLAST;

		sint32			curReceiveBytes;
		sint32			curHeaderBytes;
		ubyte			inBuffer[7];

		schar			net_strBuf[256];
		schar			strCharge[20];
		schar			strState[256];

		ubool			wouldBlock;


		//디버그용
		sint32			debugState;
//		char			resultStrs[10][50];
//		sint32			curResultIdx;
			
		void (*initialize)( void );
		sint32 (*connect)( char *ip, uint16 port );
		NETPROCESSCB netProcessCB;
		void (*init)( sint16 b );//void (*init)( ubyte b );
		void (*write)( sint8 b );
		void (*write2Bytes)( sint16 i );
		void (*write4Bytes)( sint32 i );
		void (*write8Bytes)( uint64 i );
		void (*writeStr)( char *c, uint32 len );
		void (*flush)( void );
		void (*run)( void );
		void (*setProcessCB)( NETPROCESSCB cb ); //패킷이 들어왔을때 알려줄 함수 지정		
		void (*finalize)( void );

		sint8 (*read)();
		sint16 (*read2Bytes)();
		sint32 (*read4Bytes)();
		uint64 (*read8Bytes)();
		void (*readStr)(char* c, uint32 len);

		void (*getResultMsg)(sint32 result);

	} COMM, *LPCOMM;
	
	extern COMM Comm;
	extern COMMSTATUS NetStatus;
	extern void Comm_Link( void );
	extern void Comm_SetCharge(char* pSrt);


#endif
