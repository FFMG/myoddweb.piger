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

    private string Uuid { get; set; }

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
      Connector = new MyOdd.IpcConnector(_Name);
    }

    ~Core()
    {
    }

    public bool Say(string what, uint elapse, uint fadeout )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("Say");
      ipcRequest.Add(what);
      ipcRequest.Add(elapse );
      ipcRequest.Add(fadeout);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        return false;
      }
      var i = ipcResponse.Get<int>(0);
      return (i == 1);
    }

    public string Version()
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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

    public string GetCommand( uint index )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetCommand");
      ipcRequest.Add(index);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception( "Unable to get the command at index." );
      }
      return ipcResponse.Get<string>(0);
    }

    public string GetAction( uint index )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetAction");
      ipcRequest.Add(index);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the action at index.");
      }
      return ipcResponse.Get<string>(0);
    }

    public int GetCommandCount()
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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

    public bool Execute( string module, string args, bool priviledged )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetAction");
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

    public string Getstring( uint index )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetString");
      ipcRequest.Add(index);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the string at index.");
      }
      return ipcResponse.Get<string>(0);
    }

    public string Getfile( uint index, bool quote )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("GetFile");
      ipcRequest.Add(index);
      ipcRequest.Add(quote);

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to get the file at index.");
      }
      return ipcResponse.Get<string>(0);
    }

    public string Getfolder( uint index, bool quote)
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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
      return ipcResponse.Get<string>(0);
    }

    public string Geturl( uint index, bool quote)
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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
      return ipcResponse.Get<string>(0);
    }

    public bool AddAction( string action, string path )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
      ipcRequest.Add(Uuid);
      ipcRequest.Add("AddAction");
      ipcRequest.Add(action);
      ipcRequest.Add(path );

      //  response
      var ipcResponse = Connector.Send(ipcRequest);
      if (null == ipcResponse)
      {
        throw new Exception("Unable to add the given action.");
      }
      return ipcResponse.Get<bool>(0);
    }

    public bool RemoveAction( string action, string path)
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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

    public string GetVersion()
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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

    public string FindAction( uint index, string action )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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
      return ipcResponse.Get<string>(0);
    }

    public bool Log( int type, string message )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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
  }
}
