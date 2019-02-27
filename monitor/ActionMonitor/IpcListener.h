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
#include "IIpcListener.h"
#include "threads/lock.h"
#include "os/ipclistener.h"

class IpcListener final : public IIpcListener
{
public:
  IpcListener();
  virtual ~IpcListener();

  void WaitForAllToComplete() override;
  void Add(myodd::os::IpcMessageHandler& handler) override;
  void Remove(myodd::os::IpcMessageHandler& handler) override;
  void Initialize(HWND hWnd) override;

private:

  myodd::os::IpcListener* _ipc;

  myodd::threads::Key _mutex;
};

