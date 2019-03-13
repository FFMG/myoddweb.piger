#include "stdafx.h"
#include "ActiveExeAction.h"
#include "ActionMonitor.h"

/**
 * The Default active contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const MYODD_STRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActiveExeAction::ActiveExeAction(const Action& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, hTopHWnd, szCommandLine, isPrivileged )
{
}

ActiveExeAction::~ActiveExeAction()
{
}

bool ActiveExeAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActiveExeAction::OnInitialize()
{
  //  all good
  return true;
}

void ActiveExeAction::OnExecuteInThread()
{
  auto szFile = File();
  auto szExt = myodd::files::GetExtension( szFile );;
  auto szCommand = Command();

	//  we just launch the exe by itself.
	std::vector<MYODD_STRING> argv;
	argv.push_back( szFile);
	Execute(argv, IsPrivileged(), nullptr );
}
