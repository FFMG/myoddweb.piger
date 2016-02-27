#define APP_NAME "Myoddweb Action Monitor (Piger)";
#define APP_PUBLISHER "myoddweb.com";
#define APP_URL "http://www.myoddweb.com";
#define APP_DIR "{pf}\myoddweb\piger";
#define APP_SOURCE "..\Output\Release\";

; set the version number based on the classifier version number.
#define APP_VERSION GetFileVersion( APP_SOURCE + 'ActionMonitor.exe' );

; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{E0B5F430-234F-47AE-912F-089E79259B3D}
AppVersion={#APP_VERSION}
VersionInfoVersion={#APP_VERSION}
AppName=Piger
AppVerName=Piger Alpha {#APP_VERSION}
AppPublisher={#APP_PUBLISHER}
AppPublisherURL={#APP_URL}
AppSupportURL={#APP_URL}
AppUpdatesURL={#APP_URL}
DefaultDirName={pf}\myoddweb\Piger
DefaultGroupName=Piger
OutputDir=/
OutputBaseFilename=piger-{#APP_VERSION}
Compression=lzma
SolidCompression=true

; "ArchitecturesInstallIn64BitMode=x64" requests that the install be
; done in "64-bit mode" on x64, meaning it should use the native
; 64-bit Program Files directory and the 64-bit view of the registry.
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x86 x64

; for now make sure that only admins can install this
; this is for the registry entry.
PrivilegesRequired=admin
AppMutex=MyOddweb_com_ActionMonitor

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Registry]
Root: HKLM; Subkey: SOFTWARE\Microsoft\Windows\CurrentVersion\Run; ValueType: string; ValueName: Piger; ValueData: """{app}\ActionMonitor.exe"""; Flags: uninsdeletevalue

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
;
; be sure to build the latest exe.
; select "Release Any CPU" to ensure that both x64 and x86 are built.
;
; x86 App
Source: {#APP_SOURCE}ActionMonitor.exe; DestDir: {app}; Flags: ignoreversion; Check: "not IsWin64"
Source: {#APP_SOURCE}hook.dll; DestDir: {app}; Flags: ignoreversion; Check: "not IsWin64"
Source: {#APP_SOURCE}python35.dll; DestDir: {app}; Flags: ignoreversion; Check: "not IsWin64"

; x64 App
Source: {#APP_SOURCE}ActionMonitor64.exe; DestName:ActionMonitor.exe; DestDir: {app}; Flags: ignoreversion; Check: IsWin64
Source: {#APP_SOURCE}hook64.dll; DestDir: {app}; Flags: ignoreversion; Check: IsWin64
Source: {#APP_SOURCE}python6435.dll; DestDir: {app}; Flags: ignoreversion; Check: IsWin64

; common
Source: ..\includes\python35.zip; DestDir: {app}; Flags: recursesubdirs createallsubdirs

; x86 plugins
Source: {#APP_SOURCE}LoaderPlugin.amp; DestDir: {userappdata}\myoddweb\ActionMonitor\RootCommands\__in\; Flags: ignoreversion; Check: "not IsWin64"
Source: {#APP_SOURCE}AppPaths.amp; DestDir: {userappdata}\myoddweb\ActionMonitor\RootCommands\__in\; Flags: ignoreversion; Check: "not IsWin64"
Source: {#APP_SOURCE}Dolly.amp; DestDir: {userappdata}\myoddweb\ActionMonitor\RootCommands\__in\; Flags: ignoreversion; Check: "not IsWin64"

; x64 plugins
Source: {#APP_SOURCE}LoaderPlugin64.amp; DestName:LoaderPlugin.amp; DestDir: {userappdata}\myoddweb\ActionMonitor\RootCommands\__in\; Flags: ignoreversion; Check: IsWin64
Source: {#APP_SOURCE}AppPaths64.amp; DestName:AppPaths.amp; DestDir: {userappdata}\myoddweb\ActionMonitor\RootCommands\__in\; Flags: ignoreversion; Check: IsWin64
Source: {#APP_SOURCE}Dolly64.amp; DestName:Dolly.amp; DestDir: {userappdata}\myoddweb\ActionMonitor\RootCommands\__in\; Flags: ignoreversion; Check: IsWin64

; any commands we might want to add.
Source: .\RootCommands\*; DestDir: {userappdata}\myoddweb\ActionMonitor\RootCommands\; Flags: recursesubdirs createallsubdirs

; default config
Source: profile.xml; DestDir: {userappdata}\myoddweb\ActionMonitor\; Flags: onlyifdoesntexist

[Run]
Filename: {app}\ActionMonitor.exe; Description: {cm:LaunchProgram,Piger}; Flags: nowait postinstall skipifsilent

[Icons]
Name: {group}\Piger; Filename: {app}\ActionMonitor.exe
Name: {group}\{cm:ProgramOnTheWeb,Piger}; Filename: {#APP_URL}
Name: {group}\{cm:UninstallProgram,Piger}; Filename: {uninstallexe}
Name: {commondesktop}\Piger; Filename: {app}\ActionMonitor.exe; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\Piger; Filename: {app}\ActionMonitor.exe; Tasks: quicklaunchicon
Name: {group}\{cm:UninstallProgram, Piger}; Filename: {uninstallexe}
