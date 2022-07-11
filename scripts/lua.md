# Lua Examples

## Log messages

```lua
  -- log message
  am_log( 1, "Hello from Lua" );
```

The log messages are:

- Success = 1
- Error = 2
- Warning = 3
- Message = 4
- System = 5

## Say

The first parameter is a string, then the amount of time in ml the message will appear for, and the last parameter is the fade speed, (if zero it will not fade).

```lua
  -- Say message
  am_say( "Hello world", 3000, 5 );
```

## Currently selected string

```lua
  local string = am_getString( False ); -- throw if nothing selected.
  local stringQuoted = am_getString( True );

  am_say( string, 3000, 5 );
```

## Get Folder/File/Url

```lua
  local file = am_getFile( 0, False ); -- throw if nothing selected.
  -- am_getFolder( 0, False );
  -- am_getUrl( 0, False );
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

## How to Debug

- Check the logs for lua errors
- You can use `am_say( ... )` anything for example as you are not connected.