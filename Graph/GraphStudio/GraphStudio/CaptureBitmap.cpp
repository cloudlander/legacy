// CaptureBitmap.cpp: implementation of the CCaptureBitmap class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CaptureBitmap.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC( CCaptureBitmap, CBitmap )
CCaptureBitmap :: CCaptureBitmap( )
{
    m_pPalette = NULL;
}
CCaptureBitmap:: ~CCaptureBitmap ( )
{
    if ( m_pPalette )
    {
        delete m_pPalette;
    }
}
void CCaptureBitmap::Capture( CRect &rect ,CDC& dcScreen)
{
    // cleanup old captures
    if ( m_pPalette )
    {
        DeleteObject() ;
        delete m_pPalette;
    }
    // save width and height
    m_nWidth = rect.Width() ;
    m_nHeight = rect.Height();
    //////////////////////////////////////////////////////////////////////
    // copy screen image into a bitmap object
    //////////////////////////////////////////////////////////////////////
    // create a device context that accesses the whole screen
//    CDC dcScreen;
//    dcScreen.CreateDC( "DISPLAY", NULL, NULL, NULL );
    // create an empty bitmap in memory
    CDC dcMem;
    dcMem.CreateCompatibleDC( &dcScreen );
	   CreateCompatibleBitmap( &dcScreen, m_nWidth, m_nHeight );
    dcMem.SelectObject( this );
    // copy screen into empty bitmap
    dcMem.BitBlt(0, 0, m_nWidth, m_nHeight, &dcScreen, rect.left, rect.top,SRCCOPY) ;
    // this bitmap is worthless without the current system palette, so...
    // save system palette in this bitmap's palette
    //////////////////////////////////////////////////////////////////////
    // create an empty logical palette that¡¯s big enough to hold all thecolors
    int nColors = ( 1 << ( dcScreen.GetDeviceCaps( BITSPIXEL ) *
        dcScreen.GetDeviceCaps( PLANES ) ) );
    LOGPALETTE *pLogPal = ( LOGPALETTE * )new BYTE[
        sizeof( LOGPALETTE ) + ( nColors * sizeof( PALETTEENTRY ) )];
    // initialize this empty palette¡¯s header
    pLogPal -> palVersion = 0x300;
    pLogPal -> palNumEntries = nColors;
    // load this empty palette with the system palette's colors
    ::GetSystemPaletteEntries( dcScreen.m_hDC, 0, nColors,
        ( LPPALETTEENTRY )( pLogPal -> palPalEntry ) );
    // create the palette with this logical palette
    m_pPalette = new CPalette;
    m_pPalette -> CreatePalette( pLogPal );
    // clean up
    delete []pLogPal;
    dcMem.DeleteDC() ;
//    dcScreen.DeleteDC() ;
}
void CCaptureBitmap::SaveBitmapEx( CString sFile )
{
    // create a DIB bitmap
    int bmData;
    BITMAPINFOHEADER bi;
    HANDLE hDIB = CreateDIB( bi,&bmData );
    // get a memory pointer to it
    LPBYTE lpBitmap = ( LPBYTE )::GlobalLock( hDIB );
    int bmSize = ::GlobalSize( hDIB );
    // create file
    CFile file;
    file.Open( sFile, CFile::modeCreate|CFile::modeWrite );
    // write the bitmap header
    BITMAPFILEHEADER bmfh;
    bmfh.bfType = 'MB' ; // (actually 'BM' for bitmap)
    bmfh.bfSize = sizeof( BITMAPFILEHEADER ) + bmSize;
//	bmfh.bfSize = sizeof(BITMAPFILEHEADER);
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits = bmData + sizeof(BITMAPFILEHEADER) ;
    file.Write( &bmfh,sizeof( BITMAPFILEHEADER ) );
	file.Write( &bi,sizeof( BITMAPINFOHEADER) );
    // write the bitmap body
    file.Write( lpBitmap,bmSize );
    // cleanup
    file.Close( ) ;
    ::GlobalUnlock( hDIB );
    ::GlobalFree( hDIB );
}
HANDLE CCaptureBitmap::CreateDIB(BITMAPINFOHEADER& bi,int *pbmData)
{
    ///////////////////////////////////////////
    // create DIB header from our BITMAP header
    ///////////////////////////////////////////
//    BITMAPINFOHEADER bi;
    memset( &bi, 0, sizeof( bi ) );
    bi.biSize = sizeof( BITMAPINFOHEADER );
    bi.biPlanes = 1;
    bi.biCompression = BI_RGB;
	bi.biClrUsed=0;
    // get and store dimensions of bitmap
    BITMAP bm;
    GetObject( sizeof( bm ),( LPSTR )&bm );
    bi.biWidth = bm.bmWidth ;
    bi.biHeight = bm.bmHeight;
    // get number of bits required per pixel
    int bits = bm.bmPlanes * bm.bmBitsPixel;
    if ( bits <= 1 )
        bi.biBitCount = 1;
    else if ( bits <= 4 )
        bi.biBitCount = 4;
    else if ( bits <= 8 )
        bi.biBitCount = 8;
    else
        bi.biBitCount = 24;
    // calculate color table size
    int biColorSize = 0;
    if ( bi.biBitCount != 24 ) biColorSize = (1<<bi.biBitCount);
    biColorSize *= sizeof( RGBQUAD );
    // calculate picture data size
    bi.biSizeImage = ( DWORD )bm.bmWidth * bi.biBitCount; // bits per row
    bi.biSizeImage = ( ( ( bi.biSizeImage ) + 31 ) / 32 ) * 4;
    // DWORD aligned
    bi.biSizeImage *= bm.bmHeight; // bytes required for whole bitmap
    // return size to caler in case they want to save to file
    if ( pbmData )
       *pbmData = bi.biSize + biColorSize;
//		*pbmData = sizeof(BITMAPINFOHEADER) + biColorSize + bi.biSize;
    ///////////////////////////////////////////
    // get DIB color table and picture data
    ///////////////////////////////////////////
    // allocate a hunk of memory to hold header, color table and picturedata
    HANDLE hDIB = ::GlobalAlloc( GHND, //bi.biSize + biColorSize +
        bi.biSizeImage );
    // get a memory pointer to this hunk by locking it
    LPBITMAPINFOHEADER lpbi = ( LPBITMAPINFOHEADER )::GlobalLock( hDIB );
    // copy our header structure into hunk
    *lpbi = bi;
    // get a device context and select our bitmap's palette into it
    CDC dc;
    dc.Attach( ::GetDC( NULL ) );
    CPalette *pPal = dc.SelectPalette( m_pPalette,FALSE );
    dc.RealizePalette() ;
    // load our memory hunk with the color table and picture data
    ::GetDIBits( dc.m_hDC, ( HBITMAP )m_hObject, 0, (UINT)bi.biHeight,
        (LPSTR)lpbi, //+ (WORD)lpbi->biSize+biColorSize,
        (LPBITMAPINFO)lpbi, DIB_RGB_COLORS );
    // clean up
    ::GlobalUnlock( hDIB );
    dc.SelectPalette( pPal,FALSE );
    dc.RealizePalette() ;
    // return handle to the DIB
    return hDIB;
}
