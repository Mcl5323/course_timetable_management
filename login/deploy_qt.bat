@echo off
echo ========================================
echo Qt6 Dependency Deployment Tool
echo ========================================
echo.

REM Set Qt path
set QT_DIR=C:\Qt\6.10.0\mingw_64

REM Change to script directory
cd /d "%~dp0"

REM Set target directory using short path
set TARGET_DIR=build\Desktop_Qt_6_10_0_MinGW_64_bit-Debug\debug

echo Checking Qt installation...
if not exist "%QT_DIR%\bin\windeployqt.exe" (
    echo Error: Qt not found at %QT_DIR%
    echo Please edit line 7 with your Qt path.
    echo.
    pause
    exit /b 1
)

echo Qt found at: %QT_DIR%
echo Target: %TARGET_DIR%
echo.

REM Add Qt to PATH
set PATH=%QT_DIR%\bin;%PATH%

echo Running windeployqt...
cd "%TARGET_DIR%"
windeployqt login.exe

echo.
if %ERRORLEVEL% EQU 0 (
    echo ========================================
    echo SUCCESS! You can now run login.exe
    echo ========================================
) else (
    echo ========================================
    echo FAILED! Check error messages above
    echo ========================================
)

echo.
pause
