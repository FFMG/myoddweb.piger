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
#include "ExecuteApi.h"

ExecuteApi::ExecuteApi(const std::wstring& uuid, const IActiveAction& action, IApplication& application, IMessagesHandler& messages) :
  HelperApi(action, application, messages),
  _uuid(uuid),
  _hProcess(nullptr)
{
}

ExecuteApi::~ExecuteApi()
{
}

/**
 * \brief Set this API handle.
 * \param process the process handle we are setting.
 */
void ExecuteApi::SetHandle(const HANDLE process)
{
  _hProcess = process;
}

/**
 * \brief Get the current handle value.
 * \return the current handle.
 */
HANDLE ExecuteApi::GetHandle() const
{
  return _hProcess;
}

/**
 * \brief Override function to execute a process.
 * \param module the module we want to execute.
 * \param cmdLine the arguments to pass to this module.
 * \param isPrivileged if we need to elevate this module or not.
 * \param hProcess if not null, the handle of the created process.
 * \return bool success or not.
 */
bool ExecuteApi::Execute(const wchar_t* module, const wchar_t* cmdLine, bool isPrivileged, HANDLE* hProcess) const
{
  return HelperApi::Execute(module, cmdLine, isPrivileged, hProcess);
}

/**
 * \brief Output a message on the screen
 * \see HelperApi::Say
 * \param sText what to say
 * \param elapseMiliSecondsBeforeFadeOut how long to say it for before we fade out
 * \param totalMilisecondsToShowMessage how long to show it for.
 * \return bool success or not.
 */
bool ExecuteApi::Say(const wchar_t* sText, const unsigned int elapseMiliSecondsBeforeFadeOut, const unsigned int totalMilisecondsToShowMessage) const
{
  return HelperApi::Say(sText, elapseMiliSecondsBeforeFadeOut, totalMilisecondsToShowMessage);
}