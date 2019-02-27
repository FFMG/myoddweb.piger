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
#include "ipclistener.h"
#include <afxwin.h>
#include <assert.h>
#include "ipclistenerwnd.h"

namespace myodd {
namespace os {
IpcListener::IpcListener(const wchar_t* serverName, void* parent ) :
  _pServer( nullptr ),
  _mutex( L"IpcListener" )
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
 * \brief Create the server window that will be listening to messages.
 * \param serverName the server name we wish to use.
 * \param pParent the parent that we want to pass the messages to.
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

// wait for the work to finish
void IpcListener::WaitForActiveHandlers()
{
  if( nullptr == _pServer )
  {
    return;
  }

  const auto hWnd = static_cast<IpcListenerWnd*>(_pServer)->GetSafeHwnd();
  //  lock up to make sure we only do one at a time
  MSG msg;
  while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    if (0 == ::GetWindowLongPtr(hWnd, GWLP_HWNDPARENT))
    {
      break;
    }
  }
}

/**
 * \brief Add a message handler to our list of handlers.
 *        Each will be called when a new message arrives.
 * \param handler the message handler we are adding.
 */
void IpcListener::AddMessageHandler(IpcMessageHandler& handler)
{
  // lock us in
  threads::Lock guard(_mutex);

  // look for the handler so we know if we need to remove it.
const auto it = FindMessageHandler(handler);

  // 
  if (it == _messageHandlers.end())
  {
    // add the message to the list.
    _messageHandlers.push_back(&handler);
  }
}

/**
 * \brief Remove a message handler from our list.
 * \param handler the message handler we want to remove.
 */
void IpcListener::RemoveMessageHandler(IpcMessageHandler& handler)
{
  // lock us in
  threads::Lock guard(_mutex);

  // look for the existing handler so we do not add it again.
  const auto it = FindMessageHandler(handler);
  if(it  != _messageHandlers.end() )
  {
    // we can now remove it.
    _messageHandlers.erase(it);
  }
}

/**
 * \brief Look for a message handler and return the iterator for it.
 *        This function assumes that we are locked in!
 * \param handler the handler we are looking for.
 * \return std::vector<IpcMessageHandler*>::const_iterator the iterator for this handler we are looking for ... or end()
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
 * \brief Handle a message 'request' and return a response accrodingly.
 * \param ipcRequest the given request.
 * \param ipcResponse the response container.
 * \return boolean if anybody handled the message or not.
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
