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
    CLIPBOARD_FORMAT(){ NullAll(); };
    ~CLIPBOARD_FORMAT(){ ClearAll(); }

  private:
    // ty shall not copy...
    CLIPBOARD_FORMAT(const CLIPBOARD_FORMAT&cf) { NullAll(); *this = cf; };
    const CLIPBOARD_FORMAT& operator= (const CLIPBOARD_FORMAT&cf);

  public:
    BYTE* data;
    SIZE_T  dataSize;
    UINT uFormat;
    wchar_t* dataName;

    void ClearAll()
    {
      if (data)
      {
        if (CF_ENHMETAFILE == uFormat )
        {
          DeleteEnhMetaFile((HENHMETAFILE)data);
        }
        else
        {
          delete[] data;
        }
        data = NULL;
      }
      if (dataName)
      {
        delete[] dataName;
      }
      NullAll();
    }
    void NullAll()
    {
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
