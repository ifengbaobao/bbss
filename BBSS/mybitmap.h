#ifndef MYBITMAP_H
#define MYBITMAP_H

#include <Windows.h>
#include <stdio.h>
#include<stdlib.h>
#include<vector>


class MyBitmap
{
private:
    int width;//图片的宽
    int height;//图片的高。
    int bytesPixel;//图片的一个像素占用几个字节。
    int widthBytes;//每一行的字节数。宽的字节数。
    int dataSize;//图片数据占用多少个字节。
    BITMAPFILEHEADER head; //头信息。
    BITMAPINFOHEADER info;//图片信息。
    unsigned char * data;//图片的数据。

    int index; //块的索引。
    int x; //块的x坐标。
    int y; //块的y坐标。
    int srcWidth;//原图片的宽。
    int srcHeight;//原来图片的高。

private:
    void init(int width, int height, int bytesPixel);
    void initTag(const MyBitmap & bitmap);
public:
    MyBitmap(int width=0, int height=0, int bytesPixel=4);
    MyBitmap(const MyBitmap & bitmap);
    MyBitmap & operator=(const MyBitmap & bitmap);
    ~MyBitmap();

    //设置图片为白色的图片。
    void setImageWhite();

    //保存图片到文件。
    void saveMyBitmapToFile(const char * fileName);

    //把大图分隔成小图。并且保存到bitmaps中。
    void initCutBitmap(std::vector<MyBitmap> & bitmaps,int widthCount, int heightCount);

    //从大图中剪切一块小图的数据。
    void cutBitmapData(MyBitmap & bitmap);

    //剪切所有的数据。
    void cutBitmapData(std::vector<MyBitmap> & bitmaps);

    //绘制小图到自己。
    void drawBitmap(MyBitmap & bitmap);

    //绘制小图到自己。
    void drawBitmap(std::vector<MyBitmap> & bitmaps);

    //压缩，重新设置图片的大小，并输出。
    void resize(MyBitmap* bitmap, int width, int height);

    boolean operator==(const MyBitmap & bitmap);

    boolean operator!=(const MyBitmap & bitmap);

    //获取数据地址。
    unsigned char* getData();

    BITMAPINFOHEADER* getBitmapInfo();

    BITMAPFILEHEADER * getHead();

    int getWidth();

    int getHeight();

    int getSrcWidth();

    int getSrcHeight();

    int getX();

    int getY();

    int getIndex();

    int getWidthBytes();

    int getBytesPixel();

    void setX(int x);

    void setY(int y);

    void setIndex(int index);

    void setSrcWidth(int srcWidth);

    void setSrcHeight(int srcHeight);

    int getDataSize();

private:
    //平均分配数据。切割int数据。
    void cutInt(int size, int count, int * blockSize);


};

#endif // MYBITMAP_H
