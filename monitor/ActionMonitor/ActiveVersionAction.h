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
#include "ActiveAction.h"
#include "IApplication.h";

class ActiveVersionAction final : public ActiveAction
{
public:
  ActiveVersionAction(IApplication& application, const Action& src, IVirtualMachines& virtualMachines, HWND hTopHWnd );
  virtual ~ActiveVersionAction();

protected:
  bool OnInitialize() override;
  bool OnDeInitialize() override;
  void OnExecuteInThread() override;

private:
  IApplication& _application;
};

