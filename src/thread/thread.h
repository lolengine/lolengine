//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Threading classes
// ---------------------
//

#if !defined __LOL_THREAD_H__
#define __LOL_THREAD_H__

#if defined __CELLOS_LV2__
#   include "platform/ps3/threadbase.h"
#else
#   include "threadbase.h"
#endif

namespace lol
{

class Mutex : public MutexBase
{
public:
    Mutex() : MutexBase() {}
};

class Condition : public ConditionBase
{
public:
    Condition() : ConditionBase() {}
};

class Thread : ThreadBase
{
public:
    Thread(void *(*fn)(void *), void *data) : ThreadBase(fn, data) {}
    virtual ~Thread() {}
};

} /* namespace lol */

#endif // __LOL_THREAD_H__

