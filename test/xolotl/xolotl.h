//
// Lol Engine - Xolotl algorithm test
//
// Copyright: (c) 2011 Soren Renner
//            (c) 2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

using namespace lol;

struct Voxel
{
    bool passable, lock, copymarker;
    struct Voxel *imposter;
};

struct MSV : public Voxel
{
};

struct Ray
{
    float theta, phi;
    vec3 origin, xyz, dxyz, ddxyz, lxyz, popdelta;
    struct Voxel *terminalvoxel;
    bool terminate;
    ivec3 counter;
    int scale;
    float length;
    bool di, dj, dk, changed, traced, far;
    int face;
    vec3 normal, bloxnorm, corner;
    int dtl;
};

template <int M> struct Cell : public MSV
{
    Voxel blox[M * M * M];
    vec3 bloxnorm[M * M * M];
    int m;

    void Serp(Voxel v, Voxel w)
    {
        for (int i = 0; i < M; i++)
        for (int j = 0; j < M; j++)
        for (int k = 0; k < M; k++)
            if ((i == M_2) + (j == M_2) + (k == M_2) >= 2)
                blox[(i * M + j) * M + k] = v;
            else
                blox[(i * M + j) * M + k] = w;
    }

    static const int M_2 = M / 2;
};

