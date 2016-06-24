#include "stdafx.h"
#include "ipc.h"
#include <afxwin.h>

namespace myodd {
namespace os {
class IpcWnd : public CWnd
{
public:
  IpcWnd(const wchar_t* pszClassName )
  {
    auto hInstance = GetModuleHandle( nullptr );
    WNDCLASS wndcls;
    if (!GetClassInfo(hInstance, pszClassName, &wndcls))
    {
      wndcls.style = 0;
      wndcls.lpfnWndProc = IpcWnd::WindowProc;
      wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
      wndcls.hInstance = hInstance;
      wndcls.hIcon = nullptr;
      wndcls.hCursor = LoadCursor(nullptr, IDC_ARROW);
      wndcls.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
      wndcls.lpszMenuName = nullptr;
      wndcls.lpszClassName = pszClassName;
      if (!RegisterClass(&wndcls))
      {
        TRACE(traceAppMsg, 0, _T("Can't register window class named %Ts\n"), wndcls.lpszClassName);
      }
    }

    // one way or another the class was created.
    // so we can create our listener accordingly.
    if (!this->CWnd::CreateEx(0, pszClassName, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr))
    {
      TRACE(traceAppMsg, 0, _T("Can't create window class named %Ts\n"), wndcls.lpszClassName);
    }
  }

  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
};


Ipc::Ipc(const wchar_t* serverName) : _pServer( nullptr )
{
  // create the server
  CreateServer(serverName);
}

Ipc::~Ipc()
{
  // remove the server.
  delete static_cast<IpcWnd*>(_pServer);
}

/**
 * Create the server window that will be listening to messages.
 */
void Ipc::CreateServer(const wchar_t* serverName)
{
  _pServer = new IpcWnd( serverName );
}

}
}
