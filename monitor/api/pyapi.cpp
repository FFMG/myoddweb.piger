#include "StdAfx.h"

#ifdef ACTIONMONITOR_API_PY
#include "pyapi.h"

/**
 * Todo
 * @param void
 * @return void
 */
pyapi::pyapi(ActiveAction* action) : helperapi(action)
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
 * Todo
 * @see __super::say
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::say(PyObject *self, PyObject *args)
{
  CHAR* msg;
  int slen;
  int nElapse = 0;
  int nFadeOut = 0;

  // the last argument is optional
  if (!PyArg_ParseTuple(args, "s#i|i", &msg, &slen, &nElapse, &nFadeOut))
  {
    __super::say( _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>am.say( msg, <b>elapse</b>, fade=0)</i>"), 3000, 5 );
    
    //  just return false.
    return Py_BuildValue("b", 0 );
  }

  // display the message
  // and we can now display the message.
  bool result = __super::say(helperapi::widen( msg ).c_str(), nElapse, nFadeOut );
  
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
PyObject* pyapi::version (PyObject *self, PyObject *args)
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
PyObject* pyapi::getCommand(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  // get the index number
  if (!PyArg_ParseTuple(args, "I", &idx))
  {
    __super::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.getCommand( <b>index</b> )</i>"), 3000, 5 );

    //  just return false.
    return Py_BuildValue("b", 0 );
  }

  STD_TSTRING sValue;
  if( !__super::getCommand( idx, sValue ) )
  {
    //  just return false.
    return Py_BuildValue("b", 0 );
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
PyObject* pyapi::getAction(PyObject *self, PyObject *args)
{
  STD_TSTRING sValue;
  if( !__super::getAction( sValue ) )
  {
    //  just return false.
    return Py_BuildValue("b", 0 );
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
PyObject* pyapi::getCommandCount(PyObject *self, PyObject *args)
{
  // get it
  size_t nSize = __super::getCommandCount();

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
PyObject* pyapi::execute(PyObject *self, PyObject *args)
{
  CHAR* module = NULL;
  CHAR* cmdLine = NULL;
  bool isPrivileged;
  if (!PyArg_ParseTuple(args, "s|sp", &module, &cmdLine, &isPrivileged))
  {
    __super::say( _T("<b>Error : </b> Missing Module and/or command line.<br>Format is <i>am.execute( module [, commandLine[,isPrivileged]])</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  // run it
  bool result = __super::execute(helperapi::widen( module ).c_str(), helperapi::widen( cmdLine ).c_str(), isPrivileged );

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
PyObject* pyapi::getstring(PyObject *self, PyObject *args)
{
  STD_TSTRING sValue = _T("");
  if( !__super::getString( sValue ) )
  {
    // we have nothing
    return Py_BuildValue("b", false);
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
PyObject* pyapi::getfile(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  if (!PyArg_ParseTuple(args, "I", &idx ))
  {
    __super::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.getfile( <b>index</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  STD_TSTRING sValue = _T("");
  if( !__super::getFile( idx, sValue ) )
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
PyObject* pyapi::getfolder(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  if (!PyArg_ParseTuple(args, "I", &idx ))
  {
    __super::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.getfolder( <b>index</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  STD_TSTRING sValue = _T("");
  if( !__super::getFolder( idx, sValue ) )
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
PyObject* pyapi::geturl(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  if (!PyArg_ParseTuple(args, "I", &idx ))
  {
    __super::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.geturl( <b>index</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  STD_TSTRING sValue = _T("");
  if( !__super::getURL( idx, sValue ) )
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
PyObject* pyapi::addAction(PyObject *self, PyObject *args)
{
  CHAR* szText = NULL;
  CHAR* szPath = NULL;
  
  if (!PyArg_ParseTuple(args, "ss", &szText, &szPath ))
  {
    __super::say( _T("<b>Error : </b> Missing values.<br>Format is <i>am.addAction( <b>action</b>, <b>path</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  // run it
  bool result = __super::addAction(helperapi::widen( szText ).c_str(), helperapi::widen( szPath ).c_str() );
  return Py_BuildValue("b", result );
}

/**
 * Todo
 * @see __super::removeAction
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::removeAction(PyObject *self, PyObject *args)
{
  CHAR* szText = NULL;
  CHAR* szPath = NULL;
  
  if (!PyArg_ParseTuple(args, "ss", &szText, &szPath ))
  {
    __super::say( _T("<b>Error : </b> Missing values.<br>Format is <i>am.removeAction( <b>action</b>, <b>path</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  // run it
  bool result = __super::removeAction(helperapi::widen(szText).c_str(), helperapi::widen(szPath).c_str() );
  return Py_BuildValue("b", result );
}

/**
 * Get the current Action monitor version number.
 * @see __super::getVersion
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::getVersion(PyObject *self, PyObject *args)
{
  STD_TSTRING sValue = _T("");
  if( !__super::getVersion( sValue ) )
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
PyObject* pyapi::findAction(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  CHAR* szText = NULL;
  if (!PyArg_ParseTuple(args, "Is", &idx, &szText ))
  {
    __super::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.findAction( <b>index</b>, <b>string</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  STD_TSTRING sValue = _T("");
  if( !__super::findAction( idx, helperapi::widen( szText ).c_str(), sValue ) )
  {
    // we have nothing
    return Py_BuildValue("b", false);
  }

  USES_CONVERSION;
  // we have a string
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}
#endif /*ACTIONMONITOR_API_PY*/