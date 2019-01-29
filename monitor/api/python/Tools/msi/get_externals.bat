@echo off
setlocal
rem Simple script to fetch source for external libraries

set HERE=%~dp0
if "%PCBUILD%"=="" (set PCBUILD=%HERE%..\..\PCbuild\)
if "%EXTERNALS_DIR%"=="" (set EXTERNALS_DIR=%HERE%..\..\externals\windows-installer)
if "%NUGET%"=="" (set NUGET=%EXTERNALS_DIR%\..\nuget.exe)
if "%NUGET_URL%"=="" (set NUGET_URL=https://aka.ms/nugetclidl)

set DO_FETCH=true
set DO_CLEAN=false

:CheckOpts
if "%~1"=="--python" (set PYTHON_FOR_BUILD=%2) & shift & shift & goto CheckOpts
if "%~1"=="--organization" (set ORG=%2) & shift & shift & goto CheckOpts
if "%~1"=="-c" (set DO_CLEAN=true) & shift & goto CheckOpts
if "%~1"=="--clean" (set DO_CLEAN=true) & shift & goto CheckOpts
if "%~1"=="--clean-only" (set DO_FETCH=false) & goto clean
if "x%~1" NEQ "x" goto usage

if "%DO_CLEAN%"=="false" goto fetch
:clean
echo.Cleaning up external libraries.
if exist "%EXTERNALS_DIR%" (
    rem Sometimes this fails the first time; try it twice
    rmdir /s /q "%EXTERNALS_DIR%" || rmdir /s /q "%EXTERNALS_DIR%"
)

if "%DO_FETCH%"=="false" goto end
:fetch

if "%ORG%"=="" (set ORG=python)

if "%PYTHON_FOR_BUILD%"=="" (
    echo Checking for installed python...
    py -3.6 -V >nul 2>&1 && (set PYTHON_FOR_BUILD=py -3.6)
)
if "%PYTHON_FOR_BUILD%"=="" (
    if NOT exist "%EXTERNALS_DIR%" mkdir "%EXTERNALS_DIR%"
    if NOT exist "%NUGET%" (
        echo Downloading nuget...
        rem NB: Must use single quotes around NUGET here, NOT double!
        rem Otherwise, a space in the path would break things
        powershell.exe -Command Invoke-WebRequest %NUGET_URL% -OutFile '%NUGET%'
    )
    echo Installing Python via nuget...
    "%NUGET%" install pythonx86 -ExcludeVersion -OutputDirectory "%EXTERNALS_DIR%"
    rem Quote it here; it's not quoted later because "py -3.6" wouldn't work
    set PYTHON_FOR_BUILD="%EXTERNALS_DIR%\pythonx86\tools\python.exe"
)

echo.Fetching external libraries...

set libraries=

for %%e in (%libraries%) do (
    if exist "%EXTERNALS_DIR%\%%e" (
        echo.%%e already exists, skipping.
    ) else (
        echo.Fetching %%e...
        %PYTHON_FOR_BUILD% "%PCBUILD%get_external.py" -e "%EXTERNALS_DIR%" -O %ORG% %%e
    )
)

echo.Fetching external tools...

set binaries=
rem We always use whatever's latest in the repo for these
set binaries=%binaries%     binutils
set binaries=%binaries%     gpg
set binaries=%binaries%     htmlhelp
set binaries=%binaries%     nuget
set binaries=%binaries%     redist
set binaries=%binaries%     wix

for %%b in (%binaries%) do (
    if exist "%EXTERNALS_DIR%\%%b" (
        echo.%%b already exists, skipping.
    ) else (
        echo.Fetching %%b...
        %PYTHON_FOR_BUILD% "%PCBUILD%get_external.py" -e "%EXTERNALS_DIR%" -b -O %ORG% %%b
    )
)

echo Finished.
goto end

:usage
echo.Valid options: -c, --clean, --clean-only, --organization, --python,
echo.--no-tkinter, --no-openssl
echo.
echo.Pull all sources and binaries necessary for compiling optional extension
echo.modules that rely on external libraries.
echo.
echo.The --organization option determines which github organization to download
echo.from, the --python option determines which Python 3.6+ interpreter to use
echo.with PCbuild\get_external.py.
echo.
echo.Use the -c or --clean option to remove the entire externals directory.
echo.
echo.Use the --clean-only option to do the same cleaning, without pulling in
echo.anything new.
echo.
exit /b -1

:end
