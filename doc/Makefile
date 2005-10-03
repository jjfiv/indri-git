-include ../MakeDefns
auto:
	/usr/bin/doxygen config

install: $(pkgdatadir)/doc
	$(INSTALL_DATA) IndriQueryLanguage.html main.css html/* $<

$(pkgdatadir)/doc:
	$(INSTALL_DIR) $(pkgdatadir)/doc

clean:
	rm -rf html latex rtf man
