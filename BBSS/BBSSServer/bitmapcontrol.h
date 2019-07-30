#ifndef BITMAPCONTROL_H
#define BITMAPCONTROL_H

#include <QObject>
#include<QThread>
#include"mybitmap.h"
#include<vector>

class BitmapControl : public QObject
{
    Q_OBJECT
private:
    QThread * thread;
    MyBitmap * bitmap;
    std::vector<MyBitmap> itemBitmapOld;
    std::vector<MyBitmap> itemBitmapNew;
    int widthCount;
    int heightCount;
    int quality;
    int allCount;
    int sendTag;
public:
    explicit BitmapControl(int width, int height, int bytesPixe,int widthCount = 10,int heightCount = 10);
    ~BitmapControl();

    MyBitmap * getMyBitmap();

    //设置图片质量，100最高，10，最低。
    void setQuality(int quality);
    int getQuality();

signals:
    void itemData(QVector<QByteArray> items);

public slots:
    void slotBitmapUpdata();

};

#endif // BITMAPCONTROL_H
