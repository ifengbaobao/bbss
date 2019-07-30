#include "myscreen.h"

//构造函数。
MyScreen::MyScreen()
{
    //创建显示设备的上下文。
    this->hdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    //获取屏幕的宽高。
    this->width = GetDeviceCaps(this->hdc, HORZRES);
    this->height = GetDeviceCaps(this->hdc, VERTRES);

    //根据设备上下文句柄，新建一个兼容的bitmap。
    this->hBmpScreen = CreateCompatibleBitmap(this->hdc, this->width, this->height);

    //创建兼容的设备上下文。
    this->hmdc = CreateCompatibleDC(this->hdc);

    //在将新对象选进设备环境的同时返回指向前一次被选对象的指针。作用保存原来的对象，以便完成任务时恢复它。
    this->holdbmp = (HBITMAP)SelectObject(this->hmdc, this->hBmpScreen);

    BITMAP bm;
    GetObject(this->hBmpScreen, sizeof(bm), &bm);
    this->bitsPixel = bm.bmBitsPixel;
    this->widthBytes = bm.bmWidthBytes;
}

//析构函数。
MyScreen::~MyScreen()
{
    this->hBmpScreen = (HBITMAP)SelectObject(this->hmdc, this->holdbmp);
}

//获取屏幕宽。
int MyScreen::getWidth()
{
    return this->width;
}

//获取屏幕高。
int MyScreen::getHeight()
{
    return this->height;
}

//一个像素的位数。注意是位数，不是字节数。一字节是8位。
int MyScreen::getBitsPixel()
{
    return this->bitsPixel;
}

//一行的字节数。
int MyScreen::getWidthBytes()
{
    return this->widthBytes;
}

//复制屏幕到MyBitmap中。或者说截屏。
void MyScreen::copyScreen(MyBitmap * bitmap)
{
    //把屏幕的内存数据，复制到兼容设备的内存数据中。
    BitBlt(this->hmdc, 0, 0, this->width, this->height, this->hdc, 0, 0, SRCCOPY);
    GetDIBits(this->hmdc, this->hBmpScreen, 0L, (DWORD)this->height, bitmap->getData(), (LPBITMAPINFO)(bitmap->getBitmapInfo()), (DWORD)DIB_RGB_COLORS);
}
