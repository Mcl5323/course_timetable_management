@echo off
setlocal
set PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.0\mingw_64\bin;%PATH%
set PROJECT_DIR=C:\BoonShenProject\Student Course Registration ^& Timetable Management System\course_timetable_management\login
cd /d "%PROJECT_DIR%\release"
echo Current directory: %CD%
echo Running qmake...
"C:\Qt\6.10.0\mingw_64\bin\qmake.exe" ..\login.pro CONFIG+=release
if errorlevel 1 (
    echo qmake failed!
    exit /b 1
)
echo Running make...
"C:\Qt\Tools\mingw1310_64\bin\mingw32-make.exe" -j4
if errorlevel 1 (
    echo make failed!
    exit /b 1
)
echo Build successful!
dir *.exe
