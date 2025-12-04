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

TARGET = login
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    managecoursespage.cpp \
    signupwindow.cpp \
    timetable.cpp \
    loadingdialog.cpp

HEADERS += \
    mainwindow.h \
    managecoursespage.h \
    signupwindow.h \
    timetable.h \
    loadingdialog.h

FORMS += \
    mainwindow.ui \
    managecoursespage.ui \
    signupwindow.ui \
    timetable.ui
