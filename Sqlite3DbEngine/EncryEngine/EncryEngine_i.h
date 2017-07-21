

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Jul 21 10:16:15 2017
 */
/* Compiler settings for EncryEngine.idl:
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

#ifndef __EncryEngine_i_h__
#define __EncryEngine_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISoftEncry_FWD_DEFINED__
#define __ISoftEncry_FWD_DEFINED__
typedef interface ISoftEncry ISoftEncry;
#endif 	/* __ISoftEncry_FWD_DEFINED__ */


#ifndef __SoftEncry_FWD_DEFINED__
#define __SoftEncry_FWD_DEFINED__

#ifdef __cplusplus
typedef class SoftEncry SoftEncry;
#else
typedef struct SoftEncry SoftEncry;
#endif /* __cplusplus */

#endif 	/* __SoftEncry_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ISoftEncry_INTERFACE_DEFINED__
#define __ISoftEncry_INTERFACE_DEFINED__

/* interface ISoftEncry */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISoftEncry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3044B4F5-8A04-4926-AFD4-892EB48364AC")
    ISoftEncry : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AesEcbEnc( 
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AesEcbDes( 
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AesCbcEnc( 
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [in] */ BYTE *szIV,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AesCbcDes( 
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [in] */ BYTE *szIV,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReleaseBuf( 
            /* [out][in] */ BYTE *pBuf) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISoftEncryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISoftEncry * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISoftEncry * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISoftEncry * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISoftEncry * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISoftEncry * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISoftEncry * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISoftEncry * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AesEcbEnc )( 
            ISoftEncry * This,
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AesEcbDes )( 
            ISoftEncry * This,
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AesCbcEnc )( 
            ISoftEncry * This,
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [in] */ BYTE *szIV,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AesCbcDes )( 
            ISoftEncry * This,
            /* [in] */ BYTE *szIn,
            /* [in] */ ULONG nInLen,
            /* [in] */ BYTE *szKey,
            /* [in] */ BYTE *szIV,
            /* [out] */ ULONG *pnOutLen,
            /* [retval][out] */ BYTE **ppBuf);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReleaseBuf )( 
            ISoftEncry * This,
            /* [out][in] */ BYTE *pBuf);
        
        END_INTERFACE
    } ISoftEncryVtbl;

    interface ISoftEncry
    {
        CONST_VTBL struct ISoftEncryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISoftEncry_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISoftEncry_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISoftEncry_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISoftEncry_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISoftEncry_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISoftEncry_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISoftEncry_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ISoftEncry_AesEcbEnc(This,szIn,nInLen,szKey,pnOutLen,ppBuf)	\
    ( (This)->lpVtbl -> AesEcbEnc(This,szIn,nInLen,szKey,pnOutLen,ppBuf) ) 

#define ISoftEncry_AesEcbDes(This,szIn,nInLen,szKey,pnOutLen,ppBuf)	\
    ( (This)->lpVtbl -> AesEcbDes(This,szIn,nInLen,szKey,pnOutLen,ppBuf) ) 

#define ISoftEncry_AesCbcEnc(This,szIn,nInLen,szKey,szIV,pnOutLen,ppBuf)	\
    ( (This)->lpVtbl -> AesCbcEnc(This,szIn,nInLen,szKey,szIV,pnOutLen,ppBuf) ) 

#define ISoftEncry_AesCbcDes(This,szIn,nInLen,szKey,szIV,pnOutLen,ppBuf)	\
    ( (This)->lpVtbl -> AesCbcDes(This,szIn,nInLen,szKey,szIV,pnOutLen,ppBuf) ) 

#define ISoftEncry_ReleaseBuf(This,pBuf)	\
    ( (This)->lpVtbl -> ReleaseBuf(This,pBuf) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISoftEncry_INTERFACE_DEFINED__ */



#ifndef __EncryEngineLib_LIBRARY_DEFINED__
#define __EncryEngineLib_LIBRARY_DEFINED__

/* library EncryEngineLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_EncryEngineLib;

EXTERN_C const CLSID CLSID_SoftEncry;

#ifdef __cplusplus

class DECLSPEC_UUID("32D4B109-7846-4A43-97FD-1D73FA11F3DA")
SoftEncry;
#endif
#endif /* __EncryEngineLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


