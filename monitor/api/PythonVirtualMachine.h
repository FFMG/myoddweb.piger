#pragma once

#include "pyapi.h"

class PythonVirtualMachine
{
public:
  PythonVirtualMachine();
  ~PythonVirtualMachine();

  void Initialize(); 

  int LoadFile( LPCTSTR pyFile );
  static bool IsPyExt( LPCTSTR ext );

protected:
  bool m_isInitialized;
  void InitializeFunctions();

  pyapi* _api;
  static pyapi& PythonVirtualMachine::GetApi();

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