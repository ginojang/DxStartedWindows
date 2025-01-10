#ifndef __MCERROR_H_
#define __MCERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
**----------------------------------------------------------------------------
**  Includes
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  Definitions
**----------------------------------------------------------------------------
*/
#define M_E_SUCCESS				0			// 성공
#define M_E_ERROR				-1			// 기타에러
#define M_E_BADFD				-2			// 잘못된 식별자
#define M_E_BADFILENAME			-3			// 잘못된 파일 이름
#define M_E_BADPATHNAME			-3			// 잘못된 파일 이름 : 2005.05.19 김화성과장 요청으로 lym999 추가.
#define M_E_BADSEEKPOS			-4			// 잘못된 파일 위치
#define M_E_EXIST				-5			// 해당 리소스가 이미 존재함
#define M_E_BADFORMAT			-6			// 잘못된 포맷
#define M_E_INPROGRESS			-7			// 오퍼레이션 수행중
#define M_E_INUSE				-8			// 현재 사용중이거나 이미 사용중
#define M_E_INVALID				-9			// 매개변수가 잘못되었음
#define M_E_ISCONN				-10			// 이미 연결이 설정되어 있음

#define M_E_LONGNAME			-11			// 제한길이 초과
#define M_E_NOENT				-12			// 내용 없음
#define M_E_NOSPACE				-13			// 남은 공간이 없음
#define M_E_NOTCONN				-14			// 연결이 설정되어 있지 않음
#define M_E_NOTEMPTY			-15			// 비어있지 않음
#define M_E_NOTSUP				-16			// 해당 서비스를 지원하지 않음
#define M_E_NOMEMORY			-17			// 메모리 부족
#define M_E_SHORTBUF			-18			// 버퍼가 작음
#define M_E_WOULDBLOCK			-19			// WOULDBLOCK 발생
#define M_E_TIMEOUT				-20			// 타임아웃

#define M_E_DATABIG				-21			// 데이터가 너무 큼
#define M_E_BADRECID			-22			// 잘못된 레코드 식별자
#define M_E_EOF					-23			// 파일의 끝
#define M_E_ACCESS				-24			// 접근에러
#define M_E_INVALIDHANDLE		-25			// 부적절한 핸들값
#define M_E_INVALIDSYSOP		-26			// 부적절한 System Operation
#define M_E_NOTCHANGE			-27			// 
#define M_E_NOTEXIST			-28			// 존재하지 않는 것
#define M_E_UNLOCK				-29			// Lock이 해제됨
#define M_E_LOCK				-30			// Lock을 설정할 수 없는 리소스

#define M_E_HASNOUID			-31			// UID가 존재하지 않는 리소스
#define M_E_NOLIMIT				-32			// Max값이 없음
#define M_E_ALREADYEXISTEVENT	-33			// 이미 정의된 이벤트
#define M_E_PLTEXIT				-34			// 플랫폼 종료
#define M_E_INSUFSPACE			-35			// 메모리가 모자람
#define M_E_ACCESSDENY			-36			// 접근이 거부됨
#define M_E_DUPNAME				-37			// 같은 이름의 파일이 이미 존재함
#define M_E_INVALIDSTATUS		-38			// 
#define M_E_NORES				-39			// 
#define M_E_PLOCK				-40			//  

#define M_E_GLOCK				-41			// 
#define M_E_INCORRECTPASSWORD	-42			// 
#define M_E_INVALIDRESGROUP		-43			// 
#define M_E_INVALIDTERMRES		-44			// 

#define	M_E_NOTFITSIZE			-45			// Annunciator에 사용자가 설정한 아이콘의 크기가 맞지 않는 경우

#define M_E_NOTSUPPORTGLOCK		-46			// 그룹 lock을 지원하지 않음
#define M_E_NOTSUPPORTLOCK		-47			// lock을 지원하지 않음

#define M_E_NOTSUPPORTCBELL		-48			//캐릭벨 기능을 지원안함
#define M_E_INVALIDFORMAT 		-49			//지원하지 않는 포맷임
#define M_E_TOOMANYPARAM 		-50			//단말이 지원하는 캐릭벨 착신번호마다 많은 매개 변수가 전달됨
#define M_E_NOTSCALE			-51			// 이미지 디코딩시 Scale을 지원하지 않는 경우

#define M_E_NETCLOSE			-52			// 비정상적으로 네트워크 연결이 종료된 경우
#define M_E_SOCKETCLOSE			-53			// 비정상적으로 소켓 연결이 종료된 경우

#define	M_E_NOFRAME				-54			// 해당 프레임이 존재하지 않을 때
#define M_E_NOTSUPPORTPLOCK		-55			// 개별 락 지원하지 않음

#define M_E_NOTACTIVE			-56			// 디바이스가 활성화되지 않음 

#define M_E_NETMODECHANGE		-57			// WCDMA에서 CDMA로 모드 변경

#define M_E_INVALIDSOURCE		-58			//잘못된 볼륨 소스

#define M_E_NOTRESIZE			-59			// 이미지를 Resize 못하는 경우

#define M_E_DEVCLOSE			-60			// IO 장치가 close 된 경우
	
#define M_E_OEMERROR			-61			// OEM의 사정에 의해 WIPI 어플리케이션의 특정 동작이 중지되는 경우

#define M_E_NOTSUPPORTTYPE		-62			// 해당 타입을 지원하지 않음

#define M_E_NOTFOUND			-63			// 찾을 수 없음

#define M_E_GETPROV				-64			// PROV Agent에서 데이터를 가져오지 못한 경우

#define M_E_INVALIDDATA			-65			// 데이터가 잘못됨.

#define M_E_MAXCOUNT			-66			// 최대값을 벗어남.

#define M_E_NODEVICE			-67			// 장치가 장착되지 않은 경우

#define M_E_INUSE_BY_MSASSITED	-68			// LBS가 MS Assisted 방식으로 사용 중인 경우

#define M_E_INUSE_BY_MSBASED	-69			// LBS가 MS Based 방식으로 사용 중인 경우

#define M_E_INUSE_BY_CELLBASED	-70			// LBS가 Cell Based 방식으로 사용 중인 경우

#define M_E_NODELETE			-71			// 삭제 불가능

#define M_E_NOTSUPPORTMETHOD	-72			// 지원하지 않는 메소드

#define M_E_EXPIREDDATA			-73			// MP3 파일 재생시 expire된 경우 

#define M_E_AUTHENTICATE		-74			// Authentication Error

#define M_E_NETMODEREADY		-75			// WCDMA에서 CDMA로 모드 변경이 완료되어 재접속 가능한 경우

#define M_E_UNCOMPRESS			-76			// 압축 해제 실패

#define M_E_BAD_DCF_INFORM		-77			// DCF Header 틀림 

#define M_E_DATE_EXPIRED		-78			// 만료기한 지남 

#define M_E_DEVICE_NOT_REGISTERED -79		// 단말이 망에 등록되지 않았음 

#define M_E_INVALID_OWNERSHIP	-80			// DCF의 소유자와 단말의 MIN 번호가 다름

#define M_E_BAD_DOMAIN			-81			// DCF가 단말기에서 실행할 수 있는 Domain이 아님 

#define M_E_INIT            -82			// WIPI 2.0 DLL 초기화 실패			

#define M_E_CHANGED			-83			// 상태 변화가 있음.

#define M_E_NOTCHANGED			-84			// 상태 변화가 없음.	

#define M_E_INUSE_BY_OTHER_SVCID	-85			// 상태 변화가 없음.	

#define M_E_NETDORMANT	-86			// 상태 변화가 없음.	

#define M_E_PREEMPTED       -87			// 네트워크의 PPP를 다른 APN, NAI를 이용하여 연 상태, 현재는 연결 요청 수락 안됨 

#define M_E_NOSERVICE       -88			// No Service 상태

// -- TLS관련 error 추가 
// TLS 사용 Error 범위 : 91 ~ -98
#define M_E_SSL_ERROR_BASE			-90
#define M_E_SSL_OK					(M_E_SUCCESS)
#define M_E_SSL_OUTOFMEMORY			(M_E_SSL_ERROR_BASE-1)	// Ran Out of Memory
#define M_E_SSL_INVALIDARG			(M_E_SSL_ERROR_BASE-2)	// One or More Arguments are Invalid
#define M_E_SSL_HANDLE				(M_E_SSL_ERROR_BASE-3)	// Invalid Handle
#define M_E_SSL_ABORT				(M_E_SSL_ERROR_BASE-4)	// Operation Aborted
#define M_E_SSL_CERTIFICATE			(M_E_SSL_ERROR_BASE-5)	// Certificate Error
#define M_E_SSL_FAIL				(M_E_SSL_ERROR_BASE-6)	  // Unspecified Error
#define M_E_SSL_CLOSED				(M_E_SSL_ERROR_BASE-7)	// TLS Closed
#define M_E_SSL_WOULDBLOCK			(M_E_SSL_ERROR_BASE-8)	// Wouldblock


/*
**----------------------------------------------------------------------------
**  DRM Error code
**----------------------------------------------------------------------------
*/
#define M_E_DRM_ERROR_BASE							-100
#define M_E_DRM_ERROR								(M_E_DRM_ERROR_BASE - 1)
#define M_E_DRM_INVAILDPARAM						(M_E_DRM_ERROR_BASE - 2)
#define M_E_DRM_INITCONF							(M_E_DRM_ERROR_BASE - 3)
#define M_E_DRM_INITPOLICY							(M_E_DRM_ERROR_BASE - 4)
#define M_E_DRM_INITDB								(M_E_DRM_ERROR_BASE - 5)
#define M_E_DRM_INITAGENT							(M_E_DRM_ERROR_BASE - 6)
#define M_E_DRM_POLICY								(M_E_DRM_ERROR_BASE - 7)
#define M_E_DRM_POLICY_NOTALLOWABLE					(M_E_DRM_ERROR_BASE - 8)
#define M_E_DRM_PARSEDCF							(M_E_DRM_ERROR_BASE - 9)
#define M_E_DRM_GETDCFVALUE							(M_E_DRM_ERROR_BASE - 10)
#define M_E_DRM_GETDCFCID							(M_E_DRM_ERROR_BASE - 11)
#define M_E_DRM_SYSTEM								(M_E_DRM_ERROR_BASE - 12)
#define M_E_DRM_NOTEXISTRO							(M_E_DRM_ERROR_BASE - 14)
#define M_E_DRM_NOTEXISTUSEFULRO					(M_E_DRM_ERROR_BASE - 15)
#define M_E_DRM_GETROVALUE							(M_E_DRM_ERROR_BASE - 16)
#define M_E_DRM_VERIFYROMAC							(M_E_DRM_ERROR_BASE - 17)
#define M_E_DRM_PARSERIGHTS							(M_E_DRM_ERROR_BASE - 18)
#define M_E_DRM_GETDCFCEK							(M_E_DRM_ERROR_BASE - 19)
#define M_E_DRM_HANDLERIGHTSSTATUS					(M_E_DRM_ERROR_BASE - 20)
#define M_E_DRM_EXPIRERIGHTS						(M_E_DRM_ERROR_BASE - 21)
#define M_E_DRM_INVALIDREQOFFSET					(M_E_DRM_ERROR_BASE - 22)
#define M_E_DRM_INVALIDROAPMSGSYNTAX				(M_E_DRM_ERROR_BASE - 23)
#define M_E_DRM_MAKEROAPMSG							(M_E_DRM_ERROR_BASE - 24)
#define M_E_DRM_GETROAPMSGVALUE						(M_E_DRM_ERROR_BASE - 25)
#define M_E_DRM_VERIFYTRIGGERSIGN					(M_E_DRM_ERROR_BASE - 26)
#define M_E_DRM_VERIFYMSGSIGN						(M_E_DRM_ERROR_BASE - 27)
#define M_E_DRM_MISMATCHEDVALUE						(M_E_DRM_ERROR_BASE - 28)
#define M_E_DRM_HANDLEOCSPRESP						(M_E_DRM_ERROR_BASE - 29)
#define M_E_DRM_HANDLECERTIFICATECHAIN				(M_E_DRM_ERROR_BASE - 30)
#define M_E_DRM_HANDLESTATUSMAPTBL					(M_E_DRM_ERROR_BASE - 31)
#define M_E_DRM_ROAPSTATUSABORT						(M_E_DRM_ERROR_BASE - 32)
#define M_E_DRM_ROAPSTATUSNOTSUPPORTED				(M_E_DRM_ERROR_BASE - 33)
#define M_E_DRM_ROAPSTATUSACCESSDENIED				(M_E_DRM_ERROR_BASE - 34)
#define M_E_DRM_ROAPSTATUSNOTFOUND					(M_E_DRM_ERROR_BASE - 35)
#define M_E_DRM_ROAPSTATUSMALFORMEDREQ				(M_E_DRM_ERROR_BASE - 36)
#define M_E_DRM_ROAPSTATUSUNKNOWNCRITICALEXTENSION	(M_E_DRM_ERROR_BASE - 37)
#define M_E_DRM_ROAPSTATUSUNSUPPORTEDVERSION		(M_E_DRM_ERROR_BASE - 38)
#define M_E_DRM_ROAPSTATUSUNSUPPORTEDALGORITHM		(M_E_DRM_ERROR_BASE - 39)
#define M_E_DRM_ROAPSTATUSNOCERTCHAIN				(M_E_DRM_ERROR_BASE - 40)
#define M_E_DRM_ROAPSTATUSINVALIDCERTCHAIN			(M_E_DRM_ERROR_BASE - 41)
#define M_E_DRM_ROAPSTATUSTRUSTEDROOTCERTNOTPRESENT	(M_E_DRM_ERROR_BASE - 42)
#define M_E_DRM_ROAPSTATUSSIGNATUREERROR			(M_E_DRM_ERROR_BASE - 43)
#define M_E_DRM_ROAPSTATUSDEVICETIMEERROR			(M_E_DRM_ERROR_BASE - 44)
#define M_E_DRM_ROAPSTATUSNOTREGISTERED				(M_E_DRM_ERROR_BASE - 45)
#define M_E_DRM_ROAPSTATUSINVALIDDCFHASH			(M_E_DRM_ERROR_BASE - 46)
#define M_E_DRM_ROAPSTATUSINVALIDDOMAIN				(M_E_DRM_ERROR_BASE - 47)
#define M_E_DRM_ROAPSTATUSDOMAINFULL				(M_E_DRM_ERROR_BASE - 48)
#define M_E_DRM_ROAPSTATUSDOMAINACCESSDENIED		(M_E_DRM_ERROR_BASE - 49)
#define M_E_DRM_INITCRYPTO							(M_E_DRM_ERROR_BASE - 50)
#define M_E_DRM_PARSEDCFHDR							(M_E_DRM_ERROR_BASE - 51)
#define M_E_DRM_CREATEDIR							(M_E_DRM_ERROR_BASE - 52)
#define M_E_DRM_CREATECONFIGURATONFILE				(M_E_DRM_ERROR_BASE - 53)
#define M_E_DRM_CREATETESTCONFIGURATONFILE			(M_E_DRM_ERROR_BASE - 54)
#define M_E_DRM_CREATEPOLICYFILE					(M_E_DRM_ERROR_BASE - 55)
#define M_E_DRM_INITTABLE							(M_E_DRM_ERROR_BASE - 56)
#define M_E_DRM_NOTEXISTEDDEVICEPKIFILES			(M_E_DRM_ERROR_BASE - 57)
#define M_E_DRM_HANDLEAGENTPRIVATEKEY				(M_E_DRM_ERROR_BASE - 58)
#define M_E_DRM_HANDLEAGENTCERTIFICATE				(M_E_DRM_ERROR_BASE - 59)
#define M_E_DRM_INSTALLDUPLICATEDRO					(M_E_DRM_ERROR_BASE - 60)
#define M_E_DRM_MALFORMEDPROTECTEDRO				(M_E_DRM_ERROR_BASE - 61)
#define M_E_DRM_VERIFYOCSPRESPONSE					(M_E_DRM_ERROR_BASE - 62)
#define M_E_DRM_VERIFYCERTIFICATECHAIN				(M_E_DRM_ERROR_BASE - 63)
#define M_E_DRM_READRESOURCE						(M_E_DRM_ERROR_BASE - 64)
#define M_E_DRM_SEEKRESOURCE						(M_E_DRM_ERROR_BASE - 65)


/*
**----------------------------------------------------------------------------
**  Type Definitions
**----------------------------------------------------------------------------
*/

/*
**----------------------------------------------------------------------------
**  General Declarations
**----------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Variable Declarations
**---------------------------------------------------------------------------
*/

/*
**---------------------------------------------------------------------------
**  Function(external use only) Declarations
**---------------------------------------------------------------------------
*/

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif  /* __MCERROR_H_ */

