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
#include "IAction.h"

class IActions
{
public:
	IActions();
	virtual ~IActions();

  IActions(const IActions&) = delete;
  void operator=( const IActions& ) = delete;

  virtual void CurrentActionReset() = 0;
  virtual void CurrentActionAdd(wchar_t c) = 0;
  virtual void CurrentActionBack() = 0;
  virtual void SetAction(IAction* tmpAction) = 0;
  virtual const IAction* GetCommand() = 0;
  virtual std::wstring GetCommandLine() = 0;
  virtual std::wstring ToChar() = 0;
  virtual void down() = 0;
  virtual void up() = 0;
  virtual void Initialize() = 0;
  virtual bool Add(IAction* action) = 0;
  virtual bool Remove(const std::wstring& szText, const std::wstring& szPath) = 0;
  virtual const IAction* Find(const std::wstring& szText, unsigned int idx) = 0;
};
