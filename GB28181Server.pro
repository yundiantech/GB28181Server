TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/GB28181Server/GB28181Server.cpp \
    src/MD5/HTTPDigest.cpp \
    src/MD5/MD5.cpp

HEADERS += \
    src/GB28181Server/GB28181Server.h \
    src/MD5/HTTPDigest.h \
    src/MD5/MD5.h

INCLUDEPATH += $$PWD/src

win32{

    #QMAKE_LFLAGS_DEBUG      = /DEBUG /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib
    QMAKE_LFLAGS_RELEASE     = /RELEASE /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib

    INCLUDEPATH += $$PWD/lib/win32/exosip/include \
                   $$PWD/lib/win32/mxml/include

    LIBS += $$PWD/lib/win32/exosip/lib/eXosip.lib \
            $$PWD/lib/win32/exosip/lib/osip2.lib \
            $$PWD/lib/win32/exosip/lib/osipparser2.lib \
            $$PWD/lib/win32/exosip/lib/libcares.lib \
            $$PWD/lib/win32/mxml/lib/mxml1.lib

    LIBS += WS2_32.lib Dnsapi.lib Iphlpapi.lib Qwave.lib delayimp.lib AdvAPI32.lib

}
