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
#include "ActiveBatchAction.h"

/**
* \brief The Batch active constructor
* \param src the action that is now active.
* \param hTopHWnd the window that was on top at the time the command was given.
* \param szCommandLine the given command line that is, the words after the command itself
*/
ActiveBatchAction::ActiveBatchAction(IApplication& application, const IAction& src, const HWND hTopHWnd, const std::wstring& szCommandLine) :
  ActiveAction(application, src, hTopHWnd, szCommandLine, true)
{
}

ActiveBatchAction::~ActiveBatchAction()
{
}

bool ActiveBatchAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActiveBatchAction::OnInitialize()
{
  //  all good
  return true;
}

void ActiveBatchAction::OnExecuteInThread()
{
  //  the file.
  auto szFile = File();

  //  join the two items together.
  std::vector<std::wstring> argv;
  argv.push_back( _T("cmd") );

  const auto arguments = myodd::strings::Format(_T("/c %s %s"), szFile.c_str(), GetCommandLine());
  argv.push_back(arguments);
  Execute(argv, IsPrivileged(), nullptr);
}
