
include $(top_srcdir)/build/autotools/common.am

AM_CPPFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = sample

sample_SOURCES = sample.cpp
sample_CPPFLAGS = @LOL_CFLAGS@
sample_LDFLAGS = @LOL_STATIC_LIBS@ @LOL_LIBS@
sample_DEPENDENCIES = @LOL_STATIC_LIBS@

