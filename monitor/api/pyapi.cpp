#include "stdafx.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

#include "pyapi.h"
#include "helperapi.h"

/**
 * Todo
 * @param void
 * @return void
 */
pyapi::pyapi(void)
{
}

/**
 * Todo
 * @param void
 * @return void
 */
pyapi::~pyapi(void)
{
}

static PyMethodDef amMethods[] = {
    {"say", pyapi::say, METH_VARARGS, "Display a message on the screen."},
    {"version", pyapi::version, METH_VARARGS, "Get this API version number."},
    {"getCommand", pyapi::getCommand, METH_VARARGS, "Get a certain command, return false if it does not exist."},
    {"getAction", pyapi::getAction, METH_VARARGS, "Get the action entered by the user."},
    {"getCommandCount", pyapi::getCommandCount, METH_VARARGS, "Get the number of commands."},
    {"execute", pyapi::execute, METH_VARARGS, "Execute a command, (app, command)."},
    {"getstring", pyapi::getstring, METH_VARARGS, "Get the currently selected text if any."},
    {"getfile", pyapi::getfile, METH_VARARGS, "Get a file by index, return false if not found."},
    {"getfolder", pyapi::getfolder, METH_VARARGS, "Get a folder by index, return false if not found."},
    {"geturl", pyapi::geturl, METH_VARARGS, "Get a URL by index, return false if not found."},
    {"addAction", pyapi::addAction, METH_VARARGS, "Add an action and path to the list of actions."},
    {"removeAction", pyapi::removeAction, METH_VARARGS, "Remove an action from the list."},
    {"getVersion", pyapi::getVersion, METH_VARARGS, "Get the full version string."},
    {"findAction", pyapi::findAction, METH_VARARGS, "Find an action given an index, return false or the path."},
    {NULL, NULL, 0, NULL}
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
void pyapi::Initialize( )
{
  PyImport_AppendInittab(
    "am",
    &PyInit_am
  );
}

/**
 * Todo
 * @see helperapi::say
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
    helperapi::say( _T("<b>Error : </b> Missing <i>Elapse</i> time.<br>Format is <i>am.say( msg, <b>elapse</b>, fade=0)</i>"), 3000, 5 );
    
    //  just return false.
    return Py_BuildValue("b", 0 );
  }

  // display the message
  // and we can now display the message.
  bool result = helperapi::say( msg, nElapse, nFadeOut );
  
  // return true.
  return Py_BuildValue("b", result );
}

/**
 * Todo
 * @see helperapi::version
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
 * @see helperapi::getCommand
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
    helperapi::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.getCommand( <b>index</b> )</i>"), 3000, 5 );

    //  just return false.
    return Py_BuildValue("b", 0 );
  }

  STD_TSTRING sValue;
  if( !helperapi::getCommand( idx, sValue ) )
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
 * @see helperapi::getAction
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::getAction(PyObject *self, PyObject *args)
{
  STD_TSTRING sValue;
  if( !helperapi::getAction( sValue ) )
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
 * @see helperapi::getcommandcount
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::getCommandCount(PyObject *self, PyObject *args)
{
  // get it
  int nSize = helperapi::getCommandCount();

  // and return it.
  return Py_BuildValue("i", nSize);
}

/**
 * Todo
 * @see helperapi::execute
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
    helperapi::say( _T("<b>Error : </b> Missing Module and/or command line.<br>Format is <i>am.execute( module [, commandLine[,isPrivileged]])</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  // run it
  bool result = helperapi::execute( module, cmdLine, isPrivileged );

  // return the result.
  return Py_BuildValue("b", result );
}

/**
 * Todo
 * @see helperapi::getstring
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::getstring(PyObject *self, PyObject *args)
{
  STD_TSTRING sValue = _T("");
  if( !helperapi::getString( sValue ) )
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
 * @see helperapi::getfile
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::getfile(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  if (!PyArg_ParseTuple(args, "I", &idx ))
  {
    helperapi::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.getfile( <b>index</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  STD_TSTRING sValue = _T("");
  if( !helperapi::getFile( idx, sValue ) )
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
 * @see helperapi::getfolder
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::getfolder(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  if (!PyArg_ParseTuple(args, "I", &idx ))
  {
    helperapi::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.getfolder( <b>index</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  STD_TSTRING sValue = _T("");
  if( !helperapi::getFolder( idx, sValue ) )
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
 * @see helperapi::geturl
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::geturl(PyObject *self, PyObject *args)
{
  UINT idx = 0;
  if (!PyArg_ParseTuple(args, "I", &idx ))
  {
    helperapi::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.geturl( <b>index</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  STD_TSTRING sValue = _T("");
  if( !helperapi::getURL( idx, sValue ) )
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
 * @see helperapi::addAction
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
    helperapi::say( _T("<b>Error : </b> Missing values.<br>Format is <i>am.addAction( <b>action</b>, <b>path</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  // run it
  bool result = helperapi::addAction( szText, szPath );
  return Py_BuildValue("b", result );
}

/**
 * Todo
 * @see helperapi::removeAction
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
    helperapi::say( _T("<b>Error : </b> Missing values.<br>Format is <i>am.removeAction( <b>action</b>, <b>path</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  // run it
  bool result = helperapi::removeAction( szText, szPath );
  return Py_BuildValue("b", result );
}

/**
 * Get the current Action monitor version number.
 * @see helperapi::getVersion
 * @param PyObject *
 * @param PyObject *
 * @return PyObject*
 */
PyObject* pyapi::getVersion(PyObject *self, PyObject *args)
{
  STD_TSTRING sValue = _T("");
  if( !helperapi::getVersion( sValue ) )
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
 * @see helperapi::findAction
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
    helperapi::say( _T("<b>Error : </b> Missing index number.<br>Format is <i>am.findAction( <b>index</b>, <b>string</b> )</i>"), 3000, 5 );
    return Py_BuildValue("b", false );
  }

  STD_TSTRING sValue = _T("");
  if( !helperapi::findAction( idx, szText, sValue ) )
  {
    // we have nothing
    return Py_BuildValue("b", false);
  }

  USES_CONVERSION;
  // we have a string
  return Py_BuildValue("s", T_T2A(sValue.c_str()) );
}