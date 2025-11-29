QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = login
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    managecoursespage.cpp \
    signupwindow.cpp

HEADERS += \
    mainwindow.h \
    managecoursespage.h \
    signupwindow.h

FORMS += \
    mainwindow.ui \
    managecoursespage.ui \
    signupwindow.ui
