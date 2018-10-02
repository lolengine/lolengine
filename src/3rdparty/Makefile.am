
include $(top_srcdir)/build/autotools/common.am

noinst_LIBRARIES = liblol-lua.a
if LOL_USE_BULLET
noinst_LIBRARIES += liblol-bullet.a
endif

# Only remove flags that were actually set, because we don't know
# what the compiler actually accepts.
disable_cflags_bullet = $(filter $(AM_CPPFLAGS:-W%=-Wno-%), \
    -Wno-shadow -Wno-unused -Wno-unused-parameter \
    -Wno-cast-align -Wno-cast-qual -Wno-strict-aliasing)
disable_cflags_lua = $(filter $(AM_CPPFLAGS:-W%=-Wno-%), \
    -Wno-cast-qual)

liblol_bullet_a_SOURCES = $(bullet_sources)
liblol_bullet_a_CPPFLAGS = -DB3_USE_CLEW $(AM_CPPFLAGS) -I$(srcdir)/bullet3/src $(disable_cflags_bullet)

include lol-bullet.am

liblol_lua_a_SOURCES = $(lua_sources)
liblol_lua_a_CPPFLAGS = $(AM_CPPFLAGS) -DLUA_ANSI $(disable_cflags_lua)

include lol-lua.am

EXTRA_DIST += $(imgui_sources) $(mingw_std_threads_sources) $(pegtl_sources)
EXTRA_DIST += lol-bullet.vcxproj lol-bullet.vcxproj.filters
EXTRA_DIST += lol-lua.vcxproj lol-lua.vcxproj.filters

include lol-mingw-std-threads.am
include lol-pegtl.am

# XXX: this is included by the parent Makefile instead
#include lol-imgui.am

