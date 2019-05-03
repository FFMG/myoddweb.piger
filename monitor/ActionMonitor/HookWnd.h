//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
#pragma once
#include "CommonWnd.h"
#include "IDisplay.h"
#include "IActions.h"
#include "IApplication.h"
#include "../api/IVirtualMachines.h"
#include "threads/workers.h"

#define ACTION_NONE           0x000
#define ACTION_MAINKEY_DOWN   0x001
#define ACTION_SHIFT_DOWN     0x002
#define ACTION_LSHIFT_DOWN    0x004 /* if not set then RSHIFT is down */

#define SPECIAL_KEY VK_CAPITAL

class HookWnd final : public CommonWnd, protected myodd::threads::Workers
{
public:
  explicit HookWnd(IApplication& application, IDisplay& display, IActions& actions, IVirtualMachines& virtualMachines);
  virtual ~HookWnd();

  bool Close() override;
  bool Create() override;

protected:
  static LRESULT OnHookKeyChar(WPARAM wParam, LPARAM lParam);
  LRESULT OnHookKeyDown(WPARAM wParam, LPARAM lParam);
  LRESULT OnHookKeyUp(WPARAM wParam, LPARAM lParam);
  LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;

  /**
   * \brief the virtual machines.
   */
  IVirtualMachines& _virtualMachines;

  /**
   * \brief the actions currently loaded
   */
  IActions& _actions;

  /**
   * \brief the class that will receive show/hide requests.
   */
  IDisplay& _display;

  /**
   * \brief the application manager.
   */
  IApplication& _application;

  //  the state of our special key
  unsigned long _keyState;

  static bool IsSpecialKeyDown();
  static bool IsSpecialKey( WPARAM wParam);
};