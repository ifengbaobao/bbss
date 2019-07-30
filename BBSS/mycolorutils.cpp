#include "mycolorutils.h"

MyColorUtils::MyColorUtils()
{}

void MyColorUtils::bgra2rgb(unsigned char * des, unsigned char * src, int width, int height)
{
    int widthBytes = width * 4;
    int index = 0;
    for (int i = 0; i < height; ++i)
    {
        int lineBegin = i * widthBytes;
        for (int j = 0; j < widthBytes; j += 4)
        {
            des[index] = src[lineBegin + j + 2];
            des[index + 1] = src[lineBegin + j + 1];
            des[index + 2] = src[lineBegin + j + 0];
            index += 3;
        }
    }
}

void MyColorUtils::rgb2bgra(unsigned char * des, unsigned char * src, int width, int height)
{
    int widthBytes = width * 4;
    int index = 0;
    for (int i = 0; i < height; ++i)
    {
        int lineBegin = i * widthBytes;
        for (int j = 0; j < widthBytes; j += 4)
        {
            des[lineBegin + j + 3] = 0;
            des[lineBegin + j + 2] = src[index];
            des[lineBegin + j + 1] = src[index + 1];
            des[lineBegin + j + 0] = src[index + 2];
            index += 3;
        }
    }
}

