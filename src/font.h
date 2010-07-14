
/*
 * The font object
 */

#if !defined __DH_FONT_H__
#define __DH_FONT_H__

class FontData;

class Font
{
public:
    Font(char const *path);
    ~Font();

    void Print(int x, int y, char const *str);

private:
    FontData *data;
};

#endif // __DH_FONT_H__

