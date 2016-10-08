//
//  Lol Engine
//
//  Copyright © 2010—2016 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The sample class
// ----------------
// A sample is a unique sound sample.
//

#include "engine/entity.h"

#include <stdint.h>

namespace lol
{

class sample_data;

class sample : public Entity
{
public:
    sample(char const *path);
    virtual ~sample();

protected:
    /* Inherited from Entity */
    virtual char const *GetName();
    virtual void TickGame(float seconds);

public:
    /* New methods */
    void play();
    void loop();
    void stop();

private:
    sample_data *data;
};

} /* namespace lol */

