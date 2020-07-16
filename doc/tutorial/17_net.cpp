//
//  Lol Engine — HTTP client sample
//
//  Copyright © 2016—2020 Sam Hocevar <sam@hocevar.net>
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
#include <lol/msg>

class demo : public lol::entity
{
public:
    virtual bool init_game() override
    {
        // Choose URL that works with CORS (cross-origin resource sharing)
        client.get("https://api.github.com/");
        return true;
    }

    virtual void tick_game(float seconds) override
    {
        entity::tick_game(seconds);

        switch (client.get_status())
        {
        case lol::net::http::status::success:
            lol::msg::info("downloaded %d bytes: %s\n",
                           (int)client.get_result().size(),
                           client.get_result().c_str());
            client.reset();
            break;
        case lol::net::http::status::error:
            lol::msg::info("error downloading %s\n", client.get_url().c_str());
            client.reset();
            break;
        default:
            break;
        }
    }

    lol::net::http::client client;
};

int main(int argc, char **argv)
{
    lol::sys::init(argc, argv);

    auto app = lol::app::init("Tutorial 17: HTTP", lol::ivec2(800, 600), 60.0f);
    new demo();
    app->run();

    return 0;
}

