#ifndef SCREENCONTROL_H
#define SCREENCONTROL_H

#include <QObject>
#include<QThread>
#include<QTimer>
#include"myscreen.h"
#include"mybitmap.h"

//屏幕控制。
class ScreenControl : public QObject
{
    Q_OBJECT
private:
    QThread * thread;
    MyScreen * screen;
    QTimer * timer;
    MyBitmap * bitmap;

public:
    explicit ScreenControl();
    ~ScreenControl();

    int getScreenWidth();
    int getScreenHeight();
    //一个像素的位数。注意是位数，不是字节数。一字节是8位。
    int getBitsPixel();

    //设置间隔时间。
    void setInterval(int msec);
    int getInterval();

    void start();
    void stop();

    void setMyBitmap(MyBitmap * bitmap);

signals:
    void copyScreenOver();

private slots:
    void slotTimeOut();
};

#endif // SCREENCONTROL_H
