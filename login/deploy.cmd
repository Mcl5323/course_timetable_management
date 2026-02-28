@echo off
set PATH=C:\Qt\Tools\mingw1310_64\bin;C:\Qt\6.10.0\mingw_64\bin;C:\Windows\System32;C:\Windows
cd /d "C:\BoonShenProject\Student Course Registration & Timetable Management System\course_timetable_management\login\deploy"
echo Running windeployqt...
"C:\Qt\6.10.0\mingw_64\bin\windeployqt.exe" --no-translations "Student Course Registration & Timetable Management System.exe"
echo Done!
dir
