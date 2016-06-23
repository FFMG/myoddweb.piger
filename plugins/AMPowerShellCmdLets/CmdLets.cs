using System;
using System.Management.Automation;
using System.Runtime.InteropServices;

namespace AMPowerShellCmdLets
{
  internal class SimpleIpc
  {
    public struct CopyDataStruct
    {
      public int cbData;
      public IntPtr dwData;
      [MarshalAs(UnmanagedType.LPStr)]
      public string lpData;
    }

    [DllImport("user32.dll", EntryPoint = "FindWindow", SetLastError = true)]
    static extern IntPtr FindWindowByCaption(IntPtr ZeroOnly, string lpWindowName);

    [DllImport("user32.dll", EntryPoint = "FindWindow", SetLastError = true)]
    static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    static extern IntPtr SendMessage(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);

    [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
    public static extern IntPtr SendMessageWithCopyDataStruct(IntPtr hWnd, uint msg, IntPtr wParam, ref CopyDataStruct lParam);

    public SimpleIpc()
    {
      
    }

    public IntPtr FindWindowByName( string windowName )
    {
      return FindWindow(null, windowName );
    }

    public IntPtr FindWindowByClass(string className )
    {
      return FindWindow(className, null );
    }

    public void Close(IntPtr hWindow)
    {
      //I'd double check this constant, just in case
      const uint wmClose = 0x10;
      SendMessage(hWindow, wmClose, IntPtr.Zero, IntPtr.Zero);
    }
  }

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
      var x = new SimpleIpc();
      const string name = "MyOddweb_com_ActionMonitor";

      {
        // var y = x.FindWindowByName("Calculator");
        var y = x.FindWindowByName( name );
        if (y.ToInt64() == 0)
        {
          WriteObject("Could not find 'Calculator'.");
        }
        else
        {
          var z = y.ToString();
          WriteObject($"Found 'Calculator' : {z}.");
          x.Close(y);
        }
      }
      {
        var y = x.FindWindowByClass( name );
        if (y.ToInt64() == 0)
        {
          WriteObject("Could not find 'Calculator'.");
        }
        else
        {
          var z = y.ToString();
          WriteObject($"Found 'Calculator' : {z}.");
          x.Close(y);
        }
      }

    }
  }
}
