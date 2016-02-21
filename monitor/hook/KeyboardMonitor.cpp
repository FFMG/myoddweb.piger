#include "stdafx.h"

#include "KeyboardMonitor.h"
#include "hook.h"

KeyboardMonitor* keyboardMonitor;

KeyboardMonitor::KeyboardMonitor( HANDLE hModule ) :
  m_hModule( hModule ),
  m_hhook( NULL ),
  m_bRejectKeyBoardInputs( FALSE )
{
  UWM_KEYBOARD_CHAR = RegisterWindowMessage(UWM_KEYBOARD_MSG_CHAR);
  UWM_KEYBOARD_UP   = RegisterWindowMessage(UWM_KEYBOARD_MSG_UP);
  UWM_KEYBOARD_DOWN = RegisterWindowMessage(UWM_KEYBOARD_MSG_DOWN);
}

KeyboardMonitor::~KeyboardMonitor(void)
{
  ClearHooks();
}

// -----------------------------------------------------------------------
BOOL KeyboardMonitor::HandleHook (int nCode )
{
  if(nCode < 0 || HC_ACTION != nCode )
  {
    return FALSE;
  }
  return TRUE;
}

// -----------------------------------------------------------------------
LRESULT CALLBACK KeyboardMonitor::CallLowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  //  are we handling that hook?
  if( FALSE == keyboardMonitor->HandleHook( nCode ))
  {
    return CallNextHookEx( keyboardMonitor->GetHHOOK(), nCode, wParam, lParam);
  }
  
  // By returning a non-zero value from the hook procedure, the
  // message does not get passed to the target window
  KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
  int error=GetLastError();   
  switch (nCode)
  {
  case HC_ACTION:
    {
      LRESULT hHandle = keyboardMonitor->HandleMessage( wParam, pkbhs->vkCode, pkbhs->dwExtraInfo );
      switch ( hHandle )
      {
      case 1: //  the key was handled by someone
        {
          //  if we have been instructed to reject messages simply change the message to nothing
          //  other processes are supposed to ignore WM_NULL
          //  this is not full proof, (we might not be the first hook in the queue), but the odds of 
          //  multiple hooks all trying to do the same thing is limited.
          if( keyboardMonitor->RejectKeyboadInputs() == TRUE )
          {
            return 1;
          }
        }
        break;

      case 0:
        break;
      }
    }

    // if it is a special key then we must not handle it.
    // even if it is a repeat key...
    if (keyboardMonitor->IsSpecialKey( pkbhs->vkCode ) )
    {
      keyboardMonitor->HandleMessage( wParam, VK_CAPITAL, 0 );
      // TRACE( "Rejected Caps Lock Key\n" );
      return 1;
    }
    break;  // HC_ACTION 
    
  default:
    break;
  }
  return CallNextHookEx ( keyboardMonitor->GetHHOOK(), nCode, wParam, lParam);
}// CallLowLevelKeyboardProc

// -----------------------------------------------------------------------
// Just look if one of the windows expects that key
BOOL KeyboardMonitor::IsSpecialKey( const WPARAM wParam ) const
{
  for( std::map< HWND, WPARAM >::const_iterator it = m_mapWindows.begin(); it != m_mapWindows.end(); ++it )
  {
    if( wParam == it->second )
    {
      //  yes, this window uses that key.
      return TRUE;  
    }
  }

  // if we are here we never found it.
  return FALSE;
}

// -----------------------------------------------------------------------
/**
 * Given the message that was passed we must either send it to the parent window
 * Or simply return 0L
 */
LRESULT KeyboardMonitor::HandleMessage
(
  const UINT msg, 
  const WPARAM wParam, 
  const LPARAM lParam 
)
{
  if( msg < WM_KEYFIRST || msg > WM_KEYLAST)
  {
    return 0L;
  }

	//	get the number of milli secs from start of system
	DWORD tick	=	GetTickCount();
	//
	//	do not allow items to be too quick
  if( GetLastKey( tick ) .similar( msg, wParam)  )
	{	
    //	the key we just entered has been repeated too soon for us and is rejected.
		// TRACE( "<< Repeat Key >>\n" );
		return 0L;
	}
  // save the last key so we can keep track of the timing
  // it is not uncommon for apps to request the last key more than once.
  SetLastKey( LAST_KEY(msg, wParam, tick) );

  LRESULT lResult = 0L; //  assume that nobody will handle it.
  for( std::map< HWND, WPARAM >::const_iterator it = m_mapWindows.begin(); it != m_mapWindows.end(); ++it )
  {
    HWND hWnd = it->first;
    switch( msg )
    {
    case WM_KEYDOWN:
      // they key was handled by at least one persone.
      lResult = 1L;
      PostMessage( hWnd, UWM_KEYBOARD_DOWN, wParam, lParam );
      break;

    case WM_KEYUP:
      // they key was handled by at least one persone.
      lResult = 1L;
      PostMessage(hWnd, UWM_KEYBOARD_UP, wParam, lParam );
      break;

    case WM_CHAR:
      // they key was handled by at least one persone.
      lResult = 1L;
      PostMessage(hWnd, UWM_KEYBOARD_CHAR, wParam, lParam );
      break;

    default:
      // Not handled or we don't care.
      break;
    }
  }
  return lResult;
}

// -----------------------------------------------------------------------
BOOL KeyboardMonitor::RejectKeyboadInputs( BOOL bReject  )
{
  BOOL bThen = m_bRejectKeyBoardInputs;
  m_bRejectKeyBoardInputs = bReject;
  return bThen;
}

// -----------------------------------------------------------------------
/**
 * Returns if we must reject all keyboard inputs/
 *
 * @return bool return true if we are rejecting all keyboard inputs.
 */
BOOL KeyboardMonitor::RejectKeyboadInputs( ) const
{
  return m_bRejectKeyBoardInputs;
}

// -----------------------------------------------------------------------
void KeyboardMonitor::SetLastKey( const LAST_KEY& lk )
{
	m_lk	=	lk;
}

/**
 * if the last DWORD used was more than 100 milli secs ago we can assume that it is a new key
 * so we just return an empty key.
 *
 * @param DWORD the current time so we can compare if the last key was a long time ago, 
 * @return LAST_KEY the last key that was saved.
 */
KeyboardMonitor::LAST_KEY KeyboardMonitor::GetLastKey( DWORD dwCurrentMilli)
{
	if( (dwCurrentMilli - m_lk.tick() ) > 100 /* 100 milli seconds*/)
	{
		//	it was more than 100ms so even if it is a repeat we do not care and we 
		//	should return it
    //TRACE( "<< Repeat Key %lu>>\n" );
		return LAST_KEY( 0, 0x0000);	//	took too long, do not return
	}

	//TRACE( "<< Non Repeat Key %lu>>\n", dwCurrentMilli - m_lk.tick() );

  //	that is the key that occured less than 100ms ago
	return m_lk;
}

// -----------------------------------------------------------------------
BOOL KeyboardMonitor::RemoveHwnd( HWND hWnd )
{
  std::map< HWND, WPARAM >::const_iterator iter = m_mapWindows.find( hWnd );
  if( iter == m_mapWindows.end() )
  {
    //  don't know that item
    return FALSE;
  }

  // remove it
  m_mapWindows.erase( iter );

  // do we need the hook anymore?
  if( m_mapWindows.size() == 0 )
  {
    return ClearHooks();
  }

  // if we are here then we still have some hooks needed.
  return TRUE;
}

// -----------------------------------------------------------------------
BOOL KeyboardMonitor::CreateHooks()
{
  // have we already created the hooks?
  if( NULL != GetHHOOK() )
  {
    return TRUE;
  }

  //  hook into the low level keyboard message
  HHOOK hookllKey = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)CallLowLevelKeyboardProc, (HINSTANCE)m_hModule,0);  

  //  make sure that they are all created properly.
  if( hookllKey == NULL )
  {
    return FALSE;
  }

  // save the value
  SetHHOOK( hookllKey );

  return TRUE;
}

// -----------------------------------------------------------------------
BOOL KeyboardMonitor::ClearHooks( )
{
  if( NULL == GetHHOOK() )
  {
    //  we were not hooked in the first place.
    return FALSE;
  }


  BOOL bResult = UnhookWindowsHookEx( GetHHOOK() );

  //  what ever happened, (True or False), the user no longer wants that hook.
  SetHHOOK( NULL );

  return bResult;
}

// -----------------------------------------------------------------------
BOOL KeyboardMonitor::AddHwnd( HWND hWnd, WPARAM vKey )
{
  if( !CreateHooks() )
  {
    return FALSE;
  }

  //  do we aready have that window?
  std::map< HWND, WPARAM >::const_iterator iter = m_mapWindows.find( hWnd );
  if( iter != m_mapWindows.end() )
  {
    return FALSE; //  already exists.
  }
  
  // this is new so we need to add it here.
  m_mapWindows[ hWnd ] = vKey;

  return TRUE;  //  this was added.
}