#pragma once
class VirtualMachines final
{
public:
  explicit VirtualMachines( IMessagesHandler& messagesHandler);
  ~VirtualMachines();

  // prevent copies.
  void operator=(const VirtualMachines&) = delete;
  VirtualMachines(const VirtualMachines&) = delete;

  template<class T>
  T& Get();

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
  ShellVirtualMachine& Get<ShellVirtualMachine>()
  {
    if (_svm == nullptr)
    {
      _svm = new ShellVirtualMachine();
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
  CsVirtualMachine& Get<CsVirtualMachine>()
  {
    if (_csvm == nullptr)
    {
      _csvm = new CsVirtualMachine();
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
  PowershellVirtualMachine& Get<PowershellVirtualMachine>()
  {
    if (_psvm == nullptr)
    {
      _psvm = new PowershellVirtualMachine();
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
  PluginVirtualMachine& Get<PluginVirtualMachine>()
  {
    if (_psvm == nullptr)
    {
      _plugvm = new PluginVirtualMachine();
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
  PythonVirtualMachine& Get<PythonVirtualMachine>()
  {
    if (_pvm == nullptr)
    {
      _pvm = new PythonVirtualMachine();
    }
    return *_pvm;
  }
#endif
};

