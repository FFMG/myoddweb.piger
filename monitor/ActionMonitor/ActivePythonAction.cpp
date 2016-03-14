#include "stdafx.h"
#include "ActivePythonAction.h"
#include "ActionMonitor.h"

ActivePythonAction::ActivePythonAction(const Action& src, const STD_TSTRING& szCommandLine, bool isPrivileged) :
  ActiveAction( src, szCommandLine, isPrivileged )
{
  Initialize();
}

ActivePythonAction::~ActivePythonAction()
{
  DeInitialize();
}

bool ActivePythonAction::DeInitialize()
{
  assert(Py_IsInitialized());
  return true;
}

bool ActivePythonAction::Initialize()
{
  PythonVirtualMachine* py = App().GetPythonVirtualMachine();
  if (!py->Initialize())
  {
    return false;
  }

  //  all good
  return true;
}

void ActivePythonAction::ExecuteInThread()
{
  //  the file.
  const STD_TSTRING& szFile = File();

  // Python is not thread safe 
  // and windows cannot lock the file properly
  // so we need to read the file ourselves and pass it.
  //
  // this could be a memory problem at some stage.
  //
  std::string script = "";
  if (!ReadFile(szFile.c_str(), script))
  {
    return;
  }

  // create the Python Api.
  PythonVirtualMachine* py = App().GetPythonVirtualMachine();
  pyapi* api = new pyapi( *this, script, py->GetMainPyThread() );

  //  save it.
  std::thread::id id = std::this_thread::get_id();
  py->AddApi(id, api);

  // we can now execute the thread.
  api->ExecuteInThread();

  py->RemoveApi(id);
  delete api;
}

/**
 * Read the given file and get the script out of it.
 * @param LPCTSTR pyFile the python file we want to read.
 * @param std::string& script the string that will contain the string.
 * @return boolean success or not.
 */
bool ActivePythonAction::ReadFile(LPCTSTR pyFile, std::string& script) const
{
  // clear the scruot.
  script = "";

  //  we need to convert strings.
  USES_CONVERSION;

  errno_t err;
  FILE *fp;
  if (err = fopen_s(&fp, T_T2A(pyFile), "rt"))
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
