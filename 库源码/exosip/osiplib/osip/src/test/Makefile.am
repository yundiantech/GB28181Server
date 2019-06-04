
EXTRA_DIST = tst CHECK res

if COMPILE_TESTS
noinst_PROGRAMS = torture_test turl tfrom tto tcontact tvia tcallid tcontentt trecordr troute twwwa

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src/osipparser2
AM_CFLAGS = $(SIP_CFLAGS) $(SIP_PARSER_FLAGS) $(SIP_EXTRA_FLAGS)

twwwa_SOURCES =  twwwa.c
twwwa_LDADD = $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)

tcontentt_SOURCES =  tcontentt.c
tcontentt_LDADD = $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)

turl_SOURCES =  turls.c
turl_LDADD = $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)

tfrom_SOURCES =  tfrom.c
tfrom_LDADD =  $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)

tto_SOURCES =  tto.c
tto_LDADD = $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)


tcontact_SOURCES =  tcontact.c
tcontact_LDADD =  $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)

trecordr_SOURCES =  trecordr.c
trecordr_LDADD =  $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)

troute_SOURCES =  troute.c
troute_LDADD =  $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)

tvia_SOURCES =  tvia.c
tvia_LDADD = $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)

tcallid_SOURCES =  tcallid.c
tcallid_LDADD = $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)

torture_test_SOURCES =  torture.c
torture_test_LDADD = $(top_builddir)/src/osipparser2/libosipparser2.la $(PARSER_LIB) $(EXTRA_LIB)



check:
	@echo " *******************************"
	@echo " ****** starting tests! ********"
	@echo " *******************************"
	@./$(top_srcdir)/src/test/tst ./$(top_srcdir)/src/test/res -c

	@echo ""
	@echo "In case you have a doubt, send the generated"
	@echo "log file with your comment to <amoizard@antisip.com>."
else
check:
	@echo " ************************************"
	@echo " Please use ./configure --enable-test"
	@echo " ************************************"
endif
