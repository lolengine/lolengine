//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
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

