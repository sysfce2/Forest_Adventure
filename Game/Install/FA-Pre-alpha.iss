; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "FA-Pre-alpha"
#define MyAppVersion "0.47"
#define MyAppExeName "FA-Pre-alpha.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{A00D7BAD-A2B8-410F-A1F0-8ADD7590288F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=..\..\LICENSE
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=..\..\Releases\Pre-alpha
OutputBaseFilename=FA-Pre-alpha-0.47
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Dirs]
Name: "{localappdata}\{#MyAppName}\logs"

[Files]
Source: "..\Release\FA-Pre-alpha.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release\openal32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release\sfml-audio-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release\sfml-graphics-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release\sfml-network-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release\sfml-system-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release\sfml-window-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Release\tinyxml2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\App\assets\*"; DestDir: "{commonappdata}\{#MyAppName}\assets"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

