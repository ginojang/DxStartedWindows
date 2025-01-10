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
#define M_E_SUCCESS				0			// ����
#define M_E_ERROR				-1			// ��Ÿ����
#define M_E_BADFD				-2			// �߸��� �ĺ���
#define M_E_BADFILENAME			-3			// �߸��� ���� �̸�
#define M_E_BADPATHNAME			-3			// �߸��� ���� �̸� : 2005.05.19 ��ȭ������ ��û���� lym999 �߰�.
#define M_E_BADSEEKPOS			-4			// �߸��� ���� ��ġ
#define M_E_EXIST				-5			// �ش� ���ҽ��� �̹� ������
#define M_E_BADFORMAT			-6			// �߸��� ����
#define M_E_INPROGRESS			-7			// ���۷��̼� ������
#define M_E_INUSE				-8			// ���� ������̰ų� �̹� �����
#define M_E_INVALID				-9			// �Ű������� �߸��Ǿ���
#define M_E_ISCONN				-10			// �̹� ������ �����Ǿ� ����

#define M_E_LONGNAME			-11			// ���ѱ��� �ʰ�
#define M_E_NOENT				-12			// ���� ����
#define M_E_NOSPACE				-13			// ���� ������ ����
#define M_E_NOTCONN				-14			// ������ �����Ǿ� ���� ����
#define M_E_NOTEMPTY			-15			// ������� ����
#define M_E_NOTSUP				-16			// �ش� ���񽺸� �������� ����
#define M_E_NOMEMORY			-17			// �޸� ����
#define M_E_SHORTBUF			-18			// ���۰� ����
#define M_E_WOULDBLOCK			-19			// WOULDBLOCK �߻�
#define M_E_TIMEOUT				-20			// Ÿ�Ӿƿ�

#define M_E_DATABIG				-21			// �����Ͱ� �ʹ� ŭ
#define M_E_BADRECID			-22			// �߸��� ���ڵ� �ĺ���
#define M_E_EOF					-23			// ������ ��
#define M_E_ACCESS				-24			// ���ٿ���
#define M_E_INVALIDHANDLE		-25			// �������� �ڵ鰪
#define M_E_INVALIDSYSOP		-26			// �������� System Operation
#define M_E_NOTCHANGE			-27			// 
#define M_E_NOTEXIST			-28			// �������� �ʴ� ��
#define M_E_UNLOCK				-29			// Lock�� ������
#define M_E_LOCK				-30			// Lock�� ������ �� ���� ���ҽ�

#define M_E_HASNOUID			-31			// UID�� �������� �ʴ� ���ҽ�
#define M_E_NOLIMIT				-32			// Max���� ����
#define M_E_ALREADYEXISTEVENT	-33			// �̹� ���ǵ� �̺�Ʈ
#define M_E_PLTEXIT				-34			// �÷��� ����
#define M_E_INSUFSPACE			-35			// �޸𸮰� ���ڶ�
#define M_E_ACCESSDENY			-36			// ������ �źε�
#define M_E_DUPNAME				-37			// ���� �̸��� ������ �̹� ������
#define M_E_INVALIDSTATUS		-38			// 
#define M_E_NORES				-39			// 
#define M_E_PLOCK				-40			//  

#define M_E_GLOCK				-41			// 
#define M_E_INCORRECTPASSWORD	-42			// 
#define M_E_INVALIDRESGROUP		-43			// 
#define M_E_INVALIDTERMRES		-44			// 

#define	M_E_NOTFITSIZE			-45			// Annunciator�� ����ڰ� ������ �������� ũ�Ⱑ ���� �ʴ� ���

#define M_E_NOTSUPPORTGLOCK		-46			// �׷� lock�� �������� ����
#define M_E_NOTSUPPORTLOCK		-47			// lock�� �������� ����

#define M_E_NOTSUPPORTCBELL		-48			//ĳ���� ����� ��������
#define M_E_INVALIDFORMAT 		-49			//�������� �ʴ� ������
#define M_E_TOOMANYPARAM 		-50			//�ܸ��� �����ϴ� ĳ���� ���Ź�ȣ���� ���� �Ű� ������ ���޵�
#define M_E_NOTSCALE			-51			// �̹��� ���ڵ��� Scale�� �������� �ʴ� ���

#define M_E_NETCLOSE			-52			// ������������ ��Ʈ��ũ ������ ����� ���
#define M_E_SOCKETCLOSE			-53			// ������������ ���� ������ ����� ���

#define	M_E_NOFRAME				-54			// �ش� �������� �������� ���� ��
#define M_E_NOTSUPPORTPLOCK		-55			// ���� �� �������� ����

#define M_E_NOTACTIVE			-56			// ����̽��� Ȱ��ȭ���� ���� 

#define M_E_NETMODECHANGE		-57			// WCDMA���� CDMA�� ��� ����

#define M_E_INVALIDSOURCE		-58			//�߸��� ���� �ҽ�

#define M_E_NOTRESIZE			-59			// �̹����� Resize ���ϴ� ���

#define M_E_DEVCLOSE			-60			// IO ��ġ�� close �� ���
	
#define M_E_OEMERROR			-61			// OEM�� ������ ���� WIPI ���ø����̼��� Ư�� ������ �����Ǵ� ���

#define M_E_NOTSUPPORTTYPE		-62			// �ش� Ÿ���� �������� ����

#define M_E_NOTFOUND			-63			// ã�� �� ����

#define M_E_GETPROV				-64			// PROV Agent���� �����͸� �������� ���� ���

#define M_E_INVALIDDATA			-65			// �����Ͱ� �߸���.

#define M_E_MAXCOUNT			-66			// �ִ밪�� ���.

#define M_E_NODEVICE			-67			// ��ġ�� �������� ���� ���

#define M_E_INUSE_BY_MSASSITED	-68			// LBS�� MS Assisted ������� ��� ���� ���

#define M_E_INUSE_BY_MSBASED	-69			// LBS�� MS Based ������� ��� ���� ���

#define M_E_INUSE_BY_CELLBASED	-70			// LBS�� Cell Based ������� ��� ���� ���

#define M_E_NODELETE			-71			// ���� �Ұ���

#define M_E_NOTSUPPORTMETHOD	-72			// �������� �ʴ� �޼ҵ�

#define M_E_EXPIREDDATA			-73			// MP3 ���� ����� expire�� ��� 

#define M_E_AUTHENTICATE		-74			// Authentication Error

#define M_E_NETMODEREADY		-75			// WCDMA���� CDMA�� ��� ������ �Ϸ�Ǿ� ������ ������ ���

#define M_E_UNCOMPRESS			-76			// ���� ���� ����

#define M_E_BAD_DCF_INFORM		-77			// DCF Header Ʋ�� 

#define M_E_DATE_EXPIRED		-78			// ������� ���� 

#define M_E_DEVICE_NOT_REGISTERED -79		// �ܸ��� ���� ��ϵ��� �ʾ��� 

#define M_E_INVALID_OWNERSHIP	-80			// DCF�� �����ڿ� �ܸ��� MIN ��ȣ�� �ٸ�

#define M_E_BAD_DOMAIN			-81			// DCF�� �ܸ��⿡�� ������ �� �ִ� Domain�� �ƴ� 

#define M_E_INIT            -82			// WIPI 2.0 DLL �ʱ�ȭ ����			

#define M_E_CHANGED			-83			// ���� ��ȭ�� ����.

#define M_E_NOTCHANGED			-84			// ���� ��ȭ�� ����.	

#define M_E_INUSE_BY_OTHER_SVCID	-85			// ���� ��ȭ�� ����.	

#define M_E_NETDORMANT	-86			// ���� ��ȭ�� ����.	

#define M_E_PREEMPTED       -87			// ��Ʈ��ũ�� PPP�� �ٸ� APN, NAI�� �̿��Ͽ� �� ����, ����� ���� ��û ���� �ȵ� 

#define M_E_NOSERVICE       -88			// No Service ����

// -- TLS���� error �߰� 
// TLS ��� Error ���� : 91 ~ -98
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

