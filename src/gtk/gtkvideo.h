//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The GtkVideo class
// ------------------
//

#if !defined __DH_GTKVIDEO_H__
#define __DH_GTKVIDEO_H__

#include "video.h"

class GtkVideoData;

class GtkVideo : public Video
{
public:
    GtkVideo(char const *title, int width, int height);
    virtual ~GtkVideo();

    // Inherited
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual void PreRender();
    virtual void PostRender(float milliseconds);
    virtual void FullScreen();

    // New
    void *GetWidget();

private:
    GtkVideoData *data;
};

#endif // __DH_GTKVIDEO_H__

