

lib_LTLIBRARIES = libeXosip2.la

#
# Other flags to add someday:
#	-Wcast-qual
#	-Wwrite-strings
#	-Wstrict-prototypes
#	-Werror
#
AM_CFLAGS = $(EXOSIP_FLAGS)

libeXosip2_la_SOURCES =           \
eXosip.c         eXconf.c         \
eXregister_api.c eXcall_api.c     \
eXmessage_api.c  eXtransport.c    \
jrequest.c       jresponse.c      \
jcallback.c      jdialog.c        \
udp.c            jcall.c          \
jreg.c           eXutils.c        \
jevents.c        misc.c           \
jpipe.c          jpipe.h          \
jauth.c          eXtransport.h    \
eXosip2.h

libeXosip2_la_SOURCES+= \
eXtl_udp.c \
eXtl_tcp.c \
eXtl_dtls.c \
eXtl_tls.c

libeXosip2_la_SOURCES+= \
milenage.c \
rijndael.c \
milenage.h \
rijndael.h

if BUILD_MAXSIZE
libeXosip2_la_SOURCES+= \
eXsubscription_api.c    eXoptions_api.c    \
eXinsubscription_api.c  eXpublish_api.c    \
jnotify.c               jsubscribe.c       \
inet_ntop.c             inet_ntop.h        \
jpublish.c              sdp_offans.c
endif

libeXosip2_la_LDFLAGS = -version-info $(LIBEXOSIP_SO_VERSION) -no-undefined
libeXosip2_la_LIBADD = $(EXOSIP_LIB) $(OSIP_LIBS)

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/include $(OSIP_CFLAGS)
