#include "stdafx.h"
#include "ActiveCmdAction.h"
#include "ActionMonitor.h"

/**
* The Batch active contructor
* @param const Action& src the action that is now active.
* @param HWND hTopHWnd the window that was on top at the time the command was given.
* @param const MYODD_STRING& szCommandLine the given command line that is, the words after the command itself
* @param bool isPrivileged if this action is privileged or not.
*/
ActiveCmdAction::ActiveCmdAction(IApplication& application, const IAction& src, HWND hTopHWnd, const MYODD_STRING& szCommandLine) :
  ActiveAction(application, src, hTopHWnd, szCommandLine, true)
{
}

ActiveCmdAction::~ActiveCmdAction()
{
}

bool ActiveCmdAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActiveCmdAction::OnInitialize()
{
  //  all good
  return true;
}

void ActiveCmdAction::OnExecuteInThread()
{
  //  the file.
  auto szFile = File();

  //  join the two items together.
  std::vector<MYODD_STRING> argv;
  argv.push_back( _T("cmd") );

  auto arguments = myodd::strings::Format(_T("/c %s %s"), szFile.c_str(), GetCommandLine());
  argv.push_back(arguments);
  Execute(argv, IsPrivileged(), nullptr);
}
