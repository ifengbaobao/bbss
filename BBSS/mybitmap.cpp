#include "mybitmap.h"

void MyBitmap::init(int width, int height, int bytesPixel)
{
    this->width = width;
    this->height = height;
    this->bytesPixel = bytesPixel;
    this->widthBytes = (width * bytesPixel + 3) >> 2 << 2;//必须是4的整倍数
    this->dataSize = height * this->widthBytes;

    //bitmap 文件头。
    this->head.bfType = ((WORD)('M' << 8) | 'B');
    this->head.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + this->dataSize;
    this->head.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    //bitmap 信息头。
    this->info.biSize = sizeof(BITMAPINFOHEADER);
    this->info.biWidth = width;
    this->info.biHeight = height;
    this->info.biPlanes = 1;
    this->info.biBitCount = bytesPixel * 8;//颜色要用到的位数，常用的值为1(黑白二色图), 4(16色图), 8(256色), 24(真彩色图)
    this->info.biCompression = BI_RGB; //BI_RGB bitmap是不压缩的。   //BI_BITFIELDS  565    BI_RGB   555
    this->info.biSizeImage = this->dataSize;

    this->data = new unsigned char[this->dataSize];
    memset(this->data, 0xff, this->dataSize);//图片初始化为白色的。

    this->index = 0;
    this->x = 0;
    this->y = 0;
    this->srcWidth = width;
    this->srcHeight = height;
}

void MyBitmap::initTag(const MyBitmap & bitmap)
{
    this->index = bitmap.index;
    this->x = bitmap.x;
    this->y = bitmap.y;
    this->srcWidth = bitmap.srcWidth;
    this->srcHeight = bitmap.srcHeight;
    memcpy (this->data,bitmap.data,bitmap.dataSize);
}

//构造函数。
MyBitmap::MyBitmap(int width, int height, int bytesPixel)
{
    this->init(width, height, bytesPixel);
}

MyBitmap::MyBitmap(const MyBitmap & bitmap)
{
    this->init(bitmap.width, bitmap.height, bitmap.bytesPixel);
    this->initTag(bitmap);
}

MyBitmap & MyBitmap::operator=(const MyBitmap & bitmap)
{
    if (this->data != NULL)
    {
        delete[] this->data;
    }
    this->init(bitmap.width, bitmap.height, bitmap.bytesPixel);
    this->initTag(bitmap);
    return *this;
}

//析构函数。
MyBitmap::~MyBitmap()
{
    delete[] this->data;
}

//设置图片位白色。
void MyBitmap::setImageWhite()
{
    memset(this->data, 0xff, this->dataSize);
}

//保存图片到文件。
void MyBitmap::saveMyBitmapToFile(const char * fileName)
{
    FILE * file = fopen(fileName, "wb");
    fwrite(&(this->head), sizeof(this->head), 1, file);
    fwrite(&(this->info), sizeof(this->info), 1, file);
    fwrite(this->data, this->dataSize, 1, file);
    fclose(file);
}

//把大图分隔成小图。并且保存到bitmaps中。
void MyBitmap::initCutBitmap(std::vector<MyBitmap> & bitmaps, int widthCount, int heightCount)
{
    bitmaps.clear();
    bitmaps.reserve(widthCount * heightCount);

    int * widths = new int[widthCount];
    this->cutInt(this->width, widthCount, widths);
    int * heights = new int[heightCount];
    this->cutInt(this->height, heightCount, heights);

    int index = 0;
    int y = 0;

    for (int i = 0; i < heightCount; ++i)
    {
        int x = 0;
        for (int j = 0; j < widthCount; ++j)
        {
            MyBitmap bitmap(widths[j], heights[i], this->bytesPixel);
            bitmap.index = index;
            ++index;
            bitmap.x = x;
            x += widths[j];
            bitmap.y = y;
            bitmap.srcWidth = this->width;
            bitmap.srcHeight = this->height;
            bitmaps.push_back(bitmap);
        }
        y += heights[i];
    }

    delete [] heights;
    delete [] widths;
}

//从大图中剪切一块小图的数据。
void MyBitmap::cutBitmapData(MyBitmap & bitmap)
{
    int x = bitmap.x;
    int y = bitmap.y;
    for (int i = 0; i < bitmap.height; ++i)
    {
        int begin = y * this->widthBytes + x * bitmap.bytesPixel;
        memcpy(bitmap.data + (bitmap.widthBytes * i), this->data + begin, bitmap.width * bitmap.bytesPixel);
        ++y;
    }
}

//剪切所有的数据。
void MyBitmap::cutBitmapData(std::vector<MyBitmap> & bitmaps)
{
    for(std::vector<MyBitmap>::iterator it = bitmaps.begin();it!= bitmaps.end();++it)
    {
        this->cutBitmapData(*it);
    }
}

//绘制小图到自己。
void MyBitmap::drawBitmap(MyBitmap & bitmap)
{
    int x = bitmap.x;
    int y = bitmap.y;
    for (int i = 0; i < bitmap.height; ++i)
    {
        int begin = y * this->widthBytes + x * bitmap.bytesPixel;
        memcpy(this->data + begin, bitmap.data + (bitmap.widthBytes * i), bitmap.width * bitmap.bytesPixel);
        ++y;
    }
}

//绘制小图到自己。
void MyBitmap::drawBitmap(std::vector<MyBitmap>& bitmaps)
{
    for (std::vector<MyBitmap>::iterator it = bitmaps.begin(); it != bitmaps.end(); ++it)
    {
        this->drawBitmap(*it);
    }
}

//压缩，重新设置图片的大小，并输出。
void MyBitmap::resize(MyBitmap * bitmap, int width, int height)
{
    int w_Dest = width;
    int h_Dest = height;
    unsigned char *src = this->data;
    int w_Src = this->width;
    int h_Src = this->height;

    int sw = w_Src - 1, sh = h_Src - 1, dw = w_Dest - 1, dh = h_Dest - 1;
    int B, N, x, y;
    int nPixelSize = this->bytesPixel;
    unsigned char *pLinePrev, *pLineNext;
    unsigned char *pDest = bitmap->getData();
    unsigned char *tmp;
    unsigned char *pA, *pB, *pC, *pD;

    for (int i = 0; i <= dh; ++i)
    {
        tmp = pDest + i * w_Dest*nPixelSize;
        y = i * sh / dh;
        N = dh - i * sh%dh;
        pLinePrev = src + (y++)*w_Src*nPixelSize;
        pLineNext = (N == dh) ? pLinePrev : src + y * w_Src*nPixelSize;
        for (int j = 0; j <= dw; ++j)
        {
            x = j * sw / dw * nPixelSize;
            B = dw - j * sw%dw;
            pA = pLinePrev + x;
            pB = pA + nPixelSize;
            pC = pLineNext + x;
            pD = pC + nPixelSize;
            if (B == dw)
            {
                pB = pA;
                pD = pC;
            }

            for (int k = 0; k < nPixelSize; ++k)
            {
                *tmp++ = (unsigned char)(int)(
                            (B * N * (*pA++ - *pB - *pC + *pD) + dw * N * *pB++
                             + dh * B * *pC++ + (dw * dh - dh * B - dw * N) * *pD++
                             + dw * dh / 2) / (dw * dh));
            }
        }
    }
}

boolean MyBitmap::operator==(const MyBitmap & bitmap)
{
    if (this->width != bitmap.width || this->height != bitmap.height || this->bytesPixel != bitmap.bytesPixel)
    {
        return false;
    }
    return memcmp(this->data, bitmap.data, this->dataSize) == 0;
}

boolean MyBitmap::operator!=(const MyBitmap & bitmap)
{
    return !(*this == bitmap);
}

//获取数据地址。
unsigned char * MyBitmap::getData()
{
    return this->data;
}

BITMAPINFOHEADER * MyBitmap::getBitmapInfo()
{
    return &info;
}

BITMAPFILEHEADER * MyBitmap::getHead()
{
    return &head;
}

int MyBitmap::getWidth()
{
    return this->width;
}

int MyBitmap::getHeight()
{
    return this->height;
}

int MyBitmap::getSrcWidth()
{
    return this->srcWidth;
}

int MyBitmap::getSrcHeight()
{
    return this->srcHeight;
}

int MyBitmap::getX()
{
    return this->x;
}

int MyBitmap::getY()
{
    return this->y;
}

int MyBitmap::getIndex()
{
    return this->index;
}

int MyBitmap::getWidthBytes()
{
    return this->widthBytes;
}

int MyBitmap::getBytesPixel()
{
    return this->bytesPixel;
}

void MyBitmap::setX(int x)
{
    this->x = x;
}

void MyBitmap::setY(int y)
{
    this->y = y;
}

void MyBitmap::setIndex(int index)
{
    this->index = index;
}

void MyBitmap::setSrcWidth(int srcWidth)
{
    this->srcWidth = srcWidth;
}

void MyBitmap::setSrcHeight(int srcHeight)
{
    this->srcHeight = srcHeight;
}

int MyBitmap::getDataSize()
{
    return this->dataSize;
}

//平均分配数据。切割int数据。
void MyBitmap::cutInt(int size, int count, int * blockSize)
{
    int blockSizeBase = size / count;
    int blockSizeTag = size % count;
    for (int i = 0; i < count; i++)
    {
        blockSize[i] = blockSizeBase;
        if (i < blockSizeTag)
        {
            ++(blockSize[i]);
        }
    }
}
