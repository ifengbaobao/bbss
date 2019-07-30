#ifndef SOCKETRECV_H
#define SOCKETRECV_H

#include <QObject>
#include<QThread>
#include<QUdpSocket>
#include<QMap>


class SocketRecv : public QObject
{
    Q_OBJECT
private:
    QThread * thread;
    QUdpSocket * sock;
    QMap<quint32, QMap<quint16, QByteArray> > packets;
    int i = 0 ;
public:
    explicit SocketRecv();
    ~SocketRecv();
private:
    //数据合并。
    void combine(quint32 frameIdx);

signals:
    //接受数据的信号。
    void recvData(QByteArray data);
    void bindPort(quint16 port,QHostAddress address = QHostAddress::AnyIPv4);
    void unbindPort();

private slots:
    void slotReadyRead();
    void slotBindPort(quint16 port,QHostAddress address = QHostAddress::AnyIPv4);
    void slotUnbindPort();

};

#endif // SOCKETRECV_H
