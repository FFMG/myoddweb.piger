#pragma once
#include "IVirtualMachine.h"

class VirtualMachines final
{
public:
  explicit VirtualMachines( IMessagesHandler& messagesHandler);
  ~VirtualMachines();

  // prevent copies.
  void operator=(const VirtualMachines&) = delete;
  VirtualMachines(const VirtualMachines&) = delete;

  void Destroy();

  template<class T>
  T& Get();

  template<class T>
  IVirtualMachine& Get1();

private:
  IMessagesHandler& _messagesHandler;

  template<class T>
  void Destroy();

#ifdef ACTIONMONITOR_API_LUA
private:
  LuaVirtualMachine* _lvm;
  template<>
  void Destroy<LuaVirtualMachine>()
  {
    if (_lvm == nullptr)
    {
      return;
    }

    // We have to kill all the API plugins.
    // they should be all done and completed.
    _lvm->Destroy();

    // clean up
    delete _lvm;
    _lvm = nullptr;
  }

public:
  template<>
  LuaVirtualMachine& Get<LuaVirtualMachine>()
  {
    if (_lvm == nullptr)
    {
      _lvm = new LuaVirtualMachine();
    }
    return *_lvm;
  }
#endif

#ifdef ACTIONMONITOR_S_PLUGIN
private:
  ShellVirtualMachine* _svm;
  template<>
  void Destroy<ShellVirtualMachine>()
  {
    if (_svm == nullptr)
    {
      return;
    }

    // We have to kill all the API plugins.
    // they should be all done and completed.
    _svm->Destroy();

    // clean up
    delete _svm;
    _svm = nullptr;
  }

public:
  template<>
  IVirtualMachine& Get1<ShellVirtualMachine>()
  {
    if (_svm == nullptr)
    {
      _svm = new ShellVirtualMachine( _messagesHandler );
      _svm->Initialize();
    }
    return *_svm;
  }
#endif

#ifdef ACTIONMONITOR_CS_PLUGIN
private:
  CsVirtualMachine* _csvm;
  template<>
  void Destroy<CsVirtualMachine>()
  {
    if (_csvm == nullptr)
    {
      return;
    }

    // We have to kill all the API plugins.
    // they should be all done and completed.
    _csvm->Destroy();

    // clean up
    delete _csvm;
    _csvm = nullptr;
  }

public:
  template<>
  IVirtualMachine& Get1<CsVirtualMachine>()
  {
    if (_csvm == nullptr)
    {
      _csvm = new CsVirtualMachine( _messagesHandler );
      _csvm->Initialize();
    }
    return *_csvm;
  }
#endif

#ifdef ACTIONMONITOR_PS_PLUGIN
private:
  PowershellVirtualMachine* _psvm;
  template<>
  void Destroy<PowershellVirtualMachine>()
  {
    if (_psvm == nullptr)
    {
      return;
    }

    // We have to kill all the API plugins.
    // they should be all done and completed.
    _psvm->Destroy();

    // clean up
    delete _psvm;
    _psvm = nullptr;
  }

public:
  template<>
  IVirtualMachine& Get1<PowershellVirtualMachine>()
  {
    if (_psvm == nullptr)
    {
      _psvm = new PowershellVirtualMachine( _messagesHandler );
      _psvm->Initialize();
    }
    return *_psvm;
  }
#endif

#ifdef ACTIONMONITOR_API_PLUGIN
private:
  PluginVirtualMachine* _plugvm;
  template<>
  void Destroy<PluginVirtualMachine>()
  {
    if (_plugvm == nullptr)
    {
      return;
    }

    // We have to kill all the API plugins.
    // they should be all done and completed.
    _plugvm->Destroy();

    // clean up
    delete _plugvm;
    _plugvm = nullptr;
  }

public:
  template<>
  IVirtualMachine& Get1<PluginVirtualMachine>()
  {
    if (_psvm == nullptr)
    {
      _plugvm = new PluginVirtualMachine( _messagesHandler );
      _plugvm->Initialize();
    }
    return *_plugvm;
  }
#endif

#ifdef ACTIONMONITOR_API_PY
private:
  PythonVirtualMachine* _pvm;
  template<>
  void Destroy<PythonVirtualMachine>()
  {
    if (_pvm == nullptr)
    {
      return;
    }

    // We have to kill all the API plugins.
    // they should be all done and completed.
    _pvm->Destroy();

    // clean up
    delete _pvm;
    _pvm = nullptr;
  }

public:
  template<>
  IVirtualMachine& Get1<PythonVirtualMachine>()
  {
    if (_pvm == nullptr)
    {
      _pvm = new PythonVirtualMachine( _messagesHandler );
      _pvm->Initialize();
    }
    return *_pvm;
  }
#endif
};

