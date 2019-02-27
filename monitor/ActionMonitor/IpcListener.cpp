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
#include "IpcListener.h"

IpcListener::IpcListener() : 
  _ipc(nullptr),
  _mutex( L"Ipc Listener")
{
}

IpcListener::~IpcListener()
{
  //  clear the ipc listener
  delete _ipc;
}

void IpcListener::WaitForAllToComplete()
{
}

void IpcListener::Initialize(const HWND hWnd)
{
  if (nullptr == _ipc)
  {
    // lock us in
    myodd::threads::Lock guard(_mutex );

    // if this is done already, we don't want to do it again.
    if (nullptr == _ipc)
    {
      // create the listenner and pass oursleves as the window.
      // so some messages can be routed back to us.
      _ipc = new myodd::os::IpcListener(CONF_MUTEXT, hWnd);
    }
  }
}

/**
 * \brief Add a message handler to our list of handlers.
 *        Each will be called when a new message arrives.
 * \param handler the message handler we are adding.
 */
void IpcListener::Add(myodd::os::IpcMessageHandler& handler)
{
  myodd::threads::Lock guard(_mutex );
  if (_ipc == nullptr)
  {
    return;
  }
  _ipc->AddMessageHandler(handler);
}

/**
 * \brief Remove a message handler from our list.
 * \param handler the message handler we want to remove.
 */
void IpcListener::Remove(myodd::os::IpcMessageHandler& handler)
{
  myodd::threads::Lock guard(_mutex );
  if (_ipc == nullptr)
  {
    return;
  }
  _ipc->RemoveMessageHandler(handler);
}
