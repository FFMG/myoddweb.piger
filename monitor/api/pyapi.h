#pragma once

// this is the version number for that particular API
static const double ACTIONMONITOR_API_PY_VERSION = 0.1f;

// support for Python
#include "python\include\Python.h"

class pyapi
{
protected: // all static
  pyapi(void);
  ~pyapi(void);

public:
  static void Initialize( );

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