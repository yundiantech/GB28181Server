ACLOCAL_AMFLAGS = -I scripts
EXTRA_DIST = README autogen.sh LICENSE.OpenSSL

SUBDIRS = scripts platform src include tools help

doxygen:
	doxygen help/doxygen/DoxyFile

upload: dist
	@echo "######## Maintainer only ##########";
	@echo "building files for upload in ftp://savannah.gnu.org/incoming/savannah/exosip";
	@echo "building: $(PACKAGE)-$(VERSION).tar.gz.sig";
	gpg -b $(PACKAGE)-$(VERSION).tar.gz

indent:
	indent */*/*.h
	indent */*.h
	indent */*.c
	rm -rf */*/*.h~
	rm -rf */*.h~
	rm -rf */*.c~
