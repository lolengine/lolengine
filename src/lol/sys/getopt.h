//
//  Lol Engine
//
//  Copyright © 2002—2016 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The getopt functions
// --------------------
//

namespace lol
{

class getopt
{
public:
    getopt(int argc, char ** _argv);
    getopt(int argc, char * const * _argv);
    ~getopt();

    void add_arg(int short_opt, char const *long_opt, bool has_arg);
    int parse();

    int index;
    char *arg;

private:
    std::unique_ptr<struct getopt_private> m_private;
};

}

