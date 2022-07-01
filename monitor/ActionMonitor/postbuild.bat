@echo off 
SET TargetDir=%~1
SET /A TargetArch=%~2
SET PythonDir=%TargetDir%\python\

if not exist "%PythonDir%" ( 
	mkdir "%PythonDir%"
)

if %TargetArch% == 86 (
  echo Copying files for x86 Architecture
	xcopy "%TargetDir%..\..\..\includes\python86\*.*" "%PythonDir%" /Y
	xcopy "%TargetDir%..\..\..\includes\python86d\*.*" "%PythonDir%" /Y
) else (
  if %TargetArch% == 64 ( 
    echo Copying files for x64 Architecture
    xcopy "%TargetDir%..\..\..\includes\python64\*.*" "%PythonDir%" /Y
    xcopy "%TargetDir%..\..\..\includes\python64d\*.*" "%PythonDir%" /Y
  ) else (
    echo Unknown or missing architecture.
    set errorlevel=1
    exit /b  
  )
)