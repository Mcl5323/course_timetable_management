QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Windows-specific configuration for creating standalone .exe
win32 {
    # Static linking for standalone executable
    # CONFIG += static

    # Application icon (optional - create an icon file if needed)
    # RC_ICONS = app_icon.ico
}

TARGET = "Student Course Registration & Timetable Management System"
TEMPLATE = app

# Include paths for headers
INCLUDEPATH += include \
               include/windows \
               include/dialogs \
               include/utils

# Source files
SOURCES += \
    src/main.cpp \
    src/windows/mainwindow.cpp \
    src/windows/managecoursespage.cpp \
    src/windows/signupwindow.cpp \
    src/windows/timetable.cpp \
    src/dialogs/loadingdialog.cpp

# Header files
HEADERS += \
    include/windows/mainwindow.h \
    include/windows/managecoursespage.h \
    include/windows/signupwindow.h \
    include/windows/timetable.h \
    include/dialogs/loadingdialog.h \
    include/utils/uistyles.h \
    include/datastructures.h

# UI form files
FORMS += \
    forms/mainwindow.ui \
    forms/managecoursespage.ui \
    forms/signupwindow.ui \
    forms/timetable.ui
