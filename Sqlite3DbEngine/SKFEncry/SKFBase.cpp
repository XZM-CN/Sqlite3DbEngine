#include "StdAfx.h"
#include "DBItemDef.h"
#include "SKFBase.h"
#include "BaseFuncLib.h"

/// 初始化静态变量
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
	/// 减引用次数
	m_nLoadRef--;
	if(0 == m_nLoadRef)
	{
		::FreeLibrary((HMODULE)m_hLib);
		m_hLib = NULL;
#ifdef APP_LOG_ENABLE
		//		WRITELOGTOFILE(_T("释放SKF支持库"));
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
		strLastErrInfo = _T("拒绝访问。");
		break;
	case SAR_OK:
		strLastErrInfo = _T("调用请求正确。");
		break;
	case SAR_FAIL:
		strLastErrInfo = _T("调用请求失败，原因不明。");
		break;
	case SAR_UNKOWNERR:
		strLastErrInfo = _T("异常错误。");
		break;
	case SAR_NOTSUPPORTYETERR:
		strLastErrInfo = _T("不支持的服务。");
		break;
	case SAR_FILEERR:
		strLastErrInfo = _T("文件操作错误。");
		break;
	case SAR_INVALIDHANDLEERR:
		strLastErrInfo = _T("无效的句柄。");
		break;
	case SAR_INVALIDPARAMERR:
		strLastErrInfo = _T("无效的参数。");
		break;
	case SAR_READFILEERR:
		strLastErrInfo = _T("读取文件错误。");
		break;
	case SAR_WRITEFILEERR:
		strLastErrInfo = _T("写文件错误。");
		break;
	case SAR_NAMELENERR:
		strLastErrInfo = _T("名称长度错误。");
		break;
	case SAR_KEYUSAGEERR:
		strLastErrInfo = _T("密匙用途错误。");
		break;
	case SAR_MODULUSLENERR:
		strLastErrInfo = _T("模的长度错误。");
		break;
	case SAR_NOTINITIALIZEERR:
		strLastErrInfo = _T("未初始化。");
		break;
	case SAR_OBJERR:
		strLastErrInfo = _T("对象错误。");
		break;
	case SAR_MEMORYERR:
		strLastErrInfo = _T("内存错误。");
		break;
	case SAR_TIMEOUTERR:
		strLastErrInfo = _T("超时。");
		break;
	case SAR_INDATALENERR:
		strLastErrInfo = _T("输入数据长度错误。");
		break;
	case SAR_INDATAERR:
		strLastErrInfo = _T("输入数据错误。");
		break;
	case SAR_GENRANDERR:
		strLastErrInfo = _T("生成随机数错误。");
		break;
	case SAR_HASHOBJERR:
		strLastErrInfo = _T("哈希对象错误。");
		break;
	case SAR_HASHERR:
		strLastErrInfo = _T("哈希运算错误。");
		break;
	case SAR_GENRSAKEYERR:
		strLastErrInfo = _T("产生RSA密钥错。");
		break;
	case SAR_RSAMODULUSLENERR:
		strLastErrInfo = _T("RSA密钥模长错误。");
		break;
	case SAR_CSPIMPRTPUBKEYERR:
		strLastErrInfo = _T("CSP服务导入公钥错误。");
		break;
	case SAR_RSAENCERR:
		strLastErrInfo = _T("RSA加密错误。");
		break;
	case SAR_RSADECERR:
		strLastErrInfo = _T("RSA解密错误。");
		break;
	case SAR_HASHNOTEQUALERR:
		strLastErrInfo = _T("哈希值不相等。");
		break;
	case SAR_KEYNOTFOUNTERR:
		strLastErrInfo = _T("密钥未发现。");
		break;
	case SAR_CERTNOTFOUNTERR:
		strLastErrInfo = _T("证书未发现。");
		break;
	case SAR_NOTEXPORTERR:
		strLastErrInfo = _T("对象未导出。");
		break;
	case SAR_DECRYPTPADERR:
		strLastErrInfo = _T("解密时做补丁错误。");
		break;
	case SAR_MACLENERR:
		strLastErrInfo = _T("MAC长度错误。");
		break;
	case SAR_BUFFER_TOO_SMALL:
		strLastErrInfo = _T("缓冲区不足。");
		break;
	case SAR_KEYINFOTYPEERR:
		strLastErrInfo = _T("密钥类型错误。");
		break;
	case SAR_NOT_EVENTERR:
		strLastErrInfo = _T("无事件错误。");
		break;
	case SAR_DEVICE_REMOVED:
		strLastErrInfo = _T("设备已移除。");
		break;
	case SAR_PIN_INCORRECT:
		strLastErrInfo = _T("PIN不正确。");
		break;
	case SAR_PIN_LOCKED:
		strLastErrInfo = _T("PIN被锁死。");
		break;
	case SAR_PIN_INVALID:
		strLastErrInfo = _T("PIN无效。");
		break;
	case SAR_PIN_LEN_RANGE:
		strLastErrInfo = _T("PIN长度错误。");
		break;
	case SAR_USER_ALREADY_LOGGED_IN:
		strLastErrInfo = _T("用户已经登录。");
		break;
	case SAR_USER_PIN_NOT_INITIALIZED:
		strLastErrInfo = _T("没有初始化用户口令。");
		break;
	case SAR_USER_TYPE_INVALID:
		strLastErrInfo = _T("PIN类型错误。");
		break;
	case SAR_APPLICATION_NAME_INVALID:
		strLastErrInfo = _T("应用名称无效。");
		break;
	case SAR_APPLICATION_EXISTS:
		strLastErrInfo = _T("应用已经存在。");
		break;
	case SAR_USER_NOT_LOGGED_IN:
		strLastErrInfo = _T("用户没有登录。");
		break;
	case SAR_APPLICATION_NOT_EXISTS:
		strLastErrInfo = _T("应用不存在。");
		break;
	case SAR_FILE_ALREADY_EXIST:
		strLastErrInfo = _T("文件已经存在。");
		break;
	case SAR_NO_ROOM:
		strLastErrInfo = _T("空间不足。");
		break;
	case SAR_FILE_NOT_EXIST:
		strLastErrInfo = _T("文件不存在。");
		break;
	case SAR_REACH_MAX_CONTAINER_COUNT:
		strLastErrInfo = _T("已达到最大可管理容器数。");
		break;
		strLastErrInfo = _T("无效的句柄。");
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
				strLastErrInfo.Format(_T("系统错误：%s"),lpMsgBuf);
				LocalFree(lpMsgBuf);
				lpMsgBuf = NULL;
			}
		}
		else
			strLastErrInfo.Format(_T("未知错误：%ld"),nErrCode);
		break;
	}

	return strLastErrInfo;
}