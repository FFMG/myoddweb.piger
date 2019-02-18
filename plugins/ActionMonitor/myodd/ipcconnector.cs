//This file is part of Myoddweb.Piger.
//
//    Myoddweb.Piger is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Myoddweb.Piger is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Myoddweb.Piger.  If not, see<https://www.gnu.org/licenses/gpl-3.0.en.html>.
using System;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;

// ReSharper disable once CheckNamespace
namespace ActionMonitor.Myodd
{
  internal class IpcConnector
  {
    private const int WmClose = 0x10;
    private const int WmCopyData = 0x004A;

    private enum MessageType
    {
      // ReSharper disable once UnusedMember.Local
      None = 0,
      Send = 1,
      // ReSharper disable once UnusedMember.Local
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
    private IntPtr ConnectedWindow
    {
      get
      {
        //  do we have a window... and is it still somewhat valid?
        if (_hWindow != IntPtr.Zero && IsWindow(_hWindow))
        {
          return _hWindow;
        }
        _hWindow = FindWindowByClass();
        return _hWindow;
      }
    }

    /// <summary>
    /// The handle of the window.
    /// </summary>
    private IntPtr _hWindow;

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

    [DllImport("user32.dll")]
    [return: MarshalAs(UnmanagedType.Bool)]
    static extern bool IsWindow(IntPtr hWnd);

    [DllImport("user32.dll", EntryPoint = "FindWindowEx", SetLastError = true, CharSet = CharSet.Unicode)]
    static extern IntPtr FindWindowEx(IntPtr parentHandle, IntPtr childAfter, string lclassName, string windowTitle);

    [DllImport("user32.dll", CharSet = CharSet.Auto)]
    static extern IntPtr SendMessage(IntPtr hWnd, int msg, IntPtr wParam, ref Copydatastruct lParam);
#endregion

    public IpcConnector( string serverName )
    {
      //  save the name
      ServerName = serverName;
      _hWindow = new IntPtr();
    }

    protected IntPtr FindWindowByClass()
    {
      //  look for the window.
      var hwndMessage = new IntPtr(-3);
      return FindWindowEx(hwndMessage, IntPtr.Zero, ServerName, null);
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
      if (IntPtr.Zero == hWindow)
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
      using (var mmf = MemoryMappedFile.CreateNew( msg.Guid, 10000))
      {
        // cast to IntPtr because we know it is not null.
        if (0 == (int) SendMessage( hWindow, WmCopyData, IntPtr.Zero, ref cds))
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
      if( IntPtr.Zero == hWindow )
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
        SendMessage( hWindow, WmCopyData, IntPtr.Zero, ref cds);
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
