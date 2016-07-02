using System;

namespace Am
{
  // http://www.powershellmagazine.com/2014/03/18/writing-a-powershell-module-in-c-part-1-the-basics/
  // http://www.powershellmagazine.com/2014/04/08/basics-of-writing-a-powershell-module-with-c-part-2-debugging/
  //
  // Debug
  // C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe
  // -noexit -command "&{Add-Type -Path .\AMPowerShellCmdLets.dll}"
  public class Core
  {
    //  the default connection name.
    const string _Name = "MyOddweb_com_ActionMonitor";

    private MyOdd.IpcConnector Connector { get; set; }

    public Core(string givenGuid)
    {
      // We must be given a valid uid
      givenGuid = givenGuid.Trim();
      if ( string.IsNullOrEmpty(givenGuid) )
      {
        throw new Exception("The given ID cannot be null and/or empty");
      }

      //  create the connector.
      Connector = new MyOdd.IpcConnector(_Name);
    }

    ~Core()
    {
    }

    public string Hello()
    {
      // send a request with just a number and a string
      var ipcRequest = new MyOdd.IpcData();
      ipcRequest.Add(12);
      ipcRequest.Add("Hello");
      ipcRequest.Add("Hello", false );  //  ascii

      var ipcResponse = Connector.Send(ipcRequest);
      if (null != ipcResponse)
      {
        var i = ipcResponse.Get<int>(0);
      }

      var a = Connector.Send(ipcRequest);
      Connector.Close();
      var b = Connector.Send(ipcRequest);

      return "Hello";
    }
  }
}
