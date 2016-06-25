using System;
using System.Runtime.InteropServices;

namespace MyOdd
{
  public class IpcConnector
  {
    /// <summary>
    /// This is the name of the server we are connecting to.
    /// </summary>
    public string ServerName { get; private set; }

    /// <summary>
    /// The handle of the window we are connecting to.
    /// </summary>
    private IntPtr? ConnectedWindow { get
      {
        if (null == _hWindow)
        {
          _hWindow = FindWindowByClass();
        }
        return _hWindow;
      }
    }

    /// <summary>
    /// The handle of the window.
    /// </summary>
    IntPtr? _hWindow;

    #region DllImport and Structures
    public struct CopyDataStruct
    {
      public int cbData;
      public IntPtr dwData;
      [MarshalAs(UnmanagedType.LPStr)]
      public string lpData;
    }

    [DllImport("user32.dll", EntryPoint = "FindWindow", SetLastError = true)]
    static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    static extern IntPtr SendMessage(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);

    [DllImport("user32.dll", CharSet = CharSet.Auto, SetLastError = false)]
    public static extern IntPtr SendMessageWithCopyDataStruct(IntPtr hWnd, uint msg, IntPtr wParam, ref CopyDataStruct lParam);
#endregion

    public IpcConnector( string serverName )
    {
      //  save the name
      ServerName = serverName;
      _hWindow = null;
    }

    protected IntPtr FindWindowByClass()
    {
      //  look for the window.
      return FindWindow(ServerName, null);
    }

    /// <summary>
    /// Send a close request to the main window.
    /// </summary>
    public bool Close()
    {
      IntPtr? hWindow = ConnectedWindow;
      if( null == hWindow )
      {
        return false;
      }

      //I'd double check this constant, just in case
      const uint wmClose = 0x10;

      // cast to IntPtr because we know it is not null.
      SendMessage((IntPtr)hWindow, wmClose, IntPtr.Zero, IntPtr.Zero);

      // success
      return true;
    }
  }
}
