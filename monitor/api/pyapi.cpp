#include "StdAfx.h"

#ifdef ACTIONMONITOR_API_PY
#include "pyapi.h"
#include "ActionMonitor.h"

/**
 * Todo
 * @param void
 * @return void
 */
pyapi::pyapi(const ActiveAction& action, const std::string& script, PyThreadState* mainThreadState) :
  helperapi(action),
  _script( script ),
  _mainThreadState(mainThreadState)
{
}

/**
 * Todo
 * @param void
 * @return void
 */
pyapi::~pyapi()
{
}

/**
 * Return a 'false' message and swallow any errors.
 * @return PyObject* a 'false' python object.
 */
PyObject* pyapi::Fail()
{
  //  clear all the errors.
  PyErr_Clear();

  // and return a failure.
  return Py_BuildValue("b", false );
}

/**
 * Todo
 * @see __super::say
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::Say(PyObject *self, PyObject *args)
{
  CHAR* msg;
  int slen;
  int nElapse = 0;
  int nFadeOut = 0;

  // the last argument is optional
  if (!PyArg_ParseTuple(args, "s#i|i", &msg, &slen, &nElapse, &nFadeOut))
  {
    __super::Say( _T("<b>Error : </b> Missing or more values or invalid format.<br><i>am.say( msg, elapse [, fade=0])</i>"), 3000, 5 );
    
    //  just return false.
    return Fail();
  }

  // display the message
  // and we can now display the message.
  bool result = __super::Say(helperapi::widen( msg ).c_str(), nElapse, nFadeOut );
  
  // return true.
  return Py_BuildValue("b", result );
}

/**
 * Todo
 * @see __super::version
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::Version (PyObject *self, PyObject *args)
{
  // short and sweet
  // all we need is the version number.
  return Py_BuildValue( "d", ACTIONMONITOR_API_PY_VERSION );
}

/**
 * Todo
 * @see __super::getCommand
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::GetCommand(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  // get the index number
  if (!PyArg_ParseTuple(args, "I", &idx))
  {
    __super::Say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.getCommand( <b>index</b> )</i>"), 3000, 5 );

    //  just return false.
    return Fail();
  }

  STD_TSTRING sValue;
  if( !__super::GetCommand( idx, sValue ) )
  {
    //  just return false.
    return Fail();
  }

  USES_CONVERSION;
  // or return the value
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}

/**
 * Todo
 * @see __super::getAction
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::GetAction(PyObject *self, PyObject *args)
{
  STD_TSTRING sValue;
  if( !__super::GetAction( sValue ) )
  {
    //  just return false.
    return Fail();
  }

  USES_CONVERSION;
  // or return the value
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}

/**
 * Todo
 * @see __super::getcommandcount
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::GetCommandCount(PyObject *self, PyObject *args)
{
  // get it
  size_t nSize = __super::GetCommandCount();

  // and return it.
  return Py_BuildValue("i", nSize);
}

/**
 * Todo
 * @see __super::execute
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::Execute(PyObject *self, PyObject *args)
{
  CHAR* module = NULL;
  CHAR* cmdLine = NULL;
  int isPrivileged = 0;
  if (!PyArg_ParseTuple(args, "s|sp", &module, &cmdLine, &isPrivileged))
  {
    __super::Say( _T("<b>Error : </b> Missing Module and/or command line.<br>Format is <i>am.execute( module [, commandLine[,isPrivileged=False]])</i>"), 3000, 5 );
    return Fail();
  }

  // run it
  bool result = __super::Execute(helperapi::widen( module ).c_str(), helperapi::widen( cmdLine ).c_str(), (isPrivileged==1) );

  // return the result.
  return Py_BuildValue("b", result );
}

/**
 * Todo
 * @see __super::getstring
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::Getstring(PyObject *self, PyObject *args)
{
  int iQuote = 1;
  if (!PyArg_ParseTuple(args, "|p", &iQuote))  
  {
    __super::Say(_T("<b>Error : </b> Missing index number.<br>Format is <i>am.getString( [quote=True] )</i>"), 3000, 5);
    return Fail();
  }

  STD_TSTRING sValue = _T("");
  if( !__super::GetString( sValue, iQuote == 1) )
  {
    // we have nothing
    return Py_BuildValue("b", false );
  }

  USES_CONVERSION;
  // we have a string
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}

/**
 * Todo
 * @see __super::getfile
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::Getfile(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  int iQuote = 1;
  if (!PyArg_ParseTuple(args, "I|p", &idx, &iQuote ))
  {
    __super::Say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.getfile( <b>index</b>[,quote=True] )</i>"), 3000, 5 );
    return Fail();
  }
  
  STD_TSTRING sValue = _T("");
  if( !__super::GetFile( idx, sValue, (iQuote == 1) ) )
  {
    // return false, nothing was found.
    return Py_BuildValue("b", false );
  }

  USES_CONVERSION;
  // we have a string
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}

/**
 * Todo
 * @see __super::getfolder
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::Getfolder(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  int iQuote = 1;
  if (!PyArg_ParseTuple(args, "I|p", &idx, &iQuote ))
  {
    __super::Say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.getfolder( <b>index</b> [,quote=True] )</i>"), 3000, 5 );
    return Fail();
  }

  STD_TSTRING sValue = _T("");
  if( !__super::GetFolder( idx, sValue, (iQuote == 1) ) )
  {
    // return false, nothing was found.
    return Py_BuildValue("b", false );
  }

  USES_CONVERSION;
  // we have a string
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}

/**
 * Todo
 * @see __super::geturl
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::Geturl(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  int iQuote = 1;
  if (!PyArg_ParseTuple(args, "I|p", &idx, &iQuote ))
  {
    __super::Say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.geturl( index [,quote=True] )</i>"), 3000, 5 );
    return Fail();
  }

  STD_TSTRING sValue = _T("");
  if( !__super::GetURL( idx, sValue, (iQuote == 1)) )
  {
    // return false, nothing was found.
    return Py_BuildValue("b", false );
  }

  USES_CONVERSION;
  // we have a string
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}

/**
 * Todo
 * @see __super::addAction
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::AddAction(PyObject *self, PyObject *args)
{
  CHAR* szText = NULL;
  CHAR* szPath = NULL;
  
  if (!PyArg_ParseTuple(args, "ss", &szText, &szPath ))
  {
    __super::Say( _T("<b>Error : </b> Missing values.<br>Format is <i>am.addAction( <b>action</b>, <b>path</b> )</i>"), 3000, 5 );
    return Fail();
  }

  // run it
  bool result = __super::AddAction(helperapi::widen( szText ).c_str(), helperapi::widen( szPath ).c_str() );
  return Py_BuildValue("b", result );
}

/**
 * Todo
 * @see __super::RemoveAction
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::RemoveAction(PyObject *self, PyObject *args)
{
  CHAR* szText = NULL;
  CHAR* szPath = NULL;
  
  if (!PyArg_ParseTuple(args, "ss", &szText, &szPath ))
  {
    __super::Say( _T("<b>Error : </b> Missing values.<br>Format is <i>am.removeAction( <b>action</b>, <b>path</b> )</i>"), 3000, 5 );
    return Fail();
  }

  // run it
  bool result = __super::RemoveAction(helperapi::widen(szText).c_str(), helperapi::widen(szPath).c_str() );
  return Py_BuildValue("b", result );
}

/**
 * Get the current Action monitor version number.
 * @see __super::getVersion
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::GetVersion(PyObject *self, PyObject *args)
{
  STD_TSTRING sValue = _T("");
  if( !__super::GetVersion( sValue ) )
  {
    // we have nothing
    return Py_BuildValue("b", false);
  }

  USES_CONVERSION;
  // we have a string
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}

/**
 * Find an action.
 * @see __super::findAction
 * @param PyObject *
 * @param PyObject *
 * @return PyObject* false or the path of that action.
 */
PyObject* pyapi::FindAction(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  CHAR* szText = NULL;
  if (!PyArg_ParseTuple(args, "Is", &idx, &szText ))
  {
    __super::Say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.findAction( <b>index</b>, <b>string</b> )</i>"), 3000, 5 );
    return Fail();
  }

  STD_TSTRING sValue = _T("");
  if( !__super::FindAction( idx, helperapi::widen( szText ).c_str(), sValue ) )
  {
    // we have nothing
    return Py_BuildValue("b", false);
  }

  USES_CONVERSION;
  // we have a string
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}

/**
 * Execute the current script
 * We are now in the thread.
 */
void pyapi::ExecuteInThread()
{
  assert(Py_IsInitialized() );
  
  // get the lock so we can change things.
  PyEval_AcquireLock();

  // make sure that the main thread is the active one.
  PyInterpreterState* mainInterpreterState = _mainThreadState->interp;
  PyThreadState_Swap(_mainThreadState);

  // create a new thread.
  PyThreadState * myThreadState =  PyThreadState_New(mainInterpreterState);

  // make sure that the new thread has control
  // https://docs.python.org/3/c-api/init.html
  PyThreadState_Swap(myThreadState);

  //  execute it...
  {
    PyObject *main_module = PyImport_AddModule("__main__");
    PyObject *main_dict = PyModule_GetDict(main_module);

    // we can now run our script
    const char* s = _script.c_str();
    PyObject * PyRes = PyRun_String(s, Py_file_input, main_dict, main_dict);

    CheckForPythonErrors();
    // pending calls must be cleared out
  }

  // swap back to this thread.
  PyThreadState_Swap(myThreadState);

  // clear anything left behind.
  PyThreadState_Clear(myThreadState);

  PyThreadState_Swap(NULL);

  // delete my thread.
  PyThreadState_Delete(myThreadState);

  //  give control back to main thread
  PyThreadState_Swap(_mainThreadState);

  // release the lock one last time.
  PyEval_ReleaseLock();
}

/**
 * Check for errors and log a message.
 */
void pyapi::CheckForPythonErrors()
{
  PyObject* ex = PyErr_Occurred();
  if (NULL != ex)
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

      // give the error message
      USES_CONVERSION;
      const wchar_t* msg = T_A2T(message.c_str());
      const unsigned int nElapse = 500;
      const unsigned int nFadeOut = 10;
      __super::Say(msg, nElapse, nFadeOut);
    }
  }

  // no more errors.
  PyErr_Clear();
}
#endif /*ACTIONMONITOR_API_PY*/