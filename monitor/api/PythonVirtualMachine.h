#pragma once
#ifdef ACTIONMONITOR_API_PY

#include "python\include\Python.h"
#include "pyapi.h"
#include "../threads/lock.h"

class PythonVirtualMachine
{
public:
  PythonVirtualMachine();
  ~PythonVirtualMachine();

  bool Initialize(); 

  //  check the extension.
  static bool IsExt( LPCTSTR ext );

  PyThreadState* GetMainPyThread() const {
    return _mainThreadState;
  }

  void AddApi(std::thread::id id, PyApi* api);
  void RemoveApi(std::thread::id id);

protected:
  bool m_isInitialized;
  bool InitializeFunctions();

  std::mutex _mutex;

  typedef std::map<std::thread::id, PyApi*> Apis;
  Apis _apis;
  static PyApi& PythonVirtualMachine::GetApi();

  PyThreadState* _mainThreadState;

public:
  static PyObject* Say(PyObject *self, PyObject *args);
  static PyObject* Version(PyObject *self, PyObject *args);
  static PyObject* GetCommand(PyObject *self, PyObject *args);
  static PyObject* GetAction(PyObject *self, PyObject *args);
  static PyObject* GetCommandCount(PyObject *self, PyObject *args);
  static PyObject* Execute(PyObject *self, PyObject *args);
  static PyObject* Getstring(PyObject *self, PyObject *args);
  static PyObject* Getfile(PyObject *self, PyObject *args);
  static PyObject* Getfolder(PyObject *self, PyObject *args);
  static PyObject* Geturl(PyObject *self, PyObject *args);
  static PyObject* AddAction(PyObject *self, PyObject *args);
  static PyObject* RemoveAction(PyObject *self, PyObject *args);
  static PyObject* GetVersion(PyObject *self, PyObject *args);
  static PyObject* FindAction(PyObject *self, PyObject *args);
  static PyObject* Log(PyObject *self, PyObject *args);
};
#endif /* ACTIONMONITOR_API_PY */