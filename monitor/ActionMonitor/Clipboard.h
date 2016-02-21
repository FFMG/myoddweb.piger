// Clipboard.h: interface for the CClipboard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIPBOARD_H__28ADF244_6AB0_42B9_8C5A_45FF34557182__INCLUDED_)
#define AFX_CLIPBOARD_H__28ADF244_6AB0_42B9_8C5A_45FF34557182__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>

class CClipboard  
{
public:
	CClipboard( HWND hwnd );
	virtual ~CClipboard();

public:
  BOOL GetTextFromClipboard( std::string &sText );
  
protected:  
  HWND hwndMain;

  void Init();

  struct CLIPBOARD_DATA 
  {
    std::string cdFOLDER;
    std::string cdTEXT;
    std::string cdFILENAME;
    std::vector< std::string > cdFILENAMES;
  };  //  CLIPBOARD_DATA

  //  the list of possible items
  CLIPBOARD_DATA clipboard_data;

  struct CLIPBOARD_FORMAT
  {
    CLIPBOARD_FORMAT( const CLIPBOARD_FORMAT&cf){ NullAll(); *this = cf;};
    CLIPBOARD_FORMAT(){ NullAll(); };
    ~CLIPBOARD_FORMAT(){ ClearAll(); }
    const CLIPBOARD_FORMAT& operator= ( const CLIPBOARD_FORMAT&cf)
    {
      if( this != &cf )
      {
        ClearAll();
        uFormat   = cf.uFormat;
        dataSize  = cf.dataSize;
        data = new BYTE[cf.dataSize];
        memcpy( data, cf.data, cf.dataSize );
        if( cf.dataName )
        {
          unsigned l = strlen(cf.dataName );
          dataName = new char[l+1];
          memset( dataName, 0, l+1 );
          memcpy( dataName, cf.dataName, l );
        }
        
        //  special cases.
        dataMetaFile = cf.dataMetaFile;
      }
      return *this;
    };

    BYTE *data;
    SIZE_T  dataSize;
    UINT uFormat;
    char *  dataName;

    HENHMETAFILE dataMetaFile;  //  special case only used with CF_ENHMETAFILE

    void ClearAll()
    {
      if( data )      delete [] data;
      if( dataName )  delete [] dataName;
      NullAll();
    }
    void NullAll()
    {
      dataMetaFile  = NULL;
      uFormat       = 0;
      dataSize      = 0;
      data          = NULL;
      dataName      = NULL;
    }
  };  //  CLIPBOARD_FORMAT
  void ClearStruct(std::vector<CLIPBOARD_FORMAT *> *cf);

  // Returns the data got from the Clipboard, if not NULL
  CClipboard::CLIPBOARD_FORMAT * GetDataFromClipboard( UINT format, HWND h );

protected:
  void ParseClipboard( std::vector<CLIPBOARD_FORMAT *> *cf );

protected:
  std::string implode( const std::vector<std::string> &s, std::string glue );

protected:
  HRESULT GetNameFromPIDL
  ( 
   LPCWSTR pwzType,
   IShellFolder* pFolder, 
   LPCITEMIDLIST pidl,
   LPSTR buffer, 
   DWORD lBuffer
   );
protected:
  void GetCurrentData( HWND h, std::vector<CLIPBOARD_FORMAT *> *cf );

protected:
  BOOL RestoreClipboard( HWND h, std::vector<CLIPBOARD_FORMAT *> *cf );

protected:
  void CopyDataFromForgroundWindow( HWND hPossibleWindow );
};

#endif // !defined(AFX_CLIPBOARD_H__28ADF244_6AB0_42B9_8C5A_45FF34557182__INCLUDED_)
