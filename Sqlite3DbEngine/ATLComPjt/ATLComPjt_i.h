

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Jul 24 14:40:19 2017
 */
/* Compiler settings for ATLComPjt.idl:
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

#ifndef __ATLComPjt_i_h__
#define __ATLComPjt_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDemo01_FWD_DEFINED__
#define __IDemo01_FWD_DEFINED__
typedef interface IDemo01 IDemo01;
#endif 	/* __IDemo01_FWD_DEFINED__ */


#ifndef __IDemo_FWD_DEFINED__
#define __IDemo_FWD_DEFINED__
typedef interface IDemo IDemo;
#endif 	/* __IDemo_FWD_DEFINED__ */


#ifndef __IDemo_Single_FWD_DEFINED__
#define __IDemo_Single_FWD_DEFINED__
typedef interface IDemo_Single IDemo_Single;
#endif 	/* __IDemo_Single_FWD_DEFINED__ */


#ifndef __IDemo_Apartment_FWD_DEFINED__
#define __IDemo_Apartment_FWD_DEFINED__
typedef interface IDemo_Apartment IDemo_Apartment;
#endif 	/* __IDemo_Apartment_FWD_DEFINED__ */


#ifndef __IDemo_Both_FWD_DEFINED__
#define __IDemo_Both_FWD_DEFINED__
typedef interface IDemo_Both IDemo_Both;
#endif 	/* __IDemo_Both_FWD_DEFINED__ */


#ifndef __IDemo_Free_FWD_DEFINED__
#define __IDemo_Free_FWD_DEFINED__
typedef interface IDemo_Free IDemo_Free;
#endif 	/* __IDemo_Free_FWD_DEFINED__ */


#ifndef __IDemo_Neutral_FWD_DEFINED__
#define __IDemo_Neutral_FWD_DEFINED__
typedef interface IDemo_Neutral IDemo_Neutral;
#endif 	/* __IDemo_Neutral_FWD_DEFINED__ */


#ifndef __IDemo_Both_Support_FWD_DEFINED__
#define __IDemo_Both_Support_FWD_DEFINED__
typedef interface IDemo_Both_Support IDemo_Both_Support;
#endif 	/* __IDemo_Both_Support_FWD_DEFINED__ */


#ifndef __Demo01_FWD_DEFINED__
#define __Demo01_FWD_DEFINED__

#ifdef __cplusplus
typedef class Demo01 Demo01;
#else
typedef struct Demo01 Demo01;
#endif /* __cplusplus */

#endif 	/* __Demo01_FWD_DEFINED__ */


#ifndef __Demo_FWD_DEFINED__
#define __Demo_FWD_DEFINED__

#ifdef __cplusplus
typedef class Demo Demo;
#else
typedef struct Demo Demo;
#endif /* __cplusplus */

#endif 	/* __Demo_FWD_DEFINED__ */


#ifndef __Demo_Single_FWD_DEFINED__
#define __Demo_Single_FWD_DEFINED__

#ifdef __cplusplus
typedef class Demo_Single Demo_Single;
#else
typedef struct Demo_Single Demo_Single;
#endif /* __cplusplus */

#endif 	/* __Demo_Single_FWD_DEFINED__ */


#ifndef __Demo_Apartment_FWD_DEFINED__
#define __Demo_Apartment_FWD_DEFINED__

#ifdef __cplusplus
typedef class Demo_Apartment Demo_Apartment;
#else
typedef struct Demo_Apartment Demo_Apartment;
#endif /* __cplusplus */

#endif 	/* __Demo_Apartment_FWD_DEFINED__ */


#ifndef __Demo_Both_FWD_DEFINED__
#define __Demo_Both_FWD_DEFINED__

#ifdef __cplusplus
typedef class Demo_Both Demo_Both;
#else
typedef struct Demo_Both Demo_Both;
#endif /* __cplusplus */

#endif 	/* __Demo_Both_FWD_DEFINED__ */


#ifndef __Demo_Free_FWD_DEFINED__
#define __Demo_Free_FWD_DEFINED__

#ifdef __cplusplus
typedef class Demo_Free Demo_Free;
#else
typedef struct Demo_Free Demo_Free;
#endif /* __cplusplus */

#endif 	/* __Demo_Free_FWD_DEFINED__ */


#ifndef __Demo_Neutral_FWD_DEFINED__
#define __Demo_Neutral_FWD_DEFINED__

#ifdef __cplusplus
typedef class Demo_Neutral Demo_Neutral;
#else
typedef struct Demo_Neutral Demo_Neutral;
#endif /* __cplusplus */

#endif 	/* __Demo_Neutral_FWD_DEFINED__ */


#ifndef ___IDemo_Both_SupportEvents_FWD_DEFINED__
#define ___IDemo_Both_SupportEvents_FWD_DEFINED__
typedef interface _IDemo_Both_SupportEvents _IDemo_Both_SupportEvents;
#endif 	/* ___IDemo_Both_SupportEvents_FWD_DEFINED__ */


#ifndef __Demo_Both_Support_FWD_DEFINED__
#define __Demo_Both_Support_FWD_DEFINED__

#ifdef __cplusplus
typedef class Demo_Both_Support Demo_Both_Support;
#else
typedef struct Demo_Both_Support Demo_Both_Support;
#endif /* __cplusplus */

#endif 	/* __Demo_Both_Support_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDemo01_INTERFACE_DEFINED__
#define __IDemo01_INTERFACE_DEFINED__

/* interface IDemo01 */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDemo01;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8727AEE6-43A6-41F3-A490-46BB51D4DFAA")
    IDemo01 : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDemo01Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDemo01 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDemo01 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDemo01 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDemo01 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDemo01 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDemo01 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDemo01 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDemo01Vtbl;

    interface IDemo01
    {
        CONST_VTBL struct IDemo01Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDemo01_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDemo01_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDemo01_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDemo01_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDemo01_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDemo01_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDemo01_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDemo01_INTERFACE_DEFINED__ */


#ifndef __IDemo_INTERFACE_DEFINED__
#define __IDemo_INTERFACE_DEFINED__

/* interface IDemo */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDemo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6BA7AA6C-B4CB-454A-89AD-DDC4A3171F2F")
    IDemo : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE dddddd( 
            /* [in] */ BSTR ddd) = 0;
        
        virtual /* [source][restricted][local][hidden][helpstring][id] */ HRESULT STDMETHODCALLTYPE asdasd( 
            BSTR *asdsa,
            /* [in] */ BYTE asdasdaaa) = 0;
        
        virtual /* [local][helpstring][id] */ HRESULT STDMETHODCALLTYPE asdasdaaa( 
            /* [in] */ BSTR *asd,
            /* [retval][out] */ CY *asdaaa) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDemoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDemo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDemo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDemo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDemo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDemo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDemo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDemo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *dddddd )( 
            IDemo * This,
            /* [in] */ BSTR ddd);
        
        /* [source][restricted][local][hidden][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *asdasd )( 
            IDemo * This,
            BSTR *asdsa,
            /* [in] */ BYTE asdasdaaa);
        
        /* [local][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *asdasdaaa )( 
            IDemo * This,
            /* [in] */ BSTR *asd,
            /* [retval][out] */ CY *asdaaa);
        
        END_INTERFACE
    } IDemoVtbl;

    interface IDemo
    {
        CONST_VTBL struct IDemoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDemo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDemo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDemo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDemo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDemo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDemo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDemo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDemo_dddddd(This,ddd)	\
    ( (This)->lpVtbl -> dddddd(This,ddd) ) 

#define IDemo_asdasd(This,asdsa,asdasdaaa)	\
    ( (This)->lpVtbl -> asdasd(This,asdsa,asdasdaaa) ) 

#define IDemo_asdasdaaa(This,asd,asdaaa)	\
    ( (This)->lpVtbl -> asdasdaaa(This,asd,asdaaa) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as][id] */ HRESULT STDMETHODCALLTYPE IDemo_sdasdad_Proxy( 
    IDemo * This);


void __RPC_STUB IDemo_sdasdad_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDemo_INTERFACE_DEFINED__ */


#ifndef __IDemo_Single_INTERFACE_DEFINED__
#define __IDemo_Single_INTERFACE_DEFINED__

/* interface IDemo_Single */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDemo_Single;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("429048EC-ECA3-4FEB-BAB3-688E72639F4F")
    IDemo_Single : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDemo_SingleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDemo_Single * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDemo_Single * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDemo_Single * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDemo_Single * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDemo_Single * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDemo_Single * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDemo_Single * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDemo_SingleVtbl;

    interface IDemo_Single
    {
        CONST_VTBL struct IDemo_SingleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDemo_Single_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDemo_Single_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDemo_Single_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDemo_Single_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDemo_Single_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDemo_Single_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDemo_Single_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDemo_Single_INTERFACE_DEFINED__ */


#ifndef __IDemo_Apartment_INTERFACE_DEFINED__
#define __IDemo_Apartment_INTERFACE_DEFINED__

/* interface IDemo_Apartment */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDemo_Apartment;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("581B9660-35CC-431F-92A4-AA8202FDC8BD")
    IDemo_Apartment : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDemo_ApartmentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDemo_Apartment * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDemo_Apartment * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDemo_Apartment * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDemo_Apartment * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDemo_Apartment * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDemo_Apartment * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDemo_Apartment * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDemo_ApartmentVtbl;

    interface IDemo_Apartment
    {
        CONST_VTBL struct IDemo_ApartmentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDemo_Apartment_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDemo_Apartment_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDemo_Apartment_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDemo_Apartment_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDemo_Apartment_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDemo_Apartment_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDemo_Apartment_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDemo_Apartment_INTERFACE_DEFINED__ */


#ifndef __IDemo_Both_INTERFACE_DEFINED__
#define __IDemo_Both_INTERFACE_DEFINED__

/* interface IDemo_Both */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDemo_Both;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("70BCBF73-467C-4CC7-9089-E58019DBA4C9")
    IDemo_Both : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDemo_BothVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDemo_Both * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDemo_Both * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDemo_Both * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDemo_Both * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDemo_Both * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDemo_Both * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDemo_Both * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDemo_BothVtbl;

    interface IDemo_Both
    {
        CONST_VTBL struct IDemo_BothVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDemo_Both_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDemo_Both_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDemo_Both_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDemo_Both_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDemo_Both_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDemo_Both_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDemo_Both_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDemo_Both_INTERFACE_DEFINED__ */


#ifndef __IDemo_Free_INTERFACE_DEFINED__
#define __IDemo_Free_INTERFACE_DEFINED__

/* interface IDemo_Free */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDemo_Free;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1BE2B8D4-915A-442E-81AD-D71E0035D15B")
    IDemo_Free : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDemo_FreeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDemo_Free * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDemo_Free * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDemo_Free * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDemo_Free * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDemo_Free * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDemo_Free * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDemo_Free * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDemo_FreeVtbl;

    interface IDemo_Free
    {
        CONST_VTBL struct IDemo_FreeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDemo_Free_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDemo_Free_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDemo_Free_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDemo_Free_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDemo_Free_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDemo_Free_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDemo_Free_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDemo_Free_INTERFACE_DEFINED__ */


#ifndef __IDemo_Neutral_INTERFACE_DEFINED__
#define __IDemo_Neutral_INTERFACE_DEFINED__

/* interface IDemo_Neutral */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDemo_Neutral;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("24F83B2B-E35A-4CE5-AD0D-A7CE588BD7D4")
    IDemo_Neutral : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDemo_NeutralVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDemo_Neutral * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDemo_Neutral * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDemo_Neutral * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDemo_Neutral * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDemo_Neutral * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDemo_Neutral * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDemo_Neutral * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDemo_NeutralVtbl;

    interface IDemo_Neutral
    {
        CONST_VTBL struct IDemo_NeutralVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDemo_Neutral_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDemo_Neutral_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDemo_Neutral_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDemo_Neutral_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDemo_Neutral_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDemo_Neutral_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDemo_Neutral_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDemo_Neutral_INTERFACE_DEFINED__ */


#ifndef __IDemo_Both_Support_INTERFACE_DEFINED__
#define __IDemo_Both_Support_INTERFACE_DEFINED__

/* interface IDemo_Both_Support */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDemo_Both_Support;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("487E6409-6914-49BD-A523-1F3E63BD50FC")
    IDemo_Both_Support : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDemo_Both_SupportVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDemo_Both_Support * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDemo_Both_Support * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDemo_Both_Support * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDemo_Both_Support * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDemo_Both_Support * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDemo_Both_Support * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDemo_Both_Support * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDemo_Both_SupportVtbl;

    interface IDemo_Both_Support
    {
        CONST_VTBL struct IDemo_Both_SupportVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDemo_Both_Support_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDemo_Both_Support_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDemo_Both_Support_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDemo_Both_Support_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDemo_Both_Support_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDemo_Both_Support_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDemo_Both_Support_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDemo_Both_Support_INTERFACE_DEFINED__ */



#ifndef __ATLComPjtLib_LIBRARY_DEFINED__
#define __ATLComPjtLib_LIBRARY_DEFINED__

/* library ATLComPjtLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_ATLComPjtLib;

EXTERN_C const CLSID CLSID_Demo01;

#ifdef __cplusplus

class DECLSPEC_UUID("711159BC-FA73-4F2B-8D6D-4F09F39DF500")
Demo01;
#endif

EXTERN_C const CLSID CLSID_Demo;

#ifdef __cplusplus

class DECLSPEC_UUID("A68B94DC-FD6A-4E41-99B8-8CA04D7D69CE")
Demo;
#endif

EXTERN_C const CLSID CLSID_Demo_Single;

#ifdef __cplusplus

class DECLSPEC_UUID("89178619-BDF9-4133-843A-DD103A2ED71B")
Demo_Single;
#endif

EXTERN_C const CLSID CLSID_Demo_Apartment;

#ifdef __cplusplus

class DECLSPEC_UUID("1A300033-D722-441F-8706-11B84311F128")
Demo_Apartment;
#endif

EXTERN_C const CLSID CLSID_Demo_Both;

#ifdef __cplusplus

class DECLSPEC_UUID("EED4FD28-7197-41E9-BB8D-296D131C1104")
Demo_Both;
#endif

EXTERN_C const CLSID CLSID_Demo_Free;

#ifdef __cplusplus

class DECLSPEC_UUID("368A35FB-C7B7-4D4B-A0C6-E98CB33BD161")
Demo_Free;
#endif

EXTERN_C const CLSID CLSID_Demo_Neutral;

#ifdef __cplusplus

class DECLSPEC_UUID("E2D67F03-02C4-4AA0-8E5D-ACC8E96316A3")
Demo_Neutral;
#endif

#ifndef ___IDemo_Both_SupportEvents_DISPINTERFACE_DEFINED__
#define ___IDemo_Both_SupportEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IDemo_Both_SupportEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__IDemo_Both_SupportEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7062AC4C-E768-488E-AB3F-DE64C295EBA5")
    _IDemo_Both_SupportEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IDemo_Both_SupportEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IDemo_Both_SupportEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IDemo_Both_SupportEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IDemo_Both_SupportEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IDemo_Both_SupportEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IDemo_Both_SupportEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IDemo_Both_SupportEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IDemo_Both_SupportEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IDemo_Both_SupportEventsVtbl;

    interface _IDemo_Both_SupportEvents
    {
        CONST_VTBL struct _IDemo_Both_SupportEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IDemo_Both_SupportEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IDemo_Both_SupportEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IDemo_Both_SupportEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IDemo_Both_SupportEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IDemo_Both_SupportEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IDemo_Both_SupportEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IDemo_Both_SupportEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IDemo_Both_SupportEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Demo_Both_Support;

#ifdef __cplusplus

class DECLSPEC_UUID("103F4323-ED93-4A9D-9065-C5792F738066")
Demo_Both_Support;
#endif
#endif /* __ATLComPjtLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* [local][helpstring][id] */ HRESULT STDMETHODCALLTYPE IDemo_asdasdaaa_Proxy( 
    IDemo * This,
    /* [in] */ BSTR *asd,
    /* [retval][out] */ CY *asdaaa);


/* [call_as][id] */ HRESULT STDMETHODCALLTYPE IDemo_asdasdaaa_Stub( 
    IDemo * This);



/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


