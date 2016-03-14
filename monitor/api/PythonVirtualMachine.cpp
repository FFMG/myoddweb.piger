#include "stdafx.h"
#ifdef ACTIONMONITOR_API_PY

#include "helperapi.h"
#include "PythonVirtualMachine.h"
#include "files\files.h"
#include "ActionMonitor.h"

static PyMethodDef amMethods[] = {
  { "say", PythonVirtualMachine::say, METH_VARARGS, "Display a message on the screen." },
  { "version", PythonVirtualMachine::version, METH_VARARGS, "Get this API version number." },
  { "getCommand", PythonVirtualMachine::getCommand, METH_VARARGS, "Get a certain command, return false if it does not exist." },
  { "getAction", PythonVirtualMachine::getAction, METH_VARARGS, "Get the action entered by the user." },
  { "getCommandCount", PythonVirtualMachine::getCommandCount, METH_VARARGS, "Get the number of commands." },
  { "execute", PythonVirtualMachine::execute, METH_VARARGS, "Execute a command, (app, command)." },
  { "getstring", PythonVirtualMachine::getstring, METH_VARARGS, "Get the currently selected text if any." },
  { "getfile", PythonVirtualMachine::getfile, METH_VARARGS, "Get a file by index, return false if not found." },
  { "getfolder", PythonVirtualMachine::getfolder, METH_VARARGS, "Get a folder by index, return false if not found." },
  { "geturl", PythonVirtualMachine::geturl, METH_VARARGS, "Get a URL by index, return false if not found." },
  { "addAction", PythonVirtualMachine::addAction, METH_VARARGS, "Add an action and path to the list of actions." },
  { "removeAction", PythonVirtualMachine::removeAction, METH_VARARGS, "Remove an action from the list." },
  { "getVersion", PythonVirtualMachine::getVersion, METH_VARARGS, "Get the full version string." },
  { "findAction", PythonVirtualMachine::findAction, METH_VARARGS, "Find an action given an index, return false or the path." },
  { NULL, NULL, 0, NULL }
};

static struct PyModuleDef amModule = {
  PyModuleDef_HEAD_INIT,
  "am",     /* name of module */
  NULL,     /* module documentation, may be NULL */
  -1,       /* size of per-interpreter state of the module,
            or -1 if the module keeps state in global variables. */
  amMethods
};

#if PY_MAJOR_VERSION < 3
PyObject *PyInit_am(void)
{
  PyObject *m;
  m = PyModule_Create(&amModule);
  if (!m) {
    return NULL;
  }
  return m;
}
#endif 

static PyObject*
PyInit_am(void)
{
  return PyModule_Create(&amModule);
}

/**
 * Todo
 * @param void
 * @return void
 */
PythonVirtualMachine::PythonVirtualMachine() : 
  m_isInitialized( false ),
  _mainThreadState( NULL )
{
}

/**
 * Todo
 * @param void
 * @return void
 */
PythonVirtualMachine::~PythonVirtualMachine()
{
  if (m_isInitialized)
  {
    // one last lock.
    PyEval_AcquireLock();

    // restore the thread
    PyThreadState_Swap( _mainThreadState );

    // simply close python
    Py_Finalize();
  }
}

/**
 * Todo
 * @param void
 * @return void
 */
bool PythonVirtualMachine::Initialize()
{
  // has it been done already?
  if (m_isInitialized)
  {
    return true;
  }

  // first initialise the functions.
  if (!InitializeFunctions())
  {
    return false;
  }

  // try and load the python core files.
  STD_TSTRING exe_dir = myodd::files::GetAppPath(true);
  std::wstring python_path;
  python_path += exe_dir + L"python35.zip";
  if (!myodd::files::FileExists(python_path))
  {
    return false;
  }
  Py_SetPath(python_path.c_str());

  // try and initialise it all.
  Py_Initialize();
  PyEval_InitThreads();

  // just double check that it is all fine.
  if (!Py_IsInitialized())
  {
    return false;
  }

  _mainThreadState = PyThreadState_Get();
  PyEval_ReleaseLock();

  // success.
  m_isInitialized = true;

  // done
  return true;
}

/**
* Initialise the python functions.
* @return bool if the functions were properly inisalized or not.
*/
bool PythonVirtualMachine::InitializeFunctions()
{
  return (
    PyImport_AppendInittab(
      "am",
      &PyInit_am
      ) != -1);
}


/**
 * Check if the given extension can be handled by the vm.
 * @param LPCTSTR ext the extension we want to check
 * @return bool if the given extension can be used by this vm
 */
bool PythonVirtualMachine::IsPyExt( LPCTSTR ext )
{
  return ( _tcsicmp( ext, _T("py") ) == 0 );
}

/**
 * Get the current api for this thread.
 * return pyapi& the python API for that thread.
 */
pyapi& PythonVirtualMachine::GetApi()
{
#ifndef ACTIONMONITOR_API_PY
  throw - 1;
#else
  // get our current self.
  PythonVirtualMachine* pvm = App().GetPythonVirtualMachine();

  myodd::threads::Lock guard(pvm->_mutex);
  Apis::const_iterator it = pvm->_apis.find( std::this_thread::get_id() );
  if (it == pvm->_apis.end())
  {
    throw - 1;
  }
  return *it->second;
#endif
}

void PythonVirtualMachine::AddApi(std::thread::id id, pyapi* api)
{
#ifndef ACTIONMONITOR_API_PY
  throw - 1;
#else
  myodd::threads::Lock guard(_mutex);

  // get our current self.
  _apis[std::this_thread::get_id()] = api;
#endif

}

void PythonVirtualMachine::RemoveApi(std::thread::id id )
{
#ifndef ACTIONMONITOR_API_PY
  throw - 1;
#else
  myodd::threads::Lock guard(_mutex);
  Apis::const_iterator it = _apis.find(std::this_thread::get_id());
  if (it == _apis.end())
  {
    throw - 1;
  }
  _apis.erase(it);
#endif
}

/**
 * Todo
 * @see helperapi::say
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* PythonVirtualMachine::say(PyObject *self, PyObject *args)
{
  return GetApi().say(self, args);
}

/**
* Todo
* @see helperapi::version
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::version(PyObject *self, PyObject *args)
{
  return GetApi().version(self, args);
}

/**
* Todo
* @see helperapi::getCommand
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::getCommand(PyObject *self, PyObject *args)
{
  return GetApi().getCommand(self, args);
}

/**
* Todo
* @see helperapi::getAction
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::getAction(PyObject *self, PyObject *args)
{
  return GetApi().getAction(self, args);
}

/**
* Todo
* @see helperapi::getcommandcount
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::getCommandCount(PyObject *self, PyObject *args)
{
  return GetApi().getCommandCount(self, args);
}

/**
* Todo
* @see helperapi::execute
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::execute(PyObject *self, PyObject *args)
{
  return GetApi().execute(self, args);
}

/**
* Todo
* @see helperapi::getstring
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::getstring(PyObject *self, PyObject *args)
{
  return GetApi().getstring(self, args);
}

/**
* Todo
* @see helperapi::getfile
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::getfile(PyObject *self, PyObject *args)
{
  return GetApi().getfile(self, args);
}

/**
* Todo
* @see helperapi::getfolder
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::getfolder(PyObject *self, PyObject *args)
{
  return GetApi().getfolder(self, args);
}

/**
* Todo
* @see helperapi::geturl
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::geturl(PyObject *self, PyObject *args)
{
  return GetApi().geturl(self, args);
}

/**
* Todo
* @see helperapi::addAction
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::addAction(PyObject *self, PyObject *args)
{
  return GetApi().addAction(self, args);
}

/**
* Todo
* @see helperapi::removeAction
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::removeAction(PyObject *self, PyObject *args)
{
  return GetApi().removeAction(self, args);
}

/**
* Get the current Action monitor version number.
* @see helperapi::getVersion
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::getVersion(PyObject *self, PyObject *args)
{
  return GetApi().getVersion(self, args);
}

/**
* Find an action.
* @see helperapi::findAction
* @param PyObject *
* @param PyObject *
* @return PyObject* false or the path of that action.
*/
PyObject* PythonVirtualMachine::findAction(PyObject *self, PyObject *args)
{
  return GetApi().findAction(self, args);
}

#endif  /* ACTIONMONITOR_API_PY */