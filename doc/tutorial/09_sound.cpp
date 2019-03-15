//
//  Lol Engine — Sound tutorial
//
//  Copyright © 2011—2019 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <lol/engine.h>

#include <functional>

using namespace lol;

class sound_demo : public WorldEntity
{
public:
    sound_demo()
    {
        for (auto &val : m_streams)
            val = -1;

        m_text = new Text("SPACE for sine wave, Left Click for white noise",
                          "data/font/ascii.png");
        m_text->SetPos(vec3(5, 5, 1));
        Ticker::Ref(m_text);
    }

    ~sound_demo()
    {
        Ticker::Unref(m_text);
    }

    void synth(int mode, void *buf, int bytes)
    {
        int16_t *stream = (int16_t *)buf;
        for (int i = 0; i < bytes / 2; ++i)
        {
            switch (mode)
            {
            case 0: // sine wave
                stream[i] = lol::sin(4 * i * F_TAU / bytes) > 0 ? 800 : -800;
                break;
            case 1: // white noise
                stream[i] = lol::rand(-200, 200);
                break;
            }
        }
    }

    virtual void tick_game(float seconds) override
    {
        WorldEntity::tick_game(seconds);

        auto mouse = input::mouse();
        auto keyboard = input::keyboard();

        for (int i = 0; i < 2; ++i)
        {
            if (i == 0 && !keyboard->key_pressed(input::key::SC_Space))
                continue;
            if (i == 1 && !mouse->button_pressed(input::button::BTN_Left))
                continue;

            if (m_streams[i] < 0)
            {
                auto f = std::bind(&sound_demo::synth, this, i,
                                   std::placeholders::_1,
                                   std::placeholders::_2);
                m_streams[i] = audio::start_streaming(f);
            }
            else
            {
                audio::stop_streaming(m_streams[i]);
                m_streams[i] = -1;
            }
        }
    }

    virtual void tick_draw(float seconds, Scene &scene) override
    {
        WorldEntity::tick_draw(seconds, scene);
    }

private:
    int m_streams[2];
    Text *m_text;
};

int main(int argc, char **argv)
{
    sys::init(argc, argv);

    Application app("Tutorial 9: Sound", ivec2(640, 480), 60.0f);
    new sound_demo();
    app.Run();

    return EXIT_SUCCESS;
}

