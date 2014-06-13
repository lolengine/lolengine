/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2008 Sam Hocevar <sam@zoy.org>
 *                2008 Jean-Yves Lamoureux <jylam@lnxscene.org>
 *                All Rights Reserved
 *
 *  $Id$
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What The Fuck You Want
 *  To Public License, Version 2, as published by Sam Hocevar. See
 *  http://sam.zoy.org/wtfpl/COPYING for more details.
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
