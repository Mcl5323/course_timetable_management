@echo off
REM Set PATH to include Qt libraries
set PATH=C:\Qt\6.10.0\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%

REM Run the application
cd "%~dp0build\Desktop_Qt_6_10_0_MinGW_64_bit-Debug\debug"
start login.exe

echo Application started!
pause
