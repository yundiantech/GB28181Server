
SOURCES += \
    $$PWD/src/AppConfig.cpp \
    $$PWD/src/Mutex/Cond.cpp \
    $$PWD/src/Mutex/Mutex.cpp \
    $$PWD/src/GB28181Server/GB28181Server.cpp \
    $$PWD/src/MD5/HTTPDigest.cpp \
    $$PWD/src/MD5/MD5.cpp \
    $$PWD/src/RtpReciever/RtpReciever.cpp \
    $$PWD/src/Device/CameraDevice.cpp \
    $$PWD/src/Device/VideoChannel.cpp \
    $$PWD/src/Thread/CommonThread.cpp \
    $$PWD/src/Thread/DataListThread.cpp

HEADERS += \
    $$PWD/src/types.h \
    $$PWD/src/AppConfig.h \
    $$PWD/src/Mutex/Cond.h \
    $$PWD/src/Mutex/Mutex.h \
    $$PWD/src/GB28181Server/GB28181Server.h \
    $$PWD/src/MD5/HTTPDigest.h \
    $$PWD/src/MD5/MD5.h \
    $$PWD/src/EventHandle/EventHandle.h \
    $$PWD/src/RtpReciever/RtpReciever.h \
    $$PWD/src/Device/CameraDevice.h \
    $$PWD/src/Device/VideoChannel.h \
    $$PWD/src/Thread/CommonThread.h \
    $$PWD/src/Thread/DataListThread.h

INCLUDEPATH += $$PWD/src

win32{

    #QMAKE_LFLAGS_DEBUG      = /DEBUG /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib
    QMAKE_LFLAGS_RELEASE     = /RELEASE /NODEFAULTLIB:libc.lib /NODEFAULTLIB:libcmt.lib

    include($$PWD/lib/win32/jrtplib/jrtplib.pri)

    INCLUDEPATH += $$PWD/lib/win32/exosip/include \
                   $$PWD/lib/win32/mxml/include

    LIBS += $$PWD/lib/win32/exosip/lib/eXosip.lib \
            $$PWD/lib/win32/exosip/lib/osip2.lib \
            $$PWD/lib/win32/exosip/lib/osipparser2.lib \
            $$PWD/lib/win32/exosip/lib/libcares.lib \
            $$PWD/lib/win32/mxml/lib/mxml1.lib

    LIBS += WS2_32.lib Dnsapi.lib Iphlpapi.lib Qwave.lib delayimp.lib AdvAPI32.lib

    #解决vs2017报 无法解析的外部符号 __vsnprintf
    LIBS += legacy_stdio_definitions.lib

    contains(QT_ARCH, i386) {
        message("32-bit")
        INCLUDEPATH += $$PWD/lib/win32/ffmpeg/include \
                       $$PWD/src

        LIBS += -L$$PWD/lib/win32/ffmpeg/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale

    } else {
        message("64-bit")
        INCLUDEPATH += $$PWD/lib/win64/ffmpeg/include \
                       $$PWD/src

        LIBS += -L$$PWD/lib/win64/ffmpeg/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lpostproc -lswresample -lswscale
    }
}
