//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The Application class
// ---------------------
//

#if !defined __LOL_APPLICATION_H__
#define __LOL_APPLICATION_H__

namespace lol
{

class ApplicationData;

class Application
{
public:
    Application(char const *name, ivec2 resolution, float framerate);
    ~Application();

    bool MustTick();
    void Tick();
    void Run();

    void ShowPointer(bool show);

private:
    ApplicationData *data;
};

} /* namespace lol */

#endif // __LOL_APPLICATION_H__

