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
#include "ActiveDefaultAction.h"
#include "ActionMonitor.h"

/**
 * The Default active contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const MYODD_STRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActiveDefaultAction::ActiveDefaultAction(const Action& src, const HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, hTopHWnd, szCommandLine, isPrivileged )
{
}

ActiveDefaultAction::~ActiveDefaultAction()
{
}

bool ActiveDefaultAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActiveDefaultAction::OnInitialize()
{
  //  all good
  return true;
}

void ActiveDefaultAction::OnExecuteInThread()
{
  // we need to log that we are going to run this as a default.
  // we should always try and create an ActiveAction for each known extensions.
  // otherwise, who knows how this will run, (for example and swf extension might not be able to run).
  const auto szFile = File();
  auto szExt = myodd::files::GetExtension( szFile );;
  auto szCommand = Command();
  myodd::log::LogWarning(_T("Will try and execute the command '%s' from file '%s'"), szExt.c_str(), szCommand.c_str());

	//  join the two items together.
	std::vector<MYODD_STRING> argv;
	argv.push_back( szFile);
	argv.push_back( GetCommandLine() );
	Execute(argv, IsPrivileged(), nullptr );
}
