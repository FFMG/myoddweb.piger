#include "stdafx.h"
#include "ipclistener.h"
#include <afxwin.h>
#include "../threads/lock.h"
#include <assert.h>
#include "ipclistenerwnd.h"

namespace myodd {
namespace os {
static HWND _pParentWnd = nullptr;

IpcListener::IpcListener(const wchar_t* serverName, void* parent ) :
  _pServer( nullptr )
{
  // create the server
  Create(serverName, parent );
}

IpcListener::~IpcListener()
{
  // remove the server.
  if (nullptr != _pServer)
  {
    // aquire the lock
    threads::Lock guard(_mutex);

    // now that we have the lock, do we need to do what we need to do?
    if (nullptr != _pServer)
    {
      // delete it
      delete static_cast<IpcListenerWnd*>(_pServer);

      // clear it
      _pServer = nullptr;
    }

    // release the lock(s)
  }
}

/**
 * Create the server window that will be listening to messages.
 * @param const wchar_t* serverName the server name we wish to use.
 * @param void* pParent the parent that we want to pass the messages to.
 * @return none
 */
void IpcListener::Create(const wchar_t* serverName, void* pParent)
{
  // make sure we are only creating this once.
  if (nullptr == _pServer)
  {
    threads::Lock guard(_mutex);
    if (nullptr == _pServer)
    {
      _pServer = new IpcListenerWnd(serverName, static_cast<HWND>(pParent), _mutex);
    }
  }
}

}
}
