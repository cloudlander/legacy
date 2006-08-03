/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Sep 03 13:48:00 2002
 */
/* Compiler settings for E:\My Documents\My_Projects\IEPlusATL\IEPlusATL.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "IEPlusATL.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IIEPlus, ver. 0.0,
   GUID={0x4CAFB651,0x71B4,0x43F8,{0xB6,0x3F,0x5D,0x19,0x64,0xA7,0x5B,0x75}} */


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x10001, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

const CINTERFACE_PROXY_VTABLE(3) _IIEPlusProxyVtbl = 
{
    &IID_IIEPlus,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy
};


static const PRPC_STUB_FUNCTION IIEPlus_table[] =
{
    0    /* a dummy for an empty interface */
};

const CInterfaceStubVtbl _IIEPlusStubVtbl =
{
    &IID_IIEPlus,
    0,
    3,
    &IIEPlus_table[-3],
    CStdStubBuffer_METHODS
};

#pragma data_seg(".rdata")

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };

const CInterfaceProxyVtbl * _IEPlusATL_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IIEPlusProxyVtbl,
    0
};

const CInterfaceStubVtbl * _IEPlusATL_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IIEPlusStubVtbl,
    0
};

PCInterfaceName const _IEPlusATL_InterfaceNamesList[] = 
{
    "IIEPlus",
    0
};


#define _IEPlusATL_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _IEPlusATL, pIID, n)

int __stdcall _IEPlusATL_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_IEPlusATL_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo IEPlusATL_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _IEPlusATL_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _IEPlusATL_StubVtblList,
    (const PCInterfaceName * ) & _IEPlusATL_InterfaceNamesList,
    0, // no delegation
    & _IEPlusATL_IID_Lookup, 
    1,
    1,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
