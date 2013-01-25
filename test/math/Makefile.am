
include $(top_srcdir)/build/autotools/common.am

AM_CPPFLAGS = -I$(top_srcdir)/src

EXTRA_DIST = NEWS.txt lolremez.sln remez.vcxproj remez.vcxproj.filters

noinst_PROGRAMS = pi poly remez

pi_SOURCES = pi.cpp
pi_CPPFLAGS = @LOL_CFLAGS@
pi_LDFLAGS = -u_SDL_main -uSDL_main $(top_builddir)/src/liblol.a @LOL_LIBS@
pi_DEPENDENCIES = $(top_builddir)/src/liblol.a

poly_SOURCES = poly.cpp
poly_CPPFLAGS = @LOL_CFLAGS@
poly_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@
poly_DEPENDENCIES = $(top_builddir)/src/liblol.a

remez_SOURCES = remez.cpp
remez_CPPFLAGS = @LOL_CFLAGS@
remez_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@
remez_DEPENDENCIES = $(top_builddir)/src/liblol.a

