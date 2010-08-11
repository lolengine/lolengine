
/*
 * The video interface
 */

#if !defined __DH_VIDEO_H__
#define __DH_VIDEO_H__

class Video
{
public:
    //Video(char const *title, int width, int height);
    //virtual ~Video();

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;
    virtual void Clear() = 0;
    virtual void Refresh(float milliseconds) = 0;
    virtual void FullScreen() = 0;
};

#endif // __DH_VIDEO_H__

