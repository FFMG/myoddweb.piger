#pragma once
#ifdef ACTIONMONITOR_API_PY

#include "pyapi.h"

class PythonVirtualMachine
{
public:
  PythonVirtualMachine();
  ~PythonVirtualMachine();

  bool Initialize(); 

  int Execute( LPCTSTR pyFile, const ActiveAction& action);
  static bool IsPyExt( LPCTSTR ext );

  PyThreadState* GetMainPyThread() const {
    return _mainThreadState;
  }
protected:
  bool m_isInitialized;
  bool InitializeFunctions();

  pyapi* _api;
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