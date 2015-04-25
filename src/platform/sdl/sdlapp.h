//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The SdlApp class
// ----------------
//

#include <lol/math/vector.h>

namespace lol
{

//-----------------------------------------------------------------------------
class SdlAppDisplayData;

class SdlAppDisplay
{
    friend class ApplicationDisplayData;
public:
    SdlAppDisplay(char const *title, ivec2 resolution);
    virtual ~SdlAppDisplay();

protected:
    void SetResolution(ivec2 resolution);
    void SetPosition(ivec2 position);

    void Enable();
    void Disable();

private:
    SdlAppDisplayData *data;
};

//-----------------------------------------------------------------------------
class SdlAppData;

class SdlApp
{
public:
    SdlApp(char const *title, ivec2 res, float fps);
    virtual ~SdlApp();

    void ShowPointer(bool show);
    void Tick();

private:
    SdlAppData *data;
};

} /* namespace lol */

