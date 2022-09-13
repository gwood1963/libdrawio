## -*- Mode: make; tab-width: 4; indent-tabs-mode: tabs -*-

if PLATFORM_WIN32
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) -avoid-version
else
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
endif

lib_LTLIBRARIES = libdrawio-@DRAWIO_MAJOR_VERSION@.@DRAWIO_MINOR_VERSION@.la
libdrawio_@DRAWIO_MAJOR_VERSION@_@DRAWIO_MINOR_VERSION@_includedir = $(includedir)/libdrawio-@DRAWIO_MAJOR_VERSION@.@DRAWIO_MINOR_VERSION@/libdrawio
libdrawio_@DRAWIO_MAJOR_VERSION@_@DRAWIO_MINOR_VERSION@_include_HEADERS = \
	$(top_srcdir)/inc/libdrawio/DRAWIODocument.h \
	$(top_srcdir)/inc/libdrawio/libdrawio.h

AM_CXXFLAGS = \
	-DLIBDRAWIO_BUILD=1 \
	-I$(top_srcdir)/inc \
	$(REVENGE_CFLAGS) \
	$(REVENGE_STREAM_CFLAGS) \
	$(BOOST_CFLAGS) \
	$(DEBUG_CXXFLAGS)

libdrawio_@DRAWIO_MAJOR_VERSION@_@DRAWIO_MINOR_VERSION@_la_LIBADD = \
	$(REVENGE_LIBS) \
	@LIBDRAWIO_WIN32_RESOURCE@

libdrawio_@DRAWIO_MAJOR_VERSION@_@DRAWIO_MINOR_VERSION@_la_DEPENDENCIES = @LIBDRAWIO_WIN32_RESOURCE@
libdrawio_@DRAWIO_MAJOR_VERSION@_@DRAWIO_MINOR_VERSION@_la_LDFLAGS = $(version_info) -export-dynamic -no-undefined
libdrawio_@DRAWIO_MAJOR_VERSION@_@DRAWIO_MINOR_VERSION@_la_SOURCES = \
	DRAWIODocument.cpp \
	libdrawio_utils.cpp \
	libdrawio_utils.h

if OS_WIN32

@LIBDRAWIO_WIN32_RESOURCE@ : libdrawio.rc $(libdrawio_@DRAWIO_MAJOR_VERSION@_@DRAWIO_MINOR_VERSION@_la_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource libdrawio.rc @LIBDRAWIO_WIN32_RESOURCE@

endif

EXTRA_DIST = \
	libdrawio.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	libdrawio.rc

## vim:set shiftwidth=4 tabstop=4 noexpandtab:
