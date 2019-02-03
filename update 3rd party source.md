# Upgrade 3rd party libraries / code
To build the code you might need to update some third party libraries from time to time.
  
## Lua
- Download the latest version from [https://www.lua.org/download.html](https://www.lua.org/download.html), there should only be one version called "source"
- Delete the files and folders
	- *\myoddweb.piger\monitor\api\lua\doc\*
	- *\myoddweb.piger\monitor\api\lua\src\*
	- *\myoddweb.piger\monitor\api\lua\Makefile*
	- *\myoddweb.piger\monitor\api\lua\README*
- Unpack the file in *\myoddweb.piger\monitor\api\lua\src\*

## SQLite
- Download the latest version from [https://www.sqlite.org/download.html](https://www.sqlite.org/download.html), get the amalgamation file, (the file is normally called "sqlite-amalgamation-*.zip")
- Unzip the files to *\myoddweb.piger\myodd\sqlite\*.**

## Python

### Notes

- In the example below, the version is Python 3.7.2 and the files are unpacked in that folder.
Make sure that you use the correct folder name for whatever future versions you use.
- Make sure you delete any old `pyconfig.h` that might be lurking around

### Setup

Download the lates source code from [https://www.python.org/downloads/release/](https://www.python.org/downloads/release/)
The file is called something like "Gzipped source tarball"
It has a single folder inside it, unzip it so it looks something like `.\api\python\Python-3.7.2\`

- look for pythoncore.vcxproj
  - copy it as "pythoncore86.vcxproj"
  - copy it as "pythoncore64.vcxproj"
  - copy it as "pythoncore86.vcxproj.filters"
  - copy it as "pythoncore64.vcxproj.filters"

### In python 86/64 project

  - delete pythoncore86
  - delete pythoncore64
  - locate and add the project you just copied
    - pythoncore86
    - pythoncore64

- For the x64 delete the x86 configuration
- For the x86 delete the x64 configuration
- Set the output Directory for all configurations to $(SolutionDir)Output\$(Configuration)\$(PlatformTarget)\ ($(BuildPath))

- Build 
  - the debug x64 should output to .\Output\Debug\x64
  - the debug x86 should output to .\Output\Debug\x86
  
### The projects that need Python-3

- Aditional include directory `$(SolutionDir)/monitor/api/python/Python-3.7.2/Include/` and `$(SolutionDir)/monitor/api/python/Python-3.7.2/PC/`, (just make sure that the paths are valid).  

## Google test

- Download the latest version from [https://github.com/google/googletest/releases](https://github.com/google/googletest/releases), get the amalgamation file, (the file is normally called "sqlite-amalgamation-*.zip")
- Unzip the files to *\myoddweb.piger\myodd\gtest\gtest-x.y.z\*.** where `xyz` is the new version number.     
**NB:** In the zip file only unpack the `googletest` folder, `googletest-release-x.y.z\googletest` where x.y.z is the compressed file name.

- In the x64 and x86 remove the 2 files that are pointing to the older version: 
  - gtest_main.cc
  - gtest-all.cc
- Re-add the 2 files to point to the new folder.
  - gtest_main.cc
  - gtest-all.cc
- Make sure that the files don't use 'precompiled headers', (Properties > C/C++ > Precompiled headers)
- For the x64 and x86 project, make sure that preprocessor flag `_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING` is added, (Properties > C/C++ > Preprocessor) is set.    
Make sure that the setting is for both debug and release version. 

### Project

- Update `myoddtest` and `myoddtest64` to point to the folder you unpacked the files above, \myoddweb.piger\myodd\gtest\gtest-x.y.z    
Properties > C/C++ > General > Additional Include directories 
  - `.\..\gtest\gtest-1.8.1\include;`
  - `.\..\gtest\gtest-1.8.1\`
- For the x64 and x86 project, make sure that preprocessor flag `_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING` is added, (Properties > C/C++ > Preprocessor) is set.