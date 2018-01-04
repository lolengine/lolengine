//
//  Lol Engine
//
//  Copyright © 2010—2018 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Dict class
// --------------
//

#include "engine/entity.h"

namespace lol
{

class DictData;

class Dict
{
public:
    Dict();
    ~Dict();

    int MakeSlot(std::string const &name);
    void RemoveSlot(int slotid);
    void RemoveSlot(Entity *entity);

    void SetEntity(int slotid, Entity *entity);
    Entity *GetEntity(int slotid);

private:
    DictData *data;
};

} /* namespace lol */

