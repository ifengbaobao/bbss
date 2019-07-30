#include "socketrecv.h"
#include<QMap>
#include<QDataStream>
#include<QByteArray>
#include <QMetaType>

SocketRecv::SocketRecv() : QObject(0)
{
    qRegisterMetaType<QHostAddress>("QHostAddress");

    sock = new QUdpSocket(this);
    connect(sock,&QUdpSocket::readyRead, this, &SocketRecv::slotReadyRead);


    //关于线程的：
    thread = new QThread;
    connect(thread, &QThread::finished, sock, &QUdpSocket::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    this->moveToThread(thread);
    thread->start();

    connect(this,&SocketRecv::bindPort,this,&SocketRecv::slotBindPort);
    connect(this,&SocketRecv::unbindPort,this,&SocketRecv::slotUnbindPort);
}

SocketRecv::~SocketRecv()
{
    thread->quit();
    thread->wait();
}

void SocketRecv::slotBindPort(quint16 port,QHostAddress address)
{
    sock->bind(address, port);
    //        this->slotReadyRead();
}

void SocketRecv::slotUnbindPort()
{
    sock->disconnectFromHost();
}

void SocketRecv::slotReadyRead()
{


    while(sock->hasPendingDatagrams())
    {
        qint64 len = sock->pendingDatagramSize();
        QByteArray buf(len, 0);
        sock->readDatagram(buf.data(), buf.size());

        QDataStream ds(buf);
        quint32 frameIdx;
        quint16 index;
        quint16 total;
        QByteArray sub;
        ds >> index >> total >> sub >> frameIdx;

        packets[frameIdx][index] = sub;
        if(packets[frameIdx].size() == total)
        {
            combine(frameIdx);
        }
    }
}

//数据合并。
void SocketRecv::combine(quint32 frameIdx)
{
    QByteArray buf;
    QMap<quint16, QByteArray>& framePackets = packets[frameIdx];
    for(quint16 idx = 1; idx <= framePackets.size(); ++idx)
    {
        //如果数据不完整，直接退出。
        if(framePackets.find(idx) == framePackets.end())
        {
            packets.remove(frameIdx);
            return;
        }
        buf += framePackets[idx];
    }
    packets.remove(frameIdx);
    if(packets.size()>20)
    {
        packets.clear();
    }
    emit recvData(buf);
}
