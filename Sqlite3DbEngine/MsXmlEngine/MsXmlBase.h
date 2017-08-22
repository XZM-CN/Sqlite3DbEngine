#pragma once
#include <tchar.h>
#include<msxml2.h>



#pragma warning(disable:4278)
#pragma warning(disable:4482)

// 引入MSXML解析器
#import "msxml//msxml4.dll"

using namespace MSXML2;


/*
#pragma warning( disable : 4507 34; once : 4385; error : 164 )

1.将一个warning作为一个错误
#pragma warning (error: 6260)

2.将一个warning禁用掉
#pragma warning (disable: 6011) 

3.将一个被禁用的warning启用
#pragma warning (enable: 6011)
*/
#pragma warning( disable: 4428 )

#define IXML
//#undef  IXML
#define XMLNS       _T("XML Namespaces(XML的命名空间)")
#define XMLNOTE     _T("Extensible Markup Language(可扩展置标语言)")



#ifdef IXML
	// xml文档
	typedef MSXML2::IXMLDOMDocumentPtr              IXZMDocumentPtr;
	typedef MSXML2::IXMLDOMDocument                 IXZMDocument;
	// xml声明
	typedef MSXML2::IXMLDOMProcessingInstructionPtr IXZMProcessingInstructionPtr;
	typedef MSXML2::IXMLDOMProcessingInstruction    IXZMProcessingInstruction;
	// 注释
	typedef MSXML2::IXMLDOMCommentPtr               IXZMCommentPtr;
	typedef MSXML2::IXMLDOMComment                  IXZMComment;
	// 根节点(元素)
	typedef MSXML2::IXMLDOMElementPtr               IXZMElementPtr;
	typedef MSXML2::IXMLDOMElement                  IXZMElement;
	// 节点
	typedef MSXML2::IXMLDOMNodePtr                  IXZMNodePtr;
	typedef MSXML2::IXMLDOMNode                     IXZMNode;
	// 属性
	typedef MSXML2::IXMLDOMAttributePtr             IXZMAttributePtr;
	typedef MSXML2::IXMLDOMAttribute                IXZMAttribute;
	//
	typedef MSXML2::DOMNodeType                     XZMNodeType;
#else
	/*
	// xml文档
	#define IXZMDocumentPtr              MSXML2::IXMLDOMDocumentPtr
	#define IXZMDocument                 MSXML2::IXMLDOMDocument
	// xml声明
	#define IXZMProcessingInstructionPtr MSXML2::IXMLDOMProcessingInstructionPtr
	#define IXZMProcessingInstruction    MSXML2::IXMLDOMProcessingInstruction
	// 注释
	#define IXZMCommentPtr               MSXML2::IXMLDOMCommentPtr
	#define IXZMComment                  MSXML2::IXMLDOMComment
	// 根节点(元素)
	#define IXZMElementPtr               MSXML2::IXMLDOMElementPtr
	#define IXZMElement                  MSXML2::IXMLDOMElement
	// 节点
	#define IXZMNodePtr                  MSXML2::IXMLDOMNodePtr
	#define IXZMNode                     MSXML2::IXMLDOMNode
	// 属性
	#define IXZMAttributePtr             MSXML2::IXMLDOMAttributePtr
	#define IXZMAttribute                MSXML2::IXMLDOMAttribute
	//
	#define XZMNodeType                  MSXML2::DOMNodeType
	*/
#endif


class  CMsXmlBase{
public:
	CMsXmlBase();
	~CMsXmlBase();

	int createXml(TCHAR* filePath);
	int createXmlEx(TCHAR* filePath);
	//
	int CreateDemo01(TCHAR* filePath);
	//
	int CreateDemo02(TCHAR* filePath);
	//
	int CreateDemo03(TCHAR* filePath);
	//
	int CreateDemo04(TCHAR* filePath);
	//
	int CreateDemo05(TCHAR* filePath);
	//
	int CreateDemo06(TCHAR* filePath);
	//
	int CreateDemo07(TCHAR* filePath);
	//
	int CreateDemo08(TCHAR* filePath);
	//
	int CreateDemo09(TCHAR* filePath);
	//
	void OpenXml(TCHAR* filePath);
	//
	void SystemCfg(TCHAR* filePath);

	// 文档接口实例化
	BOOL CreateDoc(IXZMDocumentPtr& pIXZMDocumentPtr);
	// 创建声明
	BOOL CreateStatement(IXZMDocumentPtr& pIXZMDocumentPtr, TCHAR* pstrTargetName,TCHAR* pstrData);
	// 创建元素
	IXZMElementPtr CreateElement(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pstrElemName,TCHAR* pstrElemValue = _T(""));
	// 创建根节点
	IXZMElementPtr CreateRoot(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pstrRootName);
	// 创建节点
	IXZMNodePtr CreateNode(IXZMDocumentPtr& pIXZMDocumentPtr, TCHAR* pstrNodeName,TCHAR* pstrNodeText,TCHAR* pstrXmlnsName = L"");
	// 创建注释
	IXZMCommentPtr CreateComment(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pCommentName);
	// 创建属性
	IXZMAttributePtr CreateAttr(IXZMDocumentPtr& pIXZMDocumentPtr, TCHAR* pAttrName,TCHAR* pAttrValue);


public:
	static int m_COMCount;
};