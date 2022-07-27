#pragma once
#ifdef ACTIONMONITOR_API_PY

#include "IVirtualMachine.h"
#include "pyapi.h"
#include <threads/lock.h>

class PythonVirtualMachine final : public IVirtualMachine
{
public:
  explicit PythonVirtualMachine(IApplication& application, IMessagesHandler& messagesHandler, IIpcListener& ipcListener );
  ~PythonVirtualMachine();
  bool Initialize() override;

  //  check the extension.
  static bool IsExt(const std::wstring& file);

  PyThreadState* GetMainPyThread() const {
    return _mainThreadState;
  }

  void AddApi(std::thread::id id, PyApi* api);
  void RemoveApi(std::thread::id id);

  void Destroy() override {};
  int Execute(const IActiveAction& action, const std::wstring& pluginFile) override;

private:
  bool m_isInitialized;
  bool InitializeFunctions();

  static PyApi& GetApi();

  PyThreadState* _mainThreadState;

  bool ReadFile(const const wchar_t* pyFile, std::wstring& script) const;

public:
  static PyObject* Say(PyObject *self, PyObject *args);
  static PyObject* Version(PyObject *self, PyObject *args);
  static PyObject* GetCommand(PyObject *self, PyObject *args);
  static PyObject* GetAction(PyObject *self, PyObject *args);
  static PyObject* GetCommandCount(PyObject *self, PyObject *args);
  static PyObject* ExecuteInScript(PyObject *self, PyObject *args);
  static PyObject* GetString(PyObject *self, PyObject *args);
  static PyObject* GetFile(PyObject *self, PyObject *args);
  static PyObject* GetFolder(PyObject *self, PyObject *args);
  static PyObject* GetUrl(PyObject *self, PyObject *args);
  static PyObject* AddAction(PyObject *self, PyObject *args);
  static PyObject* RemoveAction(PyObject *self, PyObject *args);
  static PyObject* GetVersion(PyObject *self, PyObject *args);
  static PyObject* FindAction(PyObject *self, PyObject *args);
  static PyObject* Log(PyObject *self, PyObject *args);
};
#endif /* ACTIONMONITOR_API_PY */