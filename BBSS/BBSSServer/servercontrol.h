#ifndef SERVERCONTROL_H
#define SERVERCONTROL_H

#include <QObject>
#include<QHostAddress>
#include"screencontrol.h"
#include"mybitmap.h"
#include"bitmapcontrol.h"
#include"socketsend.h"

class ServerControl : public QObject
{
    Q_OBJECT
private:
    ScreenControl * sc;
    BitmapControl * bc;
    SocketSend * ss;
    quint16 port;
    QHostAddress address;

public:
    explicit ServerControl(QObject *parent = 0);
    ~ServerControl();

    //    启动
    void start();
    //    停止
    void stop();

    //    设置间隔时间
    void setInterval(int msec);
    int getInterval();
    //    设置图片质量。10到100
    void setQuality(int quality);
    int getQuality();
    //    设置端口和ip。
    void setPortAndHostAddress(quint16 port,QHostAddress address = QHostAddress::Null);
    int getPort();
    QHostAddress getIpHostAddress();
    //设置sock的块大小。
    void setSockBlockSize(int blockSize);
    int getSockBlockSize();

    //    获取本机的广播地址。
    static QVector<QHostAddress> getLocalhostBroadcastIps();


signals:

private slots:
    void slotItemData(QVector<QByteArray> items);
};

#endif // SERVERCONTROL_H
