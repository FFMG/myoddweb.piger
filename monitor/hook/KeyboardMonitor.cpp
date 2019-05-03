#include "stdafx.h"

#include "KeyboardMonitor.h"
#include "hook.h"

KeyboardMonitor* keyboardMonitor;

KeyboardMonitor::KeyboardMonitor(const HANDLE hModule ) :
  m_hModule( hModule ),
  m_bRejectKeyBoardInputs(false),
  m_hhook(nullptr)
{
  UWM_KEYBOARD_CHAR = RegisterWindowMessage(UWM_KEYBOARD_MSG_CHAR);
  UWM_KEYBOARD_UP   = RegisterWindowMessage(UWM_KEYBOARD_MSG_UP);
  UWM_KEYBOARD_DOWN = RegisterWindowMessage(UWM_KEYBOARD_MSG_DOWN);
}

KeyboardMonitor::~KeyboardMonitor()
{
  ClearHooks();
}

// -----------------------------------------------------------------------
bool KeyboardMonitor::HandleHook ( const int nCode )
{
  if(nCode < 0 || HC_ACTION != nCode )
  {
    return false;
  }
  return true;
}

// -----------------------------------------------------------------------
LRESULT CALLBACK KeyboardMonitor::CallLowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  //  are we handling that hook?
  if(false == keyboardMonitor->HandleHook( nCode ))
  {
    return CallNextHookEx( keyboardMonitor->GetHhook(), nCode, wParam, lParam);
  }
  
  // By returning a non-zero value from the hook procedure, the
  // message does not get passed to the target window
  const auto *pkbhs = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
  switch (nCode)
  {
  case HC_ACTION:
    {
      const auto hHandle = keyboardMonitor->HandleMessage( wParam, pkbhs->vkCode, pkbhs->dwExtraInfo );
      switch ( hHandle )
      {
      case 1: //  the key was handled by someone
        {
          //  if we have been instructed to reject messages simply change the message to nothing
          //  other processes are supposed to ignore WM_NULL
          //  this is not full proof, (we might not be the first hook in the queue), but the odds of 
          //  multiple hooks all trying to do the same thing is limited.
          if( keyboardMonitor->RejectKeyboadInputs() == true)
          {
            return 1;
          }
        }
        break;

      case 0:
        break;

      default:
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
  return CallNextHookEx ( keyboardMonitor->GetHhook(), nCode, wParam, lParam);
}// CallLowLevelKeyboardProc

/**
 * \brief check if the given param means that the one of the special key was pressed.
 * \param wParam the key we are checking against.
 * \return if the given key is a special key or not.
 */
bool KeyboardMonitor::IsSpecialKey( const WPARAM wParam ) const
{
  for(auto it = m_mapWindows.begin(); it != m_mapWindows.end(); ++it )
  {
    if( wParam == it->second )
    {
      //  yes, this window uses that key.
      return true;  
    }
  }

  // if we are here we never found it.
  return false;
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
	const auto tick	=	GetTickCount();
	//
	//	do not allow items to be too quick
  if( GetLastKey( tick ) .Similar( msg, wParam)  )
	{	
    //	the key we just entered has been repeated too soon for us and is rejected.
		// TRACE( "<< Repeat Key >>\n" );
		return 0L;
	}
  // save the last key so we can keep track of the timing
  // it is not uncommon for apps to request the last key more than once.
  SetLastKey( LAST_KEY(msg, wParam, tick) );

  auto lResult = 0L; //  assume that nobody will handle it.
  for( auto it = m_mapWindows.begin(); it != m_mapWindows.end(); ++it )
  {
    const auto hWnd = it->first;
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
bool KeyboardMonitor::RejectKeyboadInputs(bool bReject  )
{
  const auto bThen = m_bRejectKeyBoardInputs;
  m_bRejectKeyBoardInputs = bReject;
  return bThen;
}

// -----------------------------------------------------------------------
/**
 * Returns if we must reject all keyboard inputs/
 *
 * @return bool return true if we are rejecting all keyboard inputs.
 */
bool KeyboardMonitor::RejectKeyboadInputs( ) const
{
  return m_bRejectKeyBoardInputs;
}

// -----------------------------------------------------------------------
void KeyboardMonitor::SetLastKey( const LAST_KEY& lk )
{
	m_lk	=	lk;
}

/**
 * \brief if the last DWORD used was more than 100 milliseconds ago we can assume that it is a new key
 * so we just return an empty key.
 *
 * \param dwCurrentMilli the current time so we can compare if the last key was a long time ago, 
 * \return LAST_KEY the last key that was saved.
 */
KeyboardMonitor::LAST_KEY KeyboardMonitor::GetLastKey( const DWORD dwCurrentMilli) const
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
bool KeyboardMonitor::RemoveHwnd( const HWND hWnd )
{
  const auto iter = m_mapWindows.find( hWnd );
  if( iter == m_mapWindows.end() )
  {
    //  don't know that item
    return false;
  }

  // remove it
  m_mapWindows.erase( iter );

  // do we need the hook anymore?
  if( m_mapWindows.size() == 0 )
  {
    return ClearHooks();
  }

  // if we are here then we still have some hooks needed.
  return true;
}

// -----------------------------------------------------------------------
bool KeyboardMonitor::CreateHooks()
{
  // have we already created the hooks?
  if(nullptr != GetHhook() )
  {
    return true;
  }

  //  hook into the low level keyboard message
  const auto hookllKey = SetWindowsHookEx(WH_KEYBOARD_LL, static_cast<HOOKPROC>(CallLowLevelKeyboardProc), static_cast<HINSTANCE>(m_hModule),0);  

  //  make sure that they are all created properly.
  if( hookllKey == nullptr)
  {
    return false;
  }

  // save the value
  SetHhook( hookllKey );

  return true;
}

// -----------------------------------------------------------------------
bool KeyboardMonitor::ClearHooks( )
{
  if( nullptr == GetHhook() )
  {
    //  we were not hooked in the first place.
    return false;
  }


  const auto bResult = UnhookWindowsHookEx( GetHhook() );

  //  what ever happened, (true or False), the user no longer wants that hook.
  SetHhook(nullptr);

  return bResult;
}

// -----------------------------------------------------------------------
bool KeyboardMonitor::AddHwnd( const HWND hWnd, const WPARAM vKey )
{
  if( !CreateHooks() )
  {
    return false;
  }

  //  do we aready have that window?
  const auto iter = m_mapWindows.find( hWnd );
  if( iter != m_mapWindows.end() )
  {
    return false; //  already exists.
  }
  
  // this is new so we need to add it here.
  m_mapWindows[ hWnd ] = vKey;

  return true;  //  this was added.
}