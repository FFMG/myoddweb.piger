using System;
using System.Management.Automation;

namespace AMPowerShellCmdLets
{
  // http://www.powershellmagazine.com/2014/03/18/writing-a-powershell-module-in-c-part-1-the-basics/
  // http://www.powershellmagazine.com/2014/04/08/basics-of-writing-a-powershell-module-with-c-part-2-debugging/
  //
  // Debug
  //  C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe
  //  -noexit -command "&{ import-module .\AMPowerShellCmdLets.dll -Verbose}"

  [Cmdlet("Am", "Salutation")]
  public class AmSalutation : Cmdlet
  {
    protected override void BeginProcessing()
    {
      const string name = "MyOddweb_com_ActionMonitor";
      var x = new MyOdd.IpcConnector(name);

      //  send a close request.
      x.Close();
    }
  }
}
