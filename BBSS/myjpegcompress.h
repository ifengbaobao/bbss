#ifndef MYJPEGCOMPRESS_H
#define MYJPEGCOMPRESS_H

#include<Windows.h>
#include<stdio.h>
#include"jpeglib.h"
#include"mybitmap.h"


class MyJpegCompress
{
private:
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    MyBitmap * bitmap;

    int quality;//压缩后jpeg图片的质量，建议是80到100.
    int bufSize;//缓存的大小。
    unsigned char * srcBuf;//rgb数据的缓存。
    unsigned char *outBuf; //压缩后数据保存位置。
    unsigned long outSize; //压缩后数据的大小。
    int row_stride ;	/* 设置一行的宽度。字节数。*/

private:
    void init(MyBitmap * bitmap, int quality);
public:
    MyJpegCompress(MyBitmap * bitmap, int quality = 100);
    MyJpegCompress(const MyJpegCompress & myJpeg);
    MyJpegCompress & operator=(const MyJpegCompress & myJpeg);
    ~MyJpegCompress();

    //bgra颜色转rgb颜色。
    void bgra2rgb(unsigned char * des, unsigned char * src, int width, int height);

    //压缩数据。
    void compress();

    void saveJpegFile(const char * fileName);

    unsigned char * getOutBuf();

    unsigned long getOutSize();
};



#endif // MYJPEGCOMPRESS_H
