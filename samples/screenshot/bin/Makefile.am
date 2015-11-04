include copperspice.am

nodist_screenshot_SOURCES=

# path to gcc (windows)
gccpath=/c/MingW/bin

# list of gcc libraries (windows)
gcclibs=libgcc_s_sjlj-1 libstdc++-6 libwinpthread-1

# list of CopperSpice libraries
cslibs=Core Gui

# location of includes
AM_CXXFLAGS+=-I..

if OSTYPE_WIN
# disable console on window
	AM_CXXFLAGS+=-mwindows

# location for 'make install'
	installdir=$(abs_top_builddir)/install
endif

# target app name
bin_PROGRAMS = screenshot

# copy required CopperSpice and gcc files for deployment
.PHONY: deploy

if OSTYPE_LINUX
deploy : all
	mkdir -p $(deploydir)
	$(LIBTOOL) --mode=install cp $(bin_PROGRAMS) $(deploydir)
	rsync -W -a $(patsubst %,$(CS_HOME)/lib/libCs%1.so*,$(cslibs)) $(deploydir)
endif

if OSTYPE_DARWIN
deployapp=$(deploydir)/$(bin_PROGRAMS).app/Contents

deploy : all
	mkdir -p $(deployapp)/Frameworks
	mkdir -p $(deployapp)/MacOS
	mkdir -p $(deployapp)/Resources
	$(LIBTOOL) --mode=install cp $(bin_PROGRAMS) $(deployapp)/MacOS
	rm -f $(deployapp)/Info.plist
	sed -e "s,@ICON@,,g" \
	-e "s,@TYPEINFO@,????,g" \
	-e "s,@EXECUTABLE@,$(bin_PROGRAMS),g" \
	$(CS_HOME)/mac/Info.plist.app > $(deployapp)/Info.plist
	rm -f $(deployapp)/PkgInfo
	echo "APPL????" > $(deployapp)/PkgInfo
	touch $(deployapp)/Resources/empty.lproj
	cp -r $(CS_HOME)/mac/qt_menu.nib $(deployapp)/Resources
	rsync -W -a $(patsubst %,$(CS_HOME)/lib/libCs%1.0.dylib,$(cslibs)) $(deployapp)/Frameworks
	for libname in $(patsubst %,libCs%1.0.dylib,$(cslibs)) ; do \
	\
	install_name_tool -change $(CS_HOME)/lib/$$libname \
	@executable_path/../Frameworks/$$libname $(deployapp)/MacOS/$(bin_PROGRAMS); \
	\
	for templib in $(patsubst %,libCs%1.0.dylib,$(cslibs)) ; do \
	install_name_tool -change $(CS_HOME)/lib/$$templib \
	@executable_path/../Frameworks/$$templib $(deployapp)/Frameworks/$$libname \
	; done \
	\
	; done
	hdiutil create -ov -srcfolder $(deploydir) -format UDBZ \
	-volname $(bin_PROGRAMS) ../$(bin_PROGRAMS).dmg
	hdiutil internet-enable -yes ../$(bin_PROGRAMS).dmg
endif

if OSTYPE_WIN
deploy : all
	mkdir -p $(deploydir)
	$(LIBTOOL) --mode=install cp $(bin_PROGRAMS) $(deploydir)
	rsync -W -a --modify-window=2 $(patsubst %,$(gccpath)/%.dll,$(gcclibs)) $(deploydir)
	rsync -W -a --modify-window=2 $(patsubst %,$(CS_HOME)/bin/libCs%1-0.dll,$(cslibs)) $(deploydir)
endif

# search path for source
VPATH = ../

# linking
screenshot_LDFLAGS=$(AM_LDFLAGS) $(patsubst %,-lCs%1,$(cslibs))

# source
screenshot_SOURCES = \
	src/main.cpp \
	src/screenshot.cpp

noinst_HEADERS = \
	src/screenshot.h

FORMS =

#TRANSLATIONS = $(wildcard ../translations/*)

#RESOURCES = screenshot.qrc

# use wilcard to obtain the dependency list
#qrc_screenshot.cpp: screenshot.qrc
#	$(wildcard ../images/*)

# required to compile qrc
#nodist_screenshot_SOURCES+=qrc_screenshot.cpp
