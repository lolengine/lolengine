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

class GtkVideoData;

class GtkVideo
{
public:
    GtkVideo(char const *title, int width, int height);

    // Inherited
    int GetWidth() const;
    int GetHeight() const;
    void PreRender();
    void PostRender(float milliseconds);

    // New
    void *GetWidget();

private:
    GtkVideoData *data;
};

#endif // __DH_GTKVIDEO_H__

