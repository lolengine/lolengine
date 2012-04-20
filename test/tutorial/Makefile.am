
AM_CPPFLAGS = -I$(top_srcdir)/src

all-local: $(noinst_PROGRAMS)
	for x in $(noinst_PROGRAMS); do \
	  test x$(MAKE_FSELF) = xno || make_fself $${x}$(EXEEXT) $${x}.self; \
	done

CLEANFILES = $(noinst_PROGRAMS:%$(EXEEXT)=%.self) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.elf) \
             $(noinst_PROGRAMS:%$(EXEEXT)=%.exe)

noinst_PROGRAMS = 01_triangle 02_cube 03_fractal

01_triangle_SOURCES = 01_triangle.cpp
01_triangle_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
01_triangle_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
01_triangle_DEPENDENCIES = $(top_builddir)/src/liblol.a

02_cube_SOURCES = 02_cube.cpp
02_cube_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
02_cube_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
02_cube_DEPENDENCIES = $(top_builddir)/src/liblol.a

03_fractal_SOURCES = 03_fractal.cpp
03_fractal_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
03_fractal_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
03_fractal_DEPENDENCIES = $(top_builddir)/src/liblol.a

