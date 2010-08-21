//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Font class
// --------------
//

#if !defined __DH_FONT_H__
#define __DH_FONT_H__

#include "entity.h"

class FontData;

class Font : public Entity
{
public:
    Font(char const *path);
    ~Font();

protected:
    /* Inherited from Entity */
    virtual Group GetGroup();
    virtual void TickRender(float delta_time);

public:
    /* New methods */
    char const *GetName();

    void Print(int x, int y, char const *str);
    void PrintBold(int x, int y, char const *str);

private:
    FontData *data;
};

#endif // __DH_FONT_H__

