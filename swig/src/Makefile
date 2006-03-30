include ../../MakeDefns
INCPATH=-I../../include $(patsubst %, -I../../contrib/%/include, $(DEPENDENCIES))
LIBPATH=-L../../obj $(patsubst %,-L../../contrib/%/obj, $(DEPENDENCIES))
LIBS=-lindri $(patsubst %,-l%, $(DEPENDENCIES))
LIBDEPS=$(wildcard ../../obj/*.o) $(wildcard $(patsubst %, ../../contrib/%/obj/*.o, $(DEPENDENCIES)))

LDFLAGS = $(LIBPATH) $(LIBS) $(CPPLDFLAGS)
SRC=interface/indri.i
PHPSRC=php/indri.i

OBJ=../obj
OUTDIR=$(OBJ)/java/edu/umass/cs/indri
OUTDIRUI=$(OBJ)/java/edu/umass/cs/indri/ui
OUTDIRPROP=$(OBJ)/java/edu/umass/cs/indri/ui/properties
OUTDIRPHP=$(OBJ)/php
JSRC=java/edu/umass/cs/indri
JSRCUI=java/edu/umass/cs/indri/ui
JAVASRC = $(notdir $(wildcard $(JSRC)/*.java))
JAVASRCUI = $(notdir $(wildcard $(JSRCUI)/*.java))
ALLSRC= $(wildcard $(JSRC)/*.java) $(wildcard $(JSRCUI)/*.java)
PROPS = $(patsubst %,$(OUTDIRPROP)/%, $(notdir $(wildcard $(JSRCUI)/properties/*)))
INTERFACES = $(wildcard interface/*.i)
PHPINTERFACES = $(wildcard php/*.i)
JAVAOBJ = $(patsubst %,$(OUTDIR)/%, $(JAVASRC:.java=.class)) $(patsubst %,$(OUTDIRUI)/%, $(JAVASRCUI:.java=.class))
SOLIB=$(OBJ)/java/libindri_jni$(JAVA_SO)
PHPLIB=$(OUTDIRPHP)/libindri_php$(PHP_SO)
JARFILES=$(OBJ)/java/indri.jar $(OBJ)/java/RetUI.jar $(OBJ)/java/IndexUI.jar
MFST=java/manifest.indri
MFSTRET=java/manifest.RetUI
MFSTIND=java/manifest.IndexUI
PCK=edu.umass.cs.indri
PCKUI=edu.umass.cs.indri.ui

DOCARGS=-private -author -version
DOCS=$(patsubst %,$(OUTDIR)/%,$(patsubst %.java,%.html,$(JAVASRC)))
DOCSUI=$(patsubst %,$(OUTDIRUI)/%,$(patsubst %.java,%.html,$(JAVASRCUI)))
DOC=$(JAVAHOME)/bin/javadoc -classpath java  $(DOCARGS)
TARGETS=
ifeq ($(WITH_SWIG), 1)
  ifeq ($(WITH_JAVA), 1)
    TARGETS += java-lib doc
  endif
  ifeq ($(WITH_PHP), 1)
    TARGETS += php-lib
  endif
endif
.PHONY: java-lib php-lib doc

#all: java-lib php/indri_phpwrap.cpp
#remove doc from target
all: $(TARGETS)


java-lib: $(OUTDIR) $(OUTDIRUI) $(OUTDIRPROP) $(OBJ)/java/docs $(PROPS) $(SOLIB) $(JARFILES)

php-lib: $(OUTDIRPHP) $(PHPLIB)

$(SOLIB): $(OBJ)/java/indri_javawrap.o $(LIBDEPS)
	$(CXX) $(JAVA_SHARED) $< -o $@ $(LDFLAGS)

$(OBJ)/java/indri_javawrap.o: java/indri_javawrap.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCPATH) $(JAVAINC)

java/indri_javawrap.cpp: $(INTERFACES)
	$(SWIG) -c++ -java -package edu.umass.cs.indri -o java/indri_javawrap.cpp -outdir $(JSRC) $(INCLUDES) $(SRC) 

$(OUTDIRPROP) $(OUTDIRUI) $(OUTDIR) $(OBJ)/java/docs $(OUTDIRPHP):
	$(INSTALL_DIR) $@

$(OUTDIR)/%.class:$(JSRC)/%.java
	$(JAVAC) -classpath java -d $(OBJ)/java $<

$(OUTDIRUI)/%.class:$(JSRCUI)/%.java
	$(JAVAC) -classpath java -d $(OBJ)/java $<

$(OUTDIRPROP)/%:$(JSRCUI)/properties/%
	cp $< $@

$(OBJ)/java/indri.jar: $(JAVAOBJ) $(MFST)
	rm -f $@
	cd $(OBJ)/java; $(JAR) cmf ../../src/$(MFST) indri.jar edu docs

$(OBJ)/java/RetUI.jar: $(JAVAOBJ) $(MFSTRET)
	rm -f $@
	cd $(OBJ)/java; $(JAR) cmf ../../src/$(MFSTRET) RetUI.jar edu docs

$(OBJ)/java/IndexUI.jar: $(JAVAOBJ) $(MFSTIND)
	rm -f $@
	cd $(OBJ)/java; $(JAR) cmf ../../src/$(MFSTIND) IndexUI.jar edu docs

php/indri_phpwrap.cpp: $(PHPINTERFACES)
	$(SWIG) -php4 -c++ -o php/indri_phpwrap.cpp -outdir php $(INCLUDES) $(PHPSRC) 

$(OUTDIRPHP)/indri_phpwrap.o: php/indri_phpwrap.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCPATH) $(PHPINCLUDE)

$(PHPLIB): $(OUTDIRPHP)/indri_phpwrap.o $(LIBDEPS)
	$(CXX) -g $(PHP_SHARED) $< -o $@ $(LDFLAGS)


$(DOCS) $(DOCSUI): $(OBJ)/java/docs

$(OBJ)/java/docs.needed: $(ALLSRC)
	touch $@
	$(DOC) -d $(OBJ)/java/docs -doctitle "Indri" -windowtitle "Indri" $(PCK) $(PCKUI)

doc: $(OBJ)/java/docs.needed



clean:
	rm -rf $(OBJ)/java $(OUTDIRPHP)
## need to add target for php solib (and fix naming conventions)
install: 
ifeq ($(WITH_SWIG), 1)
  ifeq ($(WITH_JAVA), 1)
	$(INSTALL_DIR) $(pkgdatadir)/JNIdoc
	cp -pr $(OBJ)/java/docs $(pkgdatadir)/JNIdoc
	$(INSTALL_DATA) $(JARFILES) $(pkgdatadir)
	$(INSTALL_PROGRAM) $(SOLIB) $(libdir)
  endif
  ifeq ($(WITH_PHP), 1)
	$(INSTALL_PROGRAM) $(PHPLIB) $(libdir)
  endif
endif
