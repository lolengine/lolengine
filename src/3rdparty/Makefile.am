
include $(top_srcdir)/build/autotools/common.am

noinst_LTLIBRARIES = liblol-lua.la

# Only remove flags that were actually set, because we don't know
# what the compiler actually accepts.
disable_cflags_lua = $(filter $(AM_CPPFLAGS:-W%=-Wno-%), \
    -Wno-cast-qual)

liblol_lua_la_SOURCES = $(lua_sources)
liblol_lua_la_CPPFLAGS = $(AM_CPPFLAGS) -DLUA_ANSI $(disable_cflags_lua)

include lol-lua.am

EXTRA_DIST += $(imgui_sources) $(cpp_httplib_sources)
EXTRA_DIST += lol-lua.vcxproj lol-lua.vcxproj.filters

include lol-cpp-httplib.am

# XXX: this is included by the parent Makefile instead
#include lol-imgui.am

