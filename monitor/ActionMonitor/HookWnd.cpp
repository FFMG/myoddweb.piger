#include "stdafx.h"
#include "HookWnd.h"
#include <ActionsCore.h>
#include "ActionMonitor.h"

HookWnd::HookWnd( IDisplay& display, IActions& actions, IVirtualMachines& virtualMachines) :
  CommonWnd(L"ActionMonitorHookWndClass"),
  _virtualMachines(virtualMachines),
  _actions(actions),
  _display(display),
  _keyState(ACTION_NONE),
  _activeActionsRunner( nullptr )
{
  _activeActionsRunner = new ActiveActionsRunner();
}

HookWnd::~HookWnd()
{
  delete _activeActionsRunner;
}


/**
 * \brief return if the current special key is down.
 */
bool HookWnd::IsSpecialKeyDown( )
{
  const auto vk = GetKeyState(SPECIAL_KEY);
  return ((vk & 0xffff) == 1 );
}

/**
 * \brief Return true if a param is our special key
 * \param wParam check if the given param is the special key or not.
 * \return if wParam is the special key.
 */
bool HookWnd::IsSpecialKey(const WPARAM wParam)
{
  return (wParam == SPECIAL_KEY);
}


LRESULT HookWnd::OnMessage(const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
  if( msg == WM_CLOSE )
  {
    WaitForAllWorkers([&]()
    {
      myodd::wnd::MessagePump(_hwnd);
      return true;
    });
  }

  if (msg == UWM_KEYBOARD_DOWN)
  {
    return OnHookKeyDown(wParam, lParam);
  }

  if (msg == UWM_KEYBOARD_CHAR)
  {
    return OnHookKeyChar(wParam, lParam);
  }

  if (msg == UWM_KEYBOARD_UP)
  {
    return OnHookKeyUp(wParam, lParam);
  }
  return CommonWnd::OnMessage(msg, wParam, lParam);
}

bool HookWnd::Close()
{
  //  remove the hooks
  hook_clear(_hwnd);

  // and close it.
  return CommonWnd::Close();
}

bool HookWnd::Create()
{
  // first create the window
  // if it does not work, just get out.
  if( !CommonWnd::Create() )
  {
    return false;
  }

  //  if the key is down then we need to send a message to
  //  tell the system to do the work
  if (IsSpecialKeyDown())
  {
    INPUT input[2];
    ZeroMemory(input, sizeof(input));
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = SPECIAL_KEY;

    input[0].ki.time = input[1].ki.time = GetTickCount();
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, input, sizeof(INPUT));

    //  surrender our thread time to give time for the toggle to work.
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  //  start hook the DLL
  hook_set(_hwnd, SPECIAL_KEY);

  return true;
}

LRESULT HookWnd::OnHookKeyChar(WPARAM wParam, LPARAM lParam)
{
  /**
   * The WM_CHAR message is posted to the window with the keyboard focus when a WM_KEYDOWN message is translated by the TranslateMessage function.
   * The WM_CHAR message contains the character code of the key that was pressed.
   */
  return 0L;
}

LRESULT HookWnd::OnHookKeyDown(WPARAM wParam, LPARAM lParam)
{
  TRACE("KeyDown 0x%x\n", wParam);

  /**
   * The WM_KEYDOWN message is posted to the window with the keyboard focus when a nonsystem key is pressed.
   * A nonsystem key is a key that is pressed when the ALT key is not pressed.
   */

   //  if it is the special key then tell the system that from now own
   //  we need to record keys and prevent any other key from passing accross.
  if (IsSpecialKey(wParam))
  {
    //  only do it if the key was NOT down previously.
    //  otherwise we will forever be reseting
    if ((_keyState & ACTION_MAINKEY_DOWN) != ACTION_MAINKEY_DOWN)
    {
      // we need to save the last forground window
      // because showing our dialog box will change things a bit.
      CActionMonitorApp::SetLastForegroundWindow( CWnd::GetForegroundWindow());

      //  tell the system to no longer accept any more key
      _keyState |= ACTION_MAINKEY_DOWN;
      hook_RejectKeyboad(TRUE);
      _display.Active();

      //  reset the last command
      _actions.CurrentActionReset();
      _display.Show( _actions.ToChar() );
    }
  }

  //  if the special key is not down then we don't need to go any further
  if ((_keyState & ACTION_MAINKEY_DOWN) != ACTION_MAINKEY_DOWN)
  {
    return 0L;
  }

  switch (wParam)
  {
  case VK_BACK:     // backspace 
    _actions.CurrentActionBack();
    _display.Show( _actions.ToChar() );
    break;

  case 0x0A:        // linefeed 
  case VK_TAB:      // tab 
  case VK_RETURN:   // carriage return 
  case VK_CLEAR:
    break;

  case VK_ESCAPE:
    _actions.CurrentActionReset();
    _display.Show( _actions.ToChar() );
    break;

  case VK_DOWN:
    _actions.down();
    _display.Show( _actions.ToChar() );
    break;

  case VK_CAPITAL:
    break;

  case VK_UP:
    _actions.up();
    _display.Show( _actions.ToChar() );
    break;

  case VK_SHIFT:
  case VK_RSHIFT:
    _keyState |= ACTION_SHIFT_DOWN;
    break;

  case VK_LSHIFT:
    _keyState |= (ACTION_SHIFT_DOWN | ACTION_LSHIFT_DOWN);
    break;

  default:
  {
    BYTE ks[256];
    memset(ks, 0, sizeof(ks));
    GetKeyboardState(ks);
    WORD w;
    UINT scan = 0;

    ks[VK_SHIFT] = 1;
    if ((_keyState & ACTION_SHIFT_DOWN) == ACTION_SHIFT_DOWN)
    {
      //  force the shift key down.
      //  the low level keyboad does not alway pass the key(shift) before the letter key
      //  so we might not know that the shift key is pressed.
      //  so we set it here.
      ks[VK_SHIFT] = 129;
    }

    if (0 != ToAscii(wParam, scan, ks, &w, 0))
    {
      const auto c = static_cast<TCHAR>(CHAR(w));
      //  add the current character to the list of items
      _actions.CurrentActionAdd(c);

      //  make sure that the window is visible
      //  This will also force a refresh of the window
      _display.Show( _actions.ToChar() );
    }
  }
  break;
  }
  return 0L;
}

LRESULT HookWnd::OnHookKeyUp(WPARAM wParam, LPARAM lParam)
{
  TRACE("KeyUp 0x%x\n", wParam);

  //  check the key that the user had released
  //  if it the special key then tell the user that the key is no longer down
  if (IsSpecialKey(wParam))
  {
    //  the key is no longer down
    //  but was it down at all to start with? - (sanity check)
    if ((_keyState & ACTION_MAINKEY_DOWN) == ACTION_MAINKEY_DOWN)
    {
      //  remove the fact that the key was pressed.
      //  do that next time we come around here we don't do it again
      _keyState &= ~ACTION_MAINKEY_DOWN;

      //  tell the system that we can now accept key press
      hook_RejectKeyboad(FALSE);
      _display.Inactive();

      try
      {
        //  We use the Actions to find the actual command
        //  because either it is an exact match, (and it doesn't matter)
        //  or it is not an exact match and this is what we meant to use
        // 
        //  so if the user enters 'goo french victories'
        //  we will use the first command 'google' with the arguments 'french victories'
        //
        //  we use getCommand in case the user has chosen number 1, 2 ... in the list of possible commands 
        const auto action = _actions.GetCommand();
        if (nullptr != action)
        {
          const auto szCommandLine = _actions.GetCommandLine();

          //  do the action now
          //  we might not have any, but that's not for us to decides :).
          const auto pWnd = CActionMonitorApp::GetLastForegroundWindow();
          _activeActionsRunner->QueueAndExecute(action->CreateActiveAction(_virtualMachines, pWnd, szCommandLine, false));
        }
      }
      catch (...)
      {
        //  we don't have a valid command.
      }

      // we are hidding the current action
      _actions.CurrentActionReset();

      //  hide the window
      _display.Hide();
    }
  }

  switch (wParam)
  {
  case VK_SHIFT:
  case VK_RSHIFT:
  case VK_LSHIFT:
    _keyState &= ~ACTION_SHIFT_DOWN;
    _keyState &= ~ACTION_LSHIFT_DOWN;
    break;

  default:
    break;
  }

  /**
   * The WM_KEYUP message is posted to the window with the keyboard focus when a nonsystem key is released.
   * A nonsystem key is a key that is pressed when the ALT key is not pressed,
   * or a keyboard key that is pressed when a window has the keyboard focus.
   */
  return 0L;
}
