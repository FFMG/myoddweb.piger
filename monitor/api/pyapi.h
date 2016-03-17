#pragma once

#ifdef ACTIONMONITOR_API_PY

#include "helperapi.h"

// this is the version number for that particular API
// 0.1 was the old API, not idea what version of Python it was using.
// 2.0 uses version 3.5 of Python.
static const double ACTIONMONITOR_API_PY_VERSION = 2.0;

// support for Python
#include "python\include\Python.h"

class pyapi : public helperapi
{
public:
  pyapi( const ActiveAction& action, const std::string& script, PyThreadState* mainThreadState );
  virtual ~pyapi();

public:
  PyObject* Say(PyObject *self, PyObject *args);
  PyObject* Version(PyObject *self, PyObject *args);
  PyObject* GetCommand(PyObject *self, PyObject *args);
  PyObject* GetAction(PyObject *self, PyObject *args);
  PyObject* GetCommandCount(PyObject *self, PyObject *args);
  PyObject* Execute(PyObject *self, PyObject *args);
  PyObject* Getstring(PyObject *self, PyObject *args);
  PyObject* Getfile(PyObject *self, PyObject *args);
  PyObject* Getfolder(PyObject *self, PyObject *args);
  PyObject* Geturl(PyObject *self, PyObject *args);
  PyObject* AddAction(PyObject *self, PyObject *args);
  PyObject* RemoveAction(PyObject *self, PyObject *args);
  PyObject* GetVersion(PyObject *self, PyObject *args);
  PyObject* FindAction(PyObject *self, PyObject *args);

  void ExecuteInThread();
protected:
  const std::string _script;

  PyThreadState* _mainThreadState;

  void CheckForPythonErrors();

  PyObject* Fail();
};

#endif /*# ACTIONMONITOR_API_PY*/