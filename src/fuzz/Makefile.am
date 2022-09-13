## -*- Mode: make; tab-width: 4; indent-tabs-mode: tabs -*-

noinst_PROGRAMS = drawiofuzzer

AM_CXXFLAGS = \
	-I$(top_srcdir)/inc \
	$(REVENGE_CFLAGS) \
	$(REVENGE_GENERATORS_CFLAGS) \
	$(REVENGE_STREAM_CFLAGS) \
	$(DEBUG_CXXFLAGS)

drawiofuzzer_LDADD = \
	$(top_builddir)/src/lib/libdrawio-@DRAWIO_MAJOR_VERSION@.@DRAWIO_MINOR_VERSION@.la \
	$(REVENGE_LIBS) \
	$(REVENGE_GENERATORS_LIBS) \
	$(REVENGE_STREAM_LIBS) \
	-lFuzzingEngine

drawiofuzzer_SOURCES = \
	drawiofuzzer.cpp

## vim:set shiftwidth=4 tabstop=4 noexpandtab:
