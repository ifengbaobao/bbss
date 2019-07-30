#include "myqbitmapserialize.h"
#include<QDataStream>
#include"myjpegcompress.h"
#include"myjpegdecompress.h"

MyQBitmapSerialize::MyQBitmapSerialize()
{}

//序列化。
QByteArray MyQBitmapSerialize::serialize(MyBitmap & bitmap,int quality)
{
    MyJpegCompress jpeg(&bitmap,quality);
    jpeg.compress();

    QByteArray packet;
    QDataStream ds(&packet, QIODevice::ReadWrite);
    ds << bitmap.getSrcWidth()
       << bitmap.getSrcHeight()
       << bitmap.getIndex()
       << bitmap.getX()
       << bitmap.getY()
       << (unsigned int)(jpeg.getOutSize());
    ds.writeBytes(((const char *)(jpeg.getOutBuf())),((unsigned int)(jpeg.getOutSize())));
    return packet;
}

//反序列化。
MyBitmap MyQBitmapSerialize::deserialize(QByteArray & packet)
{
    int srcWidth = 0 ;
    int srcHeight = 0;
    int index = 0 ;
    int x = 0;
    int y = 0 ;
    unsigned int outSize = 0;
    char * data = NULL;

    QDataStream ds(packet);
    ds >> srcWidth
            >> srcHeight
            >> index
            >>x
            >>y
            >>outSize;
    ds.readBytes(data,outSize);
    MyBitmap bitmap = MyJpegDecompress().decompress((unsigned char *)data,(unsigned long)outSize);
    delete[] data;
    bitmap.setX(x);
    bitmap.setY(y);
    bitmap.setIndex(index);
    bitmap.setSrcWidth(srcWidth);
    bitmap.setSrcHeight(srcHeight);
    return bitmap;
}
