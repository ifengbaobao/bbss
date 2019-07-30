#ifndef MYCOLORUTILS_H
#define MYCOLORUTILS_H


class MyColorUtils
{
private:
    MyColorUtils();
public:
    static void bgra2rgb(unsigned char * des, unsigned char * src, int width, int height);
    static void rgb2bgra(unsigned char * des, unsigned char * src, int width, int height);

};

#endif // MYCOLORUTILS_H
