#include "stdafx.h"
#include "ipclistener.h"
#include <afxwin.h>

namespace myodd {
namespace os {
class IpcListenerWnd : public CWnd
{
public:
  explicit IpcListenerWnd(const wchar_t* pszClassName )
  {
    auto hInstance = GetModuleHandle( nullptr );
    WNDCLASS wndcls;
    if (GetClassInfo(hInstance, pszClassName, &wndcls))
    {
      throw "The server already exists.";
    }

    wndcls.style = 0;
    wndcls.lpfnWndProc = IpcListenerWnd::WindowProc;
    wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
    wndcls.hInstance = hInstance;
    wndcls.hIcon = nullptr;
    wndcls.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndcls.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    wndcls.lpszMenuName = nullptr;
    wndcls.lpszClassName = pszClassName;
    if (!RegisterClass(&wndcls))
    {
      throw "Can't register IpcListener window class.";
    }

    // one way or another the class was created.
    // so we can create our listener accordingly.
    if (!this->CWnd::CreateEx(0, pszClassName, L"", 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr))
    {
      throw "Can't create IpcListener window.";
    }
  }

  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
};


IpcListener::IpcListener(const wchar_t* serverName) : _pServer( nullptr )
{
  // create the server
  Create(serverName);
}

IpcListener::~IpcListener()
{
  // remove the server.
  delete static_cast<IpcListenerWnd*>(_pServer);
}

/**
 * Create the server window that will be listening to messages.
 * @param const wchar_t* serverName the server name we wish to use.
 * @return none
 */
void IpcListener::CreateServer(const wchar_t* serverName)
{
  _pServer = new IpcListenerWnd( serverName );
}

}
}
