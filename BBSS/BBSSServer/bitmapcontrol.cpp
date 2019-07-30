#include "bitmapcontrol.h"
#include<QByteArray>
#include<QVector>
#include"myqbitmapserialize.h"
#include<QDateTime>
#include <QMetaType>
#include<QDebug>


BitmapControl::BitmapControl(int width, int height, int bytesPixe,int widthCount ,int heightCount ) : QObject(0)
{
    qRegisterMetaType<QVector<QByteArray> >("QVector<QByteArray>");
    this->widthCount = widthCount;
    this->heightCount = heightCount;
    bitmap = new MyBitmap(width,height,bytesPixe);
    allCount = widthCount*heightCount;
    itemBitmapOld.reserve(allCount);
    itemBitmapNew.reserve(allCount);
    bitmap->initCutBitmap(itemBitmapOld,widthCount,heightCount);
    bitmap->initCutBitmap(itemBitmapNew,widthCount,heightCount);

    thread = new QThread;
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    this->moveToThread(thread);
    thread->start();

    //图片质量默认是100.
    quality = 100;

    sendTag = 0;
}

BitmapControl::~BitmapControl()
{
    thread->quit();
    thread->wait();
    delete bitmap;
}

//设置图片质量，100最高，10，最低。
void BitmapControl::setQuality(int quality)
{
    this->quality = quality;
}

int BitmapControl::getQuality()
{
    return this->quality;
}

MyBitmap * BitmapControl::getMyBitmap()
{
    return this->bitmap;
}


void BitmapControl::slotBitmapUpdata()
{
    if(sendTag >= allCount)
    {
        sendTag = 0;
    }
    quint64 begin = QDateTime::currentDateTime().toMSecsSinceEpoch();
    bitmap->cutBitmapData(itemBitmapNew);

    QVector<QByteArray> temps;
    int tempTag = 0 ;
    for(std::vector<MyBitmap>::iterator newIt = itemBitmapNew.begin(),oldIt = itemBitmapOld.begin();newIt != itemBitmapNew.end();++newIt,++oldIt)
    {
        if( tempTag == sendTag || ((*newIt) != (*oldIt)))
        {
            temps.push_back(MyQBitmapSerialize::serialize(*newIt,quality));
        }
        ++tempTag;
    }
    itemBitmapNew.swap(itemBitmapOld);
    ++sendTag;
//    qDebug()<<"图片更新："<<(QDateTime::currentDateTime().toMSecsSinceEpoch() - begin);
    emit this->itemData(temps);
}

