#include "stdafx.h"
#include "ActiveAction.h"
#include "ActionMonitor.h"
#include "../myodd/threads/threads.h"

/**
 * The constructor
 * \param src the action that is now active.
 * \param virtualMachines all the current virtual machines.
 * \param hTopHWnd the window that was on top at the time the command was given.
 * \param szCommandLine the given command line that is, the words after the command itself
 * \param isPrivileged if this action is privileged or not.
 */
ActiveAction::ActiveAction(const Action& src,
                           IVirtualMachines& virtualMachines,
                           const HWND hTopHWnd,
                           const std::wstring& szCommandLine,
                           const bool isPrivileged) : 
  Action( src ), 
  _virtualMachines( virtualMachines ),
  _clipboard(nullptr),
  _szCommandLine( szCommandLine ),
  _isPrivileged( isPrivileged ),
  _hTopHWnd( hTopHWnd )
{
}

/**
 * \brief Destructor
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
  _clipboard = nullptr;
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
  const auto result = OnDeInitialize();

  // clear the clipboard.
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
 * \param pyFile the python file we want to read.
 * \param script the string that will contain the string.
 * \return boolean success or not.
 */
bool ActiveAction::ReadFile(LPCTSTR pyFile, std::string& script) const
{
  // clear the scruot.
  script = "";

  //  we need to convert strings.
  USES_CONVERSION;

  errno_t err;
  FILE *fp;
  if ((err = fopen_s(&fp, T_T2A(pyFile), "rt")))
  {
    return false;
  }

  //
  // Note that we are no longer in the realm of UNICODE here.
  // We are using Multi Byte data.
  static const UINT FILE_READ_SIZE = 100;
  size_t total = 0;
  while (!feof(fp))
  {
    // Attempt to read
    char buffer[FILE_READ_SIZE + 1];
    memset(buffer, '\0', FILE_READ_SIZE + 1);
    const auto count = fread(buffer, sizeof(char), FILE_READ_SIZE, fp);

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

void ActiveAction::UpdateEnvironmentVariables() const
{
  // the path
  UpdateEnvironmentPath();

  // the path ext
  UpdateEnvironmentPathExt();

  // the temp paths
  UpdateEnvironmentTmp();
  UpdateEnvironmentTemp();
}

/**
 * \brief Update the environment value, (if needed).
 * We also check if the value can/should be updated.
 * \param variableType the variable we are expanding.
 * \return none.
 */
void ActiveAction::UpdateEnvironmentValue(const VariableType variableType) const
{
  // values we will be using
  std::wstring keyConfig;
  std::wstring keyName;
  bool expandValue;
  
  // set the values depending on the type.
  switch (variableType)
  {
  case Path:
    keyConfig = _T("path");
    keyName = _T("Path");
    expandValue = true;
    break;

  case PathExt:
    keyConfig = _T("pathext");
    keyName = _T("PATHEXT");
    expandValue = false;  //  no expanding of that value.
    break;

  case Tmp:
    keyConfig = _T("tmp");
    keyName = _T("TMP");
    expandValue = true;
    break;

  case Temp:
    keyConfig = _T("temp");
    keyName = _T("TEMP");
    expandValue = true;
    break;

  default:
    assert(!"Unknown value type!" ); // 
    return;
  }

  const auto configName = L"os\\setenvironment\\" + keyConfig;
  if (::myodd::config::Get(configName, 1) != 1)
  {
    return;
  }

  // get the value
  MYODD_STRING sValue;
  if (!myodd::reg::LoadStringFullPath(_T("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"), keyName.c_str(), sValue, HKEY_LOCAL_MACHINE))
  {
    return;
  }

  if (true == expandValue)
  {
    // expand the value(s)
    if (!myodd::files::ExpandEnvironment(sValue, sValue))
    {
      return;
    }
  }

  try {
    // get a single critical section
    static myodd::threads::CritSection cs;

    // lock it.
    myodd::threads::AutoLock lock(cs);

    DWORD l = GetEnvironmentVariable(keyName.c_str(), NULL, 0);
    if (l == 0)
    {
      // the value does not even exist ...
      return;
    }

    // current value
    const auto sCurrentValue = new TCHAR[l + 1];
    GetEnvironmentVariable(keyName.c_str(), sCurrentValue, l);

    // if the two are not the same, then we need to update it.
    if (myodd::strings::Compare(sCurrentValue, sValue, false) != 0)
    {
      // the two values are not the same, so set it.
      SetEnvironmentVariable(keyName.c_str(), sValue.c_str());
    }

    // clean up
    delete[] sCurrentValue;
  }
  catch (...)
  {
    myodd::log::LogError(_T("Unable to updateenvironment variable: %s."), keyName.c_str());
  }
}

/**
 * Update the environment variables path
 * to make sure it is up to date.
 */
void ActiveAction::UpdateEnvironmentPath() const
{
  UpdateEnvironmentValue(ActiveAction::Path);
}

/**
 * Update the environment variables path extenstions
 * to make sure it is up to date.
 */
void ActiveAction::UpdateEnvironmentPathExt() const
{
  UpdateEnvironmentValue(ActiveAction::PathExt);
}

/**
 * Update the environment variables temp path
 * to make sure it is up to date.
 */
void ActiveAction::UpdateEnvironmentTemp() const
{
  UpdateEnvironmentValue(ActiveAction::Temp);
}

/**
* Update the environment variables path
* to make sure it is up to date.
*/
void ActiveAction::UpdateEnvironmentTmp() const
{
  UpdateEnvironmentValue( ActiveAction::Tmp );
}
