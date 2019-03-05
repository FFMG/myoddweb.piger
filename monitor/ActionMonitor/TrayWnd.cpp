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
#include "stdafx.h"
#include "TrayWnd.h"

#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER + 1)
#define IDI_TRAYICON 1

TrayWnd::TrayWnd(HICON active, HICON inactive) :
  CommonWnd( L"TrayWnd"),
  _active( active ),
  _inactive( inactive ),
  _hMenu(nullptr),
  _menuOwner(nullptr)
{
}

TrayWnd::~TrayWnd()
{
}

void TrayWnd::SetMenu(const HMENU hmenu, const HWND menuOwner)
{
  _hMenu = hmenu;
  _menuOwner = menuOwner;
  Touch();
}

bool TrayWnd::OnInitDialog()
{
  _notificationIconData.cbSize = sizeof(NOTIFYICONDATA);
  _notificationIconData.hWnd = GetSafeHwnd();
  _notificationIconData.uID = IDI_TRAYICON;
  _notificationIconData.uFlags = NIF_ICON | NIF_MESSAGE;
  _notificationIconData.hIcon = _inactive;
  _notificationIconData.uCallbackMessage = WM_TRAY_ICON_NOTIFY_MESSAGE;

  if (!Shell_NotifyIcon(NIM_ADD, &_notificationIconData))
  {
    return false;
  }
  return true;
}

LRESULT TrayWnd::OnMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_DESTROY:
    Shell_NotifyIcon(NIM_DELETE, &_notificationIconData);	// Remove Tray Item
    PostQuitMessage(0);							// Quit
    return 0;

  case WM_CLOSE:
    DestroyWindow( GetSafeHwnd() );	// Destroy Window
    return 0;

  case WM_TRAY_ICON_NOTIFY_MESSAGE:
    POINT pt;
    switch (LOWORD(lParam))
    {
    case WM_RBUTTONDOWN:
      GetCursorPos(&pt);
      OnRButtonDown(pt);
      return 0;

    case WM_LBUTTONDOWN:
      GetCursorPos(&pt);
      OnLButtonDown(pt);
      return 0;
    }
    return 0;
  }
  return CommonWnd::OnMessage(msg, wParam, lParam);
}

void TrayWnd::ShowMenu(POINT pt)
{
  if (_hMenu)
  {
    HMENU menu = GetSubMenu(_hMenu, 0);
    SetMenuDefaultItem(menu, 0, TRUE);

    SetForegroundWindow(GetSafeHwnd());
    TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, _menuOwner, nullptr );
    PostMessage(GetSafeHwnd(), WM_NULL, 0, 0);
  }
}

void TrayWnd::OnRButtonDown(POINT pt)
{
  ShowMenu(pt);
}

void TrayWnd::OnLButtonDown(POINT pt)
{
}

/**
 * \brief set the text tooltip
 * \param toolTip the tooltip text.
 */
void TrayWnd::SetToolTip(const std::wstring& toolTip)
{
  assert(toolTip.length() > 0 && toolTip.length() < 64);
  _tcscpy_s(_notificationIconData.szTip, toolTip.c_str() );
  _notificationIconData.uFlags |= NIF_TIP;
  Touch();
}

void TrayWnd::Touch()
{
  Shell_NotifyIcon(NIM_MODIFY, &_notificationIconData);
}

void TrayWnd::SetActive()
{
  _notificationIconData.hIcon = _active;
  _notificationIconData.uFlags |= NIF_ICON;
  Touch();
}

void TrayWnd::SetInactive()
{
  _notificationIconData.hIcon = _inactive;
  _notificationIconData.uFlags |= NIF_ICON;
  Touch();
}
