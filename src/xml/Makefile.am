lib_LTLIBRARIES += lib/libCsXml1.la
lib_libCsXml1_la_SOURCES =

lib_libCsXml1_la_CXXFLAGS = $(AM_CXXFLAGS) -Iinclude -Iinclude/QtCore -Iinclude/QtXml -Iprivateinclude/QtCore/private  \
	-DQT_BUILD_XML_LIB \
	-DQT_SHARED   \
	-DQT_MAKEDLL

lib_libCsXml1_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined
lib_libCsXml1_la_LIBADD = lib/libCsCore1.la

if OSTYPE_LINUX
lib_libCsXml1_la_CXXFLAGS += -I$(top_srcdir)/mkspecs/linux-g++
endif

if OSTYPE_WIN
lib_libCsXml1_la_CXXFLAGS += -I$(top_srcdir)/mkspecs/win32-g++
endif

if OSTYPE_DARWIN
lib_libCsXml1_la_CXXFLAGS += -I$(top_srcdir)/mkspecs/macx-g++
endif

XML_INCLUDES =
XML_PUBLIC_INCLUDES =
XML_PRIVATE_INCLUDES =
XML_DIRS =

include src/xml/dom/Makefile.am
include src/xml/sax/Makefile.am
include src/xml/stream/Makefile.am

XML_HEADERS_INSTALLED= \
	$(patsubst %,include/QtXml/%,$(XML_INCLUDES)) \
	$(patsubst %,include/QtXml/%,$(XML_PUBLIC_INCLUDES)) \
	$(patsubst %,privateinclude/QtXml/private/%,$(XML_PRIVATE_INCLUDES)) \
	include/QtXml \
	privateinclude/QtXml/private

BUILT_SOURCES+=.xml-header-deps.mk

$(lib_libCsXml1_la_SOURCES) : $(CORE_HEADERS_INSTALLED) $(XML_HEADERS_INSTALLED) 

-include .xml-header-deps.mk

.xml-header-deps.mk: Makefile
	@- rm -rf $@
	echo "include/QtXml:" >> $@; \
	echo "	\$$(MKDIR_P) \$$@" >> $@ ; \
	echo "privateinclude/QtXml/private:" >> $@; \
	echo "	\$$(MKDIR_P) \$$@" >> $@ ; \
	for i in $(XML_PUBLIC_INCLUDES); do \
		echo "include/QtXml/$$i: include/QtXml/$$(echo $$i | tr A-Z a-z).h | include/QtXml" >> $@; \
		echo "	\$$(INSTALL) \$$< \$$@" >> $@ ; \
	done; \
	for i in $(XML_DIRS); do \
		echo "include/QtXml/%: $$i/% | include/QtXml" >> $@ ; \
		echo "	\$$(INSTALL) \$$< \$$@" >> $@ ; \
	done;\
	for i in $(XML_DIRS); do \
		echo "privateinclude/QtXml/private/%: $$i/% | privateinclude/QtXml/private" >> $@ ; \
		echo "	\$$(INSTALL) \$$< \$$@" >> $@ ; \
	done
