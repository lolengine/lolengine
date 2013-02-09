
include $(top_srcdir)/build/autotools/common.am

AM_CPPFLAGS = -I$(top_srcdir)/src

EXTRA_DIST = NEWS.txt lolremez.sln remez.vcxproj remez.vcxproj.filters

noinst_PROGRAMS = pi poly remez

pi_SOURCES = pi.cpp
pi_CPPFLAGS = @LOL_CFLAGS@
pi_LDFLAGS = @LOL_STATIC_LIBS@ @LOL_LIBS@
pi_DEPENDENCIES = @LOL_STATIC_LIBS@

poly_SOURCES = poly.cpp
poly_CPPFLAGS = @LOL_CFLAGS@
poly_LDFLAGS = @LOL_STATIC_LIBS@ @LOL_LIBS@
poly_DEPENDENCIES = @LOL_STATIC_LIBS@

remez_SOURCES = remez.cpp
remez_CPPFLAGS = @LOL_CFLAGS@
remez_LDFLAGS = @LOL_STATIC_LIBS@ @LOL_LIBS@
remez_DEPENDENCIES = @LOL_STATIC_LIBS@

