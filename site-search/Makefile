LEMURDIR = ..
include $(LEMURDIR)/MakeDefns
INSTDIR=$(pkgdatadir)
SUBDIRS=php cgi

all: 
	$(MAKE) -C php all
clean:
	$(MAKE) -C php clean

$(INSTDIR):
	$(INSTALL_DIR) $@

install: $(INSTDIR)
	$(INSTALL_DATA) README $(INSTDIR)/README.site-search
	$(MAKE) -C php install
