#include "servercontrol.h"

ServerControl::ServerControl(QObject *parent) : QObject(parent),address(QHostAddress::Null)
{
    sc = new ScreenControl;
    bc = new BitmapControl(sc->getScreenWidth(),sc->getScreenHeight(),sc->getBitsPixel()/8);
    ss = new SocketSend;
    sc->setMyBitmap(bc->getMyBitmap());

    connect(sc,&ScreenControl::copyScreenOver,bc,&BitmapControl::slotBitmapUpdata);
    connect(bc,&BitmapControl::itemData,this,&ServerControl::slotItemData);
    port = 0;
}

ServerControl::~ServerControl()
{
    delete sc;
    delete bc;
    delete ss;
}

// 启动
void ServerControl::start()
{
    if(port == 0)
    {
        return;
    }
    sc->start();
}

// 停止
void ServerControl::stop()
{
    sc->stop();
}

// 设置间隔时间
void ServerControl::setInterval(int msec)
{
    if(msec <100)
    {
        msec = 100;
    }
    sc->setInterval(msec);
}

int ServerControl::getInterval()
{
    return sc->getInterval();
}

// 设置图片质量。10到100
void ServerControl::setQuality(int quality)
{
    if(quality > 100)
    {
        quality = 100;
    }else if(quality <10)
    {
        quality = 10;
    }
    bc->setQuality(quality);
}

int ServerControl::getQuality()
{
    return bc->getQuality();
}

//    设置端口和ip。
void ServerControl::setPortAndHostAddress(quint16 port,QHostAddress address)
{
    if(address == QHostAddress::Null)
    {
        QVector<QHostAddress> lbs = ServerControl::getLocalhostBroadcastIps();
        if(lbs.size()>0)
        {
            address = lbs.at(0);
        }
    }
    this->port = port;
    this->address = address;
    emit ss->portAndHostAddress(port,address);
}

int ServerControl::getPort()
{
    return this->port;
}

QHostAddress ServerControl::getIpHostAddress()
{
    return this->address;
}

void ServerControl::slotItemData(QVector<QByteArray> items)
{
    emit ss->sendDatas(items);
}

//    获取本机的广播地址。
QVector<QHostAddress> ServerControl::getLocalhostBroadcastIps()
{
    return SocketSend::getLocalhostBroadcastIps();
}

//设置sock的块大小。
void ServerControl::setSockBlockSize(int blockSize)
{
    ss->setBlockSize(blockSize);
}

int ServerControl::getSockBlockSize()
{
    return ss->getBlockSize();
}
