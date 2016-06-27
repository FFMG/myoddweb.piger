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

      //  send a null request
      x.Send( new myodd.IpcData() );

      // send a request with just a number
      //var y1 = new myodd.IpcData();
      //y1.Add(34);
      //x.Send( y1 );

      // send a request with just a string
      //var y2 = new myodd.IpcData();
      //y2.Add("Hello");
      //x.Send(y2);

      // send a request with just a number and a string
      var y3 = new myodd.IpcData();
      y3.Add(12);
      y3.Add("Hello");
      y3.Add("Hello", false );  //  ascii
      x.Send(y3);
    }
  }
}
