// hook.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"

#define HOOK_CPP

#include "hook.h"
#include <stdio.h>
#include <crtdbg.h>

#include "KeyboardMonitor.h"

// -----------------------------------------------------------------------
BOOL APIENTRY DllMain
(
  HANDLE hModule, 
  DWORD  ul_reason_for_call, 
  LPVOID lpReserved
)
{
  switch(ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    {
      // DisableThreadLibraryCalls( hModule );
      keyboardMonitor = new KeyboardMonitor( hModule );
		  return TRUE;
    }
    break;

  case DLL_PROCESS_DETACH:
    {
      delete keyboardMonitor;
      return TRUE;
    }
    break;
  }
  return TRUE;
}

// -----------------------------------------------------------------------
//  stop us from monitoring messages
HOOKLIB int hook_clear( HWND hWnd )
{
  // if we have no keyboard class then we have already offloaded the dll
  if( NULL == keyboardMonitor )
  {
    return FALSE;
  }

  return keyboardMonitor->RemoveHwnd( hWnd );
}// clearMyHook

// -----------------------------------------------------------------------
//  hook into a message
HOOKLIB int hook_set( HWND hWnd, WPARAM vK )
{
  // if we have no keyboard class then something is wrong.
  if( NULL == keyboardMonitor )
  {
    return FALSE;
  }

  //  add the window
  return keyboardMonitor->AddHwnd( hWnd, vK  );
} // setHook

// -----------------------------------------------------------------------
/**
 * Set the state of the keyboard reject
 *
 * @param bool set the reject flag, true to reject all the keyboards inputs.
 * @return none.
 */
HOOKLIB int hook_RejectKeyboad( int bReject )
{
  // if we have no keyboard class then we cannot reject the keyboard.
  if( NULL == keyboardMonitor )
  {
    return FALSE;
  }

  return keyboardMonitor->RejectKeyboadInputs( bReject  );
}