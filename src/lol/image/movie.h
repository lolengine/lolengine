//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The Movie class
// ---------------
//

#include <lol/image/movie.h>

namespace lol
{

class Movie
{
public:
    Movie(String const &name, ivec2 size, float fps);

    /* TODO: Rule of three */
#if 0
    Movie(Movie const &other);
    Movie & operator =(Movie other);
#endif
    ~Movie();

    void Feed(Image const &image);

private:
    class MovieData *m_data;
};

} /* namespace lol */

