//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Sample class
// ----------------
// A Sample is a unique sound sample.
//

#if !defined __LOL_SAMPLE_H__
#define __LOL_SAMPLE_H__

#include <stdint.h>

#include "entity.h"

namespace lol
{

class SampleData;

class Sample : public Entity
{
public:
    Sample(char const *path);
    virtual ~Sample();

protected:
    /* Inherited from Entity */
    virtual char const *GetName();
    virtual void TickGame(float seconds);

public:
    /* New methods */
    void Play();
    void Loop();
    void Stop();

private:
    SampleData *data;
};

} /* namespace lol */

#endif // __LOL_SAMPLE_H__

