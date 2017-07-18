#pragma once


/// 获得类工厂创建函数定义
typedef HRESULT ( __stdcall LPGetClassObject)( REFCLSID, REFIID, LPVOID* );

class CPublicFunc
{
public:
	CPublicFunc(void);
	~CPublicFunc(void);

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
	 * @brief 获得主程序路径
	 *
	 *
	 * @return 
	 */
	static CString GetAppDirByRegistry(const CString& strRegPath = _T(""), HKEY hKey = HKEY_CURRENT_USER);
	
	/**
	 * @brief 获得主程序路径
	 *
	 *
	 * @return 
	 */
	static CString GetAppDirByModular();
	
	/**
	 * @brief 获得主程序截断的路径
	 *
	 *
	 * @return 
	 */
	static CString GetAppTruncDirByModular();

	/**
	 * @brief 判断路径释放存在
	 *
	 *
	 * @param strPath 
	 * @return 
	 */
	static BOOL IsPathExist(const CString& strPath) throw();
};

