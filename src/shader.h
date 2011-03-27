//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Shader class
// ----------------
//

#if !defined __LOL_SHADER_H__
#define __LOL_SHADER_H__

namespace lol
{

class ShaderData;

class Shader
{
public:
    static Shader *Create(char const *vert, char const *frag);
    static void Destroy(Shader *shader);

    int GetAttribLocation(char const *attr) const;
    int GetUniformLocation(char const *uni) const;

    void Bind() const;

protected:
    Shader(char const *vert, char const *frag);
    ~Shader();

private:
    ShaderData *data;
};

} /* namespace lol */

#endif // __LOL_SHADER_H__

