; Inno Setup Script for llmstudio

#define MyAppName "llmstudio"
#define MyAppVersion "1.0" ; << Adjust version as needed
#define MyAppPublisher "Your Name/Company" ; << Change Publisher
#define MyAppURL "https://yourwebsite.com" ; << Change URL
#define MyAppExeName "llmstudio.exe"

; SourceDir: Path to the build output directory (containing the EXE, DLLs, plugins, binaries, models)
; IMPORTANT: Assumes this .iss file is saved ONE level ABOVE the build directory.
; Example: ProjectRoot/setup.iss, ProjectRoot/build/llmstudio.exe
#define SourceDir "..\build"

; OutputDir: Where the final setup executable will be saved. '.' means current directory.
#define OutputDir "."

; MingwBinDir define is removed, as windeployqt should handle necessary runtime libs.
; If you find you DO need specific MinGW DLLs NOT copied by windeployqt (unlikely for standard Qt apps),
; you would uncomment a line similar to the old libgomp one in [Files], pointing to your MinGW install.
; #define MingwBinDir "C:\path\to\your\mingw\bin"

[Setup]
AppId={{AUTO_GUID}} ; Generates a unique ID for the application
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName} ; Installs to Program Files (x64 if applicable)
;DisableProgramGroupPage=yes ; Keep if you don't want a Start Menu programs folder selection page
;PrivilegesRequired=admin ; Required for installing to Program Files
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64 ; Ensure 64-bit installation
OutputBaseFilename=llmstudio_Setup_{#MyAppVersion}
OutputDir={#OutputDir}
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
; Optional task to create a desktop icon
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; 1. Copy the main executable
Source: "{#SourceDir}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion

; 2. Copy everything else from the build directory (DLLs, plugins, assets from 'binaries', 'models' etc.)
;    that windeployqt and your CMake copy commands placed there.
;    Excludes the main EXE (already copied) and common build/source artifacts
;    that shouldn't be in the build output anyway but excluded just in case.
Source: "{#SourceDir}\*"; DestDir: "{app}"; \
    Excludes: "\{#MyAppExeName},*.pdb,*.debug,*.ilk,CMakeFiles\*,\*.a,\*.cmake,\*.json,\*.txt,Makefile"; \
    Flags: ignoreversion recursesubdirs createallsubdirs

; If you find specific MinGW runtime DLLs are missing after installation (e.g., libstdc++-6.dll, libgcc_s_seh-1.dll)
; AND windeployqt didn't copy them (check your {#SourceDir} first!), you might need to manually add them:
; Source: "{#MingwBinDir}\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
; Start Menu entry
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
; Optional Desktop icon entry based on the task
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
; Run the application after installation finishes
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

; [Messages] Section removed - No custom model message needed for llmstudio

; [Code] Section removed - No custom Pascal script needed for llmstudio