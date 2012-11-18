
include $(top_srcdir)/build/autotools/common.am

AM_CPPFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = xolotl

xolotl_SOURCES = xolotl.cpp xolotl.h
xolotl_CPPFLAGS = @LOL_CFLAGS@
xolotl_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@
xolotl_DEPENDENCIES = $(top_builddir)/src/liblol.a

