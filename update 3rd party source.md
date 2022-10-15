# Upgrade 3rd party libraries / code

To build the code you might need to update some third party libraries from time to time.
  
## Lua

- Download the latest version from [https://www.lua.org/download.html](https://www.lua.org/download.html), there should only be one version called "source"
- Delete the files and folders
  - *\myoddweb.piger\monitor\api\lua\doc\\*
  - *\myoddweb.piger\monitor\api\lua\src\\*
  - *\myoddweb.piger\monitor\api\lua\Makefile*
  - *\myoddweb.piger\monitor\api\lua\README*
- Unpack the file in *\myoddweb.piger\monitor\api\lua\src\\*

## SQLite

- Download the latest version from [https://www.sqlite.org/download.html](https://www.sqlite.org/download.html), get the amalgamation file, (the file is normally called "sqlite-amalgamation-*.zip")
- Unzip the files to *\myoddweb.piger\myodd\sqlite\*.**

## Python

### Notes

- In the example below, the version is Python 3.10.8 and the files are unpacked in that folder.
Make sure that you use the correct folder name for whatever future versions you use.
- Make sure you delete any old `pyconfig.h` that might be lurking around

### Setup

Download the latest source code from [https://www.python.org/downloads/source/](https://www.python.org/downloads/source/)
The file is called something like "Gzipped source tarball" normally found at the bottom
It has a single folder inside it, unzip it so it looks something like `.\api\python\Python-3.10.8\`

- Inside that folder, look for a project file, pythoncore.vcxproj, (normally ..\PCBuild\..)
  And just copy and rename it as:
  - copy it as "pythoncore86.vcxproj"
  - copy it as "pythoncore64.vcxproj"
  - copy it as "pythoncore86.vcxproj.filters"
  - copy it as "pythoncore64.vcxproj.filters"
  
Download the actual python from [https://www.python.org/downloads/release/python-3108/](https://www.python.org/downloads/release/python-3108/)

Look for the "Download Windows installer (32-bit)" and make sure it is the correct version.

- Python x86
  - Install the x86 version, make sure you select "Customize Installation" (pay attention to install location)
    - Select "Download debugging symbols"
    - Select "Download debug binaries"
  - Delete all the files in \includes\python86d\
    - copy all the \*_d.pyd files *(but not the corresponding \*.pyb files)*, \*_d.dll, \*.lib from the python install directory, (`[Install Folder]\DLLs`)
      Make sure you do not copy the *.pdb files
  - Download the Windows embeddable package (x86)
    - Delete all the files in \includes\python86\
    - Copy all the content of the package to \includes\python86\
  - In the solution delete the monitor/x86/pythoncore86 project and locate the new one in the new folder.  
  - Copy the `python310.lib` file to tne \includes\python86\ folder, that file is located in the `[Install Folder]\libs\` folder.
  - Copy the `python310_d.lib` file to tne \includes\python86d\ folder, that file is located in the `[Install Folder]\libs\` folder.
  - Remove/Uninstall the x86 version

- Python x64
  - Install the x64 version, make sure you select "Customize Installation" (pay attention to install location)
    - Select "Download debugging symbols"
    - Select "Download debug binaries"
  - Delete all the files in \includes\python64d\
    - copy all the \*_d.pyd files *(but not the corresponding \*.pyb files)*, \*_d.dll, \*.lib from the python install directory, (`[Install Folder]\DLLs`)
      Make sure you do not copy the *.pdb files
  - Download the Windows embeddable package (x64)
    - Delete all the files in \includes\python64\
    - Copy all the content of the package to \includes\python64\
  - In the solution delete the monitor/x64/pythoncore64 project and locate the new one in the new folder.  
  - Copy the `python310.lib` file to tne \includes\python64\ folder, that file is located in the `[Install Folder]\libs\` folder.
  - Copy the `python310_d.lib` file to tne \includes\python64d\ folder, that file is located in the `[Install Folder]\libs\` folder.
  - Remove/Uninstall the x64 version
  
In the solution

- "Build > Configuration manager" and make sure that the x64 version build targets the correct architecture
- In the "x86/ActionMonitor" project "C/C++ > General" Additional include directory make sure the python path is correct (All Configurations).
- In the "x64/ActionMonitor64" project "C/C++ > General" Additional include directory make sure the python path is correct (All Configurations).
- Delete the Output folder to make sure that there are no 'extra' files.
- Do a full rebuild of release/debug, if this works as expected delete the previous folder.

#### zlib

- Got to [https://www.zlib.net/](https://www.zlib.net/) and download the latest version, (currently 1.2.13).
- Unpack that version to the `.\api\python\Python-3.10.8\externals`, (you might need to create the folder `externals`)
- The folder should look something like `.\api\python\Python-3.10.8\externals\zlib-1.2.13\`

**NB**: You must make sure that the the project `$(zlibDir)` is pointing to the same version of zlib and that the name of the folder.
Look for the file "python.props" and make sure that `<zlibDir>$(ExternalsDir)\zlib-1.2.13\</zlibDir>` points to the correct folder.

### In python 86/64 project

- delete pythoncore86
- delete pythoncore64
- locate and add the project you just created
  - pythoncore86
  - pythoncore64

- For the x64 delete the x86 configuration
- For the x86 delete the x64 configuration
- Set the output Directory for all configurations to $(SolutionDir)Output\$(Configuration)\$(PlatformTarget)\

- Build
  - the debug x64 should output to .\Output\Debug\x64
  - the debug x86 should output to .\Output\Debug\x86
  
### The projects that need Python-3

- Additional include directory `$(SolutionDir)/monitor/api/python/Python-3.10.8/Include/` and `$(SolutionDir)/monitor/api/python/Python-3.10.8/PC/`, (just make sure that the paths are valid).  
- Update all the code that include something like, "python310_d.lib", (or whatever the previous version might be)
  - pyapi.h (include the lib)
  - PythonVirtualMachine.cpp (include the zip file)

## Google test

- Download the latest version from [https://github.com/google/googletest/releases](https://github.com/google/googletest/releases)
- Unzip the files to *\myoddweb.piger\gtest\gtest-x.y.z\*.** where `xyz` is the new version number.
**NB:** In the zip file only unpack the `googletest` folder, `googletest-release-x.y.z\googletest` where x.y.z is the compressed file name.

- In the x64 and x86 remove the 2 files that are pointing to the older version:
  - gtest_main.cc
  - gtest-all.cc
- Re-add the 2 files to point to the new folder.
  - gtest_main.cc
  - gtest-all.cc
- Make sure that the files don't use 'precompiled headers', (Properties > C/C++ > Precompiled headers)
- Update all the include path to the new folder.
- For the x64 and x86 project, make sure that preprocessor flag `_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING` is added, (Properties > C/C++ > Preprocessor) is set.
Make sure that the setting is for both debug and release version.

### Project

- Update `myoddtest` and `myoddtest64` to point to the folder you unpacked the files above, \myoddweb.piger\myodd\gtest\gtest-x.y.z
Properties > C/C++ > General > Additional Include directories
  - `.\..\gtest\gtest-1.12.1\include;`
  - `.\..\gtest\gtest-1.12.1\`
- For the x64 and x86 project, make sure that preprocessor flag `_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING` is added, (Properties > C/C++ > Preprocessor) is set.
