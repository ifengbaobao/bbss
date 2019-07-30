#include "screencontrol.h"
#include<QDebug>
#include<QDateTime>

ScreenControl::ScreenControl() : QObject(0)
{
    screen = new MyScreen;

    timer = new QTimer;
    setInterval(300);
    bitmap = NULL;

    connect(timer,&QTimer::timeout,this,&ScreenControl::slotTimeOut);

    thread = new QThread;
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    this->moveToThread(thread);
    thread->start();
}

ScreenControl::~ScreenControl()
{
    thread->quit();
    thread->wait();
    delete screen;
}

void ScreenControl::slotTimeOut()
{
    qint64 begin =QDateTime::currentDateTime().toMSecsSinceEpoch();
    this->screen->copyScreen(this->bitmap);
//    qDebug()<<"拷贝屏幕："<<(QDateTime::currentDateTime().toMSecsSinceEpoch() - begin);
    emit this->copyScreenOver();
}

void ScreenControl::setInterval(int msec)
{
    this->timer->setInterval(msec);
}

int ScreenControl::getInterval()
{
    this->timer->interval();
}

void ScreenControl::start()
{
    if(this->bitmap == NULL)
    {
        return;
    }
    this->timer->start();
}

void ScreenControl::stop()
{
    this->timer->stop();
}

void ScreenControl::setMyBitmap(MyBitmap * bitmap)
{
    this->bitmap = bitmap;
}

int ScreenControl::getScreenWidth()
{
    return screen->getWidth();
}

int ScreenControl::getScreenHeight()
{
    return screen->getHeight();
}

//一个像素的位数。注意是位数，不是字节数。一字节是8位。
int ScreenControl::getBitsPixel()
{
    return screen->getBitsPixel();
}
