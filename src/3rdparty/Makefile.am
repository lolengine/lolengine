
include $(top_srcdir)/build/autotools/common.am

noinst_LIBRARIES = liblol-bullet.a liblol-lua.a

# Only remove flags that were actually set, because we don't know
# what the compiler actually accepts.
disable_cflags = $(filter $(AM_CPPFLAGS:-W%=-Wno-%), \
    -Wno-shadow -Wno-unused -Wno-cast-qual -Wno-strict-aliasing)

liblol_bullet_a_SOURCES = $(bullet_sources)
liblol_bullet_a_CPPFLAGS = -DB3_USE_CLEW $(AM_CPPFLAGS) -I$(srcdir)/bullet3/src $(disable_cflags)

include lol-bullet.am

liblol_lua_a_SOURCES = $(lua_sources)
liblol_lua_a_CPPFLAGS = $(AM_CPPFLAGS) -DLUA_ANSI

include lol-lua.am

