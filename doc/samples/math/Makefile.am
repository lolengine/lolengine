
include $(top_srcdir)/build/autotools/common.am

if BUILD_SAMPLES
noinst_PROGRAMS = pi poly
endif

pi_SOURCES = pi.cpp
pi_CPPFLAGS = $(AM_CPPFLAGS)
pi_DEPENDENCIES = @LOL_DEPS@

poly_SOURCES = poly.cpp
poly_CPPFLAGS = $(AM_CPPFLAGS)
poly_DEPENDENCIES = @LOL_DEPS@

