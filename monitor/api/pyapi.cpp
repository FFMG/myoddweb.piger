#include "StdAfx.h"

#ifdef ACTIONMONITOR_API_PY
#include "pyapi.h"
#include <utility>
#include "ActionMonitor.h"

// this is the version number for that particular API
// 0.1 was the old API, not idea what version of Python it was using.
// 2.0 uses version 3.5 of Python.
// 3.0 added Log( ... )
// 3.1 C++11 code cleanup
static const double ACTIONMONITOR_API_PY_VERSION = 3.1;

/**
 * \copydoc
 */
PyApi::PyApi(const ActiveAction& action, IActions& actions, IMessagesHandler& messagesHandler, std::string script, PyThreadState* mainThreadState) :
  HelperApi(action, actions, messagesHandler),
  _script(std::move(script)),
  _mainThreadState(mainThreadState)
{
}

void PyApi::Say(const std::wstring& what ) const
{
  // ReSharper disable once CppDeclaratorNeverUsed
  const auto _ = __super::Say( what.c_str() , 3000, 5);
}

/**
 * \brief Return a 'false' message and swallow any errors.
 * \return PyObject* a 'false' python object.
 */
PyObject* PyApi::Fail()
{
  //  clear all the errors.
  PyErr_Clear();

  // and return a failure.
  return Py_BuildValue("b", false );
}

/**
 * \brief say a message
 * \see __super::say
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::Say(PyObject *self, PyObject *args) const
{
  try
  {
    char* msg;
    auto slen = 0;
    auto nElapse = 0;
    auto nFadeOut = 0;

    // the last argument is optional
    if (!PyArg_ParseTuple(args, "s#i|i", &msg, &slen, &nElapse, &nFadeOut))
    {
      Say( L"<b>Error : </b> Missing or more values or invalid format.<br><i>am.say( msg, elapse [, fade=0])</i>");

      //  just return false.
      return Fail();
    }

    // display the message
    // and we can now display the message.
    const auto result = __super::Say(HelperApi::Widen(msg).c_str(), nElapse, nFadeOut);

    // return true.
    return Py_BuildValue("b", result);
  }
  catch(...)
  {
    return Fail();
  }
}

/**
 * \brief Get the version number of this
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::Version (PyObject *self, PyObject *args)
{
  // short and sweet
  // all we need is the version number.
  return Py_BuildValue( "d", ACTIONMONITOR_API_PY_VERSION );
}

/**
 * \brief Get a command, by index
 * \see __super::GetCommand
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::GetCommand(PyObject *self, PyObject *args) const
{
  unsigned idx = 0;
  // get the index number
  if (!PyArg_ParseTuple(args, "I", &idx))
  {
    Say(L"<b>Error : </b> Missing index number.<br>Format is <i>am.getCommand( <b>index</b> )</i>");

    //  just return false.
    return Fail();
  }

  MYODD_STRING sValue;
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
 * \brief Get an action, by index
 * \see __super::GetAction
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::GetAction(PyObject *self, PyObject *args) const
{
  MYODD_STRING sValue;
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
 * \brief Get the number of commands.
 * \see __super::GetCommandCount
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::GetCommandCount(PyObject *self, PyObject *args) const
{
  // get it
  const auto nSize = __super::GetCommandCount();

  // and return it.
  return Py_BuildValue("i", nSize);
}

/**
 * \brief Execute a module with arguments.
 * \see __super::GetCommandCount
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::Execute(PyObject *self, PyObject *args) const
{
  try
  {
    char* module = nullptr;
    char* cmdLine = nullptr;
    const auto isPrivileged = 0;
    if (!PyArg_ParseTuple(args, "s|sp", &module, &cmdLine, &isPrivileged))
    {
      Say(L"<b>Error : </b> Missing Module and/or command line.<br>Format is <i>am.execute( module [, commandLine[,isPrivileged=False]])</i>");
      return Fail();
    }

    // run it
    const auto result = __super::Execute(HelperApi::Widen(module).c_str(), HelperApi::Widen(cmdLine).c_str(), (isPrivileged == 1), nullptr);

    // tell the user it did not work
    if (!result)
    {
      Say(L"<b>Error : </b> There was an error executing the request, please check the parameters.");
      return Fail();
    }

    // return the result.
    return Py_BuildValue("b", result);
  }
  catch( ... )
  {
    return Fail();
  }
}

/**
 * \brief Get a string by index,
 * \see __super::GetString
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::GetString(PyObject *self, PyObject *args) const
{
  auto iQuote = 1;
  if (!PyArg_ParseTuple(args, "|p", &iQuote))  
  {
    Say(L"<b>Error : </b> Missing index number.<br>Format is <i>am.getString( [quote=True] )</i>");
    return Fail();
  }

  MYODD_STRING sValue;
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
 * \brief Get a string by index,
 * \see __super::GetFile
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::GetFile(PyObject *self, PyObject *args) const
{
  try
  {
    unsigned idx = 0;
    auto iQuote = 1;
    if (!PyArg_ParseTuple(args, "I|p", &idx, &iQuote))
    {
      Say(L"<b>Error : </b> Missing index number.<br>Format is <i>am.getfile( <b>index</b>[,quote=True] )</i>");
      return Fail();
    }

    MYODD_STRING sValue;
    if (!__super::GetFile(idx, sValue, (iQuote == 1)))
    {
      // return false, nothing was found.
      return Py_BuildValue("b", false);
    }

    USES_CONVERSION;
    // we have a string
    return Py_BuildValue("s", T_T2A(sValue.c_str()));
  }
  catch( ... )
  {
    return Fail();
  }
}

/**
 * \brief Get a string by index,
 * \see __super::GetFolder
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::GetFolder(PyObject *self, PyObject *args) const
{
  try
  {
    unsigned idx = 0;
    auto iQuote = 1;
    if (!PyArg_ParseTuple(args, "I|p", &idx, &iQuote))
    {
      Say(L"<b>Error : </b> Missing index number.<br>Format is <i>am.getfolder( <b>index</b> [,quote=True] )</i>");
      return Fail();
    }

    MYODD_STRING sValue;
    if (!__super::GetFolder(idx, sValue, (iQuote == 1)))
    {
      // return false, nothing was found.
      return Py_BuildValue("b", false);
    }

    USES_CONVERSION;
    // we have a string
    return Py_BuildValue("s", T_T2A(sValue.c_str()));
  }
  catch( ... )
  {
    return Fail();
  }
}

/**
 * \brief Get the URL
 * \see __super::GetUrl
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::GetUrl(PyObject *self, PyObject *args) const
{
  try
  {
    unsigned idx = 0;
    auto iQuote = 1;
    if (!PyArg_ParseTuple(args, "I|p", &idx, &iQuote))
    {
      Say(L"<b>Error : </b> Missing index number.<br>Format is <i>am.geturl( index [,quote=True] )</i>");
      return Fail();
    }

    MYODD_STRING sValue;
    if (!__super::GetUrl(idx, sValue, (iQuote == 1)))
    {
      // return false, nothing was found.
      return Py_BuildValue("b", false);
    }

    USES_CONVERSION;
    // we have a string
    return Py_BuildValue("s", T_T2A(sValue.c_str()));
  }
  catch( ... )
  {
    return Fail();
  }
}

/**
 * \brief Get a string by index,
 * \see __super::AddAction
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::AddAction(PyObject *self, PyObject *args) const
{
  try
  {
    char* szText = nullptr;
    char* szPath = nullptr;

    if (!PyArg_ParseTuple(args, "ss", &szText, &szPath))
    {
      Say(L"<b>Error : </b> Missing values.<br>Format is <i>am.addAction( <b>action</b>, <b>path</b> )</i>");
      return Fail();
    }

    // run it
    const auto result = __super::AddAction(HelperApi::Widen(szText).c_str(), HelperApi::Widen(szPath).c_str());
    return Py_BuildValue("b", result);
  }
  catch( ... )
  {
    return Fail();
  }
}

/**
 * \brief Get a string by index,
 * \see __super::Remove an action
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::RemoveAction(PyObject *self, PyObject *args) const
{
  try
  {
    char* szText = nullptr;
    char* szPath = nullptr;

    if (!PyArg_ParseTuple(args, "ss", &szText, &szPath))
    {
      Say(L"<b>Error : </b> Missing values.<br>Format is <i>am.removeAction( <b>action</b>, <b>path</b> )</i>");
      return Fail();
    }

    // run it
    const auto result = __super::RemoveAction(HelperApi::Widen(szText).c_str(), HelperApi::Widen(szPath).c_str());
    return Py_BuildValue("b", result);
  }
  catch( ... )
  {
    return Fail();
  }
}

/**
 * \brief Log a message
 * \see __super::Remove an action
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::Log(PyObject *self, PyObject *args) const
{
  try
  {
    unsigned int logType = 0;
    char* szText = nullptr;

    if (!PyArg_ParseTuple(args, "Is", &logType, &szText))
    {
      Say(L"<b>Error : </b> Missing values.<br>Format is <i>am_Log( <b>logType</b>, <b>string</b> )</i>");
      return Fail();
    }

    // run it
    const auto action = myodd::strings::String2WString(szText);
    __super::Log(logType, action.c_str());
    return Py_BuildValue("b", true);
  }
  catch( ... )
  {
    return Fail();
  }
}

/**
 * \brief Get the application version number.
 * \see __super::Remove an action
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::GetVersion(PyObject *self, PyObject *args) const
{
  try
  {
    MYODD_STRING sValue = _T("");
    if (!__super::GetVersion(sValue))
    {
      // we have nothing
      return Py_BuildValue("b", false);
    }

    USES_CONVERSION;
    // we have a string
    return Py_BuildValue("s", T_T2A(sValue.c_str()));
  }
  catch( ... )
  {
    return Fail();
  }
}

/**
 * \brief Find an acton... by index/
 * \see __super::Remove an action
 * \param self the object
 * \param args the given arguments
 * \return PyObject* the result of the opreation
 */
PyObject* PyApi::FindAction(PyObject *self, PyObject *args) const
{
  try
  {
    unsigned idx = 0;
    char* szText = nullptr;
    if (!PyArg_ParseTuple(args, "Is", &idx, &szText))
    {
      Say(L"<b>Error : </b> Missing index number.<br>Format is <i>am.findAction( <b>index</b>, <b>string</b> )</i>");
      return Fail();
    }

    MYODD_STRING sValue = _T("");
    if (!__super::FindAction(idx, HelperApi::Widen(szText).c_str(), sValue))
    {
      // we have nothing
      return Py_BuildValue("b", false);
    }

    USES_CONVERSION;
    // we have a string
    return Py_BuildValue("s", T_T2A(sValue.c_str()));
  }
  catch( ... )
  {
    return Fail();
  }
}

/**
 * Execute the current script
 * We are now in the thread.
 */
void PyApi::ExecuteInThread() const
{
  assert(Py_IsInitialized() );
  
  // get the lock so we can change things.
  PyEval_AcquireLock();

  // make sure that the main thread is the active one.
  const auto  mainInterpreterState = _mainThreadState->interp;
  PyThreadState_Swap(_mainThreadState);

  // create a new thread.
  const auto  myThreadState =  PyThreadState_New(mainInterpreterState);

  // make sure that the new thread has control
  // https://docs.python.org/3/c-api/init.html
  PyThreadState_Swap(myThreadState);

  //  execute it...
  {
    const auto main_module = PyImport_AddModule("__main__");
    const auto main_dict = PyModule_GetDict(main_module);
    Py_XINCREF(main_module);
 
    const auto local_dic = PyDict_New();
    Py_XINCREF(local_dic);

    // we can now run our script
    const auto s = _script.c_str();
    const auto pyRes = PyRun_String(s, Py_file_input, main_dict, local_dic);

    CheckForPythonErrors();

    PyDict_Clear(local_dic);
    Py_XDECREF(local_dic);

    // pending calls must be cleared out
    Py_XDECREF(main_module);
  }

  // swap back to this thread.
  PyThreadState_Swap(myThreadState);

  // clear anything left behind.
  PyThreadState_Clear(myThreadState);

  PyThreadState_Swap(nullptr);

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
void PyApi::CheckForPythonErrors() const
{
  const auto  ex = PyErr_Occurred();
  if (nullptr != ex)
  {
    //  if this is a normal exist, then we don't need to show an error message.
    if (!PyErr_ExceptionMatches(PyExc_SystemExit))
    {
      PyObject *type, *value, *traceback;
      PyErr_Fetch(&type, &value, &traceback);
      PyErr_Clear();

      std::string message = "<b>Error : </b>An error was raised in the PyAPI.";
      if (type) {
        const auto temp_bytes = PyUnicode_AsEncodedString(type, "ASCII", "strict");
        if (temp_bytes != nullptr) {
          message += "<br>";
          message += PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
          Py_DECREF(temp_bytes);
        }
      }
      if (value) {
        const auto temp_bytes = PyUnicode_AsEncodedString(value, "ASCII", "strict");
        if (temp_bytes != nullptr) {
          message += "<br>";
          message += PyBytes_AS_STRING(temp_bytes); // Borrowed pointer
          Py_DECREF(temp_bytes);
        }
      }
      if (traceback) {
        const auto temp_bytes = PyUnicode_AsEncodedString(traceback, "ASCII", "strict");
        if (temp_bytes != nullptr) {
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