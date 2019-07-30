#ifndef MYJPEGDECOMPRESS_H
#define MYJPEGDECOMPRESS_H

#include<Windows.h>
#include<stdio.h>
#include"jpeglib.h"
#include"mybitmap.h"


class MyJpegDecompress
{
private:
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

public:
    MyJpegDecompress();
    ~MyJpegDecompress();

    //解压缩。
    MyBitmap decompress(unsigned char * srcData, unsigned long srcSize);

    //bgra颜色转rgb颜色。
    void rgb2bgra(unsigned char * des, unsigned char * src, int width, int height);
};


#endif // MYJPEGDECOMPRESS_H
