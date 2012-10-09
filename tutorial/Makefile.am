
include $(top_srcdir)/build/autotools/common.am

AM_CPPFLAGS = -I$(top_srcdir)/src

noinst_PROGRAMS = 01_triangle 02_cube 03_noise 04_texture 05_easymesh \
                  08_fbo 11_fractal

01_triangle_SOURCES = 01_triangle.cpp 01_triangle.lolfx
01_triangle_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
01_triangle_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
01_triangle_DEPENDENCIES = $(top_builddir)/src/liblol.a

02_cube_SOURCES = 02_cube.cpp 02_cube.lolfx
02_cube_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
02_cube_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
02_cube_DEPENDENCIES = $(top_builddir)/src/liblol.a

03_noise_SOURCES = 03_noise.cpp 03_noise.lolfx
03_noise_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
03_noise_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
03_noise_DEPENDENCIES = $(top_builddir)/src/liblol.a

04_texture_SOURCES = 04_texture.cpp 04_texture.lolfx
04_texture_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
04_texture_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
04_texture_DEPENDENCIES = $(top_builddir)/src/liblol.a

05_easymesh_SOURCES = 05_easymesh.cpp
05_easymesh_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
05_easymesh_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
05_easymesh_DEPENDENCIES = $(top_builddir)/src/liblol.a

08_fbo_SOURCES = 08_fbo.cpp 08_fbo.lolfx
08_fbo_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
08_fbo_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
08_fbo_DEPENDENCIES = $(top_builddir)/src/liblol.a

11_fractal_SOURCES = 11_fractal.cpp 11_fractal.lolfx
11_fractal_CPPFLAGS = @LOL_CFLAGS@ @PIPI_CFLAGS@
11_fractal_LDFLAGS = $(top_builddir)/src/liblol.a @LOL_LIBS@ @PIPI_LIBS@
11_fractal_DEPENDENCIES = $(top_builddir)/src/liblol.a

