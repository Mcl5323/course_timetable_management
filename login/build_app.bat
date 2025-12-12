@echo off
REM Build script for Qt application
echo Building Qt Application...

REM Set PATH to include Qt tools
set PATH=C:\Qt\6.10.0\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%

REM Navigate to build directory
cd "%~dp0build\Desktop_Qt_6_10_0_MinGW_64_bit-Debug"

REM Clean previous build
echo Cleaning previous build...
C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe clean

REM Run qmake to regenerate Makefiles
echo Running qmake...
C:\Qt\6.10.0\mingw_64\bin\qmake.exe ..\..\login.pro -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug"

REM Build the project
echo Building project...
C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build successful!
    echo Executable: build\Desktop_Qt_6_10_0_MinGW_64_bit-Debug\debug\login.exe
    echo ========================================
) else (
    echo.
    echo ========================================
    echo Build failed! Check the errors above.
    echo ========================================
)

pause
