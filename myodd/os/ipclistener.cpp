#include "stdafx.h"
#include "ipclistener.h"
#include <afxwin.h>

namespace myodd {
namespace os {
static HWND _pParentWnd = nullptr;

class IpcListenerWnd : public CWnd
{
public:
  explicit IpcListenerWnd(const wchar_t* pszClassName, HWND pParent)
  {
    //  save the parent
    _pParentWnd = pParent;

    //  the instance of this module
    auto hInstance = GetModuleHandle( nullptr );
    WNDCLASS wndcls;
    if (GetClassInfo(hInstance, pszClassName, &wndcls))
    {
      throw "The server already exists.";
    }

    wndcls.style = 0;
    wndcls.lpfnWndProc = WindowProc;
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
    // pass this to the parent window
    auto lpResult = ::SendMessageW(_pParentWnd, uMsg, wParam, lParam);

    // pass this to our window...
    ::DefWindowProc(hwnd, uMsg, wParam, lParam);

    //  return the parent result
    return lpResult;
  }
};

IpcListener::IpcListener(const wchar_t* serverName, void* parent ) :
  _pServer( nullptr )
{
  // create the server
  Create(serverName, parent );
}

IpcListener::~IpcListener()
{
  // remove the server.
  delete static_cast<IpcListenerWnd*>(_pServer);
}

/**
 * Create the server window that will be listening to messages.
 * @param const wchar_t* serverName the server name we wish to use.
 * @param void* pParent the parent that we want to pass the messages to.
 * @return none
 */
void IpcListener::Create(const wchar_t* serverName, void* pParent)
{
  _pServer = new IpcListenerWnd( serverName, (HWND)pParent );
}

}
}
