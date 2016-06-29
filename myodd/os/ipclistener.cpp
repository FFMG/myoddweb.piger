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
      _pServer = new IpcListenerWnd(serverName, static_cast<HWND>(pParent), _mutex, *this );
    }
  }
}

/**
 * Add a message handler to our list of handlers.
 * Each will be called when a new message arrives.
 * @param IpcMessageHandler& handler the message handler we are adding.
 */
void IpcListener::AddMessageHandler(IpcMessageHandler& handler)
{
  // lock us in
  threads::Lock guard(_mutex);

  // look for the handler so we know if we need to remove it.
  auto it = FindMessageHandler(handler);

  // 
  if (it == _messageHandlers.end())
  {
    // add the message to the list.
    _messageHandlers.push_back(&handler);
  }
}

/**
 * Remove a message handler from our list.
 * @param IpcMessageHandler& handler the message handler we want to remove.
 * @return none
 */
void IpcListener::RemoveMessageHandler(IpcMessageHandler& handler)
{
  // lock us in
  threads::Lock guard(_mutex);

  // look for the existing handler so we do not add it again.
  auto it = FindMessageHandler(handler);
  if(it  != _messageHandlers.end() )
  {
    // we can now remove it.
    _messageHandlers.erase(it);
  }
}

/**
 * Look for a message handler and return the iterator for it.
 * This function assumes that we are locked in!
 * @param IpcMessageHandler& handler the handler we are looking for.
 * @return std::vector<IpcMessageHandler*>::const_iterator the iterator for this handler we are looking for ... or end()
 */
std::vector<IpcMessageHandler*>::const_iterator IpcListener::FindMessageHandler(IpcMessageHandler& handler) const
{
  // This function assumes that we are locked in!
  // if we are not locked in, we could add/remove items while we are traversing this list.
  // and if we are not locked in, we could return something that might be removed.

  // look for an existing handler, (prevent double adding)
  for (auto it = _messageHandlers.begin(); it != _messageHandlers.end(); ++it)
  {
    if( *it == &handler )
    {
      // found it
      return it;
    }
  }

  // could not locate it.
  return _messageHandlers.end();
}

/**
 * Handle a message 'request' and return a response accrodingly.
 * @param const IpcData& ipcRequest the given request.
 * @param IpcData& ipcResponse the response container.
 * @return boolean if anybody handled the message or not.
 */
bool IpcListener::HandleIpcMessage(const IpcData& ipcRequest, IpcData& ipcResponse)
{
  // lock us in
  threads::Lock guard(_mutex);

  // go around and try and get someone to handle the message.
  for (auto it = _messageHandlers.begin(); it != _messageHandlers.end(); ++it)
  {
    // the message handler
    auto& handler = *(*it);

    // can we handle this message?
    IpcData ipcThisResponse(ipcResponse.GetGuid());
    if (handler.HandleIpcMessage(ipcRequest, ipcThisResponse))
    {
      // copy the response.
      ipcResponse = ipcThisResponse;

      // handled.
      return true;
    }
  }

  // if we are here, we never found anything.
  return false;
}

}
}
