#include "stdafx.h"

#ifdef _DEBUG
#   define new DEBUG_NEW
#endif

#include "helperapi.h"
#include "PythonVirtualMachine.h"
#include "files\files.h"

// define the virtual machine object.
static PythonVirtualMachine pyVirtualMachine;

/**
 * Todo
 * @param void
 * @return void
 */
PythonVirtualMachine& GetPyVirtualMachine()
{
  pyVirtualMachine.Initialize();
  return (pyVirtualMachine);
}

/**
 * Todo
 * @param void
 * @return void
 */
PythonVirtualMachine::PythonVirtualMachine(void) : 
  m_isInitialized( false )
{
}

/**
 * Todo
 * @param void
 * @return void
 */
PythonVirtualMachine::~PythonVirtualMachine(void)
{
  // simply close python
  Py_Finalize();
}

/**
 * Todo
 * @param void
 * @return void
 */
void PythonVirtualMachine::Initialize()
{
  if( !m_isInitialized )
  {
    pyapi::Initialize();

    STD_TSTRING exe_dir = myodd::files::GetAppPath(true);
    std::wstring python_path;
    python_path += exe_dir + L"python35.zip";
    Py_SetPath(python_path.c_str());

    Py_Initialize();

    PyObject *main_module = PyImport_AddModule( "__main__" );
    PyObject *main_dict = PyModule_GetDict(main_module);

    m_isInitialized = true;
  }
}

/**
 * Todo
 * @param void
 * @return void
 */
bool PythonVirtualMachine::IsPyExt( LPCTSTR ext ) const
{
  return ( _tcsicmp( ext, _T("py") ) == 0 );
}

/**
 * Todo
 * @param void
 * @return void
 */
int PythonVirtualMachine::LoadFile( LPCTSTR pyFile )
{
  if( !m_isInitialized )
  {
    return -1;
  }

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

  // we can now run our script
  LPCSTR s = script.c_str();
  if( -1 == PyRun_SimpleString( s ))
  {
    helperapi::say( _T("<b>Error : </b>An error was raised in the PyAPI"), 500, 10 );
  }

  // no more errors.
  PyErr_Clear();

  /*
  (void) PyRun_SimpleFile(fp, pyFile );
  */
  
  return 0;
}