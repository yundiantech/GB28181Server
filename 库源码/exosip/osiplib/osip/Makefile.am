ACLOCAL_AMFLAGS = -I scripts
EXTRA_DIST = README BUGS FEATURES HISTORY autogen.sh libosip2.pc.in

SUBDIRS = include src scripts help platform

rpm-package:
	cd platform && $(MAKE) rpm-package

upload: dist
	@echo "######## Maintainer only ##########";
	@echo "building files for upload in ftp://ftp-upload.gnu.org/incoming/ftp";
	@echo "building: $(PACKAGE)-$(VERSION).tar.gz.sig";
	gpg -b $(PACKAGE)-$(VERSION).tar.gz
	echo "version: 1.1" > $(PACKAGE)-$(VERSION).tar.gz.directive
	echo "directory: osip" >> $(PACKAGE)-$(VERSION).tar.gz.directive
	echo "filename: $(PACKAGE)-$(VERSION).tar.gz" >> $(PACKAGE)-$(VERSION).tar.gz.directive
	@echo "building: $(PACKAGE)-$(VERSION).tar.gz.directive.asc";
	gpg --clearsign $(PACKAGE)-$(VERSION).tar.gz.directive

doxygen:
	doxygen help/doxygen/DoxyFile

indent:
	indent */*/*.c
	indent */*/*.h
	rm -rf */*/*.c~
	rm -rf */*/*.h~

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libosip2.pc
