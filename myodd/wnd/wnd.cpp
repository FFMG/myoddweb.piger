#include "stdafx.h"
#include "wnd.h"
#include "../math/math.h"
#include <assert.h>
#include "../string/string.h"

namespace myodd{ namespace wnd{
/**
 * Private function to get the Handle of the window
 * @param HWND the parent windows that contains the ID
 * @param WORD the ID if the item we are looking for.
 * @return HWND the handle of the Window, (could be NULL).
 */
HWND _getDlgItem( HWND hwndParent, WORD id )
{
  HWND hWnd = GetDlgItem( hwndParent, id );
  assert( hWnd ); //  was this the intent?
  return hWnd;
}

/**
 * Set focus to a window.
 * @param HWND the parent windows that contains the ID
 * @param WORD the ID if the item we are looking for.
 * @return bool success or not.
 */
bool SetFocus( HWND hwndParent, WORD id )
{
  HWND hwnd = _getDlgItem( hwndParent, id );
  if( NULL == hwnd )
  {
    return false;
  }
  ::SetFocus( hwnd );
  return true;
}

/**
 * Set the text to a given window.
 * @param HWND the parent window we are setting the text to.
 * @param WORD the id of the window.
 * @param const MYODD_CHAR* the text we want to set.
 * @return bool true|false if the value is set or not.
 */
bool SetText( HWND hwndParent, WORD id, const MYODD_CHAR* lp )
{
  HWND hwnd = _getDlgItem( hwndParent, id );
  if( NULL == hwnd )
  {
    return false;
  }
  return (::SetWindowText( hwnd, (lp ? lp : _T("")) ) ? true : false);
}

/**
 * Set the text to a given window.
 * @param HWND the parent window we are setting the text to.
 * @param WORD the id of the window.
 * @param const MYODD_STRING& the text we want to set.
 * @return bool true|false if the value is set or not.
 */
bool SetText( HWND hwndParent, WORD id, const MYODD_STRING& s )
{
  return SetText( hwndParent, id, s.c_str() );
}

/**
 * Set a window text
 * @param HWND the window we are setting the text for.
 * @param const varied, the variable type we are setting the text for.
 * @return none
 */
bool SetText( HWND hwndParent, WORD id, int i)
{
  HWND hwnd = _getDlgItem( hwndParent, id );
  if( NULL == hwnd )
  {
    return false;
  }
  return (::SetWindowText( hwnd, myodd::strings::ToString( i ).c_str() ) ? true : false);
}

/**
 * Get the unsigned int value from a dialog item
 * @param HWND the parent window that holds the control
 * @param WORD the ID of the control we are getting the text from.
 * @param unsigned int the default value in case there is no number.
 * @return unsigned int the value been held by the control.
 */
unsigned int GetUInt( HWND hwndParent, WORD id, unsigned int uiDefault/*=0*/ )
{
  return static_cast<unsigned int>(GetInt( hwndParent, id, uiDefault ));
}

/**
 * Get the int value from a dialog item
 * @param HWND the parent window that holds the control
 * @param WORD the ID of the control we are getting the text from.
 * @param int the default value in case there is no number.
 * @return int the value been held by the control.
 */
int GetInt( HWND hwndParent, WORD id, int iDefault/*=0*/ )
{
  HWND hwnd = _getDlgItem( hwndParent, id );
  if( NULL == hwnd )
  {
    return iDefault;
  }

  // get the text
  MYODD_STRING s = GetText( hwndParent, id );
  size_t length = s.length();
  if( length == 0 )
  {
    return iDefault;
  }

  // we allow decimal numbers here 
  // because _tstoi(...) will remove the decimal point
  // and it is better than returning the default in this case.
  if( !myodd::strings::IsNumeric( s ))
  {
    return iDefault;
  }

  int i = _tstoi( s.c_str() );
  return i;
}

/**
 * Get the long value from a dialog item
 * @param HWND the parent window that holds the control
 * @param WORD the ID of the control we are getting the text from.
 * @param long the default value in case there is no number.
 * @return long the value been held by the control.
 */
long GetLong( HWND hwndParent, WORD id, long lDefault/*=0*/ )
{
  HWND hwnd = _getDlgItem( hwndParent, id );
  if( NULL == hwnd )
  {
    return lDefault;
  }

  // get the text
  MYODD_STRING s = GetText( hwndParent, id );
  size_t length = s.length();
  if( length == 0 )
  {
    return lDefault;
  }

  // we allow decimal numbers here 
  // because _tstoi(...) will remove the decimal point
  // and it is better than returning the default in this case.
  if( !myodd::strings::IsNumeric( s ))
  {
    return lDefault;
  }

  long l = _tstol( s.c_str() );
  return l;
}

/**
 * Get the double value from a dialog item
 * @param HWND the parent window that holds the control
 * @param WORD the ID of the control we are getting the text from.
 * @param double the default value in case there is no number.
 * @return double the value been held by the control.
 */
double GetDouble( HWND hwndParent, WORD id, double dDefault/*=0*/ )
{
  HWND hwnd = _getDlgItem( hwndParent, id );
  if( NULL == hwnd )
  {
    return dDefault;
  }

  // get the text
  MYODD_STRING s = GetText( hwndParent, id );
  size_t length = s.length();
  if( length == 0 )
  {
    return dDefault;
  }

  if( !myodd::strings::IsNumeric( s ))
  {
    return dDefault;
  }
  
  double d = _tstof( s.c_str() );
  return d;
}

/**
 * Get the text value from a dialog item, by default the values are trimmed.
 * @param HWND the parent window that holds the control
 * @param WORD the ID of the control we are getting the text from.
 * @param bool if we want to trim the return value.
 * @return MYODD_STRING the value been held by the control.
 */
MYODD_STRING GetText( HWND hwndParent, WORD id, bool bTrim /*= true*/ )
{
  HWND hwnd = _getDlgItem( hwndParent, id );
  if( NULL == hwnd )
  {
    return _T("");
  }

  // get the text lenght
  int lenght = GetWindowTextLength( hwnd );

  // get the text
  MYODD_CHAR* t = new MYODD_CHAR[ (lenght+1)*sizeof(MYODD_CHAR) ];
  memset( t, 0, (lenght+1)*sizeof(MYODD_CHAR) );
  GetWindowText( hwnd, t,(lenght+1)*sizeof(MYODD_CHAR) );

  MYODD_STRING returnValue = t;
  delete [] t;

  if( bTrim )
  {
    //  trim the return value.
    myodd::strings::Trim( returnValue );
  }
  return returnValue;
}

/**
 * Check a button
 * @param HWND the owner window
 * @param WORD the ID of the Button we are updating
 * @param int the check state.
 * @return bool success or if there was some error
 */
bool CheckButton( HWND hwndParent, WORD wID, int nCheck )
{
  HWND hWnd = _getDlgItem( hwndParent, wID );
  if( NULL == hWnd )
  {
    return false;
  }
	
  ::SendMessage(hWnd, BM_SETCHECK, nCheck, 0);
  return true;
}

/**
 * Return true is a button is checked or not.
 * @param HWND the owner window
 * @param WORD the ID of the Button we are updating
 * @return bool if the button is checked or not.
 */
bool IsButonChecked( HWND hwndParent, WORD wID )
{
  HWND hWnd = _getDlgItem( hwndParent, wID );
	if( NULL == hWnd )
  {
    return false;
  }
	return ((int)::SendMessage(hWnd, BM_GETCHECK, 0, 0) == 1);
}

/**
 * 'enable' a window
 * @param WORD the ID of the button we are 'enabeling'
 * @param bool if we are enabling the button or not.
 * @return success or false if there was a problem.
 */
bool EnableWindow( HWND hwndParent, WORD id, bool bEnable )
{
  HWND hWnd = _getDlgItem( hwndParent, id );
  if( NULL == hWnd )
  {
    return false;
  }
  return (::EnableWindow( hWnd, bEnable )?true:false);
}

/**
 * Show/Hide a dialog item.
 * @param HWND the parent window.
 * @param WORD the ID of the button we are 'enabeling'
 * @param bool if we are showing the button/dialog or not.
 * @return success or false if there was a problem.
 */
bool ShowWindow( HWND hwndParent, WORD id, bool bShow )
{
  HWND hwnd = _getDlgItem( hwndParent, id );
  if( NULL == hwnd )
  {
    return false;
  }
  return (::ShowWindow( hwnd, bShow? SW_SHOW : SW_HIDE )?true:false);
}

/**
 * Set a window text with a double value.
 * @param HWND the window we are setting the text for.
 * @param WORD the ID of the button we are 'enabeling'
 * @param const double the double we are setting the text for.
 * @param const MYODD_CHAR* the format of the text.
 * @return boolean success or not
 */
bool SetText( HWND hwndParent, WORD id, double d, const MYODD_CHAR* pszFormat)
{
  HWND hwnd = _getDlgItem( hwndParent, id );
  if( NULL == hwnd )
  {
    return false;
  }

  return (::SetWindowText( hwnd, myodd::strings::ToString( d, pszFormat ).c_str() ) ? true : false);
}

/**
 * Make sure that the value entered by the user is a valid integer.
 * @param HWND the parent window we want to ensure the ID is an int
 * @param WORD the ID of the control we are checking.
 * @return none.
*/
void MakeValidInt( HWND hWndParent, WORD id )
{
  MYODD_STRING sValue = myodd::wnd::GetText( hWndParent, id );
  if( !myodd::strings::IsNumeric( sValue, false ) )
  {
    // the new value
    MYODD_STRING sActual = myodd::strings::ToString( _tstoi(sValue.c_str()) );
    
    // set it.
    myodd::wnd::SetText( hWndParent, id, sActual );

    // move the cursor at the end of the text
    int nStartChar = (int)sActual.length();
    int nEndChar = nStartChar;
    SetSel( hWndParent, id, nStartChar, nEndChar );
  }
}

/**
 * Set the current text selection.
 * @param HWND the parent window we want to ensure the ID is an int
 * @param WORD the ID of the control we are checking.
 * @param int the start character position
 * @param int the end character position.
 * @return bool success or not.
 */
bool SetSel( HWND hWndParent, WORD id, int nStartChar, int nEndChar )
{
  HWND hWnd = _getDlgItem( hWndParent, id );
  if( NULL == hWnd )
  {
    return false;
  }
  ::SendMessage(hWnd, EM_SETSEL, nStartChar, nEndChar);
  return true;
}

/**
 * Make sure that the value entered by the user is a valid integer and is within a range.
 * @param HWND the parent window we want to ensure the ID is an int
 * @param WORD the ID of the control we are checking.
 * @param int the min valid value, (if less than that it will be set to the min value).
 * @param int the max valid value, (if more than that it will be set to that max value).
 * @return none.
*/
void MakeValidIntRange( HWND hWndParent, WORD id, int nMin, int nMax )
{
  assert(nMin <= nMax );
  if( nMin > nMax )
  {
    myodd::math::swap( nMax, nMin );
  }

  auto sValue = myodd::wnd::GetText( hWndParent, id );
  if( !myodd::strings::IsNumeric( sValue, false ) )
  {
    sValue = myodd::strings::ToString( _tstoi(sValue.c_str()) );
    myodd::wnd::SetText( hWndParent, id, sValue );
  
    const auto nStartChar = static_cast<int>(sValue.length());
    const auto nEndChar = nStartChar;
    SetSel( hWndParent, id, nStartChar, nEndChar);
  }

  // now make sure that it is withing ranges.
  const auto nActual = myodd::math::Convert<const MYODD_STRING&, int>( sValue );
  if( nActual > nMax )
  {
    sValue = myodd::strings::ToString( nMax );
    myodd::wnd::SetText( hWndParent, id, sValue );

    const auto nStartChar = static_cast<int>(sValue.length());
    const auto nEndChar = nStartChar;
    SetSel( hWndParent, id, nStartChar, nEndChar);
  }
  else if( nActual < nMin )
  {
    sValue = myodd::strings::ToString( nMin );
    myodd::wnd::SetText( hWndParent, id, sValue );

    int nStartChar = (int)sValue.length();
    int nEndChar = nStartChar;
    SetSel( hWndParent, id, nStartChar, nEndChar);
  }
}

/**
 * Make sure that the value entered by the user is a valid integer.
 * @param HWND the parent window we want to ensure the ID is an int
 * @param WORD the ID of the control we are checking.
 * @return none.
*/
void MakeValidUInt( HWND hWndParent, WORD id )
{
  MYODD_STRING sValue = myodd::wnd::GetText( hWndParent, id );
  if( !myodd::strings::IsNumeric( sValue, false ) )
  {
    int iActual = _tstoi(sValue.c_str());
    if( iActual < 0 )
    {
      iActual = 0;
    }

    MYODD_STRING sActual = myodd::strings::ToString( iActual );
    myodd::wnd::SetText( hWndParent, id, sActual );

    int nStartChar = (int)sActual.length();
    int nEndChar = nStartChar;
    SetSel( hWndParent, id, nStartChar, nEndChar);
  }
}

/**
 * Make sure that the value entered by the user is a valid double and is within a range.
 * @param HWND the parent window we want to ensure the ID is an int
 * @param WORD the ID of the control we are checking.
 * @param double the min valid value, (if less than that it will be set to the min value).
 * @param double the max valid value, (if more than that it will be set to that max value).
 * @param const MYODD_CHAR* the format we will use in case we need to re-write the text
 * @return none.
*/
void MakeValidDoubleRange( HWND hWndParent, WORD id, double nMin, double nMax, const MYODD_CHAR* lpszFormat )
{
  MYODD_STRING sValue = myodd::wnd::GetText( hWndParent, id );
  if( !myodd::strings::IsNumeric( sValue, true ) )
  {
    sValue = myodd::strings::ToString( _tstof( sValue.c_str()), lpszFormat );
    myodd::wnd::SetText( hWndParent, id, sValue );

    int nStartChar = (int)sValue.length();
    int nEndChar = nStartChar;
    SetSel( hWndParent, id, nStartChar, nEndChar);
  }

  // now make sure that it is withing ranges.
  const auto nActual = myodd::math::Convert<const MYODD_STRING&, double>( sValue );
  if( nActual > nMax )
  {
    sValue = myodd::strings::ToString( nMax, lpszFormat );
    myodd::wnd::SetText( hWndParent, id, sValue );

    const auto nStartChar = static_cast<int>(sValue.length());
    const auto nEndChar = nStartChar;
    SetSel( hWndParent, id, nStartChar, nEndChar);
  }
  else if( nActual < nMin )
  {
    sValue = myodd::strings::ToString( nMin, lpszFormat );
    myodd::wnd::SetText( hWndParent, id, sValue );

    int nStartChar = (int)sValue.length();
    int nEndChar = nStartChar;
    SetSel( hWndParent, id, nStartChar, nEndChar);
  }
}

/**
 * Make sure that the value entered by the user is a valid double.
 * @param HWND the parent window we want to ensure the ID is an double
 * @param WORD the ID of the control we are checking.
 * @param const MYODD_CHAR* the format we will use in case we need to re-write the text
 * @return none.
*/
void MakeValidDouble( HWND hWndParent, WORD id, const MYODD_CHAR* lpszFormat )
{
  MYODD_STRING sValue = myodd::wnd::GetText( hWndParent, id );
  if( !myodd::strings::IsNumeric( sValue, true ) )
  {
    MYODD_STRING sActual = myodd::strings::ToString( _tstof(sValue.c_str()), lpszFormat );
    myodd::wnd::SetText( hWndParent, id, sValue );

    int nStartChar = (int)sValue.length();
    int nEndChar = nStartChar;
    SetSel( hWndParent, id, nStartChar, nEndChar);
  }
}

/**
 * Enable/Disable all the controls in a given window.
 * We log the current state of the window so the user restore the dialog.
 * @see RestoreDialog( ... )
 * @param HWND parent window that owns all the controls.
 * @param bool enable or disable the windows.
 * @param MYODD_WINDOWS_STATE& map of all the current states.
 * @return none
 */
void EnableDialog( HWND hWndParent, bool bEnable, MYODD_WINDOWS_STATE& states )
{
  // we don't reset the list of STATES
  // because the user could be disabling a lot of windows at once.
  // and it does not really matter to us
  HWND hwnd = ::GetTopWindow( hWndParent );
  while (hwnd)
  {
    // get the current state of the window.
    bool e = ::IsWindowEnabled( hwnd ) ? true : false;

    // save it
    states[ hwnd ] = e;

    // set the new state of that window.
    if( bEnable != e )
    {
      ::EnableWindow( hwnd, bEnable );
    }

    // look for the next item
    hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT);
  }
}

/**
 * Enable/Disable all the controls in a given window to the state they were.
 * @see EnableDialog( ... )
 * @param const MYODD_WINDOWS_STATE& map of all the current states as they were.
 * @return none
 */
void RestoreDialog( const MYODD_WINDOWS_STATE& states )
{
  for( MYODD_WINDOWS_STATE::const_iterator it = states.begin();
       it != states.end();
       ++it )
  {
    // get the current state of the window.
    bool e = ::IsWindowEnabled( it->first ) ? true : false;

    // is the state about to change?
    if( e != it->second )
    {
      ::EnableWindow( it->first, it->second );
    }
  }
}

/**
 * In the case of multiple monitors, get the dimensions of the nearest one.
 * Normally the 'nearest' monitor is the current monitor.
 * @param HWND the window we are looking for the parent monitor.
 * @param RECT the rectangle that will contain the dimensions of the nearest rectangle.
 * @return none.
 */
void GetNearestMonitorRect( HWND hwnd, RECT& rect )
{
  HMONITOR hm = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
  MONITORINFO mi;
  
  mi.cbSize = sizeof( mi );
  ::GetMonitorInfo( hm, &mi );
  rect = mi.rcMonitor;
}

 #define TA_FAILED 0
 #define TA_SUCCESS_CLEAN 1
 #define TA_SUCCESS_KILL 2
 #define TA_SUCCESS_16 3

 struct TERMINFO
 {
   unsigned long   dwID ;
   unsigned long   dwThread ;
 };

 // Declare Callback Enum Functions.
 BOOL CALLBACK _TerminateAppEnum( HWND hwnd, MYODD_LPARAM lParam ) ;

 /*----------------------------------------------------------------
 unsigned long WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout )

 Purpose:
    Shut down a 32-Bit Process (or 16-bit process under Windows 95)

 Parameters:
    dwPID
       Process ID of the process to shut down.

    dwTimeout
       Wait time in milliseconds before shutting down the process.

 Return Value:
    TA_FAILED - If the shutdown failed.
    TA_SUCCESS_CLEAN - If the process was shutdown using WM_CLOSE.
    TA_SUCCESS_KILL - if the process was shut down with
       TerminateProcess().
    NOTE:  See header for these defines.
 ----------------------------------------------------------------*/ 
 unsigned long WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout )
{
  HANDLE   hProc ;
  unsigned long   dwRet ;

  // If we can't open the process with PROCESS_TERMINATE rights,
  // then we give up immediately.
  hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE, dwPID);

  if(hProc == NULL)
  {
    return TA_FAILED ;
  }

  // _TerminateAppEnum() posts WM_CLOSE to all windows whose PID
  // matches your process's.
  EnumWindows((WNDENUMPROC)_TerminateAppEnum, (MYODD_LPARAM) dwPID) ;

  // Wait on the handle. If it signals, great. If it times out,
  // then you kill it.
  if(WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0)
    dwRet=(TerminateProcess(hProc,0)?TA_SUCCESS_KILL:TA_FAILED);
  else
    dwRet = TA_SUCCESS_CLEAN ;

  CloseHandle(hProc) ;

  return dwRet ;
}

BOOL CALLBACK _TerminateAppEnum( HWND hwnd, MYODD_LPARAM lParam )
{
  unsigned long dwID ;

  GetWindowThreadProcessId(hwnd, &dwID) ;

  if(dwID == (unsigned long)lParam)
  {
    PostMessage(hwnd, WM_CLOSE, 0, 0) ;
  }

  return TRUE ;
}

void MessagePump(const HWND hwnd)
{
  MSG msg;
  while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE) > 0)
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

} //  wnd
} //  myodd