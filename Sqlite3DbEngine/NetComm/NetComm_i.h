

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Jul 28 10:13:39 2017
 */
/* Compiler settings for NetComm.idl:
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

#ifndef __NetComm_i_h__
#define __NetComm_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __INetRequest_FWD_DEFINED__
#define __INetRequest_FWD_DEFINED__
typedef interface INetRequest INetRequest;
#endif 	/* __INetRequest_FWD_DEFINED__ */


#ifndef ___INetRequestEvents_FWD_DEFINED__
#define ___INetRequestEvents_FWD_DEFINED__
typedef interface _INetRequestEvents _INetRequestEvents;
#endif 	/* ___INetRequestEvents_FWD_DEFINED__ */


#ifndef __NetRequest_FWD_DEFINED__
#define __NetRequest_FWD_DEFINED__

#ifdef __cplusplus
typedef class NetRequest NetRequest;
#else
typedef struct NetRequest NetRequest;
#endif /* __cplusplus */

#endif 	/* __NetRequest_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __INetRequest_INTERFACE_DEFINED__
#define __INetRequest_INTERFACE_DEFINED__

/* interface INetRequest */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_INetRequest;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A04418ED-7FEF-422E-9C12-22C9E4627A7F")
    INetRequest : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_MsgWnd( 
            /* [retval][out] */ LONGLONG *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_MsgWnd( 
            /* [in] */ LONGLONG newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_SyncConnect( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_SyncConnect( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_NoCookie( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Port( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Method( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Method( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PostFile( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PostFile( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DownFile( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DownFile( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ServiceID( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ServiceID( 
            /* [in] */ SHORT newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_UseIE( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_UseIE( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PrxoyServer( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PrxoyServer( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_UserAgent( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_UserAgent( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Referer( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Referer( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Cookie( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Cookie( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Location( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_RequestUrl( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Origin( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Origin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ServerFileName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Busy( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HttpStatus( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrCode( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LastErrInfo( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SendHeader( 
            /* [in] */ SHORT sType,
            /* [in] */ VARIANT_BOOL bZipFlag,
            /* [in] */ BSTR bstrAuth) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Request( 
            /* [in] */ BSTR bstrUrl) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DownloadFile( 
            /* [in] */ BSTR bstrUrl,
            /* [in] */ VARIANT_BOOL bHaveName) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Stop( 
            /* [in] */ VARIANT_BOOL bReleaseConnect) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct INetRequestVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            INetRequest * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            INetRequest * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            INetRequest * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            INetRequest * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            INetRequest * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            INetRequest * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            INetRequest * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MsgWnd )( 
            INetRequest * This,
            /* [retval][out] */ LONGLONG *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MsgWnd )( 
            INetRequest * This,
            /* [in] */ LONGLONG newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SyncConnect )( 
            INetRequest * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SyncConnect )( 
            INetRequest * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NoCookie )( 
            INetRequest * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Port )( 
            INetRequest * This,
            /* [in] */ LONG newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Method )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Method )( 
            INetRequest * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PostFile )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PostFile )( 
            INetRequest * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DownFile )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DownFile )( 
            INetRequest * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ServiceID )( 
            INetRequest * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ServiceID )( 
            INetRequest * This,
            /* [in] */ SHORT newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UseIE )( 
            INetRequest * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_UseIE )( 
            INetRequest * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PrxoyServer )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PrxoyServer )( 
            INetRequest * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UserAgent )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_UserAgent )( 
            INetRequest * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Referer )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Referer )( 
            INetRequest * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cookie )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Cookie )( 
            INetRequest * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Location )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RequestUrl )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Origin )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Origin )( 
            INetRequest * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ServerFileName )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Busy )( 
            INetRequest * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HttpStatus )( 
            INetRequest * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ErrCode )( 
            INetRequest * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastErrInfo )( 
            INetRequest * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *SendHeader )( 
            INetRequest * This,
            /* [in] */ SHORT sType,
            /* [in] */ VARIANT_BOOL bZipFlag,
            /* [in] */ BSTR bstrAuth);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Request )( 
            INetRequest * This,
            /* [in] */ BSTR bstrUrl);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *DownloadFile )( 
            INetRequest * This,
            /* [in] */ BSTR bstrUrl,
            /* [in] */ VARIANT_BOOL bHaveName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            INetRequest * This,
            /* [in] */ VARIANT_BOOL bReleaseConnect);
        
        END_INTERFACE
    } INetRequestVtbl;

    interface INetRequest
    {
        CONST_VTBL struct INetRequestVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define INetRequest_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define INetRequest_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define INetRequest_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define INetRequest_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define INetRequest_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define INetRequest_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define INetRequest_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define INetRequest_get_MsgWnd(This,pVal)	\
    ( (This)->lpVtbl -> get_MsgWnd(This,pVal) ) 

#define INetRequest_put_MsgWnd(This,newVal)	\
    ( (This)->lpVtbl -> put_MsgWnd(This,newVal) ) 

#define INetRequest_get_SyncConnect(This,pVal)	\
    ( (This)->lpVtbl -> get_SyncConnect(This,pVal) ) 

#define INetRequest_put_SyncConnect(This,newVal)	\
    ( (This)->lpVtbl -> put_SyncConnect(This,newVal) ) 

#define INetRequest_put_NoCookie(This,newVal)	\
    ( (This)->lpVtbl -> put_NoCookie(This,newVal) ) 

#define INetRequest_put_Port(This,newVal)	\
    ( (This)->lpVtbl -> put_Port(This,newVal) ) 

#define INetRequest_get_Method(This,pVal)	\
    ( (This)->lpVtbl -> get_Method(This,pVal) ) 

#define INetRequest_put_Method(This,newVal)	\
    ( (This)->lpVtbl -> put_Method(This,newVal) ) 

#define INetRequest_get_PostFile(This,pVal)	\
    ( (This)->lpVtbl -> get_PostFile(This,pVal) ) 

#define INetRequest_put_PostFile(This,newVal)	\
    ( (This)->lpVtbl -> put_PostFile(This,newVal) ) 

#define INetRequest_get_DownFile(This,pVal)	\
    ( (This)->lpVtbl -> get_DownFile(This,pVal) ) 

#define INetRequest_put_DownFile(This,newVal)	\
    ( (This)->lpVtbl -> put_DownFile(This,newVal) ) 

#define INetRequest_get_ServiceID(This,pVal)	\
    ( (This)->lpVtbl -> get_ServiceID(This,pVal) ) 

#define INetRequest_put_ServiceID(This,newVal)	\
    ( (This)->lpVtbl -> put_ServiceID(This,newVal) ) 

#define INetRequest_get_UseIE(This,pVal)	\
    ( (This)->lpVtbl -> get_UseIE(This,pVal) ) 

#define INetRequest_put_UseIE(This,newVal)	\
    ( (This)->lpVtbl -> put_UseIE(This,newVal) ) 

#define INetRequest_get_PrxoyServer(This,pVal)	\
    ( (This)->lpVtbl -> get_PrxoyServer(This,pVal) ) 

#define INetRequest_put_PrxoyServer(This,newVal)	\
    ( (This)->lpVtbl -> put_PrxoyServer(This,newVal) ) 

#define INetRequest_get_UserAgent(This,pVal)	\
    ( (This)->lpVtbl -> get_UserAgent(This,pVal) ) 

#define INetRequest_put_UserAgent(This,newVal)	\
    ( (This)->lpVtbl -> put_UserAgent(This,newVal) ) 

#define INetRequest_get_Referer(This,pVal)	\
    ( (This)->lpVtbl -> get_Referer(This,pVal) ) 

#define INetRequest_put_Referer(This,newVal)	\
    ( (This)->lpVtbl -> put_Referer(This,newVal) ) 

#define INetRequest_get_Cookie(This,pVal)	\
    ( (This)->lpVtbl -> get_Cookie(This,pVal) ) 

#define INetRequest_put_Cookie(This,newVal)	\
    ( (This)->lpVtbl -> put_Cookie(This,newVal) ) 

#define INetRequest_get_Location(This,pVal)	\
    ( (This)->lpVtbl -> get_Location(This,pVal) ) 

#define INetRequest_get_RequestUrl(This,pVal)	\
    ( (This)->lpVtbl -> get_RequestUrl(This,pVal) ) 

#define INetRequest_get_Origin(This,pVal)	\
    ( (This)->lpVtbl -> get_Origin(This,pVal) ) 

#define INetRequest_put_Origin(This,newVal)	\
    ( (This)->lpVtbl -> put_Origin(This,newVal) ) 

#define INetRequest_get_ServerFileName(This,pVal)	\
    ( (This)->lpVtbl -> get_ServerFileName(This,pVal) ) 

#define INetRequest_get_Busy(This,pVal)	\
    ( (This)->lpVtbl -> get_Busy(This,pVal) ) 

#define INetRequest_get_HttpStatus(This,pVal)	\
    ( (This)->lpVtbl -> get_HttpStatus(This,pVal) ) 

#define INetRequest_get_ErrCode(This,pVal)	\
    ( (This)->lpVtbl -> get_ErrCode(This,pVal) ) 

#define INetRequest_get_LastErrInfo(This,pVal)	\
    ( (This)->lpVtbl -> get_LastErrInfo(This,pVal) ) 

#define INetRequest_SendHeader(This,sType,bZipFlag,bstrAuth)	\
    ( (This)->lpVtbl -> SendHeader(This,sType,bZipFlag,bstrAuth) ) 

#define INetRequest_Request(This,bstrUrl)	\
    ( (This)->lpVtbl -> Request(This,bstrUrl) ) 

#define INetRequest_DownloadFile(This,bstrUrl,bHaveName)	\
    ( (This)->lpVtbl -> DownloadFile(This,bstrUrl,bHaveName) ) 

#define INetRequest_Stop(This,bReleaseConnect)	\
    ( (This)->lpVtbl -> Stop(This,bReleaseConnect) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __INetRequest_INTERFACE_DEFINED__ */



#ifndef __NetCommLib_LIBRARY_DEFINED__
#define __NetCommLib_LIBRARY_DEFINED__

/* library NetCommLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_NetCommLib;

#ifndef ___INetRequestEvents_DISPINTERFACE_DEFINED__
#define ___INetRequestEvents_DISPINTERFACE_DEFINED__

/* dispinterface _INetRequestEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__INetRequestEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("44F06F40-D4D5-4C19-AAA6-A7DA50A9F8D8")
    _INetRequestEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _INetRequestEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _INetRequestEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _INetRequestEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _INetRequestEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _INetRequestEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _INetRequestEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _INetRequestEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _INetRequestEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _INetRequestEventsVtbl;

    interface _INetRequestEvents
    {
        CONST_VTBL struct _INetRequestEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _INetRequestEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _INetRequestEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _INetRequestEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _INetRequestEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _INetRequestEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _INetRequestEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _INetRequestEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___INetRequestEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_NetRequest;

#ifdef __cplusplus

class DECLSPEC_UUID("4D80FA81-448E-4844-9326-F839178BC461")
NetRequest;
#endif
#endif /* __NetCommLib_LIBRARY_DEFINED__ */

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


