// CaptureBitmap.h: interface for the CCaptureBitmap class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CAPTUREBITMAP_H__5EDECB3D_5873_4DBF_AED4_74CB2FE0FBC5__INCLUDED_)
#define AFX_CAPTUREBITMAP_H__5EDECB3D_5873_4DBF_AED4_74CB2FE0FBC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CCaptureBitmap  : public CBitmap
{
public:
    DECLARE_DYNAMIC( CCaptureBitmap )
    CCaptureBitmap();
    virtual ~CCaptureBitmap();

    void Capture( CRect &rect,CDC& dcMem );
    void SaveBitmapEx( CString sFile );
    // Implementation
public :
    // Attributes
    int m_nWidth ;
    int m_nHeight;
    // Operations
private :
    CPalette *GetPalette(){return m_pPalette;};
    HANDLE CreateDIB( BITMAPINFOHEADER& bi,int *pbmData = NULL);
    CPalette *m_pPalette;
};
#endif // !defined(AFX_CAPTUREBITMAP_H__5EDECB3D_5873_4DBF_AED4_74CB2FE0FBC5__INCLUDED_)
