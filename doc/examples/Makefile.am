
include $(top_srcdir)/build/autotools/common.am

if BUILD_EXAMPLES
noinst_PROGRAMS = $(examples)
endif

examples = window

window_SOURCES = window.cpp
window_CPPFLAGS = $(AM_CPPFLAGS)
window_LDFLAGS = @LOL_DEPS@
