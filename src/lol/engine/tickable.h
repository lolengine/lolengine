//
//  Lol Engine
//
//  Copyright © 2010—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The tickable class
// ——————————————————
// Tickables are objects that can be ticked by the game loop and/or the render
// loop.
//

#include <cstdint>
#include <memory>

namespace lol
{

class tickable
{
public:
    tickable();
    virtual ~tickable();

    virtual void init();
    virtual void fini();
    virtual void tick_game(float seconds);
    virtual void tick_draw(float seconds, class Scene &scene);

    // Auto-registering factory
    template<typename T>
    static typename std::enable_if<std::is_base_of<tickable, T>::value, std::shared_ptr<T>>::type
    create()
    {
        auto p = std::make_shared<T>();
        return p;
    }

    // Tick groups
    struct group
    {
        enum class game
        {
            begin = 0, // must be the first element

            input,     // input should be polled before everything else
            gui,       // debug update needs to be called before the rest for init purposes
            app,       // main application update
            entity,    // default entity update
            // ----------------- // split entity update:
            player,    // player updates before AI to ensure player actions is prevalent
            ai,        // AI update
            other_0,   // other misc updates here
            other_1,   //  (same)
            other_2,   //  (same)
            other_3,   //  (same)
            // ----------------- // primitives updates
            mesh,      // update Mesh/Animation to ensure correct sync with PLY/AI
            fx,        // update FX/other to ensure correct sync with WorldPos and Meshes
            light,     // update after FX because it could some
            camera,    // update camera at the end of the frame, once everything is settled
            stats,     // stats update

            end,       // must be the last element
        };

        enum class draw
        {
            begin = (int)game::end,

            camera,   // update camera first for rendering
            texture,  // texture
            light,    //
            world,    // other misc updates here
            entity,   //
            fx,       //
            other_0,  // other misc updates here
            other_1,  //  (same)
            other_2,  //  (same)
            other_3,  //  (same)
            app,      // main application Draw
            hud,
            gui,
            capture,

            end,      // must be the next-to-last element
            none,     // this group is for non draw-ticked
        };

        enum class all
        {
            end = (int)draw::end,
        };
    };
};

} /* namespace lol */

