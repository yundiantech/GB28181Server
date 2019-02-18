TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/GB28181Server/GB28181Server.cpp

HEADERS += \
    src/GB28181Server/GB28181Server.h

INCLUDEPATH += $$PWD/src

win32{

    #QMAKE_LFLAGS_DEBUG      = /DEBUG /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib
    QMAKE_LFLAGS_RELEASE     = /RELEASE /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib

    INCLUDEPATH += $$PWD/lib/win32/exosip/include


    LIBS += $$PWD/lib/win32/exosip/lib/eXosip.lib \
            $$PWD/lib/win32/exosip/lib/osip2.lib \
            $$PWD/lib/win32/exosip/lib/osipparser2.lib \
            $$PWD/lib/win32/exosip/lib/libcares.lib

    LIBS += WS2_32.lib Dnsapi.lib Iphlpapi.lib Qwave.lib delayimp.lib AdvAPI32.lib

}
