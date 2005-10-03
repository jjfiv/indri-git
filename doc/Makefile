-include ../MakeDefns
auto:
	/usr/bin/doxygen config

install: $(pkgdatadir)/doc
	$(INSTALL_DATA) html/* $<
	$(INSTALL_DATA) IndriQueryLanguage.html main.css $(pkgdatadir)

$(pkgdatadir)/doc:
	$(INSTALL_DIR) $(pkgdatadir)/doc

clean:
	rm -rf html latex rtf man
