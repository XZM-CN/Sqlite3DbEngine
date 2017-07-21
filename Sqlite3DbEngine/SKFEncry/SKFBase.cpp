#include "StdAfx.h"
#include "DBItemDef.h"
#include "SKFBase.h"
#include "BaseFuncLib.h"

/// ��ʼ����̬����
HANDLE CSKFBase::m_hLib = NULL;
int    CSKFBase::m_nLoadRef = 0;
CCriticalSectionWrapper CSKFBase::m_sGlobalCriticalSection;

CSKFBase::CSKFBase(void)
	:m_nLastErrCode(0)
	,m_nDeviceState(-1)
	,m_cCurDisk(0)
	,m_bLockDev(false)
	,m_hDev(NULL)
	,m_strTFID("")
	,m_strLastErrInfo("")
{
}

void CSKFBase::UnInit()
{
	if(NULL == m_hLib)
		return;
	m_strTFID.Empty();
	m_strLastErrInfo.Empty();
	m_mapCard.clear();

	m_sGlobalCriticalSection.Lock();
	/// �����ô���
	m_nLoadRef--;
	if(0 == m_nLoadRef)
	{
		::FreeLibrary((HMODULE)m_hLib);
		m_hLib = NULL;
#ifdef APP_LOG_ENABLE
		//		WRITELOGTOFILE(_T("�ͷ�SKF֧�ֿ�"));
#endif
	}
	m_sGlobalCriticalSection.Unlock();
}

int CSKFBase::GetLastErrCode()
{
	return m_nLastErrCode;
}

void CSKFBase::ResetErrInfo()
{
	m_nLastErrCode = SAR_OK;
	m_strLastErrInfo.Empty();
}

CString CSKFBase::GetLastErrInfo(LONG nErrCode)
{
	CString strLastErrInfo;

	switch(nErrCode)
	{
	case ERROR_ACCESS_DENIED:
		strLastErrInfo = _T("�ܾ����ʡ�");
		break;
	case SAR_OK:
		strLastErrInfo = _T("����������ȷ��");
		break;
	case SAR_FAIL:
		strLastErrInfo = _T("��������ʧ�ܣ�ԭ������");
		break;
	case SAR_UNKOWNERR:
		strLastErrInfo = _T("�쳣����");
		break;
	case SAR_NOTSUPPORTYETERR:
		strLastErrInfo = _T("��֧�ֵķ���");
		break;
	case SAR_FILEERR:
		strLastErrInfo = _T("�ļ���������");
		break;
	case SAR_INVALIDHANDLEERR:
		strLastErrInfo = _T("��Ч�ľ����");
		break;
	case SAR_INVALIDPARAMERR:
		strLastErrInfo = _T("��Ч�Ĳ�����");
		break;
	case SAR_READFILEERR:
		strLastErrInfo = _T("��ȡ�ļ�����");
		break;
	case SAR_WRITEFILEERR:
		strLastErrInfo = _T("д�ļ�����");
		break;
	case SAR_NAMELENERR:
		strLastErrInfo = _T("���Ƴ��ȴ���");
		break;
	case SAR_KEYUSAGEERR:
		strLastErrInfo = _T("�ܳ���;����");
		break;
	case SAR_MODULUSLENERR:
		strLastErrInfo = _T("ģ�ĳ��ȴ���");
		break;
	case SAR_NOTINITIALIZEERR:
		strLastErrInfo = _T("δ��ʼ����");
		break;
	case SAR_OBJERR:
		strLastErrInfo = _T("�������");
		break;
	case SAR_MEMORYERR:
		strLastErrInfo = _T("�ڴ����");
		break;
	case SAR_TIMEOUTERR:
		strLastErrInfo = _T("��ʱ��");
		break;
	case SAR_INDATALENERR:
		strLastErrInfo = _T("�������ݳ��ȴ���");
		break;
	case SAR_INDATAERR:
		strLastErrInfo = _T("�������ݴ���");
		break;
	case SAR_GENRANDERR:
		strLastErrInfo = _T("�������������");
		break;
	case SAR_HASHOBJERR:
		strLastErrInfo = _T("��ϣ�������");
		break;
	case SAR_HASHERR:
		strLastErrInfo = _T("��ϣ�������");
		break;
	case SAR_GENRSAKEYERR:
		strLastErrInfo = _T("����RSA��Կ��");
		break;
	case SAR_RSAMODULUSLENERR:
		strLastErrInfo = _T("RSA��Կģ������");
		break;
	case SAR_CSPIMPRTPUBKEYERR:
		strLastErrInfo = _T("CSP�����빫Կ����");
		break;
	case SAR_RSAENCERR:
		strLastErrInfo = _T("RSA���ܴ���");
		break;
	case SAR_RSADECERR:
		strLastErrInfo = _T("RSA���ܴ���");
		break;
	case SAR_HASHNOTEQUALERR:
		strLastErrInfo = _T("��ϣֵ����ȡ�");
		break;
	case SAR_KEYNOTFOUNTERR:
		strLastErrInfo = _T("��Կδ���֡�");
		break;
	case SAR_CERTNOTFOUNTERR:
		strLastErrInfo = _T("֤��δ���֡�");
		break;
	case SAR_NOTEXPORTERR:
		strLastErrInfo = _T("����δ������");
		break;
	case SAR_DECRYPTPADERR:
		strLastErrInfo = _T("����ʱ����������");
		break;
	case SAR_MACLENERR:
		strLastErrInfo = _T("MAC���ȴ���");
		break;
	case SAR_BUFFER_TOO_SMALL:
		strLastErrInfo = _T("���������㡣");
		break;
	case SAR_KEYINFOTYPEERR:
		strLastErrInfo = _T("��Կ���ʹ���");
		break;
	case SAR_NOT_EVENTERR:
		strLastErrInfo = _T("���¼�����");
		break;
	case SAR_DEVICE_REMOVED:
		strLastErrInfo = _T("�豸���Ƴ���");
		break;
	case SAR_PIN_INCORRECT:
		strLastErrInfo = _T("PIN����ȷ��");
		break;
	case SAR_PIN_LOCKED:
		strLastErrInfo = _T("PIN��������");
		break;
	case SAR_PIN_INVALID:
		strLastErrInfo = _T("PIN��Ч��");
		break;
	case SAR_PIN_LEN_RANGE:
		strLastErrInfo = _T("PIN���ȴ���");
		break;
	case SAR_USER_ALREADY_LOGGED_IN:
		strLastErrInfo = _T("�û��Ѿ���¼��");
		break;
	case SAR_USER_PIN_NOT_INITIALIZED:
		strLastErrInfo = _T("û�г�ʼ���û����");
		break;
	case SAR_USER_TYPE_INVALID:
		strLastErrInfo = _T("PIN���ʹ���");
		break;
	case SAR_APPLICATION_NAME_INVALID:
		strLastErrInfo = _T("Ӧ��������Ч��");
		break;
	case SAR_APPLICATION_EXISTS:
		strLastErrInfo = _T("Ӧ���Ѿ����ڡ�");
		break;
	case SAR_USER_NOT_LOGGED_IN:
		strLastErrInfo = _T("�û�û�е�¼��");
		break;
	case SAR_APPLICATION_NOT_EXISTS:
		strLastErrInfo = _T("Ӧ�ò����ڡ�");
		break;
	case SAR_FILE_ALREADY_EXIST:
		strLastErrInfo = _T("�ļ��Ѿ����ڡ�");
		break;
	case SAR_NO_ROOM:
		strLastErrInfo = _T("�ռ䲻�㡣");
		break;
	case SAR_FILE_NOT_EXIST:
		strLastErrInfo = _T("�ļ������ڡ�");
		break;
	case SAR_REACH_MAX_CONTAINER_COUNT:
		strLastErrInfo = _T("�Ѵﵽ���ɹ�����������");
		break;
		strLastErrInfo = _T("��Ч�ľ����");
		break;
	default:
		if(nErrCode < 0x0A000000)
		{
			LPVOID lpMsgBuf = NULL;
			FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,nErrCode,0, // Default language
				(LPTSTR)&lpMsgBuf,0,NULL );
			if(NULL != lpMsgBuf)
			{
				strLastErrInfo.Format(_T("ϵͳ����%s"),lpMsgBuf);
				LocalFree(lpMsgBuf);
				lpMsgBuf = NULL;
			}
		}
		else
			strLastErrInfo.Format(_T("δ֪����%ld"),nErrCode);
		break;
	}

	return strLastErrInfo;
}