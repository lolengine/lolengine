//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
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

#if defined WIN32
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
    int id, empty = -1;

    /* If the entry is already registered, remember its ID. Look for an
     * empty slot at the same time. */
    for (id = 0; id < data->maxid; id++)
    {
        Entity *e = data->entities[id];
        if (!e)
        {
            empty = id;
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
    if (id == data->maxid || !data->entities[id])
    {
        if (id == data->maxid)
        {
            empty = data->maxid++;
            data->entities = (Entity **)realloc(data->entities,
                                                data->maxid * sizeof(Entity *));
        }

        data->entities[empty] = NULL;
        id = empty;
        data->nentities++;
    }
    else
    {
        Ticker::Ref(data->entities[id]);
    }

    return id;
}

void Dict::RemoveSlot(int id)
{
    if (Ticker::Unref(data->entities[id]) == 0)
    {
        data->entities[id] = NULL;
        data->nentities--;
    }
}


void Dict::SetEntity(int id, Entity *entity)
{
    Ticker::Ref(entity);
    data->entities[id] = entity;
}

Entity *Dict::GetEntity(int id)
{
    return data->entities[id];
}

} /* namespace lol */

