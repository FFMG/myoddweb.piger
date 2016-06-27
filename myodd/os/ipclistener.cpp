#include "stdafx.h"
#include "ipclistener.h"
#include <afxwin.h>

namespace myodd {
namespace os {
static HWND _pParentWnd = nullptr;

class IpcListenerWnd : public CWnd
{
protected:
  enum class IpcMessageType : unsigned int
  {
    None = 0,
    SendMessage = 1,
    PostMessage = 2,
    CopyMessage = 3,
  };

  struct IpcMessageStruct
  {
    //  https://www.displayfusion.com/Discussions/View/converting-c-data-types-to-c/?ID=38db6001-45e5-41a3-ab39-8004450204b3
    unsigned int uMsg;
    unsigned __int64 wParam;
    __int64 lParam;
  };

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

protected:
  static bool MessageStructFromCopyData(const COPYDATASTRUCT& cds, IpcMessageStruct& ipcMessageStructure)
  {
    //  clear the values
    memset(&ipcMessageStructure, 0, sizeof(ipcMessageStructure));

    // is the data we were given the right size?
    if (cds.cbData != sizeof(ipcMessageStructure))
    {
      //  nope, it is not, something is wrong.
      return false;
    }

    // copy the data over.
    memcpy_s(&ipcMessageStructure, sizeof(ipcMessageStructure), cds.lpData, cds.cbData);

    // success.
    return true;
  }

public:
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    //  is it a copy data message?
    // if not, then it is just a message for this window.
    if (uMsg != WM_COPYDATA)
    {
      // pass this to our window...
      return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    //  cast it to COPYDATASTRUCT
    auto pcds = (COPYDATASTRUCT*)lParam;

    // then simply reconstruct the message
    switch ( (IpcMessageType)pcds->dwData)
    {
    case IpcMessageType::SendMessage:
    {
      // pass this to the parent window
      IpcMessageStruct ims;
      if (MessageStructFromCopyData(*pcds, ims))
      {
#if defined(_WIN64)
        return ::SendMessage(_pParentWnd, ims.uMsg, ims.wParam, ims.lParam);
#else
        return ::SendMessage(_pParentWnd, ims.uMsg, static_cast<unsigned int>(ims.wParam), static_cast<long>(ims.lParam));
#endif // X64
      }
    }
    break;

    case IpcMessageType::PostMessage:
    {
      // pass this to the parent window
      IpcMessageStruct ims;
      if (MessageStructFromCopyData(*pcds, ims))
      {
        // pass this to the parent window
#if defined(_WIN64)
        return ::PostMessage(_pParentWnd, ims.uMsg, ims.wParam, ims.lParam);
#else
        return ::PostMessage(_pParentWnd, ims.uMsg, static_cast<unsigned int>(ims.wParam), static_cast<long>(ims.lParam));
#endif // X64
      }
    }
    break;

    case IpcMessageType::CopyMessage:
      {
        //  try and decrypt the message that was sent.

        //  the message must be, at the very least the size of the version number.
        // https://www.displayfusion.com/Discussions/View/converting-c-data-types-to-c/?ID=38db6001-45e5-41a3-ab39-8004450204b3
        if (pcds->cbData < sizeof(signed int))
        {
          return false;
        }

        //  get the version number
        signed int versionNumber = 0;
        auto pointer = 0;
        memcpy_s(&versionNumber, sizeof(versionNumber), pcds->lpData , sizeof(versionNumber));
        pointer += sizeof(signed int);
        auto version = 0;

        for (;pointer < pcds->cbData;)
        {
          pointer++;
        }
      }
      break;

    default:
      //  unknown message type
      break;
    }

    // if we reach this then we have an unknwon type.
    return 0;
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
