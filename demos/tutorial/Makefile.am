
include $(top_srcdir)/build/autotools/common.am

noinst_PROGRAMS = 01_triangle 02_cube 03_noise 04_texture 05_easymesh \
                  06_sprite 07_input 08_fbo 11_fractal \
                  12_voronoi

01_triangle_SOURCES = 01_triangle.cpp 01_triangle.lolfx
01_triangle_CPPFLAGS = $(AM_CPPFLAGS)
01_triangle_DEPENDENCIES = @LOL_DEPS@

02_cube_SOURCES = 02_cube.cpp 02_cube.lolfx
02_cube_CPPFLAGS = $(AM_CPPFLAGS)
02_cube_DEPENDENCIES = @LOL_DEPS@

03_noise_SOURCES = 03_noise.cpp 03_noise.lolfx
03_noise_CPPFLAGS = $(AM_CPPFLAGS)
03_noise_DEPENDENCIES = @LOL_DEPS@

04_texture_SOURCES = 04_texture.cpp 04_texture.lolfx
04_texture_CPPFLAGS = $(AM_CPPFLAGS)
04_texture_DEPENDENCIES = @LOL_DEPS@

05_easymesh_SOURCES = 05_easymesh.cpp
05_easymesh_CPPFLAGS = $(AM_CPPFLAGS)
05_easymesh_DEPENDENCIES = @LOL_DEPS@

06_sprite_SOURCES = 06_sprite.cpp 06_sprite.png
06_sprite_CPPFLAGS = $(AM_CPPFLAGS)
06_sprite_DEPENDENCIES = @LOL_DEPS@
06_sprite_LDFLAGS = $(AM_LDFLAGS)
if USE_EMSCRIPTEN
06_sprite_LDFLAGS += --preload-file 06_sprite.png
endif

07_input_SOURCES = 07_input.cpp 07_input.lolfx
07_input_CPPFLAGS = $(AM_CPPFLAGS)
07_input_DEPENDENCIES = @LOL_DEPS@

08_fbo_SOURCES = 08_fbo.cpp 08_fbo.lolfx
08_fbo_CPPFLAGS = $(AM_CPPFLAGS)
08_fbo_DEPENDENCIES = @LOL_DEPS@

11_fractal_SOURCES = 11_fractal.cpp 11_fractal.lolfx
11_fractal_CPPFLAGS = $(AM_CPPFLAGS)
11_fractal_DEPENDENCIES = @LOL_DEPS@

12_voronoi_SOURCES = 12_voronoi.cpp 12_voronoi.lolfx \
                     12_voronoi_distance.lolfx 12_voronoi_setup.lolfx \
                     12_texture_to_screen.lolfx 12_distance.lolfx
12_voronoi_CPPFLAGS = $(AM_CPPFLAGS)
12_voronoi_DEPENDENCIES = @LOL_DEPS@

13_shader_builder_SOURCES = 13_shader_builder.cpp
13_shader_builder_CPPFLAGS = $(AM_CPPFLAGS)
13_shader_builder_DEPENDENCIES = @LOL_DEPS@

