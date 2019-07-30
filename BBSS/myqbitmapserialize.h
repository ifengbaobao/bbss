#ifndef MYQBITMAPSERIALIZE_H
#define MYQBITMAPSERIALIZE_H


#include"mybitmap.h"
#include<QByteArray>

class MyQBitmapSerialize
{
private:
    MyQBitmapSerialize();

public:
    //序列化。
    static QByteArray serialize(MyBitmap & bitmap,int quality);
    //反序列化。
    static MyBitmap deserialize(QByteArray & bs);
};

#endif // MYQBITMAPSERIALIZE_H
