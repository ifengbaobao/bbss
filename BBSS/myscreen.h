#ifndef MYSCREEN_H
#define MYSCREEN_H

#include<Windows.h>
#include"mybitmap.h"

//LIBS += -lGdi32

class MyScreen
{
private:
    HDC hdc;//创建显示设备的上下文。
    HBITMAP hBmpScreen;//根据设备上下文句柄，新建一个兼容的bitmap。
    HDC hmdc;//创建兼容的设备上下文。
    HBITMAP holdbmp;//在将新对象选进设备环境的同时返回指向前一次被选对象的指针。作用保存原来的对象，以便完成任务时恢复它。
    int width;//屏幕的宽。
    int height;//屏幕的高。
    int bitsPixel;//一个像素的位数。注意是位数，不是字节数。一字节是8位。
    int widthBytes;//一行的字节数。
public:
    MyScreen();
    ~MyScreen();
    //获取屏幕的宽。
    int getWidth();
    //获取屏幕的高。
    int getHeight();
    //一个像素的位数。注意是位数，不是字节数。一字节是8位。
    int getBitsPixel();
    //一行的字节数。
    int getWidthBytes();
    //复制屏幕到MyBitmap中。或者说截屏。
    void copyScreen(MyBitmap * bitmap);
};

#endif // MYSCREEN_H
