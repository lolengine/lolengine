//
//  Lol Engine — Sound tutorial
//
//  Copyright © 2011—2020 Sam Hocevar <sam@hocevar.net>
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
#include <array>

using namespace lol;

class sound_demo : public WorldEntity
{
public:
    sound_demo()
    {
        for (int i = 0; i < 2; ++i)
        {
            auto f = std::bind(&sound_demo::synth, this, i,
                               std::placeholders::_1,
                               std::placeholders::_2);
            m_streams[i] = audio::start_streaming(f, audio::format::sint16le, 22050, 1);
        }

        for (size_t i = 0; i < m_instrument.size(); ++i)
            m_instrument[i] = (int16_t)(i % 80 * (10000 - lol::fabs(i - 10000)) * 40 / 10000);
        m_sample = sample::create(m_instrument.data(), 40000);

        m_text = new Text("SPACE for sine wave, Left Click for white noise",
                          "data/font/ascii.png");
        m_text->SetPos(vec3(5, 5, 1));
        Ticker::Ref(m_text);
    }

    ~sound_demo()
    {
        Ticker::Unref(m_text);
    }

    void synth(int channel, void *buf, int bytes)
    {
        int mode = (1 << channel) & m_mask;

        int16_t *stream = (int16_t *)buf;
        for (int i = 0; i < bytes / 2; ++i)
        {
            switch (mode)
            {
            case 2: // triangle signal
                stream[i] = (i % 128 - 64) * 8;
                break;
            case 1: // white noise
                stream[i] = lol::rand(-2048, 2048);
                break;
            case 0: // inactive
                stream[i] = 0;
                break;
            }
        }
    }

    virtual void tick_game(float seconds) override
    {
        WorldEntity::tick_game(seconds);

        auto mouse = input::mouse();
        auto keyboard = input::keyboard();

        if (keyboard->key_pressed(input::key::SC_Return))
            m_sample->play();

        if (keyboard->key_pressed(input::key::SC_Space))
            m_mask ^= 2;

        if (mouse->button_pressed(input::button::BTN_Left))
            m_mask ^= 1;
    }

    virtual bool release_game() override
    {
        for (int i = 0; i < 2; ++i)
            audio::stop_streaming(m_streams[i]);
        return true;
    }

private:
    int m_streams[2];
    int m_mask = 0;
    std::array<int16_t, 20000> m_instrument;
    sample* m_sample;
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

