#include "stdafx.h"
#ifdef ACTIONMONITOR_API_PY

#include "helperapi.h"
#include "PythonVirtualMachine.h"
#include "files\files.h"
#include "ActionMonitor.h"

static PyMethodDef amMethods[] = {
  { "say", PythonVirtualMachine::Say, METH_VARARGS, "Display a message on the screen." },
  { "version", PythonVirtualMachine::Version, METH_VARARGS, "Get this API version number." },
  { "getCommand", PythonVirtualMachine::GetCommand, METH_VARARGS, "Get a certain command, return false if it does not exist." },
  { "getAction", PythonVirtualMachine::GetAction, METH_VARARGS, "Get the action entered by the user." },
  { "getCommandCount", PythonVirtualMachine::GetCommandCount, METH_VARARGS, "Get the number of commands." },
  { "execute", PythonVirtualMachine::Execute, METH_VARARGS, "Execute a command, (app, command)." },
  { "getString", PythonVirtualMachine::Getstring, METH_VARARGS, "Get the currently selected text if any." },
  { "getFile", PythonVirtualMachine::Getfile, METH_VARARGS, "Get a file by index, return false if not found." },
  { "getFolder", PythonVirtualMachine::Getfolder, METH_VARARGS, "Get a folder by index, return false if not found." },
  { "getUrl", PythonVirtualMachine::Geturl, METH_VARARGS, "Get a URL by index, return false if not found." },
  { "addAction", PythonVirtualMachine::AddAction, METH_VARARGS, "Add an action and path to the list of actions." },
  { "removeAction", PythonVirtualMachine::RemoveAction, METH_VARARGS, "Remove an action from the list." },
  { "getVersion", PythonVirtualMachine::GetVersion, METH_VARARGS, "Get the full version string." },
  { "findAction", PythonVirtualMachine::FindAction, METH_VARARGS, "Find an action given an index, return false or the path." },
  { "log", PythonVirtualMachine::Log, METH_VARARGS, "Log messages to file." },
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

    if (PyGILState_Check()) {
      /* I have the GIL */
      // 
      PyEval_ReleaseLock();
    }

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

  bool embedded = (bool)myodd::config::get(_T("python\\useembedded"), true);
  if (embedded)
  {
    // try and load the python core files.
    MYODD_STRING exe_dir = myodd::files::GetAppPath(true);
    std::wstring python_path;
    myodd::files::Join(python_path, exe_dir, L"python\\python35.zip");
    Py_SetPath(python_path.c_str());
  }

  // try and initialise it all.
  Py_Initialize();
  if (embedded)
  {
    MYODD_STRING exe_dir = myodd::files::GetAppPath(true);
    std::wstring python_path1;
    myodd::files::Join(python_path1, exe_dir, L"python\\");

    std::wstring python_path2;
    myodd::files::Join(python_path2, exe_dir, L"python\\python35.zip");

    // join the two together
    std::wstring python_path3 = python_path1 + L";" + python_path2;
    PySys_SetPath(python_path3.c_str());
  }
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
bool PythonVirtualMachine::IsExt( LPCTSTR ext )
{
  return ( _tcsicmp( ext, _T("py") ) == 0 );
}

/**
 * Get the current api for this thread.
 * return PyApi& the python API for that thread.
 */
PyApi& PythonVirtualMachine::GetApi()
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

void PythonVirtualMachine::AddApi(std::thread::id id, PyApi* api)
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
PyObject* PythonVirtualMachine::Say(PyObject *self, PyObject *args)
{
  return GetApi().Say(self, args);
}

/**
* Todo
* @see helperapi::version
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::Version(PyObject *self, PyObject *args)
{
  return GetApi().Version(self, args);
}

/**
* Todo
* @see helperapi::getCommand
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::GetCommand(PyObject *self, PyObject *args)
{
  return GetApi().GetCommand(self, args);
}

/**
* Todo
* @see helperapi::getAction
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::GetAction(PyObject *self, PyObject *args)
{
  return GetApi().GetAction(self, args);
}

/**
* Todo
* @see helperapi::getcommandcount
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::GetCommandCount(PyObject *self, PyObject *args)
{
  return GetApi().GetCommandCount(self, args);
}

/**
* Todo
* @see helperapi::execute
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::Execute(PyObject *self, PyObject *args)
{
  return GetApi().Execute(self, args);
}

/**
* Todo
* @see helperapi::getstring
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::Getstring(PyObject *self, PyObject *args)
{
  return GetApi().Getstring(self, args);
}

/**
* Todo
* @see helperapi::getfile
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::Getfile(PyObject *self, PyObject *args)
{
  return GetApi().Getfile(self, args);
}

/**
* Todo
* @see helperapi::getfolder
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::Getfolder(PyObject *self, PyObject *args)
{
  return GetApi().Getfolder(self, args);
}

/**
* Todo
* @see helperapi::geturl
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::Geturl(PyObject *self, PyObject *args)
{
  return GetApi().Geturl(self, args);
}

/**
* Todo
* @see helperapi::addAction
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::AddAction(PyObject *self, PyObject *args)
{
  return GetApi().AddAction(self, args);
}

/**
* Todo
* @see helperapi::RemoveAction
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::RemoveAction(PyObject *self, PyObject *args)
{
  return GetApi().RemoveAction(self, args);
}

/**
* Get the current Action monitor version number.
* @see helperapi::getVersion
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::GetVersion(PyObject *self, PyObject *args)
{
  return GetApi().GetVersion(self, args);
}

/**
 * Find an action.
 * @see helperapi::findAction
 * @param PyObject *
 * @param PyObject *
 * @return PyObject* false or the path of that action.
 */
PyObject* PythonVirtualMachine::FindAction(PyObject *self, PyObject *args)
{
  return GetApi().FindAction(self, args);
}

/**
 * Log a message.
 * @see helperapi::Log
 * @param PyObject *
 * @param PyObject *
 * @return PyObject* false or the path of that action.
 */
PyObject* PythonVirtualMachine::Log(PyObject *self, PyObject *args)
{
  return GetApi().Log(self, args);
}

#endif  /* ACTIONMONITOR_API_PY */