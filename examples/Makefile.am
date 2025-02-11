
include $(top_srcdir)/build/autotools/common.am

if BUILD_EXAMPLES
noinst_PROGRAMS = $(examples)
endif

examples = 00_empty

00_empty_SOURCES = 00_empty.cpp
00_empty_CPPFLAGS = $(AM_CPPFLAGS)
00_empty_LDFLAGS = @LOL_DEPS@
