/**
 * @file BaseFuncLib.h
 *
 * Copyright (c) 2016-?,��غ��˿Ƽ�
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief ������������ʵ��
 *
 *
 *
 * @version 
 *
 * @date 2016-01-18
 *
 * @author WZH
 *
 * @history 
 *
 */
// BaseFuncLib.h : ����������ͷ�ļ�����
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <atlcomtime.h>
#include "HXRegKey.h"

#define POLYNOMIAL  0xEDB88320


/// ����๤��������������
typedef HRESULT ( __stdcall LPGetClassObject)( REFCLSID, REFIID, LPVOID* );

/// ���ԶԼ��ϴ���
class CBasePairString
{
protected:
	/// ���ԶԼ���
	CSTRING_MAP m_PairString;
	/// ��ӵ�����
	BOOL Add(const ATL::CString& key,const ATL::CString& value)
	{
		m_PairString.insert(CSTRING_PAIR(key,value));
		return TRUE;
	}

public:
	CBasePairString()
	{ 
	}
	virtual ~CBasePairString()
	{
		m_PairString.clear();
	}

	int Size()
	{
		return (int)m_PairString.size();
	}

	ATL::CString Get(const ATL::CString& key)
	{
		CSTRING_MAP::iterator map_pIter;
		map_pIter = m_PairString.find(key);
		ATLASSERT(map_pIter != m_PairString.end());
		if(map_pIter != m_PairString.end())
			return map_pIter->second;
		return _T("");
	}

	BOOL IsExist(const ATL::CString& key)
	{
		CSTRING_MAP::iterator map_pIter;
		map_pIter = m_PairString.find(key);
		if(map_pIter != m_PairString.end())
			return TRUE;
		return FALSE;
	}

	void Set(const ATL::CString& key,const ATL::CString& value)
	{
		CSTRING_MAP::iterator map_pIter;
		map_pIter = m_PairString.find(key);
		if(map_pIter != m_PairString.end())
			map_pIter->second = value;
		else
			Add(key,value);
	}

	void Set(const ATL::CString& key,DWORD dwValue)
	{
		ATL::CString strValue(_T(""));
		strValue.Format(_T("%ld"),dwValue);
		Set(key,strValue);
	}

	void Clear()
	{
		m_PairString.clear();
	}

	void Remove(const ATL::CString& key)
	{
		if(key.IsEmpty())
			return;
		CSTRING_MAP::iterator it = m_PairString.find(key);
		if(it != m_PairString.end())
			m_PairString.erase(it);
	}

	void Copy(CBasePairString* pInfo)
	{
		CSTRING_MAP::iterator map_pIter;
		for ( map_pIter = pInfo->m_PairString.begin( ); map_pIter != pInfo->m_PairString.end( ); map_pIter++ )
		{
			Set(map_pIter->first,map_pIter->second);
		}
	}

	CSTRING_MAP GetMap()
	{
		return m_PairString;
	}
};

/**
 * @ingroup 
 *
 * @brief ����������
 *
 *
 * @date 2016-01-18
 *
 * @author WZH
 *
 */
class CBaseFuncLib
{
public:
	CBaseFuncLib(void){};

	/**
	 * @brief ��������GBK�ַ���ת��ΪUTF-8����
	 *
	 *
	 * @param pSrc 
	 * @param pDst 
	 */
	static int CharToUtf8(const char* pSrc, char** pDst) throw();

	/**
	 * @brief ��������Utf8�ַ���ת��ΪChar����
	 *
	 *
	 * @param pSrc 
	 * @param pDst 
	 */
	static int Utf8ToChar(LPCSTR pSrc, char** pDst);

	/**
	 * @brief ��������UNICODE�ַ���ת��ΪChar����
	 *
	 *
	 * @param strSrc 
	 * @param pDst 
	 */
	static int Us2ToChar(const ATL::CString& strSrc, char** pDst,int nCodePage = CP_ACP);

	/**
	 * @brief ��������GBK�ַ���ת��ΪUNICODE����
	 *
	 *
	 * @param szSrc 
	 * @param pDst 
	 */
	static int CharToUS2(const char* szSrc, WCHAR** pDst,int nCodePage = CP_ACP);

	/**
	 * @brief ��������UNICODE�ַ���ת��ΪUTF-8����
	 *
	 *
	 * @param strSrc 
	 * @param pDst 
	 */
	static int US2ToUtf8(const ATL::CString& strSrc,char** pDst) throw();

	/**
	 * @brief ��������Utf8�ַ���ת��ΪUNICODE����
	 *
	 *
	 * @param pSrc 
	 * @param pDst 
	 */
	static int Utf8ToUS2(LPCSTR pSrc, WCHAR** pDst) throw();

	/**
	 * @brief GBK����ת��Ϊ��������
	 *
	 *
	 * @param strSrc 
	 * @return 
	 */
	static ATL::CString GBKToGB2312(const ATL::CString &strSrc);

	/**
	 * @brief ��������ת��ΪGBK����
	 *
	 *
	 * @param strSrc 
	 * @return 
	 */
	static ATL::CString GB2312ToGBK(const ATL::CString &strSrc);

	/**
	 * @brief ת������
	 *
	 *
	 * @param strSrc 
	 * @return 
	 */
	static ATL::CString PercentStringDecode(const ATL::CString& strSrc,BOOL bUtf8 = FALSE);

	/**
	 * @brief 
	 *
	 *
	 * @param &x 
	 * @return 
	 */
	static BYTE toHex(const BYTE &x);

	/**
	 * @brief ������ת��Ϊ�ַ���
	 *
	 *
	 * @param i 
	 * @return 
	 */
	static ATL::CString NumToStr(LONGLONG i);

	/**
	 * @brief �ַ���ת��������
	 *
	 *
	 * @param strTemp 
	 * @return 
	 */
	static DWORD StrToNum(const ATL::CString& strTemp);

	/**
	 * @brief �ַ���ת�������֣�֧�ֳ�������
	 *
	 *
	 * @param strTemp 
	 * @return 
	 */
	static void StrToNum2(const ATL::CString& strTemp,DWORD &dwNum1,DWORD &dwNum2);

	/**
	 * @brief �ֽ���ת�����ַ���
	 *
	 *
	 * @param szSrc �ֽ���Դ
	 * @param nLen �ֽ�������
	 * @param szDest Ŀ���ַ���
	 * @return 
	 */
	static void ChangeToChar(byte *szSrc,int nLen,char *szDest);

	/**
	 * @brief ���ָ��·��
	 *
	 *
	 * @param lFoldID 
	 * @return 
	 */
	static ATL::CString GetSpecialFolderPath(long lFoldID);

	/**
	 * @brief �ж��Ƿ�ΪWin 2000ϵͳ
	 *
	 *
	 * @param 
	 * @return 
	 */
	static BOOL IsW2K();

	static BOOL IsVistaOrLater();
	/**
	 * @brief �ж��Ƿ�Ϊ64λϵͳ
	 *
	 *
	 * @param hKernel
	 * @return 
	 */
	static BOOL IsWin64(HMODULE& hKernel);

	/**
	 * @brief �ж�·���ͷŴ���
	 *
	 *
	 * @param strPath 
	 * @return 
	 */
	static BOOL IsPathExist(const ATL::CString& strPath) throw();	

	/**
	 * @brief ����ָ��·��
	 *
	 *
	 * @param strLocalDir 
	 */
	static BOOL CreatePath(const ATL::CString& strLocalDir);

	/**
	 * @brief ɾ��ָ��Ŀ¼����Ŀ¼�������ļ���Ŀ¼
	 *
	 *
	 * @param strPath 
	 */
	static void DelDirFile(const ATL::CString &strPath,BOOL bDirFlag = TRUE);

	/**
	 * @brief ��ȡָ���ļ��������ݵ�������
	 *
	 *
	 * @param FileName 
	 * @param ppData 
	 * @return 
	 */
	static DWORD ReadAllData(const ATL::CString& FileName,BYTE **ppData);

	/**
	 * @brief ������д���ļ�
	 *
	 *
	 * @param strDataFile 
	 * @param pData 
	 * @param nlen 
	 * @param bEndFlag 
	 * @return 
	 */
	static BOOL WriteToFile(const ATL::CString& strDataFile,BYTE *pData,DWORD nlen,BOOL bEndFlag = FALSE);

	/**
	 * @brief ������д���ļ�
	 *
	 *
	 * @param strDataFile 
	 * @param lpBuffer 
	 * @param nlen 
	 * @param bEndFlag 
	 * @return 
	 */
	static HANDLE WriteToFile2(const ATL::CString& strDataFile,LPCVOID lpBuffer,DWORD nlen,BOOL bEndFlag, const char* strFileName, const DWORD dwLineNumber);

	/**
	 * @brief д���������־
	 *
	 *
	 * @param strLogInfo 
	 */
	static BOOL WriteLastLogToFile(DWORD nError, const char* strFileName, const DWORD dwLineNumber, const ATL::CString &strModuleName = _T(""),\
		ELogType eLogType = LOGTYPE_INVALID);

	/**
	 * @brief д��־
	 *
	 *
	 * @param strLogInfo 
	 */
	static BOOL WriteLogToFile(const ATL::CString& strLogInfo, const char* strFileName, const DWORD dwLineNumber, const ATL::CString &strModuleName = _T(""),\
		ELogType eLogType = LOGTYPE_INVALID);
	static BOOL WriteXzmLogToFile(const ATL::CString& strLogInfo, const char* strFileName, const DWORD dwLineNumber);

	/**
	 * @brief ����ɫֵת��Ϊ�ַ���
	 *
	 *
	 * @param rgbcolor 
	 * @return �����ַ�����ɫ����ʽ#000000
	 */
	static ATL::CString GetStringFromRGB(COLORREF& rgbcolor);

	/**
	 * @brief ���ַ���ת��Ϊ��ɫֵ
	 *
	 *
	 * @param strSource ��ʽ#000000
	 * @param rgbcolor 
	 * @return 
	 */
	static BOOL GetRGBFromString( ATL::CString strSource, COLORREF& rgbcolor );

	/**
	 * @brief ��ѯ���ע���ʵ��·��
	 *
	 *
	 * @param strRegPath 
	 * @return ���ز�ѯ��·��
	 */
	static CString QueryRegistryLink(const ATL::CString& strRegPath);

	/**
	 * @brief ���Ӧ�ó���ע����ַ�����ֵ
	 *
	 *
	 * @param strkeyName 
	 * @param strValue 
	 * @return 
	 */
	static BOOL GetAppStringValue(const ATL::CString& strkeyName,ATL::CString& strValue);

	/**
	 * @brief ����Ӧ�ó���ע���ַ�����ֵ
	 *
	 *
	 * @param strkeyName 
	 * @param strValue 
	 */
	static void SetAppStringValue(const ATL::CString& strkeyName,const ATL::CString& strValue);

	/**
	 * @brief ���Ӧ�ó���ע���DWORD��ֵ
	 *
	 *
	 * @param strkeyName 
	 * @param dwValue 
	 * @return 
	 */
	static BOOL GetAppDWordValue(const ATL::CString& strkeyName,DWORD& dwValue);
	
	/**
	 * @brief ����Ӧ�ó���ע���DWORD��ֵ
	 *
	 *
	 * @param strkeyName 
	 * @param dwValue 
	 */
	static void SetAppDWordValue(const ATL::CString& strkeyName,DWORD dwValue);

	/**
	 * @brief ���Ӧ�ó���ע�������Ƽ�ֵ
	 *
	 *
	 * @param strKeyName ������
	 * @param Value		 ��ֵ
	 * @param pnBytes	 ����
	 * @return 
	 */
	static BOOL GetAppBinaryValue(const ATL::CString &strKeyName,BYTE **ppValue,ULONG* pnBytes);
	
	/**
	 * @brief ����Ӧ�ó���ע�������Ƽ�ֵ
	 *
	 *
	 * @param strKeyName ������
	 * @param Value		 ��ֵ
	 * @param pnBytes	 ����
	 */
	static void SetAppBinaryValue(const ATL::CString &strKeyName,const BYTE* Value,ULONG nBytes);

	/**
	 * @brief ���������·��
	 *
	 *
	 * @return 
	 */
	static ATL::CString GetAppDir(const ATL::CString& strRegPath = _T(""), HKEY hKey = HKEY_CURRENT_USER);

	/**
	 * @brief ��������������ļ�·��
	 *
	 *
	 * @return 
	 */
	static ATL::CString GetAppDataDir(void);

	/**
	 * @brief �����ʱ�ļ�·��
	 *
	 *
	 * @return 
	 */
	static ATL::CString GetTmpPath(BOOL bAppFlag = TRUE);

	/**
	 * @brief ���������װ·�������Ϊ�գ���ʾû�а�װ
	 *
	 *
	 * @return 
	 */
	static BOOL GetAppInsPath(ATL::CString &strInsPath);

	/**
	 * @brief ����ģ�������ģ������·��
	 *
	 *
	 * @param hModule ģ����
	 * @return 
	 */
	static ATL::CString GetModulePath(HANDLE hModule = NULL);

	/**
	 * @brief ����ģ�������ģ������·��
	 *
	 *
	 * @param hModule ģ����
	 * @return 
	 */
	static ATL::CString GetAppConfigPath(HANDLE hModule = NULL);

	/**
	 * @brief ���ģ������
	 *
	 *
	 * @param hModule ģ����
	 * @return 
	 */
	static ATL::CString GetModuleName(HMODULE hModule = NULL);

	/**
	 * @brief ����ļ���С
	 *
	 *
	 * @param strFilePath 
	 * @return 
	 */
	static ULONG GetFileSize(const ATL::CString &strFilePath);

	/// ����ļ��޸�ʱ��
	static COleDateTime GetFileWriteTime(const ATL::CString& strFilePath);

	/// ��ȡ��Ϣʱ�� time_t -> DATE
	static COleDateTime GetMsgTime(ULONG ulTotalSecond);

	//ת��ʱ�� DATE -> time_t
	static DWORD CBaseFuncLib::LotusToTime( double dt );

	/**
	 * @brief ��ȡ�ļ���ϣֵ
	 *
	 *
	 * @param strFilePath 
	 * @return 
	 */
	static ATL::CString GetFileSha(const ATL::CString& strFilePath);

	/**
	 * @brief ����ļ���С����ʾ����
	 *
	 *
	 * @param dwFileSize 
	 * @param iPoint 
	 * @return 
	 */
	static ATL::CString GetShowSize(DWORD dwFileSize,int iPoint = 2);

	/**
	 * @brief ִ��Ӧ�ã��ɴ������У������Ƿ�ȴ��������Ƿ���ʾ����
	 *
	 *
	 * @param szAppPath 
	 * @param szCommandLine 
	 * @param bWaitFlag 
	 * @param bShowFlag 
	 */
	static DWORD RunExe(LPCTSTR szAppPath,LPCTSTR szCommandLine = _T(""),BOOL bWaitFlag = TRUE,BOOL bShowFlag = TRUE);

	/**
	 * @brief URL���뵽UTF-8
	 *
	 *
	 * @param strPara 
	 * @return 
	 */
	static ATL::CString UrlEncode(const ATL::CString& strPara,BOOL bFlag = TRUE);

	/**
	 * @brief URL���뵽UTF-8
	 *
	 *
	 * @param strPara 
	 * @return 
	 */
	static char* UrlEncodeToChar(const ATL::CString& strPara);

	/**
	 * @brief ���뵽�ļ�
	 *
	 *
	 * @param strPara 
	 * @param hFileHandle 
	 */
	static void UrlEncodeToFile(const ATL::CString& strPara,HANDLE hFileHandle,BOOL bTryFlag =FALSE);

	/**
	 * @brief ���ݴ��������ҳ��򴰿�
	 *
	 *
	 * @param strWndName 
	 * @return 
	 */
	static HWND GetAppWnd(const ATL::CString& strWndName = _T("IExplore"),BOOL bVisibleFlag = TRUE);

	/**
	 * @brief �˳������������е�ָ�����ڳ���
	 *
	 *
	 * @param strWndName �������֣�ȱʡΪ�˳�IE���� 
	 */
	static void QuitApp(const ATL::CString& strWndName = _T("IExplore"),BOOL bVisibleFlag = TRUE);

	/**
	 * @brief ��ָ����URL
	 *
	 *
	 * @param strUrl 
	 */
	static void OpenUrl(const ATL::CString& strUrl);

	/**
	 * @brief ����ָ�����ݵ�ϵͳճ����
	 *
	 *
	 * @param strCopy 
	 */
	static BOOL CopyStringToClipboard(const ATL::CString& strCopy);

	/**
	 * @brief ����ָ���Ŀⴴ��ָ���ӿ�
	 *
	 *
	 * @param lpszFilename 
	 * @param clsid 
	 * @param iid 
	 * @param ppv 
	 */
	static HINSTANCE CreateInstance( LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid, void ** ppv );
	
	/**
	 * @brief ����ָ����������ָ���ӿ�
	 *
	 *
	 * @param hInst 
	 * @param clsid 
	 * @param iid 
	 * @param ppv 
	 */
	static void CreateInterface( HINSTANCE hInst, REFCLSID clsid, REFIID iid, void ** ppv );

	static BOOL IsProcessExist( LPCTSTR lpszProcessName,BOOL bFlag = FALSE);

	//��ȡϵͳ��־��Ӧ�������
	static int GetCategory(int EventType,CString& EventCategory);
	//��ȡ��־��Ӧ��������
	static int GetInfoClass(int EventType,CString& EventCategory);

	static bool QueryValue(const char* ValueName, const char* szModuleName, char* RetStr, int RetLen);
	static DWORD GetSpecialFileSize(const CString szModuleName);

	static bool   GetFileDescription(const char* szModuleName, char* RetStr, int RetLen);
	static bool   GetFileVersion(const char* szModuleName, char* RetStr, int RetLen);
	static bool   GetInternalName(const char* szModuleName, char* RetStr, int RetLen);
	static bool   GetCompanyName(const char* szModuleName, char* RetStr, int RetLen);
	static bool   GetLegalCopyright(const char* szModuleName, char* RetStr, int RetLen);
	static bool   GetOriginalFilename(const char* szModuleName, char* RetStr, int RetLen);
	static bool   GetProductName(const char* szModuleName, char* RetStr, int RetLen);
	static bool   GetProductVersion(const char* szModuleName, char* RetStr, int RetLen);
	static BOOL	  GetExtFile(DWORD dwExt,const CString& strVal);
	static BOOL	  GetFileInfo(const CString& strFilePath,CString& strCorpName,\
		CString& strVersion,CString& strSize,CString& strDesc);

	static BOOL	  GetUserNameForSID (TCHAR* StringSid, TCHAR* strUserName, long nUserNameLen);

	static void InitTable();
	static UINT Crc32(UINT crc, char *buff, int len);

protected:
	/// ��������
	static ATL::CString	m_strWndName;
	/// �����Ƿ�ɼ�
	static BOOL		m_bVisibleFlag;
	/// ���ھ��
	static HWND		m_hWnd;

	/// ��־�ļ�
	static ATL::CString m_strLogFile;
	/// Ӧ��·��
	static ATL::CString m_strAppPath;

	static BOOL m_bTable;
	static UINT m_uTable[256];

	static BOOL CALLBACK EnumAppProc(HWND hWnd, LPARAM lp);
	
	
};

#define WRITELOGTOFILE(X) CBaseFuncLib::WriteLogToFile(X, __FILE__, __LINE__)

#define WRITEXZMLOGTOFILE(X) CBaseFuncLib::WriteXzmLogToFile(X, __FILE__, __LINE__)

#define WRITELOGTOFILE2(X,Y,Z) CBaseFuncLib::WriteLogToFile(X, __FILE__, __LINE__, Y,Z)

#define WRITELASTLOGTOFILE(X) CBaseFuncLib::WriteLastLogToFile(X, __FILE__, __LINE__)

#define WRITELASTLOGTOFILE2(X,Y) CBaseFuncLib::WriteLastLogToFile(X, __FILE__, __LINE__, Y)

#define WRITETOFILE2(W,X,Y,Z) CBaseFuncLib::WriteToFile2(W,X,Y,Z, __FILE__, __LINE__)

#define	 CREATE_MYTHREAD(func, lpParam, dwID)	( ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, (LPVOID)lpParam, 0, (dwID) ? &dwID:0) )

