#include "stdafx.h"
#include "ActivePythonAction.h"
#include "ActionMonitor.h"

/**
 * The Python contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const STD_TSTRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActivePythonAction::ActivePythonAction(const Action& src, HWND hTopHWnd, const STD_TSTRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, hTopHWnd, szCommandLine, isPrivileged )
{
}

ActivePythonAction::~ActivePythonAction()
{
}

bool ActivePythonAction::OnDeInitialize()
{
  assert(Py_IsInitialized());
  return true;
}

bool ActivePythonAction::OnInitialize()
{
  PythonVirtualMachine* py = App().GetPythonVirtualMachine();
  if (!py->Initialize())
  {
    return false;
  }

  //  all good
  return true;
}

void ActivePythonAction::OnExecuteInThread()
{
  //  the file.
  const STD_TSTRING& szFile = File();

  // Python is not thread safe 
  // and windows cannot lock the file properly
  // so we need to read the file ourselves and pass it.
  //
  // this could be a memory problem at some stage.
  //
  std::string script = "";
  if (!ReadFile(szFile.c_str(), script))
  {
    return;
  }

  // create the Python Api.
  PythonVirtualMachine* py = App().GetPythonVirtualMachine();
  PyApi* api = new PyApi( *this, script, py->GetMainPyThread() );

  //  save it.
  std::thread::id id = std::this_thread::get_id();
  py->AddApi(id, api);

  // we can now execute the thread.
  api->ExecuteInThread();

  py->RemoveApi(id);
  delete api;
}
