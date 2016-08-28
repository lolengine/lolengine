
include $(top_srcdir)/build/autotools/common.am

noinst_LIBRARIES = liblol-lua.a

liblol_lua_a_SOURCES = $(lua_sources)
liblol_lua_a_CPPFLAGS = $(AM_CPPFLAGS) -DLUA_ANSI

lua_sources = \
    lapi.c lapi.h lauxlib.c lauxlib.h lbaselib.c lbitlib.c \
    lcode.c lcode.h lcorolib.c lctype.c lctype.h ldblib.c \
    ldebug.c ldebug.h ldo.c ldo.h ldump.c lfunc.c lfunc.h \
    lgc.c lgc.h linit.c liolib.c llex.c llex.h llimits.h \
    lmathlib.c lmem.c lmem.h loadlib.c lobject.c lobject.h \
    lopcodes.c lopcodes.h loslib.c lparser.c lparser.h \
    lstate.c lstate.h lstring.c lstring.h lstrlib.c lprefix.h \
    ltable.c ltable.h ltablib.c ltm.c ltm.h lutf8lib.c lutf8lib.h \
    luaconf.h lua.h lualib.h lundump.c lundump.h lvm.c lvm.h \
    lzio.c lzio.h

