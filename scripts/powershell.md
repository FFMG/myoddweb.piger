## Examples

### GetForegroundWindow

Get the foreground window at the time of the call.
The value returned is a IntPtr or a window handle, (32bit integer really).

```ps1
  $signature = @"
    [DllImport("user32.dll")]
    public static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X,int Y, int cx, int cy, uint uFlags);

    static readonly IntPtr HWND_TOPMOST = new IntPtr(-1);

    const UInt32 SWP_NOSIZE = 0x0001;
    const UInt32 SWP_NOMOVE = 0x0002;
    const UInt32 TOPMOST_FLAGS = SWP_NOMOVE | SWP_NOSIZE;

    public static void MakeTopMost (IntPtr fHandle)
    {
            SetWindowPos(fHandle, HWND_TOPMOST, 0, 0, 0, 0, TOPMOST_FLAGS);
    }
  "@

  $app = Add-Type -MemberDefinition $signature -Name Win32Window -Namespace ScriptFanatic.WinAPI -ReferencedAssemblies System.Windows.Forms -Using System.Windows.Forms -PassThru

  $hwnd = $am.GetForegroundWindow()
  if($hWnd -ne 0)
  {
    $null = $app::MakeTopMost($hWnd)
  }
  else
  {
    Write-Host "$hWnd is 0"
  }
```

### Log messages

```ps1
  $am.Log( 1, "Hello from powershell" )
```

or, (to prevent output).

```ps1
    [void]($am.Log( 1, "Hello from powershell" ))
```

The log messages are:

- Success = 1
- Error = 2
- Warning = 3
- Message = 4
- System = 5

### Say

The first parameter is a string, then the amount of time in ml the message will appear for, and the last parameter is the fade speed, (if zero it will not fade).

```ps1
  # Say message 
  [void]($am.Say( "Hello world", 3000, 5 )
```

### Currently selected string

```ps1
  Try
  {
    $string = $am.GetString( $false ) # throw if nothing selected.
    $stringQuoted = $am.GetString( $true )
  }
  Catch [ArgumentOutOfRangeException]
  {
    # Caught exception!
    $string = "";
    $stringQuoted = "";
  }
  Catch
  {
    # Caught another exception!
    $string = "";
    $stringQuoted = "";
  }
```

### Get Folder/File/Url

```ps1
  Try
  {
    $file = $am.GetFile( 0, $false ) # throw if nothing selected.
    # $am.GetFolder( 0, $false ) # throw if nothing selected.
    # $am.GetUrl( 0, $false ) # throw if nothing selected.
  }
  Catch [ArgumentOutOfRangeException]
  {
    # No file at this index.
    $file = ""
  }
  Catch
  {
    Write-Host "No files/Folder/Url selected"
  }
```

## Exceptions

- Say, *None*.
  - *Exception* if the Elapse parameter is invalid, (cannot be zero).
  - *Exception* if the Fade parameter is invalid.
- Version, *None*.
- GetCommand, *ArgumentOutOfRangeException* if the command index does not exist.
- GetAction, *Exception* if the current action name cannot be found.
- GetCommandCount, *None*.
- Execute, *None*.
- Getstring, *Exception* if we have no selected string.
- Getfile, *ArgumentOutOfRangeException* if there is no file at the index.
- Getfolder, *ArgumentOutOfRangeException* if there is no folder at the index.
- Geturl, *ArgumentOutOfRangeException* if there is no url at the index.
- GetVersion, *None*
- Log, *None*.
- AddAction, *None*.
- RemoveAction, *None*.
- FindAction, *ArgumentOutOfRangeException* if the action at the index does not exist.

**Note**: In some cases, an *Exception* will be thrown if there are other problem, but the main reason would be because ActionMonitor.exe was closed while the script itself is still running.

You can check the log file for error/warning messages.