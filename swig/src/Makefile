include ../../MakeDefns
CONTRIBDEPENDS=lemur antlr xpdf
INCPATH=-I../../include $(patsubst %, -I../../contrib/%/include, $(CONTRIBDEPENDS))
#LIBPATH=-L../../obj $(patsubst %,-L../../contrib/%/obj, $(CONTRIBDEPENDS))
#LIBS=-lindri $(patsubst %,-l%, $(CONTRIBDEPENDS))
LIBS=$(wildcard ../../obj/*.o) $(wildcard $(patsubst %, ../../contrib/%/obj/*.o, $(CONTRIBDEPENDS)))

LDFLAGS = $(LIBPATH) $(LIBS) $(CPPLDFLAGS)
SRC=interface/indri.i
PHPSRC=php/indri.i
LFLAGS=-shared
OBJ=../obj
OUTDIR=$(OBJ)/java/edu/umass/cs/indri
OUTDIRUI=$(OBJ)/java/edu/umass/cs/indri/ui
OUTDIRPROP=$(OBJ)/java/edu/umass/cs/indri/ui/properties
JSRC=java/edu/umass/cs/indri
JSRCUI=java/edu/umass/cs/indri/ui
JAVASRC = $(notdir $(wildcard $(JSRC)/*.java))
JAVASRCUI = $(notdir $(wildcard $(JSRCUI)/*.java))
ALLSRC= $(JAVASRC) $(JAVASRCUI)
PROPS = $(patsubst %,$(OUTDIRPROP)/%, $(notdir $(wildcard $(JSRCUI)/properties/*)))
INTERFACES = $(wildcard interface/*.i)
PHPINTERFACES = $(wildcard php/*.i)
JAVAOBJ = $(patsubst %,$(OUTDIR)/%, $(JAVASRC:.java=.class)) $(patsubst %,$(OUTDIRUI)/%, $(JAVASRCUI:.java=.class))
SOLIB=$(OBJ)/java/libindri.so
JARFILES=$(OBJ)/java/indri.jar $(OBJ)/java/RetUI.jar $(OBJ)/java/IndexUI.jar
MFST=java/manifest.indri
MFSTRET=java/manifest.RetUI
MFSTIND=java/manifest.IndexUI
PCK=edu.umass.cs.indri
PCKUI=edu.umass.cs.indri.ui
PHPINC=-I/usr/dan/users9/dfisher/ciir/local/include/php -I/usr/dan/users9/dfisher/ciir/local/include/php/main -I/usr/dan/users9/dfisher/ciir/local/include/php/Zend -I/usr/dan/users9/dfisher/ciir/local/include/php/TSRM
DOCARGS=-private -author -version
DOCS=$(patsubst %,$(OUTDIR)/%,$(patsubst %.java,%.html,$(JAVASRC)))
DOCSUI=$(patsubst %,$(OUTDIRUI)/%,$(patsubst %.java,%.html,$(JAVASRCUI)))
DOC=$(JAVAHOME)/bin/javadoc -classpath java  $(DOCARGS)

.PHONY: java-lib doc

#all: java-lib php/indri_phpwrap.cpp
all: java-lib php/libindri.so


java-lib: $(OUTDIR) $(OUTDIRUI) $(OUTDIRPROP) $(PROPS) $(SOLIB) doc $(JARFILES)

$(SOLIB): $(OBJ)/java/indri_javawrap.o $(LIBS)
	$(CXX) $(LFLAGS) $< -o $@ $(LDFLAGS)

$(OBJ)/java/indri_javawrap.o: java/indri_javawrap.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCPATH) $(JAVAINC)

java/indri_javawrap.cpp: $(INTERFACES)
	$(SWIG) -c++ -java -package edu.umass.cs.indri -o java/indri_javawrap.cpp -outdir $(JSRC) $(INCLUDES) $(SRC) 

php/indri_phpwrap.cpp: $(PHPINTERFACES)
	$(SWIG) -php4 -c++ -o php/indri_phpwrap.cpp -outdir php $(INCLUDES) $(PHPSRC) 
php/indri_phpwrap.o: php/indri_phpwrap.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCPATH) $(PHPINC)

php/libindri.so: php/indri_phpwrap.o $(LIBS)
	$(CXX) -g $(LFLAGS) $< -o $@ $(LDFLAGS)

$(OUTDIRPROP) $(OUTDIRUI) $(OUTDIR) $(OBJ)/java/docs:
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

$(DOCS) $(DOCSUI): $(OBJ)/java/docs

doc:
	$(DOC) -d $(OBJ)/java/docs -doctitle "Indri" -windowtitle "Indri" $(PCK) $(PCKUI)


clean:
	rm -rf $(OBJ)/java

install:
	$(INSTALL_DATA) $(JARFILES) $(pkgdatadir)
	$(INSTALL_PROGRAM) $(SOLIB) $(libdir)

