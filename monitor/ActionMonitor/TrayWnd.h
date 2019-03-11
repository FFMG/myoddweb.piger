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
#include "ITray.h"

class TrayWnd final : public CommonWnd
{
public:
  explicit TrayWnd( HICON active, HICON inactive );
  ~TrayWnd();

  /**
   * \brief Set the menu handler.
   * \param hmenu the menu item
   * \param menuOwner that will be notified when a message is sent.
   */
  void SetMenu(HMENU hmenu, HWND menuOwner);
  void SetToolTip(const std::wstring& toolTip);
  void SetActive();
  void SetInactive();

protected:
  LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
  bool OnInitDialog() override;

  void OnRButtonDown(POINT pt);
  void OnLButtonDown(POINT pt);
  void ShowMenu(POINT pt);

  /**
   * \brief touch the tray so no values are now visible.
   */
  void Touch();

  NOTIFYICONDATA _notificationIconData;
  HICON _active;
  HICON _inactive;
  HMENU _hMenu;
  HWND _menuOwner;
};

