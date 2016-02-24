// Clipboard.h: interface for the Clipboard class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _clipboard_h_
#define _clipboard_h_

#pragma once

#include <vector>
#include <string>

class Clipboard  
{
public:
	Clipboard();
	virtual ~Clipboard();

  void Get( CWnd* cwnd );

public:
  BOOL GetTextFromClipboard( STD_TSTRING& sText, BOOL bQuote = TRUE ) const;
  BOOL GetText( STD_TSTRING& sText ) const;
  BOOL GetFolder( STD_TSTRING& sText, UINT idx ) const;
  BOOL GetFileNames( STD_TSTRING& sText, UINT idx ) const;
  BOOL GetURL( STD_TSTRING& sText, UINT idx ) const;
  
protected:  
  CWnd* cwndMain;

  void Init();

  struct CLIPBOARD_DATA 
  {
    STD_TSTRING cdTEXT;
    std::map<STD_TSTRING, STD_TSTRING > cdFILENAMES;
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
          size_t l = _tcslen( cf.dataName );
          dataName = new TCHAR[l+1];
          memset( dataName, 0, l+1 );
          _tcsncpy_s( dataName, l, cf.dataName, l);
        }
        
        //  special cases.
        dataMetaFile = cf.dataMetaFile;
      }
      return *this;
    };

    BYTE* data;
    SIZE_T  dataSize;
    UINT uFormat;
    TCHAR* dataName;

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

  typedef std::vector<CLIPBOARD_FORMAT *> V_CF;

  void ResetClipboardFormats(V_CF& cf) const;

  // Returns the data got from the Clipboard, if not NULL
  Clipboard::CLIPBOARD_FORMAT * GetDataFromClipboard( UINT format );

protected:
  void ParseClipboard( V_CF& cf );
  BOOL ParseExplorerData( CWnd* cwnd );

  void ResetClipBoardData();
  void AddFileName( const STD_TSTRING& s );

protected:
  HRESULT GetNameFromPIDL
  ( 
   LPCWSTR pwzType,
   IShellFolder* pFolder, 
   LPCITEMIDLIST pidl,
   LPTSTR buffer, 
   DWORD lBuffer
   );
protected:
  void GetCurrentData( CWnd* wnd, V_CF& cf );
  
protected:
  BOOL RestoreClipboard( CWnd* wnd, V_CF& cf );

protected:
  void CopyDataFromForgroundWindow( CWnd* cPossibleWindow );
};

#endif // _clipboard_h_
