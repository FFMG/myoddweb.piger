#include "stdafx.h"
#ifdef ACTIONMONITOR_API_PY

#include "PythonVirtualMachine.h"
#include "files/files.h"
#include "ActionMonitor.h"

static PyMethodDef amMethods[] = {
  { "say", PythonVirtualMachine::Say, METH_VARARGS, "Display a message on the screen." },
  { "version", PythonVirtualMachine::Version, METH_VARARGS, "Get this API version number." },
  { "getCommand", PythonVirtualMachine::GetCommand, METH_VARARGS, "Get a certain command, return false if it does not exist." },
  { "getAction", PythonVirtualMachine::GetAction, METH_VARARGS, "Get the action entered by the user." },
  { "getCommandCount", PythonVirtualMachine::GetCommandCount, METH_VARARGS, "Get the number of commands." },
  { "execute", PythonVirtualMachine::ExecuteInScript, METH_VARARGS, "Execute a command, (app, command)." },
  { "getString", PythonVirtualMachine::GetString, METH_VARARGS, "Get the currently selected text if any." },
  { "getFile", PythonVirtualMachine::GetFile, METH_VARARGS, "Get a file by index, return false if not found." },
  { "getFolder", PythonVirtualMachine::GetFolder, METH_VARARGS, "Get a folder by index, return false if not found." },
  { "getUrl", PythonVirtualMachine::GetUrl, METH_VARARGS, "Get a URL by index, return false if not found." },
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

PyMODINIT_FUNC PyInit_am(void)
{
  return PyModule_Create(&amModule );
}

/**
 * \copydoc
 */
PythonVirtualMachine::PythonVirtualMachine(IActions& actions, IMessagesHandler& messagesHandler, IIpcListener& ipcListener) :
  IVirtualMachine(actions, messagesHandler, ipcListener ),
  m_isInitialized( false ),
  _mainThreadState( nullptr )
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

  bool embedded = ::myodd::config::Get( L"python\\useembedded", true);
  if (embedded)
  {
    // try and load the python core files.
    MYODD_STRING exe_dir = myodd::files::GetAppPath(true);
    std::wstring python_path;
    myodd::files::Join(python_path, exe_dir, L"python\\python37.zip");
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
    myodd::files::Join(python_path2, exe_dir, L"python\\python37.zip");

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
 * Read the given file and get the script out of it.
 * \param pyFile the python file we want to read.
 * \param script the string that will contain the string.
 * \return boolean success or not.
 */
bool PythonVirtualMachine::ReadFile(const std::wstring& pyFile, std::string& script) const
{
  // clear the scruot.
  script = "";

  errno_t err;
  FILE *fp;
  const auto asciiString = myodd::strings::WString2String(pyFile);
  if (err = fopen_s(&fp, asciiString.c_str(), "rt"))
  {
    return false;
  }

  //
  // Note that we are no longer in the realm of UNICODE here.
  // We are using Multi Byte data.
  static const UINT FILE_READ_SIZE = 100;
  size_t  count, total = 0;
  while (!feof(fp))
  {
    // Attempt to read
    char buffer[FILE_READ_SIZE + 1];
    memset(buffer, '\0', FILE_READ_SIZE + 1);
    count = fread(buffer, sizeof(char), FILE_READ_SIZE, fp);

    buffer[count] = '\0';

    // was there a problem?
    if (ferror(fp))
    {
      break;
    }

    // add it to the script
    script += buffer;

    // Total up actual bytes read
    total += count;
  }

  // we are done with the file.
  fclose(fp);

  // success.
  return true;
}

int PythonVirtualMachine::Execute(const ActiveAction& action, const std::wstring& pluginFile)
{
  // Python is not thread safe 
  // and windows cannot lock the file properly
  // so we need to read the file ourselves and pass it.
  //
  // this could be a memory problem at some stage.
  //
  std::string script = "";
  if (!ReadFile(pluginFile, script))
  {
    return -1;
  }

  const auto api = new PyApi(action, GetActions(), GetMessagesHandler(), script, GetMainPyThread());  //  save it.
  const auto id = std::this_thread::get_id();
  AddApi(id, api);

  auto result = -1;

  try
  {
    // we can now execute the thread.
    api->ExecuteInThread();
    result = 0;
  }
  catch ( ... )
  {
    result = -1;
  }

  // and remove it when we are done.
  RemoveApi(id);
  delete api;
  return 0;
}

/**
 * Check if a given file extension is used by this API or not.
 * @param const MYODD_STRING& file the file we are checking
 * @return bool true|false if the given extension is LUA or not.
 */
bool PythonVirtualMachine::IsExt(const MYODD_STRING& file )
{
  return myodd::files::IsExtension(file, _T("py"));
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
  auto& pvm = static_cast<PythonVirtualMachine&>(App().VirtualMachinesHandler().Get<PythonVirtualMachine>());

  myodd::threads::Lock guard(pvm._mutex);
  const auto it = pvm._apis.find( std::this_thread::get_id() );
  if (it == pvm._apis.end())
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
PyObject* PythonVirtualMachine::ExecuteInScript(PyObject *self, PyObject *args)
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
PyObject* PythonVirtualMachine::GetString(PyObject *self, PyObject *args)
{
  return GetApi().GetString(self, args);
}

/**
* Todo
* @see helperapi::getfile
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::GetFile(PyObject *self, PyObject *args)
{
  return GetApi().GetFile(self, args);
}

/**
* Todo
* @see helperapi::getfolder
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::GetFolder(PyObject *self, PyObject *args)
{
  return GetApi().GetFolder(self, args);
}

/**
* Todo
* @see helperapi::geturl
* @param PyObject *
* @param PyObject *
* @return PyObject*
*/
PyObject* PythonVirtualMachine::GetUrl(PyObject *self, PyObject *args)
{
  return GetApi().GetUrl(self, args);
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