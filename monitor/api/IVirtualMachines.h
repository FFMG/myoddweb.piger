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

class IVirtualMachine;
class IVirtualMachines
{
public:
  enum class Type
  {
    None,
    Lua,
    Python,
    LegacyPlugin,
    Powershell,
    CSharp,
    Shell
  };

  explicit IVirtualMachines();
  virtual ~IVirtualMachines();

  IVirtualMachines(const IVirtualMachines&) = delete;
  void operator=(const IVirtualMachines&) = delete;

  /**
   * \brief destroy all the virtual machines.
   */
  virtual void Destroy() = 0;

  /**
   * \brief Get a virtual machine of a certain type
   *        We will throw if the virtual machine does not exist.
   * \return the virtual machine
   */
  virtual IVirtualMachine& Get( Type type) = 0;
};
