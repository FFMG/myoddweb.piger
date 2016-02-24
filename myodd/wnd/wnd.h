#pragma once

#ifndef __Wnd_h__
#define __Wnd_h__

/**
 * Do all the common window calls.
 * for example getwindowtext() or setchecked() and so forth.
 */
#include "../common/includes.h"
#include "../string/string.h"
#include <map>

// -- Forward declaration.
namespace myodd{ namespace wnd{
  typedef std::map< HWND, bool> MYODD_WINDOWS_STATE;

  // -- Strings
  STD_TSTRING GetText( HWND hwndParent, WORD id, bool bTrim = true ); 

  // -- numbers.
  int GetInt      ( HWND hwndParent, WORD id, int iDefault=0 ); 
  long GetLong    ( HWND hwndParent, WORD id, long lDefault=0 );
  unsigned int GetUInt( HWND hwndParent, WORD id, unsigned int uiDefault=0 );
  double GetDouble( HWND hwndParent, WORD id, double dDefault=0 ); 

  // -- Helper functions
  void MakeValidUInt( HWND hwndParent, WORD id );
  void MakeValidInt( HWND hwndParent, WORD id ); 
  void MakeValidIntRange(HWND hwndParent, WORD id, int nMin, int nMax );
  void MakeValidDouble( HWND hwndParent, WORD id, LPCTSTR lpszFormat = _T("%f") ); 
  void MakeValidDoubleRange( HWND hwndParent, WORD id, double nMin, double nMax, LPCTSTR lpszFormat = _T("%f") );

  bool SetFocus( HWND hwndParent, WORD id );

  bool SetText( HWND hwndParent, WORD id, const STD_TSTRING& s );
  bool SetText( HWND hwndParent, WORD id, const TCHAR* lp );
  bool SetText( HWND hwndParent, WORD id, int i );
  bool SetText( HWND hwndParent, WORD id, double d, const TCHAR* pszFormat );

  // -- windows helpers.
  bool EnableWindow(HWND hWndParent, WORD id, bool bEnable );
  bool ShowWindow( HWND hWndParent, WORD id, bool bShow );
  bool SetSel( HWND hWndParent, WORD id, int nStartChar, int nEndChar );

  // -- Buttons
  bool CheckButton( HWND hwndParent, WORD id, int nCheck );
  bool IsButonChecked( HWND hwndParent, WORD wID );

  void EnableDialog( HWND hWnd, bool bEnable, MYODD_WINDOWS_STATE& states );
  void RestoreDialog( const MYODD_WINDOWS_STATE& states );

  void GetNearestMonitorRect( HWND hwndParent, RECT& rect );

  // -- kill an app.
  DWORD WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout ) ;
} //  wnd
} //  myodd

#endif // __Wnd_h__
