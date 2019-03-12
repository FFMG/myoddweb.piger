# .NET Example

## Reference

Your project must inclucde a reference to the action monitor dll, `using ActionMonitor.Interfaces;`

### Other references

You need to import and install your own references next to the binary.

Do not overwrite newer versions when installing your dll.

## Interfaces

- `IAction`: The action interface is the expected class that will be created, loaded, run when your binary is launched.
  - `Task InitializeAsync(IActionMonitor monitor, CancellationToken token )` function called to allow your class/binary to 'prepare' for the action.
  - `Task<bool> GoAsync(CancellationToken token)`: Starting point of the action, you can run the action from now on.
- `IActionMonitor`: The object that can be called to run all the various actions.

```cs
  public interface IActionMonitor
  {
    /// <summary>
    /// Output a message on the screen.
    /// </summary>
    /// <param name="what">The message</param>
    /// <param name="elapse">For how long, (ms)</param>
    /// <param name="fadeout">The fadeout speed.</param>
    /// <returns></returns>
    bool Say(string what, uint elapse, uint fadeout);

    /// <summary>
    /// Get the Powershell version number.
    /// </summary>
    /// <returns></returns>
    string Version();

    /// <summary>
    /// Get a command at a certain index.
    /// We throw if the value is out of range, use GetCommandCount(...)
    /// Command #0 is the full path of this script.
    /// </summary>
    /// <param name="index">the index we want</param>
    /// <returns>The command</returns>
    string GetCommand(uint index);

    /// <summary>
    /// Get the action name.
    /// </summary>
    /// <returns>The name of the action that we called.</returns>
    string GetAction();

    /// <summary>
    /// Get the number of arguments entered after the command.
    /// </summary>
    /// <returns>The number of commands entered.</returns>
    int GetCommandCount();

    /// <summary>
    /// Execute a process.
    /// </summary>
    /// <param name="module">The process we want to execute.</param>
    /// <param name="args">the arguments to pass</param>
    /// <param name="priviledged">if we want it to be elevated or not.</param>
    /// <returns></returns>
    bool Execute(string module, string args, bool priviledged);

    /// <summary>
    /// Get the currently selected string, (if any).
    /// </summary>
    /// <param name="quote"></param>
    /// <returns></returns>
    string Getstring(bool quote);

    /// <summary>
    /// Get a selected file at an index.
    /// </summary>
    /// <param name="index">the index we want</param>
    /// <param name="quote">if we want to quote the text or not.</param>
    /// <returns></returns>
    string Getfile(uint index, bool quote);

    /// <summary>
    /// Get the currenctly selected folder by index.
    /// </summary>
    /// <param name="index">The folder we are looking for.</param>
    /// <param name="quote">If we want to quote the response or not.</param>
    /// <returns></returns>
    string Getfolder(uint index, bool quote);

    /// <summary>
    /// Get the url by index.
    /// </summary>
    /// <param name="index">The url we are looking for.</param>
    /// <param name="quote">If we want to quote the response or not.</param>
    /// <returns></returns>
    string Geturl(uint index, bool quote);

    /// <summary>
    /// Get the application version number.
    /// </summary>
    /// <returns>The app version number.</returns>
    string GetVersion();

    /// <summary>
    /// Log a message to the main application log.
    /// </summary>
    /// <param name="type"></param>
    /// <param name="message"></param>
    /// <returns>Success or not</returns>
    bool Log(int type, string message);

    /// <summary>
    /// Add an action
    /// </summary>
    /// <param name="action">The action we want to add.</param>
    /// <param name="path">The path of the action we are adding.</param>
    /// <returns>Success or not</returns>
    bool AddAction(string action, string path);

    /// <summary>
    /// Remove an action given a path
    /// </summary>
    /// <param name="action">The action we want to remove.</param>
    /// <param name="path">The path of the action we want to remove.</param>
    /// <returns>Success or not</returns>
    bool RemoveAction(string action, string path);

    /// <summary>
    /// Find an action at a given index.
    /// So if we are looking for a command "stuff", there could be more than one.
    /// And index 0, 1, ..., x are the posible numbers.
    /// </summary>
    /// <param name="index">The action index we are looking for.</param>
    /// <param name="action">The action we want.</param>
    /// <returns>Success or not</returns>
    string FindAction(uint index, string action);

    /// <summary>
    /// Get the foreground window at the time of the call. 
    /// </summary>
    /// <returns>IntPtr the current fogregorund window.</returns>
    IntPtr GetForegroundWindow();
  }
```

## Extension

Your plugin must have the `.amp-net` extension.

If you are using Visual studio, you can set the project extension in the `csproj` file

```xml
  ...
  <TargetExt>.amp-net</TargetExt>
  ...
```