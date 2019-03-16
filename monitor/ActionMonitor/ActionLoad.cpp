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
#include "ActionLoad.h"
#include "ActionsCore.h"
#include "ActiveLoadAction.h"

/**
 * \brief Constructor
 * \param application the application to close the app
 */
ActionLoad::ActionLoad(IApplication& application) :
  Action( ACTION_CORE_LOAD, L""),
  _application(application)
{
}

/**
 * \copydoc
 */
ActionLoad::~ActionLoad()
{
}

/**
 * \copydoc
 */
IActiveAction* ActionLoad::CreateActiveAction(IVirtualMachines& virtualMachines, CWnd* pWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) const
{
  //  get the last forground window handle
  const auto hTopHWnd = pWnd ? pWnd->GetSafeHwnd() : nullptr;

  //  display the version.
  return new ActiveLoadAction(_application, *this, hTopHWnd);
}