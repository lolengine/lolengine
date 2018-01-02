
include $(top_srcdir)/build/autotools/common.am

if BUILD_TUTORIAL
noinst_PROGRAMS = $(tutorials)
endif

tutorials =
if LOL_USE_GL
tutorials += 01_triangle 02_cube 03_noise 04_texture 05_easymesh \
             06_sprite 07_input 08_fbo 09_sound 11_fractal \
             12_voronoi 13_shader_builder 14_lol_lua 15_lolimgui
endif
tutorials += 16_movie

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
05_easymesh_LDFLAGS = $(AM_LDFLAGS) @LOL_LUA_DEPS@
05_easymesh_DEPENDENCIES = @LOL_DEPS@ @LOL_LUA_DEPS@

06_sprite_SOURCES = 06_sprite.cpp 06_sprite.png
06_sprite_CPPFLAGS = $(AM_CPPFLAGS)
06_sprite_DEPENDENCIES = @LOL_DEPS@
06_sprite_LDFLAGS = $(AM_LDFLAGS)
if LOL_USE_EMSCRIPTEN
06_sprite_LDFLAGS += --preload-file 06_sprite.png
endif

07_input_SOURCES = 07_input.cpp 07_input.lolfx
07_input_CPPFLAGS = $(AM_CPPFLAGS)
07_input_DEPENDENCIES = @LOL_DEPS@

08_fbo_SOURCES = 08_fbo.cpp 08_fbo.lolfx
08_fbo_CPPFLAGS = $(AM_CPPFLAGS)
08_fbo_DEPENDENCIES = @LOL_DEPS@

09_sound_SOURCES = 09_sound.cpp
09_sound_CPPFLAGS = $(AM_CPPFLAGS)
09_sound_DEPENDENCIES = @LOL_DEPS@

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

14_lol_lua_SOURCES = 14_lol_lua.cpp
14_lol_lua_CPPFLAGS = $(AM_CPPFLAGS)
14_lol_lua_LDFLAGS = $(AM_LDFLAGS) @LOL_LUA_DEPS@
14_lol_lua_DEPENDENCIES = @LOL_DEPS@ @LOL_LUA_DEPS@

15_lolimgui_SOURCES = 15_lolimgui.cpp
15_lolimgui_CPPFLAGS = $(AM_CPPFLAGS)
15_lolimgui_DEPENDENCIES = @LOL_DEPS@

16_movie_SOURCES = 16_movie.cpp
16_movie_CPPFLAGS = $(AM_CPPFLAGS)
16_movie_DEPENDENCIES = @LOL_DEPS@

