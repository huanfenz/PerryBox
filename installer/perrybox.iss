; PerryBox Inno Setup 安装包脚本
; 由 make_installer.bat 调用，打包 windeployqt 之后的 build\release\ 全部文件
; 注意：版本号需与 src/ui/mainwindow.h 中的 APP_VERSION 保持同步

#define MyAppName "PerryBox"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Perry"
#define MyAppExeName "PerryBox.exe"

[Setup]
AppId={{FD2F753C-9DF3-42CE-B27E-12AA1731F70F}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
UninstallDisplayIcon={app}\{#MyAppExeName}
OutputDir=..\dist
OutputBaseFilename=PerryBox-Setup-{#MyAppVersion}
SetupIconFile=..\BirdIcon4.ico
Compression=lzma2/max
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequiredOverridesAllowed=dialog

[Languages]
Name: "chinesesimplified"; MessagesFile: "ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "..\build\release\*"; DestDir: "{app}"; Excludes: "*.o,moc_*.cpp,moc_*.h,qrc_*.cpp,ui_*.h,Makefile*,*.prl,object_script.*"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#MyAppName}}"; Flags: nowait postinstall skipifsilent
