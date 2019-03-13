#include "stdafx.h"
#include "ActiveBatchAction.h"

/**
* The Batch active contructor
* @param const Action& src the action that is now active.
* @param HWND hTopHWnd the window that was on top at the time the command was given.
* @param const MYODD_STRING& szCommandLine the given command line that is, the words after the command itself
* @param bool isPrivileged if this action is privileged or not.
*/
ActiveBatchAction::ActiveBatchAction(const Action& src, const HWND hTopHWnd, const MYODD_STRING& szCommandLine) :
  ActiveAction(src, hTopHWnd, szCommandLine, true)
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
  std::vector<MYODD_STRING> argv;
  argv.push_back( _T("cmd") );

  const auto arguments = myodd::strings::Format(_T("/c %s %s"), szFile.c_str(), GetCommandLine());
  argv.push_back(arguments);
  Execute(argv, IsPrivileged(), nullptr);
}
