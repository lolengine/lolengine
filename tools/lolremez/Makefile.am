
include $(top_srcdir)/build/autotools/common.am

EXTRA_DIST += NEWS.txt lolremez.sln remez.vcxproj remez.vcxproj.filters

noinst_PROGRAMS = lolremez

lolremez_SOURCES = lolremez.cpp
lolremez_CPPFLAGS = $(AM_CPPFLAGS)
lolremez_DEPENDENCIES = @LOL_DEPS@

