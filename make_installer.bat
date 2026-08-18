@echo off
setlocal

set QtPath=D:\Qt\Qt5.14.2\5.14.2\mingw73_64
set MingwPath=D:\Qt\Qt5.14.2\Tools\mingw730_64
set ISCC="D:\Program Files (x86)\Inno Setup 6\ISCC.exe"
set PATH=%MingwPath%\bin;%QtPath%\bin;%PATH%

rem Kill running instance
tasklist /FI "IMAGENAME eq PerryBox.exe" 2>NUL | find /I "PerryBox.exe" >NUL
if errorlevel 1 (
    echo PerryBox.exe is not running.
) else (
    echo Killing PerryBox.exe...
    taskkill /F /IM PerryBox.exe >NUL 2>&1
    timeout /t 1 >NUL
)

rem Build release
if not exist build mkdir build
cd build

echo Running qmake (release)...
qmake ..\PerryBox.pro -spec win32-g++ "CONFIG+=release"
if errorlevel 1 (
    echo qmake failed!
    cd ..
    exit /b 1
)

echo Building (release)...
mingw32-make -j4
if errorlevel 1 (
    echo Build failed!
    cd ..
    exit /b 1
)
cd ..

rem Deploy Qt runtime manually (windeployqt misjudges this Qt install as debug)
set Dest=build\release
echo Deploying Qt runtime DLLs...
copy /Y "%QtPath%\bin\Qt5Core.dll"   %Dest% >NUL || goto :deployfail
copy /Y "%QtPath%\bin\Qt5Gui.dll"    %Dest% >NUL || goto :deployfail
copy /Y "%QtPath%\bin\Qt5Widgets.dll" %Dest% >NUL || goto :deployfail
copy /Y "%MingwPath%\bin\libgcc_s_seh-1.dll"  %Dest% >NUL || goto :deployfail
copy /Y "%MingwPath%\bin\libstdc++-6.dll"     %Dest% >NUL || goto :deployfail
copy /Y "%MingwPath%\bin\libwinpthread-1.dll" %Dest% >NUL || goto :deployfail
xcopy /E /I /Y "%QtPath%\plugins\platforms" %Dest%\platforms >NUL || goto :deployfail
xcopy /E /I /Y "%QtPath%\plugins\styles"    %Dest%\styles >NUL || goto :deployfail

rem Compile installer (output: dist\PerryBox-Setup-<version>.exe)
echo Building installer...
%ISCC% installer\perrybox.iss
if errorlevel 1 (
    echo Installer build failed!
    exit /b 1
)

echo Done! Installer is in the dist folder.
exit /b 0

:deployfail
echo DLL deployment failed!
exit /b 1
