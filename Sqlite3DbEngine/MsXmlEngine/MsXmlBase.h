#pragma once
#include <tchar.h>
#include<msxml2.h>



#pragma warning(disable:4278)
#pragma warning(disable:4482)

// ����MSXML������
#import "msxml//msxml4.dll"

using namespace MSXML2;


/*
#pragma warning( disable : 4507 34; once : 4385; error : 164 )

1.��һ��warning��Ϊһ������
#pragma warning (error: 6260)

2.��һ��warning���õ�
#pragma warning (disable: 6011) 

3.��һ�������õ�warning����
#pragma warning (enable: 6011)
*/
#pragma warning( disable: 4428 )

#define IXML
//#undef  IXML
#define XMLNS       _T("XML Namespaces(XML�������ռ�)")
#define XMLNOTE     _T("Extensible Markup Language(����չ�ñ�����)")



#ifdef IXML
	// xml�ĵ�
	typedef MSXML2::IXMLDOMDocumentPtr              IXZMDocumentPtr;
	typedef MSXML2::IXMLDOMDocument                 IXZMDocument;
	// xml����
	typedef MSXML2::IXMLDOMProcessingInstructionPtr IXZMProcessingInstructionPtr;
	typedef MSXML2::IXMLDOMProcessingInstruction    IXZMProcessingInstruction;
	// ע��
	typedef MSXML2::IXMLDOMCommentPtr               IXZMCommentPtr;
	typedef MSXML2::IXMLDOMComment                  IXZMComment;
	// ���ڵ�(Ԫ��)
	typedef MSXML2::IXMLDOMElementPtr               IXZMElementPtr;
	typedef MSXML2::IXMLDOMElement                  IXZMElement;
	// �ڵ�
	typedef MSXML2::IXMLDOMNodePtr                  IXZMNodePtr;
	typedef MSXML2::IXMLDOMNode                     IXZMNode;
	// ����
	typedef MSXML2::IXMLDOMAttributePtr             IXZMAttributePtr;
	typedef MSXML2::IXMLDOMAttribute                IXZMAttribute;
	//
	typedef MSXML2::DOMNodeType                     XZMNodeType;
#else
	/*
	// xml�ĵ�
	#define IXZMDocumentPtr              MSXML2::IXMLDOMDocumentPtr
	#define IXZMDocument                 MSXML2::IXMLDOMDocument
	// xml����
	#define IXZMProcessingInstructionPtr MSXML2::IXMLDOMProcessingInstructionPtr
	#define IXZMProcessingInstruction    MSXML2::IXMLDOMProcessingInstruction
	// ע��
	#define IXZMCommentPtr               MSXML2::IXMLDOMCommentPtr
	#define IXZMComment                  MSXML2::IXMLDOMComment
	// ���ڵ�(Ԫ��)
	#define IXZMElementPtr               MSXML2::IXMLDOMElementPtr
	#define IXZMElement                  MSXML2::IXMLDOMElement
	// �ڵ�
	#define IXZMNodePtr                  MSXML2::IXMLDOMNodePtr
	#define IXZMNode                     MSXML2::IXMLDOMNode
	// ����
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

	// �ĵ��ӿ�ʵ����
	BOOL CreateDoc(IXZMDocumentPtr& pIXZMDocumentPtr);
	// ��������
	BOOL CreateStatement(IXZMDocumentPtr& pIXZMDocumentPtr, TCHAR* pstrTargetName,TCHAR* pstrData);
	// ����Ԫ��
	IXZMElementPtr CreateElement(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pstrElemName,TCHAR* pstrElemValue = _T(""));
	// �������ڵ�
	IXZMElementPtr CreateRoot(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pstrRootName);
	// �����ڵ�
	IXZMNodePtr CreateNode(IXZMDocumentPtr& pIXZMDocumentPtr, TCHAR* pstrNodeName,TCHAR* pstrNodeText,TCHAR* pstrXmlnsName = L"");
	// ����ע��
	IXZMCommentPtr CreateComment(IXZMDocumentPtr& pIXZMDocumentPtr,TCHAR* pCommentName);
	// ��������
	IXZMAttributePtr CreateAttr(IXZMDocumentPtr& pIXZMDocumentPtr, TCHAR* pAttrName,TCHAR* pAttrValue);


public:
	static int m_COMCount;
};