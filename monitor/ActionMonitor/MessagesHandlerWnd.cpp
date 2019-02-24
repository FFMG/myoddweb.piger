#include "stdafx.h"
#include "MessagesHandlerWnd.h"
#include "ActionsCore.h"

MessagesHandlerWnd::MessagesHandlerWnd() :
  _szClassName(L"ActionMonitorMessagesWindowClass"), 
  _hwnd(nullptr)
{
  memset(&_wc, 0, sizeof(WNDCLASSEX));
}

MessagesHandlerWnd::~MessagesHandlerWnd()
{
}

LRESULT CALLBACK MessagesHandlerWnd::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if (UWM_DISPLAYMESSAGE == msg)
  {
    const auto messageHanderData = reinterpret_cast<Msg*>(lParam);
    const auto result = messageHanderData->Parent().Show(messageHanderData->Text(), messageHanderData->Elapse(), messageHanderData->FadeOut());
    delete messageHanderData;

    return result ? 1L : 0L;
  }

  switch (msg)
  {
  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_CREATE:
    {
      // auto cs = reinterpret_cast<CREATESTRUCT *>(lParam);
    }
    break;

  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

bool MessagesHandlerWnd::Show(IMessagesHandler& parent, const std::wstring& sText, long elapseMiliSecondsBeforeFadeOut, long totalMilisecondsToShowMessage) const
{
  return 1L == SendMessage(_hwnd, UWM_DISPLAYMESSAGE, 0, reinterpret_cast<LPARAM>(new Msg(parent, sText, elapseMiliSecondsBeforeFadeOut, totalMilisecondsToShowMessage)));
}

bool MessagesHandlerWnd::CreateClass()
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
  _wc.lpfnWndProc = MessagesHandlerWnd::WndProc;
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

bool MessagesHandlerWnd::Create()
{
  if( !CreateClass())
  {
    return false;
  }

  // Step 2: Creating the Window
  _hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    _szClassName.c_str(),
    L"Action Monitor Messages Window",
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
  return true;
}

bool MessagesHandlerWnd::Close()
{
  if (_hwnd == nullptr)
  {
    return false;
  }
  SendMessage(_hwnd, WM_CLOSE, 0, 0);
  _hwnd = nullptr;
  return true;
}
