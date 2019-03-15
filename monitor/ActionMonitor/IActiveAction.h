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

class IActiveAction
{
public:
  IActiveAction();

  void operator=(const IActiveAction& ) = delete;
  IActiveAction( const IActiveAction&) = delete;

	virtual ~IActiveAction() = default;

  virtual void ExecuteInThread() = 0;
  virtual bool Initialize() = 0;
  virtual bool DeInitialize() = 0;
};
