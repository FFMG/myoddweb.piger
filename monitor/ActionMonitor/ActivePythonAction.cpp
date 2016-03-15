#include "stdafx.h"
#include "ActivePythonAction.h"
#include "ActionMonitor.h"

ActivePythonAction::ActivePythonAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, szCommandLine, isPrivileged )
{
  Initialize();
}

ActivePythonAction::~ActivePythonAction()
{
  DeInitialize();
}

bool ActivePythonAction::DeInitialize()
{
  assert(Py_IsInitialized());
  return true;
}

bool ActivePythonAction::Initialize()
{
  PythonVirtualMachine* py = App().GetPythonVirtualMachine();
  if (!py->Initialize())
  {
    return false;
  }

  //  all good
  return true;
}

void ActivePythonAction::ExecuteInThread()
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
  pyapi* api = new pyapi( *this, script, py->GetMainPyThread() );

  //  save it.
  std::thread::id id = std::this_thread::get_id();
  py->AddApi(id, api);

  // we can now execute the thread.
  api->ExecuteInThread();

  py->RemoveApi(id);
  delete api;
}
