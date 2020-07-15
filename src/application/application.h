//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <memory> // std::shared_ptr

//
// The app class
// —————————————
//

namespace lol
{

class app
{
public:
    app(char const *name, ivec2 resolution, float framerate);
    ~app();

    bool must_tick();
    void tick();
    void run();

    // UI interface
    void show_pointer(bool show);

    // Display interface
    class display
    {
    public:
        virtual ~display() {}

        virtual void start_frame() {}
        virtual void end_frame() {}

        // pos/size/... methods
        virtual void set_resolution(ivec2 resolution) {}
        virtual ivec2 resolution() const { return ivec2(0); }

        virtual void set_position(ivec2 position) {}
    };

    // Internal data
    class data
    {
    public:
        data() {}
        virtual ~data() {}

        virtual void show_pointer(bool show) {}
        virtual void tick() {}
    };

private:
    std::shared_ptr<display> m_display;
    std::shared_ptr<data> m_data;
};

} // namespace lol
