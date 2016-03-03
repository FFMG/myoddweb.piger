#pragma once

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
  pyapi();
  virtual ~pyapi();

public:
  PyObject* say(PyObject *self, PyObject *args);
  PyObject* version(PyObject *self, PyObject *args);
  PyObject* getCommand(PyObject *self, PyObject *args);
  PyObject* getAction(PyObject *self, PyObject *args);
  PyObject* getCommandCount(PyObject *self, PyObject *args);
  PyObject* execute(PyObject *self, PyObject *args);
  PyObject* getstring(PyObject *self, PyObject *args);
  PyObject* getfile(PyObject *self, PyObject *args);
  PyObject* getfolder(PyObject *self, PyObject *args);
  PyObject* geturl(PyObject *self, PyObject *args);
  PyObject* addAction(PyObject *self, PyObject *args);
  PyObject* removeAction(PyObject *self, PyObject *args);
  PyObject* getVersion(PyObject *self, PyObject *args);
  PyObject* findAction(PyObject *self, PyObject *args);
};