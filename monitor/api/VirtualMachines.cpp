#include "stdafx.h"
#include "VirtualMachines.h"

VirtualMachines::VirtualMachines(IMessagesHandler& messagesHandler) :
  _messagesHandler( messagesHandler )
#ifdef ACTIONMONITOR_API_LUA
  , _lvm(nullptr)
#endif
#ifdef ACTIONMONITOR_S_PLUGIN
  , _svm(nullptr)
#endif
#ifdef ACTIONMONITOR_CS_PLUGIN
  , _csvm(nullptr)
#endif
#ifdef ACTIONMONITOR_PS_PLUGIN
  , _psvm(nullptr)
#endif
#ifdef ACTIONMONITOR_API_PLUGIN
  , _plugvm(nullptr)
#endif
#ifdef ACTIONMONITOR_API_PY
  , _pvm(nullptr)
#endif
{
}

VirtualMachines::~VirtualMachines()
{
#ifdef ACTIONMONITOR_API_LUA
  Destroy<LuaVirtualMachine>();
#endif

#ifdef ACTIONMONITOR_S_PLUGIN
  Destroy<ShellVirtualMachine>();
#endif

#ifdef ACTIONMONITOR_CS_PLUGIN
  Destroy<CsVirtualMachine>();
#endif

#ifdef ACTIONMONITOR_PS_PLUGIN
  Destroy<PowershellVirtualMachine>();
#endif

#ifdef ACTIONMONITOR_API_PLUGIN
  Destroy<PluginVirtualMachine>();
#endif

#ifdef ACTIONMONITOR_API_PY
  Destroy<PythonVirtualMachine>();
#endif
}

