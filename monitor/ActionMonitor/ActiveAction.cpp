// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActiveAction.h"
#include "ActionsCore.h"
#include "ActionMonitor.h"

/**
 * The contructor
 * @param const Action& src the action that is now active.
 * @param HWND hTopHWnd the window that was on top at the time the command was given.
 * @param const STD_TSTRING& szCommandLine the given command line that is, the words after the command itself
 * @param bool isPrivileged if this action is privileged or not.
 */
ActiveAction::ActiveAction(const Action& src, 
                           HWND hTopHWnd,
                           const STD_TSTRING& szCommandLine, 
                           bool isPrivileged) : 
  Action( src ), 
  _clipboard(NULL),
  _szCommandLine( szCommandLine ),
  _isPrivileged( isPrivileged ),
  _hTopHWnd( hTopHWnd )
{
}

/**
 * Destructor
 * @param void
 * @return void
 */
ActiveAction::~ActiveAction()
{
  ClearClipboard();
}

/**
 * Clear the clipboard and free the memory.
 */
void ActiveAction::ClearClipboard()
{
  delete _clipboard;
  _clipboard = NULL;
}

void ActiveAction::CreateClipboard()
{
  //  clear the old one.
  ClearClipboard();

  // we must always get the clipboard because we never know for certain if the APIs might call
  // some functions. This will also reset the values if need be.
  //
  // so copy the text that the user could have currently selected or copy the name of the file that is probably selected
  // tell the clipboard to copy the data of the last known foreground window.
  CWnd* cwnd = CActionMonitorApp::GetLastForegroundWindow();
  size_t maxClipboardMemory = CActionMonitorApp::GetMaxClipboardMemory();
  _clipboard = new Clipboard(cwnd, maxClipboardMemory);
}

bool ActiveAction::Initialize()
{
  //  just create the clipboard.
  CreateClipboard();

  // all done.
  return OnInitialize();
}

bool ActiveAction::DeInitialize()
{
  bool result = OnDeInitialize();

  // clear the cipboard.
  ClearClipboard();

  // all done
  return result;
}

/** 
 * This is where we will be executing the code.
 */
void ActiveAction::ExecuteInThread()
{
  // try and refresh the environment variables.
  UpdateEnvironmentVariables();

	//	call the derived class
  OnExecuteInThread();
}

/**
 * Read the given file and get the script out of it.
 * @param LPCTSTR pyFile the python file we want to read.
 * @param std::string& script the string that will contain the string.
 * @return boolean success or not.
 */
bool ActiveAction::ReadFile(LPCTSTR pyFile, std::string& script) const
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

void ActiveAction::UpdateEnvironmentVariables()
{
  // the path
  UpdateEnvironmentPath();

  // the path ext
  UpdateEnvironmentPathExt();

  // the temp paths
  UpdateEnvironmentTmp();
  UpdateEnvironmentTemp();
}

void ActiveAction::UpdateEnvironmentPath(const STD_TSTRING& keyName)
{
  // get the value
  STD_TSTRING sValue;
  if (!myodd::reg::LoadStringFullPath(_T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"), keyName.c_str(), sValue, HKEY_LOCAL_MACHINE))
  {
    return;
  }

  // expand the value(s)
  if (!myodd::files::ExpandEnvironment(sValue, sValue))
  {
    return;
  }

  DWORD l = GetEnvironmentVariable(keyName.c_str(), NULL, 0);
  if (l == 0)
  {
    // the value does not even exist ...
    return;
  }

  // current value
  TCHAR* sCurrentValue = new TCHAR[l + 1];
  GetEnvironmentVariable(keyName.c_str(), sCurrentValue, l );

  // if the two are not the same, then we need to update it.
  if (myodd::strings::icompare(sCurrentValue, sValue) != 0)
  {
    // the two values are not the same, so set it.
    SetEnvironmentVariable(keyName.c_str(), sValue.c_str());
  }

  // clean up
  delete[] sCurrentValue;
}

/**
 * Update the environment variables path
 * to make sure it is up to date.
 */
void ActiveAction::UpdateEnvironmentPath()
{
  UpdateEnvironmentPath(_T("Path"));
}

/**
 * Update the environment variables path extenstions
 * to make sure it is up to date.
 */
void ActiveAction::UpdateEnvironmentPathExt()
{
  // get the value
  STD_TSTRING sValue;
  if (!myodd::reg::LoadStringFullPath(_T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"), _T("PATHEXT"), sValue, HKEY_LOCAL_MACHINE))
  {
    return;
  }

  // set it.
  SetEnvironmentVariable(_T("PATHEXT"), sValue.c_str());
}

/**
 * Update the environment variables temp path
 * to make sure it is up to date.
 */
void ActiveAction::UpdateEnvironmentTemp()
{
  UpdateEnvironmentPath(_T("TEMP"));
}

/**
* Update the environment variables path
* to make sure it is up to date.
*/
void ActiveAction::UpdateEnvironmentTmp()
{
  UpdateEnvironmentPath(_T("TMP"));
}
