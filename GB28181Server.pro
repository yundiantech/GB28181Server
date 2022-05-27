#-------------------------------------------------
#
# Project created by QtCreator 2019-02-19T09:56:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GB28181Server
TEMPLATE = app

UI_DIR  = obj/Gui
MOC_DIR = obj/Moc
OBJECTS_DIR = obj/Obj

#将输出文件直接放到源码目录下的bin目录下，将dll都放在了次目录中，用以解决运行后找不到dll的问
#DESTDIR=$$PWD/bin/
contains(QT_ARCH, i386) {
    message("32-bit")
    DESTDIR = $${PWD}/bin32
} else {
    message("64-bit")
    DESTDIR = $${PWD}/bin64
}
QMAKE_CXXFLAGS += -std=c++11


SOURCES += src/main.cpp \
    src/MainWindow.cpp \
    src/widget/DeviceTreeWidgetItem.cpp \
    src/widget/ChannelTreeWidgetItem.cpp \
    src/widget/ShowVideoWidget.cpp

HEADERS += \
    src/MainWindow.h \
    src/widget/DeviceTreeWidgetItem.h \
    src/widget/ChannelTreeWidgetItem.h \
    src/widget/ShowVideoWidget.h

FORMS += \
    src/MainWindow.ui \
    src/widget/ShowVideoWidget.ui

### GB28181Server ### Begin
    include($$PWD/GB28181Server/GB28181Server.pri)
### GB28181Server ### End

INCLUDEPATH += $$PWD/src
