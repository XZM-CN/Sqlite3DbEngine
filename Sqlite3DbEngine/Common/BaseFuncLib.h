/**
 * @file BaseFuncLib.h
 *
 * Copyright (c) 2016-?,天地和兴科技
 * All rights reserved.
 *
 * @ingroup 
 *
 * @brief 基础公共函数实现
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
// BaseFuncLib.h : 基础函数库头文件定义
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <atlcomtime.h>
#include "HXRegKey.h"

#define POLYNOMIAL  0xEDB88320


/// 获得类工厂创建函数定义
typedef HRESULT ( __stdcall LPGetClassObject)( REFCLSID, REFIID, LPVOID* );

/// 属性对集合处理
class CBasePairString
{
protected:
	/// 属性对集合
	CSTRING_MAP m_PairString;
	/// 添加到集合
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
 * @brief 基础函数库
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
	 * @brief 将给定的GBK字符串转换为UTF-8编码
	 *
	 *
	 * @param pSrc 
	 * @param pDst 
	 */
	static int CharToUtf8(const char* pSrc, char** pDst) throw();

	/**
	 * @brief 将给定的Utf8字符串转换为Char编码
	 *
	 *
	 * @param pSrc 
	 * @param pDst 
	 */
	static int Utf8ToChar(LPCSTR pSrc, char** pDst);

	/**
	 * @brief 将给定的UNICODE字符串转换为Char编码
	 *
	 *
	 * @param strSrc 
	 * @param pDst 
	 */
	static int Us2ToChar(const ATL::CString& strSrc, char** pDst,int nCodePage = CP_ACP);

	/**
	 * @brief 将给定的GBK字符串转换为UNICODE编码
	 *
	 *
	 * @param szSrc 
	 * @param pDst 
	 */
	static int CharToUS2(const char* szSrc, WCHAR** pDst,int nCodePage = CP_ACP);

	/**
	 * @brief 将给定的UNICODE字符串转换为UTF-8编码
	 *
	 *
	 * @param strSrc 
	 * @param pDst 
	 */
	static int US2ToUtf8(const ATL::CString& strSrc,char** pDst) throw();

	/**
	 * @brief 将给定的Utf8字符串转换为UNICODE编码
	 *
	 *
	 * @param pSrc 
	 * @param pDst 
	 */
	static int Utf8ToUS2(LPCSTR pSrc, WCHAR** pDst) throw();

	/**
	 * @brief GBK编码转换为简体中文
	 *
	 *
	 * @param strSrc 
	 * @return 
	 */
	static ATL::CString GBKToGB2312(const ATL::CString &strSrc);

	/**
	 * @brief 简体中文转换为GBK编码
	 *
	 *
	 * @param strSrc 
	 * @return 
	 */
	static ATL::CString GB2312ToGBK(const ATL::CString &strSrc);

	/**
	 * @brief 转换文字
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
	 * @brief 将数字转换为字符串
	 *
	 *
	 * @param i 
	 * @return 
	 */
	static ATL::CString NumToStr(LONGLONG i);

	/**
	 * @brief 字符串转换到数字
	 *
	 *
	 * @param strTemp 
	 * @return 
	 */
	static DWORD StrToNum(const ATL::CString& strTemp);

	/**
	 * @brief 字符串转换到数字，支持超大数字
	 *
	 *
	 * @param strTemp 
	 * @return 
	 */
	static void StrToNum2(const ATL::CString& strTemp,DWORD &dwNum1,DWORD &dwNum2);

	/**
	 * @brief 字节流转换到字符串
	 *
	 *
	 * @param szSrc 字节流源
	 * @param nLen 字节流长度
	 * @param szDest 目标字符串
	 * @return 
	 */
	static void ChangeToChar(byte *szSrc,int nLen,char *szDest);

	/**
	 * @brief 获得指定路径
	 *
	 *
	 * @param lFoldID 
	 * @return 
	 */
	static ATL::CString GetSpecialFolderPath(long lFoldID);

	/**
	 * @brief 判断是否为Win 2000系统
	 *
	 *
	 * @param 
	 * @return 
	 */
	static BOOL IsW2K();

	static BOOL IsVistaOrLater();
	/**
	 * @brief 判断是否为64位系统
	 *
	 *
	 * @param hKernel
	 * @return 
	 */
	static BOOL IsWin64(HMODULE& hKernel);

	/**
	 * @brief 判断路径释放存在
	 *
	 *
	 * @param strPath 
	 * @return 
	 */
	static BOOL IsPathExist(const ATL::CString& strPath) throw();	

	/**
	 * @brief 创建指定路径
	 *
	 *
	 * @param strLocalDir 
	 */
	static BOOL CreatePath(const ATL::CString& strLocalDir);

	/**
	 * @brief 删除指定目录及子目录的所有文件和目录
	 *
	 *
	 * @param strPath 
	 */
	static void DelDirFile(const ATL::CString &strPath,BOOL bDirFlag = TRUE);

	/**
	 * @brief 读取指定文件所有数据到缓冲区
	 *
	 *
	 * @param FileName 
	 * @param ppData 
	 * @return 
	 */
	static DWORD ReadAllData(const ATL::CString& FileName,BYTE **ppData);

	/**
	 * @brief 将数据写入文件
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
	 * @brief 将数据写入文件
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
	 * @brief 写最近出错日志
	 *
	 *
	 * @param strLogInfo 
	 */
	static BOOL WriteLastLogToFile(DWORD nError, const char* strFileName, const DWORD dwLineNumber, const ATL::CString &strModuleName = _T(""),\
		ELogType eLogType = LOGTYPE_INVALID);

	/**
	 * @brief 写日志
	 *
	 *
	 * @param strLogInfo 
	 */
	static BOOL WriteLogToFile(const ATL::CString& strLogInfo, const char* strFileName, const DWORD dwLineNumber, const ATL::CString &strModuleName = _T(""),\
		ELogType eLogType = LOGTYPE_INVALID);
	static BOOL WriteXzmLogToFile(const ATL::CString& strLogInfo, const char* strFileName, const DWORD dwLineNumber);

	/**
	 * @brief 将颜色值转换为字符串
	 *
	 *
	 * @param rgbcolor 
	 * @return 返回字符串颜色，格式#000000
	 */
	static ATL::CString GetStringFromRGB(COLORREF& rgbcolor);

	/**
	 * @brief 将字符串转换为颜色值
	 *
	 *
	 * @param strSource 格式#000000
	 * @param rgbcolor 
	 * @return 
	 */
	static BOOL GetRGBFromString( ATL::CString strSource, COLORREF& rgbcolor );

	/**
	 * @brief 查询获得注册表实际路径
	 *
	 *
	 * @param strRegPath 
	 * @return 返回查询的路径
	 */
	static CString QueryRegistryLink(const ATL::CString& strRegPath);

	/**
	 * @brief 获得应用程序注册表字符串键值
	 *
	 *
	 * @param strkeyName 
	 * @param strValue 
	 * @return 
	 */
	static BOOL GetAppStringValue(const ATL::CString& strkeyName,ATL::CString& strValue);

	/**
	 * @brief 设置应用程序注册字符串键值
	 *
	 *
	 * @param strkeyName 
	 * @param strValue 
	 */
	static void SetAppStringValue(const ATL::CString& strkeyName,const ATL::CString& strValue);

	/**
	 * @brief 获得应用程序注册表DWORD键值
	 *
	 *
	 * @param strkeyName 
	 * @param dwValue 
	 * @return 
	 */
	static BOOL GetAppDWordValue(const ATL::CString& strkeyName,DWORD& dwValue);
	
	/**
	 * @brief 设置应用程序注册表DWORD键值
	 *
	 *
	 * @param strkeyName 
	 * @param dwValue 
	 */
	static void SetAppDWordValue(const ATL::CString& strkeyName,DWORD dwValue);

	/**
	 * @brief 获得应用程序注册表二进制键值
	 *
	 *
	 * @param strKeyName 键名称
	 * @param Value		 键值
	 * @param pnBytes	 长度
	 * @return 
	 */
	static BOOL GetAppBinaryValue(const ATL::CString &strKeyName,BYTE **ppValue,ULONG* pnBytes);
	
	/**
	 * @brief 设置应用程序注册表二进制键值
	 *
	 *
	 * @param strKeyName 键名称
	 * @param Value		 键值
	 * @param pnBytes	 长度
	 */
	static void SetAppBinaryValue(const ATL::CString &strKeyName,const BYTE* Value,ULONG nBytes);

	/**
	 * @brief 获得主程序路径
	 *
	 *
	 * @return 
	 */
	static ATL::CString GetAppDir(const ATL::CString& strRegPath = _T(""), HKEY hKey = HKEY_CURRENT_USER);

	/**
	 * @brief 获得主程序数据文件路径
	 *
	 *
	 * @return 
	 */
	static ATL::CString GetAppDataDir(void);

	/**
	 * @brief 获得临时文件路径
	 *
	 *
	 * @return 
	 */
	static ATL::CString GetTmpPath(BOOL bAppFlag = TRUE);

	/**
	 * @brief 获得主程序安装路径，如果为空，表示没有安装
	 *
	 *
	 * @return 
	 */
	static BOOL GetAppInsPath(ATL::CString &strInsPath);

	/**
	 * @brief 根据模块句柄获得模块所在路径
	 *
	 *
	 * @param hModule 模块句柄
	 * @return 
	 */
	static ATL::CString GetModulePath(HANDLE hModule = NULL);

	/**
	 * @brief 根据模块句柄获得模块所在路径
	 *
	 *
	 * @param hModule 模块句柄
	 * @return 
	 */
	static ATL::CString GetAppConfigPath(HANDLE hModule = NULL);

	/**
	 * @brief 获得模块名称
	 *
	 *
	 * @param hModule 模块句柄
	 * @return 
	 */
	static ATL::CString GetModuleName(HMODULE hModule = NULL);

	/**
	 * @brief 获得文件大小
	 *
	 *
	 * @param strFilePath 
	 * @return 
	 */
	static ULONG GetFileSize(const ATL::CString &strFilePath);

	/// 获得文件修改时间
	static COleDateTime GetFileWriteTime(const ATL::CString& strFilePath);

	/// 获取消息时间 time_t -> DATE
	static COleDateTime GetMsgTime(ULONG ulTotalSecond);

	//转换时间 DATE -> time_t
	static DWORD CBaseFuncLib::LotusToTime( double dt );

	/**
	 * @brief 获取文件哈希值
	 *
	 *
	 * @param strFilePath 
	 * @return 
	 */
	static ATL::CString GetFileSha(const ATL::CString& strFilePath);

	/**
	 * @brief 获得文件大小的显示文字
	 *
	 *
	 * @param dwFileSize 
	 * @param iPoint 
	 * @return 
	 */
	static ATL::CString GetShowSize(DWORD dwFileSize,int iPoint = 2);

	/**
	 * @brief 执行应用，可带命令行，控制是否等待结束，是否显示窗口
	 *
	 *
	 * @param szAppPath 
	 * @param szCommandLine 
	 * @param bWaitFlag 
	 * @param bShowFlag 
	 */
	static DWORD RunExe(LPCTSTR szAppPath,LPCTSTR szCommandLine = _T(""),BOOL bWaitFlag = TRUE,BOOL bShowFlag = TRUE);

	/**
	 * @brief URL编码到UTF-8
	 *
	 *
	 * @param strPara 
	 * @return 
	 */
	static ATL::CString UrlEncode(const ATL::CString& strPara,BOOL bFlag = TRUE);

	/**
	 * @brief URL编码到UTF-8
	 *
	 *
	 * @param strPara 
	 * @return 
	 */
	static char* UrlEncodeToChar(const ATL::CString& strPara);

	/**
	 * @brief 编码到文件
	 *
	 *
	 * @param strPara 
	 * @param hFileHandle 
	 */
	static void UrlEncodeToFile(const ATL::CString& strPara,HANDLE hFileHandle,BOOL bTryFlag =FALSE);

	/**
	 * @brief 根据窗口名字找程序窗口
	 *
	 *
	 * @param strWndName 
	 * @return 
	 */
	static HWND GetAppWnd(const ATL::CString& strWndName = _T("IExplore"),BOOL bVisibleFlag = TRUE);

	/**
	 * @brief 退出可能正在运行的指定窗口程序
	 *
	 *
	 * @param strWndName 窗口名字，缺省为退出IE程序 
	 */
	static void QuitApp(const ATL::CString& strWndName = _T("IExplore"),BOOL bVisibleFlag = TRUE);

	/**
	 * @brief 打开指定的URL
	 *
	 *
	 * @param strUrl 
	 */
	static void OpenUrl(const ATL::CString& strUrl);

	/**
	 * @brief 拷贝指定内容到系统粘贴板
	 *
	 *
	 * @param strCopy 
	 */
	static BOOL CopyStringToClipboard(const ATL::CString& strCopy);

	/**
	 * @brief 根据指定的库创建指定接口
	 *
	 *
	 * @param lpszFilename 
	 * @param clsid 
	 * @param iid 
	 * @param ppv 
	 */
	static HINSTANCE CreateInstance( LPCTSTR lpszFilename, REFCLSID clsid, REFIID iid, void ** ppv );
	
	/**
	 * @brief 根据指定库句柄创建指定接口
	 *
	 *
	 * @param hInst 
	 * @param clsid 
	 * @param iid 
	 * @param ppv 
	 */
	static void CreateInterface( HINSTANCE hInst, REFCLSID clsid, REFIID iid, void ** ppv );

	static BOOL IsProcessExist( LPCTSTR lpszProcessName,BOOL bFlag = FALSE);

	//获取系统日志相应级别的名
	static int GetCategory(int EventType,CString& EventCategory);
	//获取日志相应的类型名
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
	/// 窗口名称
	static ATL::CString	m_strWndName;
	/// 窗口是否可见
	static BOOL		m_bVisibleFlag;
	/// 窗口句柄
	static HWND		m_hWnd;

	/// 日志文件
	static ATL::CString m_strLogFile;
	/// 应用路径
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

