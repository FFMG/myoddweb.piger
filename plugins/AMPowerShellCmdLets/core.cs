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
    private const string Name = "MyOddweb_com_ActionMonitor";

    private MyOdd.IpcConnector Connector { get; }

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
      Connector = new MyOdd.IpcConnector(Name);
    }

    /// <summary>
    /// Output a message on the screen.
    /// </summary>
    /// <param name="what">The message</param>
    /// <param name="elapse">For how long, (ms)</param>
    /// <param name="fadeout">The fadeout speed.</param>
    /// <returns></returns>
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
      var i = ipcResponse?.Get<int>(0);
      return i == 1;
    }

    /// <summary>
    /// Get the Powershell version number.
    /// </summary>
    /// <returns></returns>
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

    /// <summary>
    /// Get a command at a certain index.
    /// We throw if the value is out of range, use GetCommandCount(...)
    /// Command #0 is the full path of this script.
    /// </summary>
    /// <param name="index">the index we want</param>
    /// <returns>The command</returns>
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

    /// <summary>
    /// Get the action name.
    /// </summary>
    /// <returns>The name of the action that we called.</returns>
    public string GetAction()
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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

    /// <summary>
    /// Get the number of arguments entered after the command.
    /// </summary>
    /// <returns>The number of commands entered.</returns>
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

    /// <summary>
    /// Execute a process.
    /// </summary>
    /// <param name="module">The process we want to execute.</param>
    /// <param name="args">the arguments to pass</param>
    /// <param name="priviledged">if we want it to be elevated or not.</param>
    /// <returns></returns>
    public bool Execute( string module, string args, bool priviledged )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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

    /// <summary>
    /// Get the currently selected string, (if any).
    /// </summary>
    /// <param name="quote"></param>
    /// <returns></returns>
    public string Getstring( bool quote )
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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

    /// <summary>
    /// Get a selected file at an index.
    /// </summary>
    /// <param name="index">the index we want</param>
    /// <param name="quote">if we want to quote the text or not.</param>
    /// <returns></returns>
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

    /// <summary>
    /// Get the currenctly selected folder by index.
    /// </summary>
    /// <param name="index">The folder we are looking for.</param>
    /// <param name="quote">If we want to quote the response or not.</param>
    /// <returns></returns>
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

    /// <summary>
    /// Get the url by index.
    /// </summary>
    /// <param name="index">The url we are looking for.</param>
    /// <param name="quote">If we want to quote the response or not.</param>
    /// <returns></returns>
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

    /// <summary>
    /// Get the application version number.
    /// </summary>
    /// <returns>The app version number.</returns>
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

    /// <summary>
    /// Log a message to the main application log.
    /// </summary>
    /// <param name="type"></param>
    /// <param name="message"></param>
    /// <returns>Success or not</returns>
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

    /// <summary>
    /// Add an action
    /// </summary>
    /// <param name="action">The action we want to add.</param>
    /// <param name="path">The path of the action we are adding.</param>
    /// <returns>Success or not</returns>
    public bool AddAction(string action, string path)
    {
      //  request
      var ipcRequest = new MyOdd.IpcData();
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

    /// <summary>
    /// Remove an action given a path
    /// </summary>
    /// <param name="action">The action we want to remove.</param>
    /// <param name="path">The path of the action we want to remove.</param>
    /// <returns>Success or not</returns>
    public bool RemoveAction(string action, string path)
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

    /// <summary>
    /// Find an action at a given index.
    /// So if we are looking for a command "stuff", there could be more than one.
    /// And index 0, 1, ..., x are the posible numbers.
    /// </summary>
    /// <param name="index">The action index we are looking for.</param>
    /// <param name="action">The action we want.</param>
    /// <returns>Success or not</returns>
    public string FindAction(uint index, string action)
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

      if (ipcResponse.IsInt(0))
      {
        if (0 == ipcResponse.Get<int>(0))
        {
          throw new Exception("Unable to find the action at the given index.");
        }

        //  not sure what that number is
        throw new Exception("Could not understand the return error.");
      }
      return ipcResponse.Get<string>(0);
    }
  }
}
