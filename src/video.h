//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The Video interface
// -------------------
// Not sure yet whether this should exist.
//

#if !defined __DH_VIDEO_H__
#define __DH_VIDEO_H__

class Video
{
public:
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual void PreRender() = 0;
    virtual void PostRender(float milliseconds) = 0;
    virtual void FullScreen() = 0;
};

#endif // __DH_VIDEO_H__

