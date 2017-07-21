

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Jul 21 14:25:42 2017
 */
/* Compiler settings for JsonEngine.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __JsonEngine_i_h__
#define __JsonEngine_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IJsonService_FWD_DEFINED__
#define __IJsonService_FWD_DEFINED__
typedef interface IJsonService IJsonService;
#endif 	/* __IJsonService_FWD_DEFINED__ */


#ifndef __IParaService_FWD_DEFINED__
#define __IParaService_FWD_DEFINED__
typedef interface IParaService IParaService;
#endif 	/* __IParaService_FWD_DEFINED__ */


#ifndef __JsonService_FWD_DEFINED__
#define __JsonService_FWD_DEFINED__

#ifdef __cplusplus
typedef class JsonService JsonService;
#else
typedef struct JsonService JsonService;
#endif /* __cplusplus */

#endif 	/* __JsonService_FWD_DEFINED__ */


#ifndef __ParaService_FWD_DEFINED__
#define __ParaService_FWD_DEFINED__

#ifdef __cplusplus
typedef class ParaService ParaService;
#else
typedef struct ParaService ParaService;
#endif /* __cplusplus */

#endif 	/* __ParaService_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_JsonEngine_0000_0000 */
/* [local] */ 

typedef 
enum ESqliteDataType
    {	SQLITEDB_DATATYPE_INTEGER	= 1,
	SQLITEDB_DATATYPE_FLOAT	= ( SQLITEDB_DATATYPE_INTEGER + 1 ) ,
	SQLITEDB_DATATYPE_TEXT	= ( SQLITEDB_DATATYPE_FLOAT + 1 ) ,
	SQLITEDB_DATATYPE_BLOB	= ( SQLITEDB_DATATYPE_TEXT + 1 ) ,
	SQLITEDB_DATATYPE_NULL	= ( SQLITEDB_DATATYPE_BLOB + 1 ) 
    } 	EDBDataType;

typedef 
enum EHXCodingType
    {	CODINGTYPE_UNKNOWN	= 0,
	CODINGTYPE_ANSI	= ( CODINGTYPE_UNKNOWN + 1 ) ,
	CODINGTYPE_US2	= ( CODINGTYPE_ANSI + 1 ) ,
	CODINGTYPE_UTF8	= ( CODINGTYPE_US2 + 1 ) ,
	CODINGTYPE_UTF16	= ( CODINGTYPE_UTF8 + 1 ) 
    } 	ECodingType;

typedef 
enum EHXJsonType
    {	JSONTYPE_NULL	= 0,
	JSONTYPE_INT	= ( JSONTYPE_NULL + 1 ) ,
	JSONTYPE_UINT	= ( JSONTYPE_INT + 1 ) ,
	JSONTYPE_REAL	= ( JSONTYPE_UINT + 1 ) ,
	JSONTYPE_STRING	= ( JSONTYPE_REAL + 1 ) ,
	JSONTYPE_BOOL	= ( JSONTYPE_STRING + 1 ) ,
	JSONTYPE_ARRAY	= ( JSONTYPE_BOOL + 1 ) ,
	JSONTYPE_OBJECT	= ( JSONTYPE_ARRAY + 1 ) 
    } 	EJsonType;

typedef 
enum EHXSafeLogType
    {	SAFELOGTYPE_UNKNOWN	= 0,
	SAFELOGTYPE_UNWHITEFILEDENY	= 0x4,
	SAFELOGTYPE_UNWHITEFILERUN	= 0x5,
	SAFELOGTYPE_WHITEFILERUN	= 0x6,
	SAFELOGTYPE_PROTECTEXE	= 0x8,
	SAFELOGTYPE_USBACCESS	= 0x9,
	SAFELOGTYPE_PROTECTDIR	= 0x10,
	SAFELOGTYPE_PROTECTREG	= 0x11,
	SAFELOGTYPE_ALERTDISK	= 0x3,
	SAFELOGTYPE_USBOPT_CREATEFILE	= 0x80,
	SAFELOGTYPE_USBOPT_WRITEFILE	= 0x81,
	SAFELOGTYPE_USBOPT_DELETEFILE	= 0x82,
	SAFELOGTYPE_USBOPT_RENAMEFILE	= 0x83,
	SAFELOGTYPE_SELFRUN	= 4096,
	SAFELOGTYPE_OPERATION	= 8192
    } 	ESafeLogType;




extern RPC_IF_HANDLE __MIDL_itf_JsonEngine_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_JsonEngine_0000_0000_v0_0_s_ifspec;

#ifndef __IJsonService_INTERFACE_DEFINED__
#define __IJsonService_INTERFACE_DEFINED__

/* interface IJsonService */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IJsonService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F8809EDC-3A88-42FC-BF64-85CA00205B91")
    IJsonService : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_CodingType( 
            /* [retval][out] */ ECodingType *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_CodingType( 
            /* [in] */ ECodingType newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_FilePath( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_FilePath( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ParseString( 
            /* [in] */ BSTR bstrContent,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ParseFile( 
            /* [in] */ BSTR bstrFilePath,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ EJsonType *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_IntValue( 
            /* [in] */ BSTR bstrKeyName,
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_StringValue( 
            /* [in] */ BSTR bstrKeyName,
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ChildValue( 
            BSTR bstrKeyName,
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ArraySize( 
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_IsExist( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ObjectString( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrInfo( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetValueType( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ EJsonType *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetStringValue( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetBoolValue( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetIntValue( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetUIntValue( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ ULONGLONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetDoubleValue( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ DOUBLE *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetVariantValue( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetArrayValue( 
            /* [in] */ SHORT sIndex,
            /* [out] */ BSTR *pKey,
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetChildByName( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ IJsonService **pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetChildByIndex( 
            /* [in] */ LONG nIndex,
            /* [retval][out] */ IJsonService **pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CreateChild( 
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ IJsonService **pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE PutChild( 
            /* [in] */ BSTR bstrKeyName,
            /* [in] */ IJsonService *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetString( 
            /* [retval][out] */ IParaService **pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Save( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IJsonServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IJsonService * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IJsonService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IJsonService * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IJsonService * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IJsonService * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IJsonService * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IJsonService * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CodingType )( 
            IJsonService * This,
            /* [retval][out] */ ECodingType *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CodingType )( 
            IJsonService * This,
            /* [in] */ ECodingType newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FilePath )( 
            IJsonService * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FilePath )( 
            IJsonService * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ParseString )( 
            IJsonService * This,
            /* [in] */ BSTR bstrContent,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ParseFile )( 
            IJsonService * This,
            /* [in] */ BSTR bstrFilePath,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IJsonService * This,
            /* [retval][out] */ EJsonType *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            IJsonService * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IntValue )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [in] */ LONGLONG newVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_StringValue )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [in] */ BSTR newVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ChildValue )( 
            IJsonService * This,
            BSTR bstrKeyName,
            /* [in] */ VARIANT newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ArraySize )( 
            IJsonService * This,
            /* [retval][out] */ ULONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsExist )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ObjectString )( 
            IJsonService * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrInfo )( 
            IJsonService * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetValueType )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ EJsonType *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetStringValue )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetBoolValue )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetIntValue )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetUIntValue )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ ULONGLONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetDoubleValue )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ DOUBLE *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetVariantValue )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetArrayValue )( 
            IJsonService * This,
            /* [in] */ SHORT sIndex,
            /* [out] */ BSTR *pKey,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetChildByName )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ IJsonService **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetChildByIndex )( 
            IJsonService * This,
            /* [in] */ LONG nIndex,
            /* [retval][out] */ IJsonService **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *CreateChild )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [retval][out] */ IJsonService **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *PutChild )( 
            IJsonService * This,
            /* [in] */ BSTR bstrKeyName,
            /* [in] */ IJsonService *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IJsonService * This,
            /* [retval][out] */ IParaService **pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IJsonService * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IJsonService * This);
        
        END_INTERFACE
    } IJsonServiceVtbl;

    interface IJsonService
    {
        CONST_VTBL struct IJsonServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IJsonService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IJsonService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IJsonService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IJsonService_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IJsonService_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IJsonService_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IJsonService_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IJsonService_get_CodingType(This,pVal)	\
    ( (This)->lpVtbl -> get_CodingType(This,pVal) ) 

#define IJsonService_put_CodingType(This,newVal)	\
    ( (This)->lpVtbl -> put_CodingType(This,newVal) ) 

#define IJsonService_get_FilePath(This,pVal)	\
    ( (This)->lpVtbl -> get_FilePath(This,pVal) ) 

#define IJsonService_put_FilePath(This,newVal)	\
    ( (This)->lpVtbl -> put_FilePath(This,newVal) ) 

#define IJsonService_ParseString(This,bstrContent,pVal)	\
    ( (This)->lpVtbl -> ParseString(This,bstrContent,pVal) ) 

#define IJsonService_ParseFile(This,bstrFilePath,pVal)	\
    ( (This)->lpVtbl -> ParseFile(This,bstrFilePath,pVal) ) 

#define IJsonService_get_Type(This,pVal)	\
    ( (This)->lpVtbl -> get_Type(This,pVal) ) 

#define IJsonService_get_Value(This,pVal)	\
    ( (This)->lpVtbl -> get_Value(This,pVal) ) 

#define IJsonService_put_IntValue(This,bstrKeyName,newVal)	\
    ( (This)->lpVtbl -> put_IntValue(This,bstrKeyName,newVal) ) 

#define IJsonService_put_StringValue(This,bstrKeyName,newVal)	\
    ( (This)->lpVtbl -> put_StringValue(This,bstrKeyName,newVal) ) 

#define IJsonService_put_ChildValue(This,bstrKeyName,newVal)	\
    ( (This)->lpVtbl -> put_ChildValue(This,bstrKeyName,newVal) ) 

#define IJsonService_get_ArraySize(This,pVal)	\
    ( (This)->lpVtbl -> get_ArraySize(This,pVal) ) 

#define IJsonService_get_IsExist(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> get_IsExist(This,bstrKeyName,pVal) ) 

#define IJsonService_get_ObjectString(This,pVal)	\
    ( (This)->lpVtbl -> get_ObjectString(This,pVal) ) 

#define IJsonService_get_ErrInfo(This,pVal)	\
    ( (This)->lpVtbl -> get_ErrInfo(This,pVal) ) 

#define IJsonService_GetValueType(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> GetValueType(This,bstrKeyName,pVal) ) 

#define IJsonService_GetStringValue(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> GetStringValue(This,bstrKeyName,pVal) ) 

#define IJsonService_GetBoolValue(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> GetBoolValue(This,bstrKeyName,pVal) ) 

#define IJsonService_GetIntValue(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> GetIntValue(This,bstrKeyName,pVal) ) 

#define IJsonService_GetUIntValue(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> GetUIntValue(This,bstrKeyName,pVal) ) 

#define IJsonService_GetDoubleValue(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> GetDoubleValue(This,bstrKeyName,pVal) ) 

#define IJsonService_GetVariantValue(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> GetVariantValue(This,bstrKeyName,pVal) ) 

#define IJsonService_GetArrayValue(This,sIndex,pKey,pVal)	\
    ( (This)->lpVtbl -> GetArrayValue(This,sIndex,pKey,pVal) ) 

#define IJsonService_GetChildByName(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> GetChildByName(This,bstrKeyName,pVal) ) 

#define IJsonService_GetChildByIndex(This,nIndex,pVal)	\
    ( (This)->lpVtbl -> GetChildByIndex(This,nIndex,pVal) ) 

#define IJsonService_CreateChild(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> CreateChild(This,bstrKeyName,pVal) ) 

#define IJsonService_PutChild(This,bstrKeyName,pVal)	\
    ( (This)->lpVtbl -> PutChild(This,bstrKeyName,pVal) ) 

#define IJsonService_GetString(This,pVal)	\
    ( (This)->lpVtbl -> GetString(This,pVal) ) 

#define IJsonService_Save(This)	\
    ( (This)->lpVtbl -> Save(This) ) 

#define IJsonService_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IJsonService_INTERFACE_DEFINED__ */


#ifndef __IParaService_INTERFACE_DEFINED__
#define __IParaService_INTERFACE_DEFINED__

/* interface IParaService */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IParaService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E485B568-C7DB-47F0-9904-0331373AC31C")
    IParaService : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddNumber( 
            /* [in] */ BSTR bstrName,
            /* [in] */ ULONG nValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddString( 
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrValue) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsExist( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNumber( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetString( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RemovePara( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetJson( 
            /* [retval][out] */ IJsonService **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParaServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IParaService * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IParaService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IParaService * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IParaService * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IParaService * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IParaService * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IParaService * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddNumber )( 
            IParaService * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ ULONG nValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddString )( 
            IParaService * This,
            /* [in] */ BSTR bstrName,
            /* [in] */ BSTR bstrValue);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsExist )( 
            IParaService * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNumber )( 
            IParaService * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetString )( 
            IParaService * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IParaService * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RemovePara )( 
            IParaService * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IParaService * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetJson )( 
            IParaService * This,
            /* [retval][out] */ IJsonService **pVal);
        
        END_INTERFACE
    } IParaServiceVtbl;

    interface IParaService
    {
        CONST_VTBL struct IParaServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParaService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IParaService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IParaService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IParaService_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IParaService_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IParaService_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IParaService_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IParaService_AddNumber(This,bstrName,nValue)	\
    ( (This)->lpVtbl -> AddNumber(This,bstrName,nValue) ) 

#define IParaService_AddString(This,bstrName,bstrValue)	\
    ( (This)->lpVtbl -> AddString(This,bstrName,bstrValue) ) 

#define IParaService_IsExist(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> IsExist(This,bstrName,pVal) ) 

#define IParaService_GetNumber(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> GetNumber(This,bstrName,pVal) ) 

#define IParaService_GetString(This,bstrName,pVal)	\
    ( (This)->lpVtbl -> GetString(This,bstrName,pVal) ) 

#define IParaService_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IParaService_RemovePara(This,bstrName)	\
    ( (This)->lpVtbl -> RemovePara(This,bstrName) ) 

#define IParaService_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#define IParaService_GetJson(This,pVal)	\
    ( (This)->lpVtbl -> GetJson(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IParaService_INTERFACE_DEFINED__ */



#ifndef __JsonEngineLib_LIBRARY_DEFINED__
#define __JsonEngineLib_LIBRARY_DEFINED__

/* library JsonEngineLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_JsonEngineLib;

EXTERN_C const CLSID CLSID_JsonService;

#ifdef __cplusplus

class DECLSPEC_UUID("B0608C1D-7C13-4298-AC27-01AC8FF2AB2F")
JsonService;
#endif

EXTERN_C const CLSID CLSID_ParaService;

#ifdef __cplusplus

class DECLSPEC_UUID("076016AC-0050-4330-B3A1-8D4460CEA0EC")
ParaService;
#endif
#endif /* __JsonEngineLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


