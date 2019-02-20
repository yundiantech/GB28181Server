#-------------------------------------------------
#
# Project created by QtCreator 2019-02-19T09:56:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GB28181Server
TEMPLATE = app


SOURCES += src/main.cpp \
    src/MainWindow.cpp \
    src/widget/DeviceTreeWidgetItem.cpp

HEADERS += \
    src/MainWindow.h \
    src/widget/DeviceTreeWidgetItem.h

FORMS += \
    src/MainWindow.ui

### GB28181Server ### Begin
    include($$PWD/GB28181Server/GB28181Server.pri)
### GB28181Server ### End

INCLUDEPATH += $$PWD/src
