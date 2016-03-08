// Clipboard.h: interface for the Clipboard class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _clipboard_h_
#define _clipboard_h_

#pragma once

#include <vector>
#include <string>
#include "ClipboardData.h"

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

  typedef std::vector<ClipboardData *> V_CF;

  void ResetClipboardFormats(V_CF& cf) const;

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
  bool RestoreClipboard( CWnd* wnd, V_CF& cf );

protected:
  void CopyDataFromForgroundWindow( CWnd* cPossibleWindow );
};
#endif // _clipboard_h_
