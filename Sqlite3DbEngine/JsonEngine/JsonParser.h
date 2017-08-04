//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/07/27  
// ><创建时间 ><  2017年:07月:27日   21时:27分:10秒  
// ><文件     ><  JsonParser.h  
// ><文件路径 ><  D:\Sqlite3DbEngine\JsonEngine  
// ><隶属工程><   
// ><当前用户 ><  Administrator  
// ><作者     ><  
// ><出处     ><     jsoncpp扩展
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.   
// ><目的     ><     
// ><         ><  1.    JSON(JavaScript Object Notation) 是一种轻量级的数据交换格式。JSON采用完全独立于语言的文本格式,这些特性使JSON成为理想的数据交换语言。易于人阅读和编写,同时也易于机器解析和生成。
// ><         ><  2.   “名称/值”对的集合(A collection of name/value pairs)。不同的语言中,它被理解为
// ><         ><  2.1  对象(object),
// ><         ><  2.2  记录(record),
// ><         ><  2.3  结构(struct),
// ><         ><  2.4  字典(dictionary),
// ><         ><  2.5  哈希表(hash table),
// ><         ><  2.6  有键列表(keyed list),
// ><         ><  2.7  关联数组 (associative array)。
// ><         ><  3.  值的有序列表(An ordered list of values)。在大部分语言中,它被理解为数组(array)。
// ><         ><  4.   
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:07月:27日   21时:27分:10秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
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

	/**
	 * @brief             SaveToFile
	 *
	 * @Function          保存到指定文件
	 *
	 * @param[  _In_   ]  strJsonFile 文件的绝对路径
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL SaveToFile(const CString& strJsonFile);

	/**
	 * @brief             Parse
	 *
	 * @Function          解析json字符串
	 *
	 * @param[  _In_   ]  pJsonContent 是json字符串,必须为utf8
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool Parse(const char* pJsonContent);

	/**
	 * @brief             ParseFile
	 *
	 * @Function          解析json文件
	 *
	 * @param[  _In_   ]  strJsonFile 文件的绝对路径
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool ParseFile(const CString& strJsonFile);

	/**
	 * @brief             GetType
	 *
	 * @Function          获得当前节点类型
	 *
	 * @param[  _In_   ]  
	 *
	 * @param[  _Out_  ]  
	 *
	 * @param[_Out_opt_]  
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	Json::ValueType GetType();

	/**
	 * @brief             GetChildTypeByName
	 *
	 * @Function          获得子节点
	 *
	 * @param[  _In_   ]  
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
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

	/**
	 * @brief             GetChildJson
	 *
	 * @Function          根据节点名称获取子节点
	 *
	 * @param[  _In_   ]  strName 节点名称
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	CJsonParser* GetChildJson(const CString& strName);

	/**
	 * @brief             GetChildJson
	 *
	 * @Function          根据序号获取子节点
	 *
	 * @param[  _In_   ]  nIndex 子节点序号
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	CJsonParser* GetChildJson(const int nIndex);

	std::vector<std::string> GetChildNames();

	HRESULT GetObjectString(BSTR* pVal);

	/// 获得错误信息
	CString GetErrInfo()
	{
		return m_strErrInfo;
	}

	/**
	 * @brief             CreateJsonDemoToFile
	 *
	 * @Function          创建Json格式内容并存储到strJsonFile.Json文件中去
	 *
	 * @param[  _In_   ]  strJsonFile 文件的绝对路径
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	 */
	BOOL CreateJsonDemoToFile(const CString& strJsonFile);

	/**
	 * @brief             CreateJsonDemoToString
	 *
	 * @Function          创建一个Json到字符串strResult中
	 *
	 * @param[  _Out_  ]  strResult 字符串将创建的Json赋值给strResult
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL CreateJsonDemoToString(BSTR* strResult);

	/**
	 * @brief             ParsesJsonFromToFile
	 *
	 * @Function          解析strJsonFile.Json文件中的Json内容
	 *
	 * @param[  _In_   ]  strJsonFile 文件的绝对路径
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL ParseJsonFromFile(const CString& strJsonFile);
	/**
	 * @brief             ParseJsonFromString
	 *
	 * @Function          解析strJsonFile字符串中Json的内容
	 *
	 * @param[  _In_   ]  strJsonFile 文件的绝对路径
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL ParseJsonFromString(const CString& strJsonFile);

	/**
	 * @brief             JsonDesignFormatTest00/1/2/3/4/5/6/7/8
	 *
	 * @Function          Json设置格式测试
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	 */
	void JsonDesignFormatTest00();
	void JsonDesignFormatTest01();
	void JsonDesignFormatTest02();
	void JsonDesignFormatTest03();
	void JsonDesignFormatTest04();
	void JsonDesignFormatTest05();
	void JsonDesignFormatTest06();
	void JsonDesignFormatTest07();
	void JsonDesignFormatTest08();

	Json::Value*  GetObjPtr(){
		return GetRootNode();
	}
	void OutputStyledJson(void);

	/**
	 * @brief             SetObjAsChild
	 *
	 * @Function          设置子节点内容，本函数只是对key和value的值进行必要的转码
	 *
	 * @param[  _In_   ]  strName key 值
	 *
	 * @param[  _In_   ]  strValue value值
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool SetObjAsChild(const CString& strName,const CString& strValue);
protected:

	/// 是否内存
	void Release();

	bool GetBoolChildByName(const char* pName);

	/**
	 * @brief             GetIntChildByName
	 *
	 * @Function          根据名字获取子节点整数值
	 *
	 * @param[  _In_   ]  pName,子节点名字
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	int GetIntChildByName(const char* pName);

	/**
	 * @brief             GetStringChildByName
	 *
	 * @Function          根据名字获取子节字符串
	 *
	 * @param[  _In_   ]  pName 子节点名字
	 *
	 * @return            返回值需调用释放
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	char* GetStringChildByName(const char* pName);

	/**
	 * @brief             GetUintChildByName
	 *
	 * @Function          根据名字获取子节点非符号整数值
	 *
	 * @param[  _In_   ]  pName 子节点名字
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	unsigned long long GetUintChildByName(const char* pName);

	/**
	 * @brief             GetInt64ChildByName
	 *
	 * @Function          根据名字获取子节点64位非符号整数值
	 *
	 * @param[  _In_   ]  pName 子节点名字
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	long long GetInt64ChildByName(const char* pName);
	/// 获取子节点浮点
	double GetDoubleChildByName(const char* pName);

	/// 获得节点类型
	Json::ValueType GetChildTypeByName(const char* pName);

	/**
	 * @brief             GetStringChildByName
	 *
	 * @Function          根据序号获取子节点字符串
	 *
	 * @param[  _In_   ]  nIndex 子节点序号
	 *
	 * @return            返回值需释放
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	char* GetStringChildByName(const int nIndex);

	/**
	 * @brief             GetBoolChildByIndex
	 *
	 * @Function          根据序号获取子节点布尔值
	 *
	 * @param[  _In_   ]  nIndex 子节点序号
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool GetBoolChildByIndex(const int nIndex);

	/**
	 * @brief             GetIntChildByIndex
	 *
	 * @Function          根据序号获取子节点整数值
	 *
	 * @param[  _In_   ]  nIndex 子节点序号
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	int	GetIntChildByIndex(const int nIndex);

	/**
	 * @brief             GetUintChildByIndex
	 *
	 * @Function          根据序号获取子节点非符号整数值
	 *
	 * @param[  _In_   ]  nIndex 子节点序号
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	unsigned long long GetUintChildByIndex(const int nIndex);

	/**
	 * @brief             GetInt64ChildByIndex
	 *
	 * @Function          根据序号获取子节点64位非符号整数值
	 *
	 * @param[  _In_   ]  nIndex 子节点序号
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	long long GetInt64ChildByIndex(const int nIndex);

	/**
	 * @brief             GetChildNodeByIndex
	 *
	 * @Function          根据序号获得子节点
	 *
	 * @param[  _In_   ]  nIndex 节点序号
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	Json::Value* GetChildNodeByIndex(const int nIndex);

	/**
	 * @brief             GetChildJson
	 *
	 * @Function          根据名字获取子节点
	 *
	 * @param[  _In_   ]  pName 子节点名字
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	CJsonParser* GetChildJson(const char* pName); 

	/**
	 * @brief             GetChildNode
	 *
	 * @Function          获得子节点
	 *
	 * @param[  _In_   ]  pName 子节点名字
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	 */
	Json::Value* GetChildNode(const char* pName);
	Json::Value* GetRootNode(const char* pName = NULL);

	/**
	 * @brief             SetObjAsChild
	 *
	 * @Function          设置子节点内容
	 *
	 * @param[  _In_   ]  pName key名称
	 *
	 * @param[  _In_   ]  pValue value字符串，到SetChild函数内部在重新组装
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool SetObjAsChild(const char* pName,const char* pValue);

	/**
	 * @brief             GetReadReader
	 *
	 * @Function          获得只读解析器
	 *
	 * @param[  _In_   ]  pRoot根节点
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	static CJsonParser* GetReadReader(Json::Value* pRoot);

protected:	
	//  json文件根节点
	Json::Value* m_pRoot;

	// 临时对象
	Json::Value m_TempValue;

	// 是否需要自己管理内存释放
	bool m_bDeleteSelf;

	// 编码类型
	ECodingType	m_eCodingType;

	// 错误描述
	CString	m_strErrInfo;
};