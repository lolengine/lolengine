//
// Deus Hax (working title)
// Copyright (c) 2010 Sam Hocevar <sam@hocevar.net>
//

//
// The SdlVideo class
// ------------------
//

#if !defined __DH_SDLVIDEO_H__
#define __DH_SDLVIDEO_H__

class SdlVideoData;

class SdlVideo
{
public:
    SdlVideo(char const *title, int width, int height);
    ~SdlVideo();

    int GetWidth() const;
    int GetHeight() const;
    void PreRender();
    void PostRender(float milliseconds);
    void FullScreen();

private:
    SdlVideoData *data;
};

#endif // __DH_SDLVIDEO_H__

