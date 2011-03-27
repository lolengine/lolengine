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
// The Audio interface
// -------------------
// Helper functions to set up the audio device.
//

#if !defined __LOL_AUDIO_H__
#define __LOL_AUDIO_H__

#include <stdint.h>

namespace lol
{

class Audio
{
public:
    static void Setup(int channels);
};

} /* namespace lol */

#endif // __LOL_AUDIO_H__

