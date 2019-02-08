#include "stdafx.h"
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
  return myodd::files::IsExtension(file, _T("ps1"));
}

/**
 * \brief create the full command line argument that will be passed to powershell
 * \param dllFullpath the full path of the dll
 * \param pluginPath the path to the plugin
 * \param uuid the unique id
 */
MYODD_STRING PowershellVirtualMachine::GetCommandLineArguments(const std::wstring& dllFullpath, const std::wstring& pluginPath, const std::wstring& uuid)
{
  // we need to recreate the code

  /**
    # get the full content as code.
    $code = [IO.File]::ReadAllText(pluginPath)
   */

  // or

  /**
    $code = @"
      using Am;
      using System.Windows.Forms;

      // Hello World! program
      namespace Foo
      {
        public class Bar
        {
          static public void Boo( Am.Core am )
          {
            DialogResult res = MessageBox.Show("Are you sure you want to Delete", "Confirmation", MessageBoxButtons.OKCancel, MessageBoxIcon.Information);
            if (res == DialogResult.OK) {
                am.Say( "You have clicked Ok Button", 3000, 5 );
                //Some task…
            }
            if (res == DialogResult.Cancel) {
                am.Say( "You have clicked Cancel Button", 3000, 5 );
                //Some task…
            }
          }
        }
      }
    "@

    $assem = (
      "System.Windows.Forms.dll",
      "AMPowerShellCmdLets.dll"
        )

    Add-Type -TypeDefinition $code -ReferencedAssemblies $assem -Language CSharp
    [Foo.Bar]::Boo( $am )
  */

  // the object
  auto am = myodd::strings::Format(_T("$am = New-Object Am.Core \"%s\""), uuid.c_str());

  // The simply execute the 
  return myodd::strings::Format(_T("-command \"&{$policy = Get-ExecutionPolicy; Set-ExecutionPolicy RemoteSigned -Force; Add-Type -Path '%s'; %s; . '%s' ;Set-ExecutionPolicy $policy -Force; }"),
    dllFullpath.c_str(),
    am.c_str(),
    pluginPath.c_str()
  );
}
#endif /*ACTIONMONITOR_CS_PLUGIN*/