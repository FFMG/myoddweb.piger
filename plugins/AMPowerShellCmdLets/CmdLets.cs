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

      // send a request with just a number and a string
      var ipcRequest = new myodd.IpcData();
      ipcRequest.Add(12);
      ipcRequest.Add("Hello");
      ipcRequest.Add("Hello", false );  //  ascii

      var ipcResponse = x.Send(ipcRequest);
      if (null != ipcResponse)
      {
        var i = ipcResponse.Get<int>(0);
      }

      var a = x.Send(ipcRequest);
      x.Close();
      var b = x.Send(ipcRequest);
    }
  }
}
