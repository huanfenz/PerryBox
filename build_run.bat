@echo off
setlocal

set QtPath=D:\Qt\Qt5.14.2\5.14.2\mingw73_64
set MingwPath=D:\Qt\Qt5.14.2\Tools\mingw730_64
set PATH=%MingwPath%\bin;%QtPath%\bin;%PATH%

set BUILD_TYPE=debug
if /I "%1"=="release" set BUILD_TYPE=release

tasklist /FI "IMAGENAME eq PerryBox.exe" 2>NUL | find /I "PerryBox.exe" >NUL
if errorlevel 1 (
    echo PerryBox.exe is not running.
) else (
    echo Killing PerryBox.exe...
    taskkill /F /IM PerryBox.exe >NUL 2>&1
    timeout /t 1 >NUL
)

if not exist build mkdir build
cd build

echo Running qmake (%BUILD_TYPE%)...
qmake ..\PerryBox.pro -spec win32-g++ "CONFIG+=%BUILD_TYPE%"
if errorlevel 1 (
    echo qmake failed!
    exit /b 1
)

echo Building (%BUILD_TYPE%)...
mingw32-make -j4
if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo Build succeeded!
echo Running...
start %BUILD_TYPE%\PerryBox.exe
