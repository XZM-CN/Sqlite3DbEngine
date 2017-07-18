#pragma once


/// ����๤��������������
typedef HRESULT ( __stdcall LPGetClassObject)( REFCLSID, REFIID, LPVOID* );

class CPublicFunc
{
public:
	CPublicFunc(void);
	~CPublicFunc(void);

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
	 * @brief ���������·��
	 *
	 *
	 * @return 
	 */
	static CString GetAppDirByRegistry(const CString& strRegPath = _T(""), HKEY hKey = HKEY_CURRENT_USER);
	
	/**
	 * @brief ���������·��
	 *
	 *
	 * @return 
	 */
	static CString GetAppDirByModular();
	
	/**
	 * @brief ���������ضϵ�·��
	 *
	 *
	 * @return 
	 */
	static CString GetAppTruncDirByModular();

	/**
	 * @brief �ж�·���ͷŴ���
	 *
	 *
	 * @param strPath 
	 * @return 
	 */
	static BOOL IsPathExist(const CString& strPath) throw();
};

