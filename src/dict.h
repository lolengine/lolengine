//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Dict class
// --------------
//

#if !defined __DH_DICT_H__
#define __DH_DICT_H__

#include "entity.h"

class DictData;

class Dict
{
public:
    Dict();
    ~Dict();

    int MakeSlot(char const *name);
    void RemoveSlot(int id);

    void SetEntity(int id, Entity *entity);
    Entity *GetEntity(int id);

private:
    DictData *data;
};

#endif // __DH_DICT_H__

