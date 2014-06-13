/*
 *  libpipi       Pathetic image processing interface library
 *  Copyright (c) 2004-2008 Sam Hocevar <sam@zoy.org>
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

/* pipi-template.h: the magic template preprocessing file
 *
 * Define the following macros before including this file:
 *  * TEMPLATE_FLAGS is set to a list of toggle flags, a binary OR of:
 *    - SET_FLAG_GRAY
 *    - SET_FLAG_WRAP
 *    - SET_FLAG_8BIT
 *  * TEMPLATE_FILE is set to the template file. The following macros
 *    will be defined when including it. Their value depend on the flags
 *    specified above:
 *    - FLAG_GRAY is set to 0 or 1
 *    - FLAG_WRAP is set to 0 or 1
 *    - FLAG_8BIT is set to 0 or 1
 *    - T(x) expands x by adding relevant information, eg. x##_gray_wrap
 */

#if !defined FLAG_GRAY
#   if (TEMPLATE_FLAGS) & SET_FLAG_GRAY
#      define FLAG_GRAY 1
#      define T_GRAY(x) CAT(x, _gray)
#      include __FILE__
#      undef FLAG_GRAY
#      undef T_GRAY
#   endif
#   define FLAG_GRAY 0
#   define T_GRAY(x) x
#   include __FILE__
#   undef FLAG_GRAY
#   undef T_GRAY

#elif !defined FLAG_WRAP
#   if (TEMPLATE_FLAGS) & SET_FLAG_WRAP
#      define FLAG_WRAP 1
#      define T_WRAP(x) CAT(x, _wrap)
#      include __FILE__
#      undef FLAG_WRAP
#      undef T_WRAP
#   endif
#   define FLAG_WRAP 0
#   define T_WRAP(x) x
#   include __FILE__
#   undef FLAG_WRAP
#   undef T_WRAP

#elif !defined FLAG_8BIT
#   if (TEMPLATE_FLAGS) & SET_FLAG_8BIT
#      define FLAG_8BIT 1
#      define T_8BIT(x) CAT(x, _8bit)
#      include __FILE__
#      undef FLAG_8BIT
#      undef T_8BIT
#   endif
#   define FLAG_8BIT 0
#   define T_8BIT(x) x
#   include __FILE__
#   undef FLAG_8BIT
#   undef T_8BIT

#else
#   define CAT(x, y) x ## y
#   define T(x) T_8BIT(T_WRAP(T_GRAY(x)))
#   include TEMPLATE_FILE
#   undef CAT
#   undef S
#endif

