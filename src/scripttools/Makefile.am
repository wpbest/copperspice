lib_LTLIBRARIES += lib/libCsScriptTools1.la
lib_libCsScriptTools1_la_SOURCES =

lib_libCsScriptTools1_la_CXXFLAGS = $(AM_CXXFLAGS) -Iinclude -Iinclude/QtCore -Iinclude/QtGui \
   -Iinclude/QtScriptTools -Iprivateinclude/QtCore -Iprivateinclude/QtGui   \
   -Iprivateinclude/QtScriptTools -Isrc/scripttools/debugging   \
	-DQT_BUILD_SCRIPTTOOLS_LIB \
	-DQT_SHARED   \
	-DQT_MAKEDLL

lib_libCsScriptTools1_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined
lib_libCsScriptTools1_la_LIBADD = lib/libCsCore1.la lib/libCsScript1.la lib/libCsGui1.la

if OSTYPE_LINUX
lib_libCsScriptTools1_la_CXXFLAGS += -I$(top_srcdir)/mkspecs/linux-g++
endif

if OSTYPE_WIN
lib_libCsScriptTools1_la_CXXFLAGS += -I$(top_srcdir)/mkspecs/win32-g++
endif

if OSTYPE_DARWIN
lib_libCsScriptTools1_la_CXXFLAGS += -I$(top_srcdir)/mkspecs/macx-g++
endif

SCRIPTTOOLS_INCLUDES =
SCRIPTTOOLS_PUBLIC_INCLUDES =
SCRIPTTOOLS_PRIVATE_INCLUDES =
SCRIPTTOOLS_DIRS =

include src/scripttools/debugging/Makefile.am

SCRIPTTOOLS_HEADERS_INSTALLED= \
	$(patsubst %,include/QtScriptTools/%,$(SCRIPTTOOLS_INCLUDES)) \
	$(patsubst %,include/QtScriptTools/%,$(SCRIPTTOOLS_PUBLIC_INCLUDES)) \
	$(patsubst %,privateinclude/QtScriptTools/private/%,$(SCRIPTTOOLS_PRIVATE_INCLUDES))
	include/QtScriptTools privateinclude/QtScriptTools/private

BUILT_SOURCES+=.scripttools-header-deps.mk

$(lib_libCsScriptTools1_la_SOURCES) : $(CORE_HEADERS_INSTALLED) $(GUI_HEADERS_INSTALLED) \
      $(SCRIPT_HEADERS_INSTALLED)  $(SCRIPTTOOLS_HEADERS_INSTALLED)

-include .scripttools-header-deps.mk

.scripttools-header-deps.mk: Makefile
	@- rm -rf $@
	echo "include/QtScriptTools:" >> $@; \
	echo "	\$$(MKDIR_P) \$$@" >> $@ ; \
	echo "privateinclude/QtScriptTools/private:" >> $@; \
	echo "	\$$(MKDIR_P) \$$@" >> $@ ; \
	for i in $(SCRIPTTOOLS_PUBLIC_INCLUDES); do \
		echo "include/QtScriptTools/$$i: include/QtScriptTools/$$(echo $$i | tr A-Z a-z).h | include/QtScriptTools" >> $@; \
		echo "	\$$(INSTALL) \$$< \$$@" >> $@ ; \
	done; \
	for i in $(SCRIPTTOOLS_DIRS); do \
		echo "include/QtScriptTools/%: $$i/% | include/QtScriptTools" >> $@ ; \
		echo "	\$$(INSTALL) \$$< \$$@" >> $@ ; \
	done;\
	for i in $(SCRIPTTOOLS_DIRS); do \
		echo "privateinclude/QtScriptTools/private/%: $$i/% | privateinclude/QtScriptTools/private" >> $@ ; \
		echo "	\$$(INSTALL) \$$< \$$@" >> $@ ; \
	done
