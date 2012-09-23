//
// Lol Engine
//
// Copyright: (c) 2010-2012 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>
#include <cstdlib>

#include "core.h"

#if defined _WIN32 || defined _XBOX
#   define strcasecmp _stricmp
#endif

using namespace std;

namespace lol
{

/*
 * Dict implementation class
 */

class DictData
{
    friend class Dict;

public:
    DictData() :
        entities(0),
        maxid(0),
        nentities(0)
    {
        /* Nothing to do */
    }

    ~DictData()
    {
#if !LOL_RELEASE
        if (nentities)
            Log::Error("still %i entities in dict\n", nentities);
#endif
        free(entities);
    }

private:
    Entity **entities;
    int maxid, nentities;
};

/*
 * Public Dict class
 */

Dict::Dict()
  : data(new DictData())
{
}

Dict::~Dict()
{
    delete data;
}

int Dict::MakeSlot(char const *name)
{
    int slotid, empty = -1;

    /* If the entry is already registered, remember its ID. Look for an
     * empty slot at the same time. */
    for (slotid = 0; slotid < data->maxid; slotid++)
    {
        Entity *e = data->entities[slotid];
        if (!e)
        {
            empty = slotid;
            break;
        }
        else
        {
            char const *oldname = e->GetName();
            if (*oldname == '<')
            {
                while (*oldname && *oldname != '>')
                    oldname++;
                while (*oldname == '>')
                    oldname++;
                while (*oldname == ' ')
                    oldname++;
            }

            if (!strcasecmp(name, oldname))
                break;
        }
    }

    /* If this is a new entry, create a new slot for it. */
    if (slotid == data->maxid || !data->entities[slotid])
    {
        if (slotid == data->maxid)
        {
            empty = data->maxid++;
            data->entities = (Entity **)realloc(data->entities,
                                                data->maxid * sizeof(Entity *));
        }

        data->entities[empty] = NULL;
        slotid = empty;
        data->nentities++;
    }
    else
    {
        Ticker::Ref(data->entities[slotid]);
    }

    return slotid;
}

void Dict::RemoveSlot(int slotid)
{
    if (Ticker::Unref(data->entities[slotid]) == 0)
    {
        data->entities[slotid] = NULL;
        data->nentities--;
    }
}

void Dict::RemoveSlot(Entity *entity)
{
    for (int slotid = 0; slotid < data->maxid; slotid++)
        if (data->entities[slotid] == entity)
        {
            RemoveSlot(slotid);
            return;
        }

#if !LOL_RELEASE
    Log::Error("removing unregistered entity %p (%s)\n",
               entity, entity->GetName());
#endif
}

void Dict::SetEntity(int slotid, Entity *entity)
{
    Ticker::Ref(entity);
    data->entities[slotid] = entity;
}

Entity *Dict::GetEntity(int slotid)
{
    return data->entities[slotid];
}

} /* namespace lol */

