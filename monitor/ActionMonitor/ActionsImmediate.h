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

#include <threads/lock.h>
#include "Actions.h"
#include "ActiveActions.h"

class ActionsImmediate final :
  public Actions, ActiveActions
{
public:
  explicit ActionsImmediate(IApplication& application, const std::wstring& directoryToParse, IActions& parentActions, IVirtualMachines& virtualMachines);
  virtual ~ActionsImmediate() = default;

  ActionsImmediate(const ActionsImmediate&) = delete;
  void operator=(const ActionsImmediate&) = delete;

  void Initialize() override;
  void WaitForAll();

protected:
  void DoThem();

private:
  /**
   * \brief no reserved directories here.
   */
  bool IsReservedDir(const wchar_t*)const override { return false; }

  /**
   * \brief the sub directory that contains the actions we want to run.
   */
  const std::wstring _directoryToParse;

  /**
   * \brief the parent actions.
   */
  IActions& _parentActions;

  /**
   * \brief all the virtual machines.
   */
  IVirtualMachines& _virtualMachines;

  /**
   * \brief the mutex that manages the runners
   */
  myodd::threads::Key _mutex;
};
