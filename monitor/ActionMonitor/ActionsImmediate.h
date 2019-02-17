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

#include "Actions.h"
#include "ActiveActions.h"

class ActionsImmediate final :
  public Actions, ActiveActions
{
public:
  explicit ActionsImmediate( const wchar_t* subDir );
  virtual ~ActionsImmediate() = default;

public:
  void Init() override;
  void WaitForAll();

protected:
  //  no reserved directories here.
  bool IsReservedDir( const wchar_t*)const override { return false;}

protected:
  /**
   * \brief the sub directory that contains the actions we want to run.
   */
  const std::wstring m_subDir;
  void DoThem();
};
