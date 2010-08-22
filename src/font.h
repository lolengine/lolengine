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
    virtual char const *GetName();
    virtual Group GetGroup();
    virtual void TickDraw(float deltams);

public:
    /* New methods */
    void Print(int x, int y, char const *str);
    void PrintBold(int x, int y, char const *str);

private:
    FontData *data;
};

#endif // __DH_FONT_H__

