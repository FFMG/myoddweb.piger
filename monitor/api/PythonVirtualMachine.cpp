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
  _api( NULL )
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
    // simply close python
    Py_Finalize();
  }
  delete _api;
}

/**
 * Todo
 * @param void
 * @return void
 */
void PythonVirtualMachine::Initialize()
{
  if (m_isInitialized)
  {
    return;
  }

  // first initialise the functions.
  InitializeFunctions();

  STD_TSTRING exe_dir = myodd::files::GetAppPath(true);
  std::wstring python_path;
  python_path += exe_dir + L"python35.zip";
  Py_SetPath(python_path.c_str());

  Py_Initialize();

  m_isInitialized = true;
}

/**
* Todo
* @param void
* @return void
*/
void PythonVirtualMachine::InitializeFunctions()
{
  PyImport_AppendInittab(
    "am",
    &PyInit_am
    );
}


/**
 * Todo
 * @param void
 * @return void
 */
bool PythonVirtualMachine::IsPyExt( LPCTSTR ext )
{
  return ( _tcsicmp( ext, _T("py") ) == 0 );
}

/**
 * Todo
 * @param void
 * @return void
 */
int PythonVirtualMachine::LoadFile( LPCTSTR pyFile, ActiveAction* action )
{
  Initialize();

  if( !m_isInitialized )
  {
    return -1;
  }

  delete _api;
  _api = new pyapi( action );

  USES_CONVERSION;
  // Python is not thread safe 
  // and windows cannot lock the file properly
  // so we need to read the file ourselves and pass it.
  //
  // this could be a memory problem at some stage.
  errno_t err;
  FILE *fp;
  if( err = fopen_s(&fp, T_T2A( pyFile ), "rt" ) )
  {
    return -1;
  }

  //
  // Note that we are no longer in the realm of UNICODE here.
  // We are using Multi Byte data.
  static const UINT FILE_READ_SIZE = 100;
  size_t  count, total = 0;
  std::string script = "";
  while( !feof( fp ) )
  {
    // Attempt to read
    char buffer[ FILE_READ_SIZE + 1];
    memset( buffer, '\0', FILE_READ_SIZE+1 );
    count = fread( buffer, sizeof( char ), FILE_READ_SIZE, fp );

    buffer[ count ] = '\0';

    // was there a problem?
    if( ferror( fp ) ) 
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

  PyObject *main_module = PyImport_AddModule("__main__");
  PyObject *main_dict = PyModule_GetDict(main_module);
  
  // we can now run our script
  LPCSTR s = script.c_str();
  PyObject * PyRes = PyRun_String(s, Py_file_input, main_dict, main_dict);
  PyObject* ex = PyErr_Occurred();
  if( NULL != ex)
  {
    //  if this is a normal exist, then we don't need to show an error message.
    if (!PyErr_ExceptionMatches(PyExc_SystemExit)) 
    {
      PyObject *type, *value, *traceback;
      PyErr_Fetch(&type, &value, &traceback);
      PyErr_Clear();

      std::string message = "<b>Error : </b>An error was raised in the PyAPI.";
      if (type) {
        PyObject * temp_bytes = PyUnicode_AsEncodedString(type, "ASCII", "strict");
        if (temp_bytes != NULL) {
          message += "<br>";
          message += PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
          Py_DECREF(temp_bytes);
        }
      }
      if (value) {
        PyObject * temp_bytes = PyUnicode_AsEncodedString(value, "ASCII", "strict");
        if (temp_bytes != NULL) {
          message += "<br>";
          message += PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
          Py_DECREF(temp_bytes);
        }
      }
      if (traceback) {
        PyObject * temp_bytes = PyUnicode_AsEncodedString(traceback, "ASCII", "strict");
        if (temp_bytes != NULL) {
          message += "<br>";
          message += PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
          Py_DECREF(temp_bytes);
        }
      }
      Py_XDECREF(type);
      Py_XDECREF(value);
      Py_XDECREF(traceback);

      USES_CONVERSION;
      helperapi h(action);
      h.say(T_A2T( message.c_str() ), 500, 10);
    }
  }

  // no more errors.
  PyErr_Clear();

  /*
  (void) PyRun_SimpleFile(fp, pyFile );
  */
  
  return 0;
}

pyapi& PythonVirtualMachine::GetApi()
{
#ifndef ACTIONMONITOR_API_PY
  throw - 1;
#else
  // get our current self.
  PythonVirtualMachine* pvm = App().GetPythonVirtualMachine();
  return *pvm->_api;
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