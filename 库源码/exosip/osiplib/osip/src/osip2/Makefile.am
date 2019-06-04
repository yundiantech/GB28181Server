
EXTRA_DIST = fsm.h xixt.h

lib_LTLIBRARIES = libosip2.la

libosip2_la_SOURCES =  \
ict_fsm.c      ist_fsm.c      nict_fsm.c          nist_fsm.c    \
ict.c          ist.c          nict.c              nist.c        \
fsm_misc.c     osip.c         osip_transaction.c  osip_event.c  \
port_fifo.c    osip_dialog.c  osip_time.c

if BUILD_MT
libosip2_la_SOURCES+=port_sema.c port_thread.c port_condv.c
endif

libosip2_la_LDFLAGS = -version-info $(LIBOSIP_SO_VERSION) ../osipparser2/libosipparser2.la $(FSM_LIB) $(EXTRA_LIB) -no-undefined


AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = $(SIP_CFLAGS) $(SIP_FSM_FLAGS) $(SIP_EXTRA_FLAGS)


