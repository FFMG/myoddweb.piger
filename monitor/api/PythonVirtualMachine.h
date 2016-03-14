#pragma once
#ifdef ACTIONMONITOR_API_PY

#include "pyapi.h"
#include "../threads/lock.h"

class PythonVirtualMachine
{
public:
  PythonVirtualMachine();
  ~PythonVirtualMachine();

  bool Initialize(); 

  //  check the extension.
  static bool IsPyExt( LPCTSTR ext );

  PyThreadState* GetMainPyThread() const {
    return _mainThreadState;
  }

  void AddApi(std::thread::id id, pyapi* api);
  void RemoveApi(std::thread::id id);

protected:
  bool m_isInitialized;
  bool InitializeFunctions();

  std::mutex _mutex;

  typedef std::map<std::thread::id, pyapi*> Apis;
  Apis _apis;
  static pyapi& PythonVirtualMachine::GetApi();

  PyThreadState* _mainThreadState;

public:
  static PyObject* say(PyObject *self, PyObject *args);
  static PyObject* version(PyObject *self, PyObject *args);
  static PyObject* getCommand(PyObject *self, PyObject *args);
  static PyObject* getAction(PyObject *self, PyObject *args);
  static PyObject* getCommandCount(PyObject *self, PyObject *args);
  static PyObject* execute(PyObject *self, PyObject *args);
  static PyObject* getstring(PyObject *self, PyObject *args);
  static PyObject* getfile(PyObject *self, PyObject *args);
  static PyObject* getfolder(PyObject *self, PyObject *args);
  static PyObject* geturl(PyObject *self, PyObject *args);
  static PyObject* addAction(PyObject *self, PyObject *args);
  static PyObject* removeAction(PyObject *self, PyObject *args);
  static PyObject* getVersion(PyObject *self, PyObject *args);
  static PyObject* findAction(PyObject *self, PyObject *args);
};

#endif /* ACTIONMONITOR_API_PY */