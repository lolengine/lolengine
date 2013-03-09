
include $(top_srcdir)/build/autotools/common.am

EXTRA_DIST += NEWS.txt lolremez.sln remez.vcxproj remez.vcxproj.filters

noinst_PROGRAMS = pi poly remez

pi_SOURCES = pi.cpp
pi_CPPFLAGS = $(AM_CPPFLAGS)
pi_DEPENDENCIES = @LOL_DEPS@

poly_SOURCES = poly.cpp
poly_CPPFLAGS = $(AM_CPPFLAGS)
poly_DEPENDENCIES = @LOL_DEPS@

remez_SOURCES = remez.cpp
remez_CPPFLAGS = $(AM_CPPFLAGS)
remez_DEPENDENCIES = @LOL_DEPS@

