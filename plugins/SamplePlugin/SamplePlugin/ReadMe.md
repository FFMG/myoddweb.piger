#  SamplePlugin Project Overview

## Remarks

### Building

- Make sure that the inlcude path for amplugin.h is valid
- Make sure that the "aditional library" path is valid.
  Project > Properties > Linker > "Aditional Library properties"


### Plugin
- "SamplePlugin.cpp" contains the one and only function "am_Msg( ... )", that function is used
to manage all the various plugins call, (action, init, deinit etc...)

- All the plugins have AM_MSG_INIT called once at launch
- All the plugins have AM_MSG_DEINIT called once at closing time

### Debug

- If the plugin is not copied to the **__in** folder. 
- The target path is **$(SolutionDir)..\..\Output\Debug\ActionMonitor64d.exe** you can/should edit it to where ever your debug execute is.
- x64/x86 make sure that you build both x86 and x64, be sure to know what version you are installing
Piger will give an error if you are trying to mix platforms.

- Automatic copy
  - The dll is copied to "%appdata%\myoddweb\ActionMonitor\RootCommands\__in\", if you going to install things somehwere else
  good luck to you
  - Make sure that piger is not running, otherwise, you will get an error.

**Please don't try and mix platforms, (x86/x64), it will not work**

**Please don't try and debug with a release build, it wil, (probably), not work**