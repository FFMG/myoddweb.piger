#include "stdafx.h"
#include "VirtualMachines.h"

VirtualMachines::VirtualMachines(IApplication& application, IMessagesHandler& messagesHandler, IIpcListener& ipcListener) :
  IVirtualMachines(),
  _messagesHandler( messagesHandler ),
  _application(application),
  _ipcListener(ipcListener),
  _key( L"Virtual machines" )
{
}

VirtualMachines::~VirtualMachines()
{
  Destroy();
}

void VirtualMachines::Destroy()
{
  myodd::threads::Lock lock(_key);

  // delete them all, one at a time
  for( auto vm : _machines )
  {
    vm.second->Destroy();
    delete vm.second;
  }
  // remove all
  _machines.clear();
}

IVirtualMachine& VirtualMachines::Get(IVirtualMachines::Type type)
{
  myodd::threads::Lock lock(_key);

  //  check if this already exists.
  const auto current = _machines.find(type);
  if (current != _machines.end())
  {
    return *current->second;
  }

  // it does not, we need to add it here.
  IVirtualMachine* vm;
  switch( type )
  {
#ifdef ACTIONMONITOR_API_LUA
  case Type::Lua:
    vm = new LuaVirtualMachine(_application, _messagesHandler, _ipcListener);
    break;
#endif
#ifdef ACTIONMONITOR_API_PY
  case Type::Python:
    vm = new PythonVirtualMachine(_application, _messagesHandler, _ipcListener);;
    break;
#endif
#ifdef ACTIONMONITOR_API_PLUGIN
  case Type::LegacyPlugin:
    vm = new PluginVirtualMachine(_application, _messagesHandler, _ipcListener);
    break;
#endif
#ifdef ACTIONMONITOR_PS_PLUGIN
  case Type::Powershell:
    vm = new PowershellVirtualMachine(_application, _messagesHandler, _ipcListener);
    break;
#endif
#ifdef ACTIONMONITOR_CS_PLUGIN
  case Type::CSharp:
    vm = new CsVirtualMachine(_application, _messagesHandler, _ipcListener);
    break;
#endif
#ifdef ACTIONMONITOR_S_PLUGIN
  case Type::Shell:
    vm = new ShellVirtualMachine(_application, _messagesHandler, _ipcListener);
    break;
#endif

  default:
    throw std::logic_error( "This virtual machine type is not supported for this build.");
  }

  vm->Initialize();
  _machines[type] = vm;
  return *vm;
}