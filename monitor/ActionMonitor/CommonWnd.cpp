#include "stdafx.h"
#include "CommonWnd.h"

CommonWnd::CommonWnd(const std::wstring& className) :
  _szClassName(className ),
  _hwnd(nullptr)
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
  return true;
}

void CommonWnd::OnPaint()
{

}

LRESULT CommonWnd::OnMessage( const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
  switch (msg)
  {
  case WM_CLOSE:
    DestroyWindow(_hwnd );
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  default:
    break;
  }
  return DefWindowProc(_hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK CommonWnd::WndProc( const HWND hwnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
  auto obj = reinterpret_cast<CommonWnd*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  if (msg == WM_CREATE)
  {
    const auto cs = reinterpret_cast<CREATESTRUCT *>(lParam);
    if (cs != nullptr)
    {
      obj = static_cast<CommonWnd*> (cs->lpCreateParams);
      if (obj != nullptr)
      {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(obj));
      }
    }
  }

  LRESULT result = 0L;
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
    }
    return result;
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool CommonWnd::CreateClass()
{
  memset(&_wc, 0, sizeof(WNDCLASSEX));
  const auto hInstance = GetModuleHandle(nullptr); ;
  if (GetClassInfoEx(hInstance, _szClassName.c_str(), &_wc))
  {
    return true;
  }
  //Step 1: Registering the Window Class
  _wc.cbSize = sizeof(WNDCLASSEX);
  _wc.style = 0;
  _wc.lpfnWndProc = WndProc;
  _wc.cbClsExtra = 0;
  _wc.cbWndExtra = 0;
  _wc.hInstance = hInstance;
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
    CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
    nullptr,
    nullptr,
    _wc.hInstance,
    this);

  if (_hwnd == nullptr)
  {
    return false;
  }

  OnInitDialog();

  return true;
}

bool CommonWnd::Close()
{
  if (_hwnd == nullptr)
  {
    return false;
  }
  SendMessage(_hwnd, WM_CLOSE, 0, 0);
  _hwnd = nullptr;

  return true;
}