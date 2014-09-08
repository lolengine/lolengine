//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Dict class
// --------------
//

#include "entity.h"

namespace lol
{

class DictData;

class Dict
{
public:
    Dict();
    ~Dict();

    int MakeSlot(char const *name);
    void RemoveSlot(int slotid);
    void RemoveSlot(Entity *entity);

    void SetEntity(int slotid, Entity *entity);
    Entity *GetEntity(int slotid);

private:
    DictData *data;
};

} /* namespace lol */

