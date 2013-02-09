
include $(top_srcdir)/build/autotools/common.am

AM_CPPFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = xolotl

xolotl_SOURCES = xolotl.cpp xolotl.h
xolotl_CPPFLAGS = @LOL_CFLAGS@
xolotl_LDFLAGS = @LOL_STATIC_LIBS@ @LOL_LIBS@
xolotl_DEPENDENCIES = @LOL_STATIC_LIBS@

