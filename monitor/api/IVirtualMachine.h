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
#include "IMessagesHandler.h"
#include "IActions.h"
#include "ActiveAction.h"
#include "IIpcListener.h"

class IVirtualMachine
{
public:
  explicit IVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& iIpcListener);
  virtual ~IVirtualMachine();

  IVirtualMachine(const IVirtualMachine&) = delete;
  void operator=(const IVirtualMachine&) = delete;

  /**
   * \brief tell the virtual machine to execute the given action
   * \param action the action we are calling
   * \param pluginFile the location of the action, (as it might not be the same as the action itself)
   * \return a number related to the virtual machine.
   */
  virtual int Execute(const ActiveAction& action, const std::wstring& pluginFile) = 0;

  /**
   * \brief Tell the virtual machine to stop all the running actions and destroy them
   *        The virtual machine can keep the action running and simply detach itself from the running action if needed.
   */
  virtual void Destroy() = 0;

  /**
   * \brief Give a chance to the virtual machine to initialize.
   * \return if the initialization worked ork not.
   */
  virtual bool Initialize() = 0;
protected:
  /**
   * \brief Get the message handler.
   */
  IMessagesHandler& GetMessagesHandler() const
  {
    return _messagesHandler;
  }

  IActions& GetActions() const
  {
    return _actions;
  }

  IIpcListener& GetIpcListener() const
  {
    return _ipcListener;
  }
private:
  /**
   * \param the message handler that the virtual machine can use.
   */
  IMessagesHandler& _messagesHandler;

  /**
   * \param the collection of actions
   */
  IActions& _actions;

  /**
   * \brief the ipc listener.
   */
  IIpcListener& _ipcListener;
};
