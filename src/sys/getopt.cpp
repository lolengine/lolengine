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

#include <lol/engine-internal.h>

#if HAVE_GETOPT_H
#   include <getopt.h>
#endif
#if HAVE_UNISTD_H
#   include <unistd.h>
#endif

namespace lol
{

struct getopt_private
{
    getopt_private(int argc, char * const * _argv)
      : m_argc(argc),
        m_argv(_argv)
    {}

#if HAVE_GETOPT_LONG
    typedef option optdesc;
#else
    struct optdesc
    {
        char const *name;
        int has_arg;
        int *flag;
        int val;
    };
#endif

    int m_argc;
    char * const *m_argv;

    String m_optstring;
    array<optdesc> m_opts;
};

getopt::getopt(int argc, char ** _argv)
  : index(1),
    arg(nullptr),
    m_private(new getopt_private(argc, _argv))
{
}

getopt::getopt(int argc, char * const * _argv)
  : index(1),
    arg(nullptr),
    m_private(new getopt_private(argc, _argv))
{
}

getopt::~getopt()
{
}

void getopt::add_arg(int short_opt, char const *long_opt, bool has_arg)
{
    getopt_private::optdesc o = { long_opt, has_arg, nullptr, short_opt };
    m_private->m_opts.push(o);

    if (isalnum(short_opt))
    {
        m_private->m_optstring += (char)short_opt;
        if (has_arg)
            m_private->m_optstring += ':';
    }
}

int getopt::parse()
{
    int longindex = 0; // FIXME: what is this?

#if HAVE_GETOPT_LONG
    int ret;
    optind = this->index;
    optarg = this->arg;
    m_private->m_opts.push(getopt_private::optdesc { nullptr, 0, nullptr, 0 });
    ret = getopt_long(m_private->m_argc, m_private->m_argv, m_private->m_optstring.C(),
                      (option const *)m_private->m_opts.data(), &longindex);
    this->index = optind;
    this->arg = optarg;
    return ret;

#else
    /* XXX: this getopt_long implementation should not be trusted for other
     * applications without any serious peer reviewing. It “just works” with
     * zzuf and a few libcaca programs but may fail miserably in other
     * programs. */
    char **argv = (char **)(uintptr_t)m_private->m_argv;
    char *flag;

    if (this->index >= m_private->m_argc)
        return -1;

    flag = argv[this->index];

    if (flag[0] == '-' && flag[1] != '-')
    {
        char const *tmp;
        int ret = flag[1];

        if (ret == '\0')
            return -1;

        tmp = strchr(m_private->m_optstring.C(), ret);
        if (!tmp || ret == ':')
            return '?';

        this->index++;
        if (tmp[1] == ':')
        {
            if (flag[2] != '\0')
                this->arg = flag + 2;
            else if (this->index < m_private->m_argc)
                this->arg = argv[this->index++];
            else
                goto too_few;
            return ret;
        }

        if (flag[2] != '\0')
        {
            flag[1] = '-';
            this->index--;
            argv[this->index]++;
        }

        return ret;
    }

    if (flag[0] == '-' && flag[1] == '-')
    {
        if (flag[2] == '\0')
            return -1;

        for (int i = 0; m_private->m_opts[i].name; i++)
        {
            size_t l = strlen(m_private->m_opts[i].name);

            if (strncmp(flag + 2, m_private->m_opts[i].name, l))
                continue;

            switch (flag[2 + l])
            {
            case '=':
                if (!m_private->m_opts[i].has_arg)
                    goto bad_opt;
                longindex = i;
                this->index++;
                this->arg = flag + 2 + l + 1;
                return m_private->m_opts[i].val;
            case '\0':
                longindex = i;
                this->index++;
                if (m_private->m_opts[i].has_arg)
                {
                    if (this->index < m_private->argc)
                        this->arg = argv[this->index++];
                    else
                        goto too_few;
                }
                return m_private->m_opts[i].val;
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
#endif
}

} // namespace lol

