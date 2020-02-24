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

#include <vector>
#include <string>
#include <cstring>

namespace lol
{

class getopt
{
public:
    getopt(int argc, char ** argv)
      : m_argc(argc),
        m_argv(argv)
    {
    }

    getopt(int argc, char * const * argv)
      : m_argc(argc),
        m_argv(argv)
    {
    }

    void add_opt(int short_opt, char const *long_opt, bool has_arg)
    {
        optdesc o { long_opt, has_arg, nullptr, short_opt };
        m_opts.push_back(o);

        /* “The standards require that the argument [to isalnum()] is either
         * EOF or a value that is representable in the type unsigned char.” */
        if ((int)(unsigned char)short_opt == short_opt && isalnum(short_opt))
        {
            m_optstring += (char)short_opt;
            if (has_arg)
                m_optstring += ':';
        }
    }

    int parse()
    {
        int longindex = 0; // FIXME: what is this?

        /* XXX: this getopt_long implementation should not be trusted for other
         * applications without any serious peer reviewing. It “just works” with
         * zzuf and a few libcaca programs but may fail miserably in other
         * programs. */
        char **argv = (char **)(uintptr_t)m_argv;
        char *flag;

        if (this->index >= m_argc)
            return -1;

        flag = argv[this->index];

        if (flag[0] == '-' && flag[1] != '-')
        {
            char const *tmp;
            int ret = flag[1];

            if (ret == '\0')
                return -1;

            tmp = strchr(m_optstring.c_str(), ret);
            if (!tmp || ret == ':')
                return '?';

            ++this->index;
            if (tmp[1] == ':')
            {
                if (flag[2] != '\0')
                    this->arg = flag + 2;
                else
                {
                    if (this->index >= m_argc)
                        goto too_few;
                    this->arg = argv[this->index++];
                }
                return ret;
            }

            if (flag[2] != '\0')
            {
                flag[1] = '-';
                --this->index;
                ++argv[this->index];
            }

            return ret;
        }

        if (flag[0] == '-' && flag[1] == '-')
        {
            if (flag[2] == '\0')
                return -1;

            for (int i = 0; m_opts[i].name; ++i)
            {
                size_t l = strlen(m_opts[i].name);

                if (strncmp(flag + 2, m_opts[i].name, l))
                    continue;

                switch (flag[2 + l])
                {
                case '=':
                    if (!m_opts[i].has_arg)
                        goto bad_opt;
                    longindex = i;
                    ++this->index;
                    this->arg = flag + 2 + l + 1;
                    return m_opts[i].val;
                case '\0':
                    longindex = i;
                    ++this->index;
                    if (m_opts[i].has_arg)
                    {
                        if (this->index >= m_argc)
                            goto too_few;
                        this->arg = argv[this->index++];
                    }
                    return m_opts[i].val;
                default:
                    break;
                }
            }
        bad_opt:
            fprintf(stderr, "%s: unrecognized option `%s'\n", argv[0], flag);
            return '?';

        too_few:
            fprintf(stderr, "%s: option `%s' requires an argument\n",
                    argv[0], flag);
            return '?';
        }

        return -1;
    }

    int index = 1;
    char *arg = nullptr;

private:
    struct optdesc
    {
        char const *name;
        int has_arg;
        int *flag;
        int val;
    };

    int m_argc;
    char * const *m_argv;

    std::string m_optstring;
    std::vector<optdesc> m_opts;
};

} // namespace lol

