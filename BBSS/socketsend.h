#ifndef SOCKETSEND_H
#define SOCKETSEND_H

#include <QObject>
#include<QByteArray>
#include<QUdpSocket>
#include<QVector>

class SocketSend : public QObject
{
    Q_OBJECT
private:
    quint32 frameIdx;
    QThread * thread;
    QUdpSocket * sock;
    quint16 port;
    QHostAddress address;
    int blockSize;
public:
    explicit SocketSend();
    ~SocketSend();
//    获取本机的广播地址。
    static QVector<QHostAddress> getLocalhostBroadcastIps();
    void setBlockSize(int blockSize);
    int getBlockSize();
signals:
    void sendData(QByteArray data);
    void sendDatas(QVector<QByteArray> data);
    void portAndHostAddress(quint16 port,QHostAddress address = QHostAddress::Broadcast);

private slots:
    void slotSendData(QByteArray data);
    void slotSendDatas(QVector<QByteArray> data);
    void slotPortAndHostAddress(quint16 port,QHostAddress address = QHostAddress::Broadcast);
};

#endif // SOCKETSEND_H
