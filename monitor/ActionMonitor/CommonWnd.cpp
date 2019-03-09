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
#include "CommonWnd.h"

CommonWnd::CommonWnd(const std::wstring& className) :
  _szClassName(className ),
  _hwnd( nullptr ),
  _hinstance( nullptr )
{
  memset(&_wc, 0, sizeof(WNDCLASSEX));
  _wc.cbSize = sizeof(WNDCLASSEX);
}

CommonWnd::~CommonWnd()
{
  // make sure we are closed
  CommonWnd::Close();
}

bool CommonWnd::OnInitDialog()
{
  // we do nothing.
  return true;
}

void CommonWnd::OnPaint()
{
  //  do nothing
}

LRESULT CommonWnd::OnMessage( const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
  // simply return the default behaviour.
  return DefWindowProc(_hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK CommonWnd::WndProc( const HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
  LRESULT result = 0L;

  //  look for the parent.
  auto obj = reinterpret_cast<CommonWnd*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  // if this is a create message, then we will save the pointer.
  // the pointer was passed to us while created.
  if (msg == WM_CREATE)
  {
    const auto cs = reinterpret_cast<CREATESTRUCT *>(lParam);
    if (cs != nullptr)
    {
      obj = static_cast<CommonWnd*> (cs->lpCreateParams);
      if (obj != nullptr)
      {
        obj->_hwnd = hwnd;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(obj));

        // call the OnMessage as it might call
        // DefWindowProc(... ) eventually
        result = obj->OnMessage(msg, wParam, lParam);
        if (result == -1L)
        {
          obj->_hwnd = nullptr;
          return result;
        }

        // then let the user do something
        obj->OnInitDialog();

        // and return what WM_CREATE was alway going to do.
        return result;
      }
    }
  }

  if (obj != nullptr)
  {
    result = obj->OnMessage(msg, wParam, lParam);
    switch (msg)
    {
    case WM_PAINT:
      obj->OnPaint();
      break;

    case WM_CLOSE:
    case WM_DESTROY:
      SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
      break;

    default:
      return result;
    }
    return result;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool CommonWnd::CreateClass()
{
  memset(&_wc, 0, sizeof(WNDCLASSEX));
  _hinstance = GetModuleHandle(nullptr);
  if (GetClassInfoEx(_hinstance, _szClassName.c_str(), &_wc))
  {
    return true;
  }
  //Step 1: Registering the Window Class
  _wc.cbSize = sizeof(WNDCLASSEX);
  _wc.style = CS_HREDRAW | CS_VREDRAW;
  _wc.lpfnWndProc = WndProc;
  _wc.cbClsExtra = 0;
  _wc.cbWndExtra = 0;
  _wc.hInstance = _hinstance;
  _wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
  _wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  _wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
  _wc.lpszMenuName = nullptr;
  _wc.lpszClassName = _szClassName.c_str();
  _wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

  if (!RegisterClassEx(&_wc))
  {
    return false;
  }
  return true;
}

/**
 * \brief create the window and wait 'close'
 * \return if there was an error or not.
 */
bool CommonWnd::CreateAndWait()
{
  // first we try and create
  if( !Create() )
  {
    return false;
  }

  // and now we wait...
  const auto wait = std::chrono::milliseconds(10);
  for (;;)
  {
    // pump messages
    myodd::wnd::MessagePump(nullptr);

    // if _hwnd does not exist anymore
    // then we can get out.
    if(_hwnd == nullptr )
    {
      break;
    }
    std::this_thread::yield();
    std::this_thread::sleep_for(wait);
  }

  // all good.
  return true;
}

bool CommonWnd::Create()
{
  if (!CreateClass())
  {
    return false;
  }

  // Step 2: Creating the Window
  _hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    _szClassName.c_str(),
    _szClassName.c_str(),
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 
    CW_USEDEFAULT, 
    CW_USEDEFAULT, 
    CW_USEDEFAULT,
    nullptr,
    nullptr,
    _wc.hInstance,
    this);

  if (_hwnd == nullptr)
  {
    return false;
  }
  return true;
}

bool CommonWnd::Close()
{
  if (_hwnd == nullptr)
  {
    return false;
  }
  SendMessage(_hwnd, WM_CLOSE, 0, 0);
  DestroyWindow(_hwnd);
  _hwnd = nullptr;

  return true;
}

/**
 * \brief hide the taskbar.
 */
void CommonWnd::HideTaskBar() const
{
  // hide the window right way
  // fading will re-show the window.
  ShowWindow(GetSafeHwnd(), SW_HIDE);

  // set the window style
  SetWindowLong(GetSafeHwnd(), GWL_STYLE, DS_SETFONT | WS_POPUP);

  auto style = WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED;
  style &= ~WS_EX_APPWINDOW;
  SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, style); //  no taskbar!

  // show/hide for the style to take effect.
  ShowWindow(GetSafeHwnd(), SW_SHOW);
  ShowWindow(GetSafeHwnd(), SW_HIDE);
}
