
include $(top_srcdir)/build/autotools/common.am

EXTRA_DIST += NEWS.txt lolremez.sln remez.vcxproj remez.vcxproj.filters

noinst_PROGRAMS = lolremez

lolremez_SOURCES = \
    lolremez.cpp solver.cpp solver.h matrix.h
lolremez_CPPFLAGS = $(AM_CPPFLAGS)
lolremez_DEPENDENCIES = @LOL_DEPS@

