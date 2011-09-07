
AM_CPPFLAGS = -I$(top_srcdir)/src

bench: lol-bench$(EXEEXT)
	./lol-bench$(EXEEXT)

all-local: quad$(EXEEXT) lol-bench$(EXEEXT)
	test x$(MAKE_FSELF) = xno || make_fself quad$(EXEEXT) quad.self
	test x$(MAKE_FSELF) = xno || make_fself sandbox$(EXEEXT) sandbox.self
	test x$(MAKE_FSELF) = xno || make_fself lol-bench$(EXEEXT) lol-bench.self
	test x$(MAKE_FSELF) = xno || make_fself lol-test$(EXEEXT) lol-test.self

CLEANFILES = quad.self sandbox lol-bench.self lol-test.self

noinst_PROGRAMS = quad sandbox lol-bench lol-test

TESTS = lol-test

lol_test_SOURCES = lol-test.cpp matrix.cpp half.cpp trig.cpp
lol_test_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
lol_test_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
lol_test_DEPENDENCIES = $(top_builddir)/src/liblol.a

lol_bench_SOURCES = lol-bench.cpp
lol_bench_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
lol_bench_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
lol_bench_DEPENDENCIES = $(top_builddir)/src/liblol.a

quad_SOURCES = debug/quad.cpp
quad_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
quad_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
quad_DEPENDENCIES = $(top_builddir)/src/liblol.a

sandbox_SOURCES = debug/sandbox.cpp
sandbox_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
sandbox_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
sandbox_DEPENDENCIES = $(top_builddir)/src/liblol.a

