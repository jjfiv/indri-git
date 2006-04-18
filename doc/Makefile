-include ../MakeDefns
DOXYGEN = /usr/dan/users9/dfisher/ciir/doxygen-1.4.2/bin/doxygen
auto:
	$(DOXYGEN) config

install: $(pkgdatadir)/doc
	-$(INSTALL_DATA) html/* $<
	$(INSTALL_DATA) IndriQueryLanguage.html main.css $(pkgdatadir)

$(pkgdatadir)/doc:
	$(INSTALL_DIR) $(pkgdatadir)/doc

clean:
	rm -rf html latex rtf man
