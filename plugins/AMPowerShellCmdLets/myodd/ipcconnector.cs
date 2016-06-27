using AMPowerShellCmdLets.myodd;
using System;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;

namespace MyOdd
{
  public class IpcConnector
  {
    private const int WmClose = 0x10;
    private const int WmCopyData = 0x004A;

    private enum MessageType
    {
      None = 0,
      Send = 1,
      Post = 2,
      Copy = 3
    }

    /// <summary>
    /// This is the name of the server we are connecting to.
    /// </summary>
    public string ServerName { get; private set; }

    /// <summary>
    /// The handle of the window we are connecting to.
    /// </summary>
    private IntPtr? ConnectedWindow => _hWindow ?? (_hWindow = FindWindowByClass());

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
    internal struct Copydatastruct
    {
      public IntPtr dwData;       // Specifies data to be passed 
      public int cbData;          // Specifies the data size in bytes 
      public IntPtr lpData;       // Pointer to data to be passed 
    }
    [DllImport("user32.dll", EntryPoint = "FindWindow", SetLastError = true)]
    static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    static extern IntPtr SendMessage(IntPtr hWnd, int msg, IntPtr wParam, ref Copydatastruct lParam);
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
      public uint uMsg;

      public ulong wParam;

      public long lParam;
    }

    /// <summary>
    /// Send a message as a reconstructed string.
    /// </summary>
    /// <param name="msg">The IpcData we want to send.</param>
    /// <returns>The IpcData response data</returns>
    public IpcData Send( IpcData msg )
    {
      // look for the listener.
      var hWindow = ConnectedWindow;
      if (null == hWindow)
      {
        return null;
      }

      // the message we want to send.
      var cds = new Copydatastruct
      {
        dwData = (IntPtr)MessageType.Copy, //  copy message...
        cbData = msg.GetSize(),
        lpData = msg.GetPtr()
      };

      // open the shared memory that should have been created.
      using (var mmf = MemoryMappedFile.CreateNew( msg.Guid.ToString(), 10000))
      {
        // cast to IntPtr because we know it is not null.
        if (0 == (int) SendMessage((IntPtr) hWindow, WmCopyData, IntPtr.Zero, ref cds))
        {
          return null;
        }

        // open the shared memory that should have been created.
        using (var mmfvs = mmf.CreateViewStream())
        {
          using (var reader = new BinaryReader(mmfvs))
          {
            // read the first 4 bytes for the size.
            var dataSize = BitConverter.ToInt32( reader.ReadBytes( sizeof(int)), 0 );

            // all the byes 
            var bytes = reader.ReadBytes( dataSize );

            // parse the response data into an IpcData
            return new IpcData( bytes );
          }
        }
      }
    }

    /// <summary>
    /// Send a close request to the main window.
    /// </summary>
    public bool Close()
    {
      var hWindow = ConnectedWindow;
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
      var pMyStruct = Marshal.AllocHGlobal(myStructSize);

      try
      {
        Marshal.StructureToPtr(myStruct, pMyStruct, true);

        var cds = new Copydatastruct
        {
          dwData = (IntPtr)MessageType.Send, //  send message
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
