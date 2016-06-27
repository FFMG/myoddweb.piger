using AMPowerShellCmdLets.myodd;
using System;
using System.Runtime.InteropServices;

namespace MyOdd
{
  public class IpcConnector
  {
    const int WmClose = 0x10;
    const int WmCopyData = 0x004A;

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
    
    /// <summary> 
    /// The COPYDATASTRUCT structure contains data to be passed to another  
    /// application by the WM_COPYDATA message.  
    /// </summary> 
    [StructLayout(LayoutKind.Sequential)]
    internal struct COPYDATASTRUCT
    {
      public IntPtr dwData;       // Specifies data to be passed 
      public int cbData;          // Specifies the data size in bytes 
      public IntPtr lpData;       // Pointer to data to be passed 
    }
    [DllImport("user32.dll", EntryPoint = "FindWindow", SetLastError = true)]
    static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    static extern IntPtr SendMessage(IntPtr hWnd, int msg, IntPtr wParam, ref COPYDATASTRUCT lParam);
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

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    internal struct MyStruct
    {
      public UInt32 uMsg;

      public ulong wParam;

      public long lParam;
    }

    /// <summary>
    /// Send a message as a reconstructed string.
    /// </summary>
    /// <param name="msg">The IpcData we want to send.</param>
    /// <returns></returns>
    public bool Send(IpcData msg )
    {
      IntPtr? hWindow = ConnectedWindow;
      if (null == hWindow)
      {
        return false;
      }

      var cds = new COPYDATASTRUCT
      {
        dwData = (IntPtr)3, //  copy message...
        cbData = msg.GetSize(),
        lpData = msg.GetPtr()
      };

      // cast to IntPtr because we know it is not null.
      SendMessage((IntPtr)hWindow, WmCopyData, IntPtr.Zero, ref cds);
      return true;
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

      var myStruct = new MyStruct
      {
        uMsg = WmClose,
        wParam = 0,
        lParam = 0
      };

      var myStructSize = Marshal.SizeOf(myStruct);
      IntPtr pMyStruct = Marshal.AllocHGlobal(myStructSize);

      try
      {
        Marshal.StructureToPtr(myStruct, pMyStruct, true);

        var cds = new COPYDATASTRUCT
        {
          dwData = (IntPtr)1, //  send message
          cbData = myStructSize,
          lpData = pMyStruct
        };

        // cast to IntPtr because we know it is not null.
        SendMessage((IntPtr)hWindow, WmCopyData, IntPtr.Zero, ref cds);
      }
      finally
      {
        Marshal.FreeHGlobal(pMyStruct);
      }
      // success
      return true;
    }
  }
}
