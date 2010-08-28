//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "core.h"

#if defined WIN32
#   define strcasecmp _stricmp
#endif

/*
 * Dict implementation class
 */

class DictData
{
    friend class Dict;

public:
    DictData() :
        entities(0),
        nentities(0)
    {
        /* Nothing to do */
    }

    ~DictData()
    {
        if (nentities)
            fprintf(stderr, "ERROR: still %i entities in dict\n", nentities);
        free(entities);
    }

private:
    Entity **entities;
    int nentities;
};

/*
 * Public Dict class
 */

Dict::Dict()
{
    data = new DictData();
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
    for (id = 0; id < data->nentities; id++)
    {
        Entity *e = data->entities[id];
        if (!e)
            empty = id;
        else if (!strcasecmp(name, e->GetName()))
            break;
    }

    /* If this is a new entry, create a new slot for it. */
    if (id == data->nentities)
    {
        if (empty == -1)
        {
            empty = data->nentities++;
            data->entities = (Entity **)realloc(data->entities,
                                           data->nentities * sizeof(Entity *));
        }

        data->entities[empty] = NULL;
        id = empty;
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
        data->entities[id] = NULL;
    data->nentities--;
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

