# C Sharp Example

The c# script is not a compiled .NET but a c# script that will be executed.

If you want to run a .NET application, have a look at [the .NET script](dotnet.md)

## Expected format

### Class name

The expected class is `ActionMonitor.Plugin.{YourCommandName}`

So if your command is "`Bar`" then your *must* have a class `ActionMonitor.Plugin.Bar` somewhere.

```cs
using System;

namespace ActionMonitor.Plugin
{
  public class Bar
  {
    ...
  }
}
```

### The contructor

In your class, your constructor must take an "\IActionMonitor\" object

### 'Go' Function

One function is expected ... `Go` and that function will be the start of all the work your amzing script will do.

### Loaded assemblies

The loaded assemblies are:

- `System`
- `System.Core`
- `System.Xml.Linq`
- `System.Data.DataSetExtensions`
- `Microsoft.CSharp`
- `System.Data`
- `System.Net.Http`
- `System.Xml`

And of course the `ActionMonitor.Interfaces`.

### Complete example

In the root command directory, create a file `Bar.cs`

```cs
using System;
using ActionMonitor.Interfaces;

namespace ActionMonitor.Plugin
{
  public class Bar
  {
    private IActionMonitor _am;
    public Bar( IActionMonitor am )
    {
      _am = am;
    }

    public void Go()
    {
      Console.WriteLine("Hello World!");

      // Keep the console window open in debug mode.
      Console.WriteLine("Press any key to exit.");
      Console.ReadKey();
    }
  }
}
```

## `IActionMonitor` Functions

The available functions are:

- `bool Say( string what, uint elapse, uint fadeout)` : Show a message on the screen
- `string Version()`: The powershell version number.
- `string GetVersion()`: The plugin version number.
- `int GetCommandCount()`: The number of words added.
- `string GetCommand( int )`: Get the command, (space separated)
- `string GetAction()`: Get the action that we typed, (the command name)
- `bool Execute( string module, string args, bool priviledged )`: Execute a module, (exe, dll, file ...)
  Note that you can technically execute directly from c#, but it might be more convinient to use the Monitor.
- `string Getstring( bool quote )`: If the user/cursor had selected a string, get the value.
- `string Getfile( uint index, bool quote )`: If one or more files are selected get it by index.
- `string Getfolder( uint index, bool quote)`: If one or more folders are selected, get it by index.
- `string Geturl( uint index, bool quote)`: If one or more urls are selected, get it by index.
- `bool Log( int type, string message )`: Log a message
  - Success = 1
  - Error = 2
  - Warning = 3
  - Message = 4
  - System = 5
- `AddAction(string action, string path)`: Add another c# action, (the path does not have to be RootCommand)
- `bool RemoveAction(string action, string path)`: Remove ... an action
- `string FindAction(uint index, string action)`: Find a matching action, (the index number is in case there are more than one action)
- `IntPtr GetForegroundWindow()`: Get the window that had focus at the time of the action.

## Errors/Log

Errors are logged on screen and in the log file.

## But is is just a powershell script isn't it ...

Yes, yes it is
