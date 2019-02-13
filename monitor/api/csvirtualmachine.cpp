#include "stdafx.h"
#include <fstream>
#ifdef ACTIONMONITOR_CS_PLUGIN

#include <os/ipclistener.h>
#include "csvirtualmachine.h"

CsVirtualMachine::CsVirtualMachine() :
  PowershellVirtualMachine()
{
}

CsVirtualMachine::~CsVirtualMachine()
{
}

/**
 * \brief Check if a given file extension is used by this API or not.
 * \param file the file we are checking
 * \return bool true|false if the given extension is powershell or not.
 */
bool CsVirtualMachine::IsExt(const MYODD_STRING& file)
{
  return myodd::files::IsExtension(file, _T("CS"));
}

/**
 * \brief create the full command line argument that will be passed to powershell
 * \param action the action we are working with
 * \param dllFullPath the full path of the dll
 * \param dllInterfaceFullPath the interface full path.
 * \param pluginPath the path to the plugin
 * \param uuid the unique id
 */
MYODD_STRING CsVirtualMachine::GetCommandLineArguments(
  const ActiveAction& action,
  const std::wstring& dllFullPath,
  const std::wstring& dllInterfaceFullPath,
  const std::wstring& pluginPath,
  const std::wstring& uuid
) const
{
  // the command is our class name.
  auto className = action.Command();

  // we want to create the powershell script
  auto script = myodd::strings::Format(

    L"# Press a key\n"
    L"function __amPressKey()\n"
    L"{\n"
    L"  #press key\n"
    L"  Write-Host -NoNewLine 'Press any key to continue...';\n"
    L"  $null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown');\n"
    L"}\n"
    L"\n"

    L"# Include the dll, (so the c# can access it)\n"
    L"Add-Type -Path \"%s\"\n"
    L"Add-Type -Path \"%s\"\n"
    L"\n"

    L"# Create the Action monitor object to pass to the c# code\n"
    L"$am = New-Object ActionMonitor.Core \"%s\"\n"
    L"\n"

    L"try {\n"
    L"  # Load the code.\n"
    L"  $code = [IO.File]::ReadAllText(\"%s\")\n"
    L"  \n"

    L"  # Pas it the common assemblies\n"
    L"  $assem = (\n"
    L"    \"Microsoft.CSharp\",\n"
    L"    \"System\",\n"
    L"    \"System.Core\",\n"
    L"    \"System.Data\",\n"
    L"    \"System.Data.DataSetExtensions\",\n"
    L"    \"System.Deployment\",\n"
    L"    \"System.Drawing\",\n"
    L"    \"System.Net.Http\",\n"
    L"    \"System.Windows.Forms\",\n"
    L"    \"System.Linq\",\n"
    L"    \"System.Threading.Tasks\",\n"
    L"    \"System.Xml\",\n"
    L"    \"System.Xml.Linq\",\n"
    L"    \"%s\",\n"
    L"    \"%s\"\n"
    L"  )\n"
    L"\n"
    L"  # Putting it all together\n"
    L"  Add-Type -TypeDefinition $code -ReferencedAssemblies $assem -Language CSharp\n"
    L"  $plugin = New-Object Am.%s $am\n"
    L"  [void]($plugin.Go())\n"
    L"\n"
    L"} catch {\n"
    L"  # Give the error message\n"
    L"  $ErrorMessage = $_.Exception.Message\n"
    L"\n"
    L"  $fullMessage = \"There was an error in script '%s', The error message was \"\"$ErrorMessage\"\"\"\n"
    L"  [void]($am.Log( 2, $fullMessage ))\n"
    L"  Write-Warning $fullMessage\n"
    L"  # And then wait\n"
    L"  __amPressKey;\n"
    L"}\n"
    , dllFullPath.c_str()           // create object
    , dllInterfaceFullPath.c_str()  // the interface
    , uuid.c_str()                  // unique id
    , pluginPath.c_str()            // the code itself.
    , dllFullPath.c_str()           // assemblies+Dll
    , dllInterfaceFullPath.c_str()  //   + interface
    , className.c_str()             // the class name
    , pluginPath.c_str()            // the code itself, (for the error).
  );

  std::wstring file;
  myodd::files::GetFullTempFileName(file, L"file", L"ps1");
  std::wofstream myfile;
  myfile.open(file.c_str());
  myfile << script.c_str();
  myfile.close();

  // The simply execute the 
  return myodd::strings::Format(
    _T("-command \"&{$policy = Get-ExecutionPolicy; Set-ExecutionPolicy RemoteSigned -Force; Add-Type -Path '%s'; . '%s';Set-ExecutionPolicy $policy -Force; }\""),
    dllFullPath.c_str(),
    file.c_str()
  );
}
#endif /*ACTIONMONITOR_CS_PLUGIN*/