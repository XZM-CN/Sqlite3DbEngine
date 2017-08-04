//////////////////////////////////������Ϣ///////////////////////////////////////////////////////  
// ><�������� ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><�������� ><  2017/07/27  
// ><����ʱ�� ><  2017��:07��:27��   21ʱ:27��:10��  
// ><�ļ�     ><  JsonParser.h  
// ><�ļ�·�� ><  D:\Sqlite3DbEngine\JsonEngine  
// ><��������><   
// ><��ǰ�û� ><  Administrator  
// ><����     ><  
// ><����     ><     jsoncpp��չ
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.   
// ><Ŀ��     ><     
// ><         ><  1.    JSON(JavaScript Object Notation) ��һ�������������ݽ�����ʽ��JSON������ȫ���������Ե��ı���ʽ,��Щ����ʹJSON��Ϊ��������ݽ������ԡ��������Ķ��ͱ�д,ͬʱҲ���ڻ������������ɡ�
// ><         ><  2.   ������/ֵ���Եļ���(A collection of name/value pairs)����ͬ��������,�������Ϊ
// ><         ><  2.1  ����(object),
// ><         ><  2.2  ��¼(record),
// ><         ><  2.3  �ṹ(struct),
// ><         ><  2.4  �ֵ�(dictionary),
// ><         ><  2.5  ��ϣ��(hash table),
// ><         ><  2.6  �м��б�(keyed list),
// ><         ><  2.7  �������� (associative array)��
// ><         ><  3.  ֵ�������б�(An ordered list of values)���ڴ󲿷�������,�������Ϊ����(array)��
// ><         ><  4.   
// ><��Ƽ��� ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////�����޸�///////////////////////////////////////////////////////  
// ><����     ><  xzm  
// ><�޸����� ><  2017��:07��:27��   21ʱ:27��:10��  
// ><ԭ��     ><    
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
* @brief json������װ��
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
	 * @Function          ���浽ָ���ļ�
	 *
	 * @param[  _In_   ]  strJsonFile �ļ��ľ���·��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL SaveToFile(const CString& strJsonFile);

	/**
	 * @brief             Parse
	 *
	 * @Function          ����json�ַ���
	 *
	 * @param[  _In_   ]  pJsonContent ��json�ַ���,����Ϊutf8
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool Parse(const char* pJsonContent);

	/**
	 * @brief             ParseFile
	 *
	 * @Function          ����json�ļ�
	 *
	 * @param[  _In_   ]  strJsonFile �ļ��ľ���·��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool ParseFile(const CString& strJsonFile);

	/**
	 * @brief             GetType
	 *
	 * @Function          ��õ�ǰ�ڵ�����
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
	 * @Function          ����ӽڵ�
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
	 * @Function          ���ݽڵ����ƻ�ȡ�ӽڵ�
	 *
	 * @param[  _In_   ]  strName �ڵ�����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	CJsonParser* GetChildJson(const CString& strName);

	/**
	 * @brief             GetChildJson
	 *
	 * @Function          ������Ż�ȡ�ӽڵ�
	 *
	 * @param[  _In_   ]  nIndex �ӽڵ����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	CJsonParser* GetChildJson(const int nIndex);

	std::vector<std::string> GetChildNames();

	HRESULT GetObjectString(BSTR* pVal);

	/// ��ô�����Ϣ
	CString GetErrInfo()
	{
		return m_strErrInfo;
	}

	/**
	 * @brief             CreateJsonDemoToFile
	 *
	 * @Function          ����Json��ʽ���ݲ��洢��strJsonFile.Json�ļ���ȥ
	 *
	 * @param[  _In_   ]  strJsonFile �ļ��ľ���·��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	 */
	BOOL CreateJsonDemoToFile(const CString& strJsonFile);

	/**
	 * @brief             CreateJsonDemoToString
	 *
	 * @Function          ����һ��Json���ַ���strResult��
	 *
	 * @param[  _Out_  ]  strResult �ַ�����������Json��ֵ��strResult
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL CreateJsonDemoToString(BSTR* strResult);

	/**
	 * @brief             ParsesJsonFromToFile
	 *
	 * @Function          ����strJsonFile.Json�ļ��е�Json����
	 *
	 * @param[  _In_   ]  strJsonFile �ļ��ľ���·��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL ParseJsonFromFile(const CString& strJsonFile);
	/**
	 * @brief             ParseJsonFromString
	 *
	 * @Function          ����strJsonFile�ַ�����Json������
	 *
	 * @param[  _In_   ]  strJsonFile �ļ��ľ���·��
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	BOOL ParseJsonFromString(const CString& strJsonFile);

	/**
	 * @brief             JsonDesignFormatTest00/1/2/3/4/5/6/7/8
	 *
	 * @Function          Json���ø�ʽ����
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
	 * @Function          �����ӽڵ����ݣ�������ֻ�Ƕ�key��value��ֵ���б�Ҫ��ת��
	 *
	 * @param[  _In_   ]  strName key ֵ
	 *
	 * @param[  _In_   ]  strValue valueֵ
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool SetObjAsChild(const CString& strName,const CString& strValue);
protected:

	/// �Ƿ��ڴ�
	void Release();

	bool GetBoolChildByName(const char* pName);

	/**
	 * @brief             GetIntChildByName
	 *
	 * @Function          �������ֻ�ȡ�ӽڵ�����ֵ
	 *
	 * @param[  _In_   ]  pName,�ӽڵ�����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	int GetIntChildByName(const char* pName);

	/**
	 * @brief             GetStringChildByName
	 *
	 * @Function          �������ֻ�ȡ�ӽ��ַ���
	 *
	 * @param[  _In_   ]  pName �ӽڵ�����
	 *
	 * @return            ����ֵ������ͷ�
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	char* GetStringChildByName(const char* pName);

	/**
	 * @brief             GetUintChildByName
	 *
	 * @Function          �������ֻ�ȡ�ӽڵ�Ƿ�������ֵ
	 *
	 * @param[  _In_   ]  pName �ӽڵ�����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	unsigned long long GetUintChildByName(const char* pName);

	/**
	 * @brief             GetInt64ChildByName
	 *
	 * @Function          �������ֻ�ȡ�ӽڵ�64λ�Ƿ�������ֵ
	 *
	 * @param[  _In_   ]  pName �ӽڵ�����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	long long GetInt64ChildByName(const char* pName);
	/// ��ȡ�ӽڵ㸡��
	double GetDoubleChildByName(const char* pName);

	/// ��ýڵ�����
	Json::ValueType GetChildTypeByName(const char* pName);

	/**
	 * @brief             GetStringChildByName
	 *
	 * @Function          ������Ż�ȡ�ӽڵ��ַ���
	 *
	 * @param[  _In_   ]  nIndex �ӽڵ����
	 *
	 * @return            ����ֵ���ͷ�
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	char* GetStringChildByName(const int nIndex);

	/**
	 * @brief             GetBoolChildByIndex
	 *
	 * @Function          ������Ż�ȡ�ӽڵ㲼��ֵ
	 *
	 * @param[  _In_   ]  nIndex �ӽڵ����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool GetBoolChildByIndex(const int nIndex);

	/**
	 * @brief             GetIntChildByIndex
	 *
	 * @Function          ������Ż�ȡ�ӽڵ�����ֵ
	 *
	 * @param[  _In_   ]  nIndex �ӽڵ����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	int	GetIntChildByIndex(const int nIndex);

	/**
	 * @brief             GetUintChildByIndex
	 *
	 * @Function          ������Ż�ȡ�ӽڵ�Ƿ�������ֵ
	 *
	 * @param[  _In_   ]  nIndex �ӽڵ����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	unsigned long long GetUintChildByIndex(const int nIndex);

	/**
	 * @brief             GetInt64ChildByIndex
	 *
	 * @Function          ������Ż�ȡ�ӽڵ�64λ�Ƿ�������ֵ
	 *
	 * @param[  _In_   ]  nIndex �ӽڵ����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	long long GetInt64ChildByIndex(const int nIndex);

	/**
	 * @brief             GetChildNodeByIndex
	 *
	 * @Function          ������Ż���ӽڵ�
	 *
	 * @param[  _In_   ]  nIndex �ڵ����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	Json::Value* GetChildNodeByIndex(const int nIndex);

	/**
	 * @brief             GetChildJson
	 *
	 * @Function          �������ֻ�ȡ�ӽڵ�
	 *
	 * @param[  _In_   ]  pName �ӽڵ�����
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	CJsonParser* GetChildJson(const char* pName); 

	/**
	 * @brief             GetChildNode
	 *
	 * @Function          ����ӽڵ�
	 *
	 * @param[  _In_   ]  pName �ӽڵ�����
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
	 * @Function          �����ӽڵ�����
	 *
	 * @param[  _In_   ]  pName key����
	 *
	 * @param[  _In_   ]  pValue value�ַ�������SetChild�����ڲ���������װ
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	bool SetObjAsChild(const char* pName,const char* pValue);

	/**
	 * @brief             GetReadReader
	 *
	 * @Function          ���ֻ��������
	 *
	 * @param[  _In_   ]  pRoot���ڵ�
	 *
	 * @return            
	 *
	 * @Date xzm_@_2017/06/30  13:40:11
	*/
	static CJsonParser* GetReadReader(Json::Value* pRoot);

protected:	
	//  json�ļ����ڵ�
	Json::Value* m_pRoot;

	// ��ʱ����
	Json::Value m_TempValue;

	// �Ƿ���Ҫ�Լ������ڴ��ͷ�
	bool m_bDeleteSelf;

	// ��������
	ECodingType	m_eCodingType;

	// ��������
	CString	m_strErrInfo;
};