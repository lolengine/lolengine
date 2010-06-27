
class VideoData;

class Video
{
public:
    Video(char const *title, int width, int height);
    ~Video();

    int GetWidth() const;
    int GetHeight() const;
    void Refresh(float milliseconds);
    void FullScreen();

private:
    VideoData *data;
};
