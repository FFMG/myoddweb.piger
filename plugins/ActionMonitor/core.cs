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
using ActionMonitor.Interfaces;
using ActionMonitor.Myodd;

namespace ActionMonitor
{
  // http://www.powershellmagazine.com/2014/03/18/writing-a-powershell-module-in-c-part-1-the-basics/
  // http://www.powershellmagazine.com/2014/04/08/basics-of-writing-a-powershell-module-with-c-part-2-debugging/
  //
  // Debug
  // C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe
  // -noexit -command "&{Add-Type -Path .\ActionMonitor.dll}"
  public class Core : IActionMonitor
  {
    /// <summary>
    /// Unique name for our IPC connector.
    /// </summary>
    private const string Name = "MyOddweb_com_ActionMonitor";

    /// <summary>
    /// The connector.
    /// </summary>
    private IpcConnector Connector { get; }

    /// <summary>
    /// Our unique identifiers.
    /// It allows us to connect back to Piger
    /// </summary>
    private string Uuid { get; }

    /// <summary>
    /// The contructor
    /// </summary>
    /// <param name="givenUuid">The UUID we are using to keep the link between us and the Action monitor class.</param>
    public Core(string givenUuid)
    {
      // We must be given a valid uid
      givenUuid = givenUuid.Trim();
      if ( string.IsNullOrEmpty(givenUuid) )
      {
        throw new Exception("The given UUID cannot be null and/or empty");
      }
      Uuid = givenUuid;

      //  create the connector.
      Connector = new IpcConnector(Name);
    }

    #region IActionMonitor
    /// <inheritdoc />
    public bool Say(string what, uint elapse, uint fadeout )
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("Say");
      ipcRequest.Add(what);
      ipcRequest.Add(elapse );
      ipcRequest.Add(fadeout);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("There was a problem sending the 'Say(...)' messages over to the monitor.");
      }
      var i = ipcResponse.Get<int>(0);
      return i == 1;
    }

    /// <inheritdoc />
    public string Version()
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("Version");

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the version number of the powershell plugin");
      }
      return ipcResponse.Get<string>(0);
    }

    /// <inheritdoc />
    public string GetCommand( uint index )
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetCommand");
      ipcRequest.Add(index);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception( "Unable to get the command at index." );
      }

      if(ipcResponse.IsInt(0) )
      {
        if( 0 == ipcResponse.Get<int>(0) )
        {
          throw new ArgumentOutOfRangeException( nameof(index), "The command you are trying to get does not exist.");
        }

        //  not sure what that number is
        throw new Exception("Could not understand the return error.");
      }
      return ipcResponse.Get<string>(0);
    }

    /// <inheritdoc />
    public string GetAction()
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetAction");

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the action.");
      }

      if (ipcResponse.IsInt(0))
      {
        if (0 == ipcResponse.Get<int>(0))
        {
          throw new Exception("Unable to get the command name.");
        }

        //  not sure what that number is
        throw new Exception("Could not understand the return error.");
      }
      return ipcResponse.Get<string>(0);
    }

    /// <inheritdoc />
    public int GetCommandCount()
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetCommandCount");

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the number of commands");
      }
      return ipcResponse.Get<int>(0);
    }

    /// <inheritdoc />
    public bool Execute( string module, string args, bool priviledged )
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("Execute");
      ipcRequest.Add(module);
      ipcRequest.Add(args);
      ipcRequest.Add(priviledged);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to execute this request.");
      }
      return ipcResponse.Get<bool>(0);
    }

    /// <inheritdoc />
    public string Getstring( bool quote )
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetString");
      ipcRequest.Add(quote);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the selected string.");
      }

      if (ipcResponse.IsInt(0))
      {
        if (0 == ipcResponse.Get<int>(0))
        {
          throw new Exception("Unable to get the selected string.");
        }

        //  not sure what that number is
        throw new Exception("Could not understand the return error.");
      }
      return ipcResponse.Get<string>(0);
    }

    /// <inheritdoc />
    public string Getfile( uint index, bool quote )
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetFile");
      ipcRequest.Add(index);
      ipcRequest.Add(quote);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new ArgumentOutOfRangeException(nameof(index), "Unable to get the file at index.");
      }

      if (ipcResponse.IsInt(0))
      {
        if (0 == ipcResponse.Get<int>(0))
        {
          throw new Exception("Unable to get the selected file, probably because there isn't one.");
        }

        //  not sure what that number is
        throw new Exception("Could not understand the return error.");
      }
      return ipcResponse.Get<string>(0);
    }

    /// <inheritdoc />
    public string Getfolder( uint index, bool quote)
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetFolder");
      ipcRequest.Add(index);
      ipcRequest.Add(quote);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the folder at index.");
      }

      if (ipcResponse.IsInt(0))
      {
        if (0 == ipcResponse.Get<int>(0))
        {
          throw new ArgumentOutOfRangeException( nameof(index), "Unable to get the selected file, probably because there isn't one.");
        }

        //  not sure what that number is
        throw new Exception("Could not understand the return error.");
      }
      return ipcResponse.Get<string>(0);
    }

    /// <inheritdoc />
    public string Geturl( uint index, bool quote)
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetUrl");
      ipcRequest.Add(index);
      ipcRequest.Add(quote);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the url at index.");
      }

      if (ipcResponse.IsInt(0))
      {
        if (0 == ipcResponse.Get<int>(0))
        {
          throw new ArgumentOutOfRangeException(nameof(index), "Unable to get the selected file, probably because there isn't one.");
        }

        //  not sure what that number is
        throw new Exception("Could not understand the return error.");
      }
      return ipcResponse.Get<string>(0);
    }

    /// <inheritdoc />
    public string GetVersion()
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetVersion");

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the version number");
      }
      return ipcResponse.Get<string>(0);
    }

    /// <inheritdoc />
    public bool Log( int type, string message )
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("Log");
      ipcRequest.Add(type);
      ipcRequest.Add(message);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to log the message.");
      }
      return ipcResponse.Get<bool>(0);
    }

    /// <inheritdoc />
    public bool AddAction(string action, string path)
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("AddAction");
      ipcRequest.Add(action);
      ipcRequest.Add(path);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to add the given action.");
      }
      return ipcResponse.Get<bool>(0);
    }

    /// <inheritdoc />
    public bool RemoveAction(string action, string path)
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("RemoveAction");
      ipcRequest.Add(action);
      ipcRequest.Add(path);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to remove the given action.");
      }
      return ipcResponse.Get<bool>(0);
    }

    /// <inheritdoc />
    public string FindAction(uint index, string action)
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("FindAction");
      ipcRequest.Add(index);
      ipcRequest.Add(action);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to find the action at the given index.");
      }

      if (ipcResponse.IsInt(0))
      {
        if (0 == ipcResponse.Get<int>(0))
        {
          throw new ArgumentOutOfRangeException( nameof(index), "Unable to find the action at the given index.");
        }

        //  not sure what that number is
        throw new Exception("Could not understand the return error.");
      }
      return ipcResponse.Get<string>(0);
    }

    /// <inheritdoc />
    public IntPtr GetForegroundWindow()
    {
      //  request
      var ipcRequest = new IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetForegroundWindow");

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to find the action at the given index.");
      }
      return (IntPtr)ipcResponse.Get<int>(0);
    }
    #endregion
  }
}
