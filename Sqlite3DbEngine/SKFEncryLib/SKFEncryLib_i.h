

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jul 20 17:51:43 2017
 */
/* Compiler settings for SKFEncryLib.idl:
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

#ifndef __SKFEncryLib_i_h__
#define __SKFEncryLib_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISafeService_FWD_DEFINED__
#define __ISafeService_FWD_DEFINED__
typedef interface ISafeService ISafeService;
#endif 	/* __ISafeService_FWD_DEFINED__ */


#ifndef ___ISafeServiceEvents_FWD_DEFINED__
#define ___ISafeServiceEvents_FWD_DEFINED__
typedef interface _ISafeServiceEvents _ISafeServiceEvents;
#endif 	/* ___ISafeServiceEvents_FWD_DEFINED__ */


#ifndef __SafeService_FWD_DEFINED__
#define __SafeService_FWD_DEFINED__

#ifdef __cplusplus
typedef class SafeService SafeService;
#else
typedef struct SafeService SafeService;
#endif /* __cplusplus */

#endif 	/* __SafeService_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ISafeService_INTERFACE_DEFINED__
#define __ISafeService_INTERFACE_DEFINED__

/* interface ISafeService */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISafeService;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("546FAE59-D650-4787-8EA2-A6C7F421B99A")
    ISafeService : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISafeServiceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISafeService * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISafeService * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISafeService * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISafeService * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISafeService * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISafeService * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISafeService * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISafeServiceVtbl;

    interface ISafeService
    {
        CONST_VTBL struct ISafeServiceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISafeService_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISafeService_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISafeService_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISafeService_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISafeService_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISafeService_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISafeService_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISafeService_INTERFACE_DEFINED__ */



#ifndef __SKFEncryLibLib_LIBRARY_DEFINED__
#define __SKFEncryLibLib_LIBRARY_DEFINED__

/* library SKFEncryLibLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_SKFEncryLibLib;

#ifndef ___ISafeServiceEvents_DISPINTERFACE_DEFINED__
#define ___ISafeServiceEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISafeServiceEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__ISafeServiceEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2F2A8D12-62C7-4F19-89EA-89D9098B07CF")
    _ISafeServiceEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ISafeServiceEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _ISafeServiceEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _ISafeServiceEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _ISafeServiceEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _ISafeServiceEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _ISafeServiceEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _ISafeServiceEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _ISafeServiceEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _ISafeServiceEventsVtbl;

    interface _ISafeServiceEvents
    {
        CONST_VTBL struct _ISafeServiceEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISafeServiceEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _ISafeServiceEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _ISafeServiceEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _ISafeServiceEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _ISafeServiceEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _ISafeServiceEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _ISafeServiceEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ISafeServiceEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SafeService;

#ifdef __cplusplus

class DECLSPEC_UUID("638FC147-287F-4FDE-9648-72ADA0771D4F")
SafeService;
#endif
#endif /* __SKFEncryLibLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


