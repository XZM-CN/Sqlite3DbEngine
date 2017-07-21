

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Jul 21 14:26:32 2017
 */
/* Compiler settings for LogEngine.idl:
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

#ifndef __LogEngine_i_h__
#define __LogEngine_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILogService_FWD_DEFINED__
#define __ILogService_FWD_DEFINED__
typedef interface ILogService ILogService;
#endif 	/* __ILogService_FWD_DEFINED__ */


#ifndef __ISysEventLog_FWD_DEFINED__
#define __ISysEventLog_FWD_DEFINED__
typedef interface ISysEventLog ISysEventLog;
#endif 	/* __ISysEventLog_FWD_DEFINED__ */


#ifndef __LogService_FWD_DEFINED__
#define __LogService_FWD_DEFINED__

#ifdef __cplusplus
typedef class LogService LogService;
#else
typedef struct LogService LogService;
#endif /* __cplusplus */

#endif 	/* __LogService_FWD_DEFINED__ */


#ifndef __SysEventLog_FWD_DEFINED__
#define __SysEventLog_FWD_DEFINED__

#ifdef __cplusplus
typedef class SysEventLog SysEventLog;
#else
typedef struct SysEventLog SysEventLog;
#endif /* __cplusplus */

#endif 	/* __SysEventLog_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_LogEngine_0000_0000 */
/* [local] */ 

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




extern RPC_IF_HANDLE __MIDL_itf_LogEngine_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_LogEngine_0000_0000_v0_0_s_ifspec;

#ifndef __ILogService_INTERFACE_DEFINED__
#define __ILogService_INTERFACE_DEFINED__

/* interface ILogService */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILogService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F89A033F-7DB5-48EE-AA9D-9BDF5B596A60")
    ILogService : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteSysLog( 
            /* [in] */ SHORT nUserType,
            /* [in] */ BSTR bstrLID,
            /* [in] */ BSTR bstrName,
            /* [in] */ DATE dtTime,
            /* [in] */ BSTR bstrLog,
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteAppLog( 
            /* [in] */ ESafeLogType eLogType,
            /* [in] */ SHORT nFlag,
            /* [in] */ DATE dtTime,
            /* [in] */ BSTR bstrFilePath,
            /* [in] */ BSTR bstrLog,
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteUsbLog( 
            /* [in] */ SHORT nFlag,
            /* [in] */ BSTR bstrUID,
            /* [in] */ DATE dtTime,
            /* [in] */ BSTR bstrLog,
            /* [retval][out] */ ULONG *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Del( 
            /* [in] */ ESafeLogType eLogType,
            /* [in] */ LONG nBeforeDay) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILogServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogService * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogService * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILogService * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILogService * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILogService * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILogService * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteSysLog )( 
            ILogService * This,
            /* [in] */ SHORT nUserType,
            /* [in] */ BSTR bstrLID,
            /* [in] */ BSTR bstrName,
            /* [in] */ DATE dtTime,
            /* [in] */ BSTR bstrLog,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteAppLog )( 
            ILogService * This,
            /* [in] */ ESafeLogType eLogType,
            /* [in] */ SHORT nFlag,
            /* [in] */ DATE dtTime,
            /* [in] */ BSTR bstrFilePath,
            /* [in] */ BSTR bstrLog,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteUsbLog )( 
            ILogService * This,
            /* [in] */ SHORT nFlag,
            /* [in] */ BSTR bstrUID,
            /* [in] */ DATE dtTime,
            /* [in] */ BSTR bstrLog,
            /* [retval][out] */ ULONG *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Del )( 
            ILogService * This,
            /* [in] */ ESafeLogType eLogType,
            /* [in] */ LONG nBeforeDay);
        
        END_INTERFACE
    } ILogServiceVtbl;

    interface ILogService
    {
        CONST_VTBL struct ILogServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogService_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILogService_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILogService_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILogService_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILogService_WriteSysLog(This,nUserType,bstrLID,bstrName,dtTime,bstrLog,pVal)	\
    ( (This)->lpVtbl -> WriteSysLog(This,nUserType,bstrLID,bstrName,dtTime,bstrLog,pVal) ) 

#define ILogService_WriteAppLog(This,eLogType,nFlag,dtTime,bstrFilePath,bstrLog,pVal)	\
    ( (This)->lpVtbl -> WriteAppLog(This,eLogType,nFlag,dtTime,bstrFilePath,bstrLog,pVal) ) 

#define ILogService_WriteUsbLog(This,nFlag,bstrUID,dtTime,bstrLog,pVal)	\
    ( (This)->lpVtbl -> WriteUsbLog(This,nFlag,bstrUID,dtTime,bstrLog,pVal) ) 

#define ILogService_Del(This,eLogType,nBeforeDay)	\
    ( (This)->lpVtbl -> Del(This,eLogType,nBeforeDay) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILogService_INTERFACE_DEFINED__ */


#ifndef __ISysEventLog_INTERFACE_DEFINED__
#define __ISysEventLog_INTERFACE_DEFINED__

/* interface ISysEventLog */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISysEventLog;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F528E9E2-4D32-4566-AEBC-8E7C6747476B")
    ISysEventLog : public IDispatch
    {
    public:
        virtual /* [helpcontext][helpstring][id] */ HRESULT STDMETHODCALLTYPE GetEventList( 
            /* [in] */ LONG Index,
            /* [out] */ LONG *EventLog) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetEventLogSize( 
            /* [in] */ BSTR strFileName,
            /* [out] */ LONG *nLogNum) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearEventLogList( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISysEventLogVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISysEventLog * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISysEventLog * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISysEventLog * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISysEventLog * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISysEventLog * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISysEventLog * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISysEventLog * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpcontext][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetEventList )( 
            ISysEventLog * This,
            /* [in] */ LONG Index,
            /* [out] */ LONG *EventLog);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetEventLogSize )( 
            ISysEventLog * This,
            /* [in] */ BSTR strFileName,
            /* [out] */ LONG *nLogNum);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearEventLogList )( 
            ISysEventLog * This);
        
        END_INTERFACE
    } ISysEventLogVtbl;

    interface ISysEventLog
    {
        CONST_VTBL struct ISysEventLogVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISysEventLog_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISysEventLog_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISysEventLog_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISysEventLog_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISysEventLog_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISysEventLog_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISysEventLog_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISysEventLog_GetEventList(This,Index,EventLog)	\
    ( (This)->lpVtbl -> GetEventList(This,Index,EventLog) ) 

#define ISysEventLog_GetEventLogSize(This,strFileName,nLogNum)	\
    ( (This)->lpVtbl -> GetEventLogSize(This,strFileName,nLogNum) ) 

#define ISysEventLog_ClearEventLogList(This)	\
    ( (This)->lpVtbl -> ClearEventLogList(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISysEventLog_INTERFACE_DEFINED__ */



#ifndef __LogEngineLib_LIBRARY_DEFINED__
#define __LogEngineLib_LIBRARY_DEFINED__

/* library LogEngineLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_LogEngineLib;

EXTERN_C const CLSID CLSID_LogService;

#ifdef __cplusplus

class DECLSPEC_UUID("A17E2017-C02C-468D-A32C-1081B7F010DD")
LogService;
#endif

EXTERN_C const CLSID CLSID_SysEventLog;

#ifdef __cplusplus

class DECLSPEC_UUID("DA4D81A6-0D6E-4DFF-B4F5-B9EEF3D3F420")
SysEventLog;
#endif
#endif /* __LogEngineLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


