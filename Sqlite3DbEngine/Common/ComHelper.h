#pragma once


class CComHelper
{
protected:
	/// 模块句柄集合
	static STRINGHANDLE_MAP	m_mapInstance;

	/// 是否代码保护
	static SHORT		m_nCodeProtected;

public:
	CComHelper(void)
	{
	}

	/// 获得模块临时路径
	static CString GetComTempPath(BOOL bCreateFlag = TRUE);

	/// 获得模块安装路径
	static CString GetAppInsPath();

	static HINSTANCE GetComHandle(const ATL::CString& strModelName);

	/**
	 * @brief 释放指定模块
	 *
	 *
	 */
	static void FreeHandle(const ATL::CString& strModelName);

	/**
	 * @brief 释放所有模块
	 *
	 *
	 */
	static void FreeAllHandle();

	/// 获得代码保护标记
	static SHORT GetSourceProtected();
};