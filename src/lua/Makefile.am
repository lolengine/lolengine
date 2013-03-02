
include $(top_srcdir)/build/autotools/common.am

noinst_LIBRARIES = liblollua.a

# Only remove flags that were actually set, because we don't know
# what the compiler actually accepts.
#disable_cflags = $(filter $(AM_CPPFLAGS:-W%=-Wno-%), \
#    -Wno-shadow -Wno-unused -Wno-cast-qual -Wno-strict-aliasing \
#    -Wno-reorder -Wno-maybe-uninitialized -Wno-narrowing \
#    -Wno-parentheses)

liblollua_a_SOURCES = $(lua_sources)
liblollua_a_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir) $(disable_cflags)

lua_sources = \
    lapi.c lapi.h lauxlib.c lauxlib.h lbaselib.c lbitlib.c \
    lcode.c lcode.h lcorolib.c lctype.c lctype.h ldblib.c \
    ldebug.c ldebug.h ldo.c ldo.h ldump.c lfunc.c lfunc.h \
    lgc.c lgc.h linit.c liolib.c llex.c llex.h llimits.h \
    lmathlib.c lmem.c lmem.h loadlib.c lobject.c lobject.h \
    lopcodes.c lopcodes.h loslib.c lparser.c lparser.h \
    lstate.c lstate.h lstring.c lstring.h lstrlib.c \
    ltable.c ltable.h ltablib.c ltm.c ltm.h lua.c luac.c \
    luaconf.h lua.h lualib.h lundump.c lundump.h lvm.c lvm.h \
    lzio.c lzio.h

