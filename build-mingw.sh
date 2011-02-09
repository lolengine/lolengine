#!/bin/sh

set -e

PATH="$PATH:./contrib/gtk-2.22.1/bin"
M4PATH="./contrib/gtk-2.22.1/share/aclocal"
export M4PATH # This looks necessary

# SDL include path
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/sdl-1.2.14/include"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/sdl-image-1.2.10/include"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/sdl-mixer-1.2.11/include"

# GTK include path
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/gtk-2.22.1/lib/glib-2.0/include"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/gtk-2.22.1/lib/gtk-2.0/include"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/gtk-2.22.1/include/glib-2.0"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/gtk-2.22.1/include/gtk-2.0"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/gtk-2.22.1/include/cairo"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/gtk-2.22.1/include/pango-1.0"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/gtk-2.22.1/include/gdk-pixbuf-2.0"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/gtk-2.22.1/include/atk-1.0"
CPPFLAGS="$CPPFLAGS -I`pwd`/contrib/gtkglarea-2.0.1/include"

# SDL library path
LDFLAGS="$LDFLAGS -L`pwd`/contrib/sdl-1.2.14/lib"
LDFLAGS="$LDFLAGS -L`pwd`/contrib/sdl-image-1.2.10/lib"
LDFLAGS="$LDFLAGS -L`pwd`/contrib/sdl-mixer-1.2.11/lib"

# GTK library path
LDFLAGS="$LDFLAGS -L`pwd`/contrib/gtk-2.22.1/lib"
LDFLAGS="$LDFLAGS -L`pwd`/contrib/gtk-2.22.1/bin"
LDFLAGS="$LDFLAGS -L`pwd`/contrib/gtkglarea-2.0.1/lib"

# Prevent issues with GTK+ internals
CPPFLAGS="$CPPFLAGS -mms-bitfields"

# We don't want our binaries to depend on MinGW
LDFLAGS="$LDFLAGS -static-libgcc -static-libstdc++"

# Help our build scripts a bit here
GTK_LIBS="$GTK_LIBS -lgtkgl-2.0 -lopengl32 -lgdi32"
GTK_LIBS="$GTK_LIBS -lgtk-win32-2.0 -lgdk-win32-2.0"
GTK_LIBS="$GTK_LIBS -lglib-2.0 -lgthread-2.0 -lgobject-2.0"

LOL_LIBS="$LOL_LIBS -lSDL_image -lSDL_mixer -lSDLmain -lSDL -lopengl32"

# Bootstrap, configure and make
./bootstrap 
./configure CPPFLAGS="$CPPFLAGS" LDFLAGS="$LDFLAGS" \
            GTK_LIBS="$GTK_LIBS" LOL_LIBS="$LOL_LIBS"
make

