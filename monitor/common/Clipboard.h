// Clipboard.h: interface for the Clipboard class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _clipboard_h_
#define _clipboard_h_

#pragma once

#include <vector>
#include <string>
#include "ClipboardData.h"
#include "ClipboardsData.h"

class Clipboard  
{
public:
	Clipboard( CWnd* mainWnd, size_t maxMemory );
  virtual ~Clipboard();
  Clipboard(const Clipboard& rhs);
  const Clipboard& operator=(const Clipboard& rhs);

protected:
  void Init(CWnd* mainWnd);

public:
  bool GetTextFromClipboard( STD_TSTRING& sText, bool bQuote ) const;
  bool GetText( STD_TSTRING& sText, bool bQuote ) const;

  bool GetFolder( STD_TSTRING& sText, size_t idx, bool bQuote ) const;
  bool GetFile( STD_TSTRING& sText, size_t idx, bool bQuote ) const;
  bool GetURL( STD_TSTRING& sText, size_t idx, bool bQuote ) const;
  
protected:  
  typedef std::vector<ClipboardData *> V_CF;

  void ResetClipboardFormats(V_CF& cf) const;

protected:
  void ParseClipboard( V_CF& cf );

  void ResetClipBoardData();
  void AddFileName( const STD_TSTRING& s );
  void SetText(const STD_TSTRING& s);

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

protected:
  //  the list of possible items
  ClipboardsData _clipboardData;
  size_t _maxMemory;
};
#endif // _clipboard_h_
