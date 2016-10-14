# Upgrade 3rd party libraries / code
To build the code you might need to update some third party libraries from time to time.
  
## Lua
## Python

- Download the latest code from the Windows section of [Python.org](https://www.python.org/)    
You will need 3 files, the code for x86/x64 as well as the code itself.
	- The x64 is called "Windows x86-64 embeddable zip file"
	- The x86 is called "Windows x86 embeddable zip file"
	- The code is called "Gzipped source tarball", (for some reason it is not a xip file).

### Copy the files
#### Source code
- delete all the files from *myoddweb.piger\monitor\api\python*, (this is to prevent old code from creeping in)    
**Execpt for:**
	- myoddweb.piger\monitor\api\python\PCbuild\pythoncore*.vcxproj
	- myoddweb.piger\monitor\api\python\PCbuild\pythoncore*.vcxproj.filters
	- myoddweb.piger\monitor\api\python\PCbuild\pythoncore*.vcxproj.user
- Spend some time comparing the old and new *myoddweb.piger\monitor\api\python\PCbuild\pythoncore.vcxproj*, check if anything from the old ***.vcxproj** should be added to the new one. Also check if anything from the old one needs to be put in to the correcsponding ***64.vcxproj** project. 
- Unzip the source code to the Python directory, *myoddweb.piger\monitor\api\python*.
- Open the project
	- Make sure that all the files are deleted properly from the filters, (you should have picked it up by comparing the old/new ***.vcxproj.filters** project filter files.
- delete all the files from *myoddweb.piger\includes\python64*, (this is to prevent old code from creeping in)
- Unzip all the "Windows x86-64 embeddable zip file" files into *myoddweb.piger\includes\python64*
	- You can ignore the **python*.exe** files.
- delete all the files from *myoddweb.piger\includes\python*, (this is to prevent old code from creeping in)
- Unzip all the "Windows x86 embeddable zip file" files into *myoddweb.piger\includes\python*
	- You can ignore the **python*.exe** files.

#### Debug files
Installing the debug symbols is a little trickier

##### x86
- Download and run the "Windows x86 web-based installer"
- Uncheck every single option apart from the "*download debugging symbols*" and "*Download debug binaries (...)*", change the install location to somewhere more practical.
- Go to that install library and in the sub directory "\DLLs\*" look for all the ***_d.pyd** files, copy all those files to "myoddweb.piger\includes\python86d\*.*"
- Run the install again, but this time select "uninstall" and remove everything. Everything should be removed.

##### x64
- Download and run the "Windows x86-64 web-based installer"
- Uncheck every single option apart from the "*download debugging symbols*" and "*Download debug binaries (...)*", change the install location to somewhere more practical.
- Go to that install library and in the sub directory "\DLLs\*" look for all the ***_d.pyd** files, copy all those files to "myoddweb.piger\includes\python64d\*.*"
- Run the install again, but this time select "uninstall" and remove everything. Everything should be removed.