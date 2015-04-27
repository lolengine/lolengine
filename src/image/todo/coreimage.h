/*
 *  Lol Engine
 *
 *  Copyright © 2004—2008 Sam Hocevar <sam@hocevar.net>
 *            © 2008 Jean-Yves Lamoureux <jylam@lnxscene.org>
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What the Fuck You Want
 *  to Public License, Version 2, as published by the WTFPL Task Force.
 *  See http://www.wtfpl.net/ for more details.
 */

/*
 * coreimage.m: CoreImage (OSX) I/O functions
 */

#include "config.h"
#ifdef USE_COCOA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#import <Cocoa/Cocoa.h>

#include "pipi.h"
#include "pipi-internals.h"

struct pipi_codec_coreimage
{
        NSBitmapFormat format;
};


#endif
