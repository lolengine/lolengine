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
// The Application class
// ---------------------
//

namespace lol
{

class ApplicationDisplayData;

class ApplicationDisplay
{
    friend class Scene;

public:
    ApplicationDisplay(char const *name, ivec2 resolution);
    virtual ~ApplicationDisplay();

    virtual void start_frame();
    virtual void end_frame();

    // pos/size/... methods
    virtual void set_resolution(ivec2 resolution);
    virtual ivec2 resolution() const;

    virtual void SetPosition(ivec2 position);

private:
    ApplicationDisplayData *data;
};


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

} // namespace lol

