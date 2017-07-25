#pragma once
#include "json.h"
#include "BaseFuncLib.h"
#include "JsonEngine_i.h"

/**
*
*
* @Function List 
*
* @author WZH
*
* @Date 2011-09-25
*
*
* @brief json解析包装类
*
* @ingroup WeiboEngine
**/

class CJsonParser
{
public:
	CJsonParser(Json::Value* pRoot = NULL);
	~CJsonParser();

	void PutCodingType(ECodingType eCodingType)
	{
		m_eCodingType = eCodingType;
	}

	ECodingType GetCodingType()
	{
		return m_eCodingType;
	}

	bool GetValue(VARIANT *pVal);

	ULONG GetArraySize();

	bool IsMember(const CString& strKeyName);

	bool SetIntValue(const CString& strKeyName, LONGLONG lnVal);
	bool SetStringValue(const CString& strKeyName, const CString& strValue);
	bool SetValue(const CString& strKeyName, CComVariant& varValue);

	CJsonParser* CreateChildJson(const CString& strKeyName);

	/// 保存到指定文件
	BOOL SaveToFile(const CString& strJsonFile);

	/// 解析json字符串
	/*
	@param	pJsonContent json字符串，必须为utf8
	*/
	bool Parse(const char* pJsonContent);

	/// 解析json文件
	/*
	@param	strJsonFile
	*/
	bool ParseFile(const CString& strJsonFile);

	/// 获得当前节点类型
	Json::ValueType GetType();

	/// 获得子节点
	Json::ValueType GetChildTypeByName(const CString& strName);

	CString GetStringChildByName(const CString& strName);

	bool GetBoolChildByName(const CString& strName);

	int GetIntChildByName(const CString& strName);

	unsigned long long GetUIntChildByName(const CString& strName);

	long long GetLongLongChildByName(const CString& strName);

	double GetDoubleChildByName(const CString& strName);

	bool GetVariantChildByName(const CString& strName,CComVariant& varVal);
	bool GetVariantChildByKey(char *szName,CComVariant& varVal);

	bool GetArrayValue(short sIndex,CComBSTR& bstrKeyName,CComVariant& varVal);

	/// 根据节点名称获取子节点
	/*
	@param	nIndex 子节点序号
	*/
	CJsonParser* GetChildJson(const CString& strName);

	/// 根据序号获取子节点
	/*
	@param	nIndex 子节点序号
	*/
	CJsonParser* GetChildJson(const int nIndex);

	/// 设置子节点
	bool SetChild(const CString& strName,const CString& strValue);

	std::vector<std::string> GetChildNames();

	HRESULT GetObjectString(BSTR* pVal);

	/// 获得错误信息
	CString GetErrInfo()
	{
		return m_strErrInfo;
	}

	BOOL CreateJsonDemoToFile(const CString& strJsonFile);
protected:

	/// 是否内存
	void Release();

	bool GetBoolChildByName(const char* pName);

	/// 根据名字获取子节点整数值
	/*
	@param	pName，子节点名字
	*/
	int	GetIntChildByName(const char* pName);

	/// 根据名字获取子节字符串
	/*
	@param	pName 子节点名字
	*/
	/*返回值需调用释放*/
	char* GetStringChildByName(const char* pName);

	/// 根据名字获取子节点非符号整数值
	/*
	@param	pName	in，子节点名字
	*/
	unsigned long long GetUintChildByName(const char* pName);

	/// 根据名字获取子节点64位非符号整数值
	/*
	@param	pName	in，子节点名字
	*/
	long long GetInt64ChildByName(const char* pName);
	/// 获取子节点浮点
	double GetDoubleChildByName(const char* pName);

	/// 获得节点类型
	Json::ValueType GetChildTypeByName(const char* pName);

	/// 根据序号获取子节点字符串
	/*
	@param	nIndex 子节点序号
	*/
	/*返回值需释放*/
	char* GetStringChildByName(const int nIndex);

	/// 根据序号获取子节点布尔值
	/*
	@param	nIndex	in，子节点序号
	*/
	bool GetBoolChildByIndex(const int nIndex);

	/// 根据序号获取子节点整数值
	/*
	@param	nIndex	in，子节点序号
	*/
	int	GetIntChildByIndex(const int nIndex);

	/// 根据序号获取子节点非符号整数值
	/*
	@param	nIndex，子节点序号
	*/
	unsigned long long GetUintChildByIndex(const int nIndex);

	/// 根据序号获取子节点64位非符号整数值
	/*
	@param	nIndex，子节点序号
	*/
	long long GetInt64ChildByIndex(const int nIndex);

	/// 根据序号获得子节点
	Json::Value* GetChildNodeByIndex(const int nIndex);

	/// 根据名字获取子节点
	/*
	@param	pName，子节点名字
	*/
	CJsonParser* GetChildJson(const char* pName); 

	/// 获得子节点
	Json::Value* GetChildNode(const char* pName);

	/// 设置子节点内容
	bool SetChild(const char* pName,const char* pValue);

	/// 获得只读解析器
	static CJsonParser* GetReadReader(Json::Value* pRoot);

protected:	
	///  json文件根节点
	Json::Value* m_pRoot;

	/// 临时对象
	Json::Value m_TempValue;

	///  是否需要自己管理内存释放
	bool m_bDeleteSelf;

	/// 编码类型
	ECodingType	m_eCodingType;

	/// 错误描述
	CString	m_strErrInfo;
};