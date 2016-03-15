#include "stdafx.h"
#include "ActivePluginAction.h"
#include "ActionMonitor.h"

ActivePluginAction::ActivePluginAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, szCommandLine, isPrivileged )
{
  Initialize();
}

ActivePluginAction::~ActivePluginAction()
{
  DeInitialize();
}

bool ActivePluginAction::OnDeInitialize()
{
  // nothing to do.
  return true;
}

bool ActivePluginAction::OnInitialize()
{
  //  all good
  return true;
}

void ActivePluginAction::ExecuteInThread()
{
  //  the file.
  const STD_TSTRING& szFile = File();

  // create the Python Api.
  PluginVirtualMachine* pvm = App().GetPluginVirtualMachine();
  pluginapi* api = new pluginapi( *this );

  //add it to the list
  pvm->AddApi(api);
  
  //  save it.
  // we can now execute the thread.
  pvm->ExecuteInThread( szFile.c_str() );

  // we can now dispose of the lua
  pvm->DisposeApi();
  delete api;
}