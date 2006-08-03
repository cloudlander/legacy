/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Sep 03 13:48:00 2002
 */
/* Compiler settings for E:\My Documents\My_Projects\IEPlusATL\IEPlusATL.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __IEPlusATL_h__
#define __IEPlusATL_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IIEPlus_FWD_DEFINED__
#define __IIEPlus_FWD_DEFINED__
typedef interface IIEPlus IIEPlus;
#endif 	/* __IIEPlus_FWD_DEFINED__ */


#ifndef __IEPlus_FWD_DEFINED__
#define __IEPlus_FWD_DEFINED__

#ifdef __cplusplus
typedef class IEPlus IEPlus;
#else
typedef struct IEPlus IEPlus;
#endif /* __cplusplus */

#endif 	/* __IEPlus_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IIEPlus_INTERFACE_DEFINED__
#define __IIEPlus_INTERFACE_DEFINED__

/* interface IIEPlus */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IIEPlus;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4CAFB651-71B4-43F8-B63F-5D1964A75B75")
    IIEPlus : public IUnknown
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IIEPlusVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IIEPlus __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IIEPlus __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IIEPlus __RPC_FAR * This);
        
        END_INTERFACE
    } IIEPlusVtbl;

    interface IIEPlus
    {
        CONST_VTBL struct IIEPlusVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIEPlus_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IIEPlus_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IIEPlus_Release(This)	\
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IIEPlus_INTERFACE_DEFINED__ */



#ifndef __IEPLUSATLLib_LIBRARY_DEFINED__
#define __IEPLUSATLLib_LIBRARY_DEFINED__

/* library IEPLUSATLLib */
/* [helpstring][version][uuid] */ 


//EXTERN_C const IID LIBID_IEPLUSATLLib;

EXTERN_C const CLSID CLSID_IEPlus;

#ifdef __cplusplus

class DECLSPEC_UUID("83C64D70-F259-4384-8443-217351436BE6")
IEPlus;
#endif
#endif /* __IEPLUSATLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
