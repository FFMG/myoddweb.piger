@echo off 
SET TargetDir=%~1
SET PythonDir=%TargetDir%\python\

echo "%TargetDir%..\..\..\python64\*.*"

if not exist "%PythonDir%" mkdir "%PythonDir%"
xcopy "%TargetDir%..\..\..\includes\python64\*.*" "%PythonDir%" /Y
xcopy "%TargetDir%..\..\..\includes\python64d\*.*" "%PythonDir%" /Y