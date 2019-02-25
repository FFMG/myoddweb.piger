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
#pragma once
#include "os/ipclistener.h"

class IIpcListener
{
public:
  IIpcListener();
  virtual ~IIpcListener();

  IIpcListener(const IIpcListener&) = delete;
  void operator=(const IIpcListener&) = delete;

  /**
   * \brief wait for all the windows to close/complete.
   */
  virtual void WaitForAllToComplete() = 0;

  /**
   * \brief Add a message handler to our list of handlers.
   *        Each will be called when a new message arrives.
   * \param handler the message handler we are adding.
   */
  virtual void Add(myodd::os::IpcMessageHandler& handler) = 0;

  /**
   * \brief Remove a message handler from our list.
   * \param handler the message handler we want to remove.
   */
  virtual void Remove(myodd::os::IpcMessageHandler& handler) = 0;

  /**
   * \brief Initialize the listener.
   * \param hWnd the window that will handle the messages
   */
  virtual void Initialize( HWND hWnd ) = 0;
};

