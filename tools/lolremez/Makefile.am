
include $(top_srcdir)/build/autotools/common.am

EXTRA_DIST += NEWS.txt lolremez.sln lolremez.vcxproj lolremez.vcxproj.filters

if BUILD_TOOLS
noinst_PROGRAMS = lolremez
endif

lolremez_SOURCES = \
    lolremez.cpp solver.cpp solver.h matrix.h expression.h
lolremez_CPPFLAGS = $(AM_CPPFLAGS)
lolremez_DEPENDENCIES = @LOL_DEPS@

