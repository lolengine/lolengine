
AM_CPPFLAGS = -I$(top_srcdir)/src

if USE_CPPUNIT
cppunit_tests = lol-test
endif

noinst_PROGRAMS = $(cppunit_tests)

TESTS = $(cppunit_tests)

lol_test_SOURCES = lol-test.cpp matrix.cpp
lol_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
lol_test_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
lol_test_LDADD = $(CPPUNIT_LIBS)
lol_test_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
lol_test_DEPENDENCIES = $(top_builddir)/src/liblol.a

