include ../MakeDefns

auto:
	/usr/dar/tmp8/dfisher/software/doxygen-1.2.16/bin/doxygen config

clean:
	rm -rf html latex rtf man

install:
	$(INSTALL_DIR) $(pkgdatadir)/doc
	$(INSTALL_DIR) $(pkgdatadir)/JNIdoc/edu/umass/cs/indri/ui
	$(INSTALL_DATA) "Indri Query Language Reference 092004.doc" "Indri Query Language Reference 092004.pdf" "querylang.txt" $(pkgdatadir)/doc
	$(INSTALL_DATA) html/*.html $(pkgdatadir)/doc
	$(INSTALL_DATA) html/*.png $(pkgdatadir)/doc
	$(INSTALL_DATA) html/*.css $(pkgdatadir)/doc
	$(INSTALL_DATA) JNIdoc/*.html $(pkgdatadir)/JNIdoc
	$(INSTALL_DATA) JNIdoc/*.css $(pkgdatadir)/JNIdoc
	$(INSTALL_DATA) JNIdoc/edu/umass/cs/indri/*.html $(pkgdatadir)/JNIdoc/edu/umass/cs/indri
	$(INSTALL_DATA) JNIdoc/edu/umass/cs/indri/ui/*.html $(pkgdatadir)/JNIdoc/edu/umass/cs/indri/ui
