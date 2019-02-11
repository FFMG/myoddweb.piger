#pragma once

#ifdef ACTIONMONITOR_API_PY

#include "helperapi.h"

// support for Python
# if defined(_DEBUG)
#   pragma comment(lib,"python37_d.lib")
# elif defined(Py_LIMITED_API)
#   pragma comment(lib,"python37.lib")
# endif /* _DEBUG */

#include "Python.h"

class PyApi : public HelperApi
{
public:
  PyApi( const ActiveAction& action, std::string script, PyThreadState* mainThreadState );
  virtual ~PyApi() = default;

public:
  static PyObject* Version(PyObject *self, PyObject *args);

  PyObject* Say(PyObject *self, PyObject *args) const;
  PyObject* GetCommand(PyObject *self, PyObject *args) const;
  PyObject* GetAction(PyObject *self, PyObject *args) const;
  PyObject* GetCommandCount(PyObject *self, PyObject *args) const;
  PyObject* Execute(PyObject *self, PyObject *args) const;
  PyObject* GetString(PyObject *self, PyObject *args) const;
  PyObject* GetFile(PyObject *self, PyObject *args) const;
  PyObject* GetFolder(PyObject *self, PyObject *args) const;
  PyObject* GetUrl(PyObject *self, PyObject *args) const;
  PyObject* AddAction(PyObject *self, PyObject *args) const;
  PyObject* RemoveAction(PyObject *self, PyObject *args) const;
  PyObject* GetVersion(PyObject *self, PyObject *args) const;
  PyObject* FindAction(PyObject *self, PyObject *args) const;
  PyObject* Log(PyObject *self, PyObject *args) const;

  void ExecuteInThread();
protected:
  /**
   * \brief the script that we are running
   */
  const std::string _script;

  /**
   * \brief the main thread state
   */
  PyThreadState* _mainThreadState;

  void CheckForPythonErrors() const;

  /**
   * \brief Return a 'false' message and swallow any errors.
   * \return PyObject* a 'false' python object.
   */
  static PyObject* Fail();
};

#endif /*# ACTIONMONITOR_API_PY*/