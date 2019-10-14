#include "socketsend.h"
#include<QHostAddress>
#include<QThread>
#include<QUdpSocket>
#include<QDataStream>
#include<QByteArray>
#include<QVector>
#include<QDateTime>
#include<QDebug>
#include<QNetworkInterface>
#include <QMetaType>
#include<QDebug>

SocketSend::SocketSend() : QObject(0)
{
    qRegisterMetaType<QHostAddress>("QHostAddress");
    this->port = 0;
    frameIdx = 0;
    blockSize = 256;
    sock = new QUdpSocket(this);
    sock->bind();

    connect(this,&SocketSend::sendData,this,&SocketSend::slotSendData);
    connect(this,&SocketSend::sendDatas,this,&SocketSend::slotSendDatas);

    //关于线程的：
    thread = new QThread;
    connect(thread, &QThread::finished, sock, &QUdpSocket::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    this->moveToThread(thread);
    thread->start();

    connect(this,&SocketSend::portAndHostAddress,this,&SocketSend::slotPortAndHostAddress);

}

SocketSend::~SocketSend()
{
    thread->quit();
    thread->wait();
}

void SocketSend::slotSendData(QByteArray data)
{
    int tempBlockSize = this->blockSize - 12;// 256-12;
    ushort total = data.size()/tempBlockSize;
    if(data.size() % tempBlockSize)
    {
        ++total;
    }
    ++frameIdx;
    for(ushort index=1; index<=total; ++index)
    {
        QByteArray sub = data.mid((index-1)*tempBlockSize, tempBlockSize);
        QByteArray packet;
        QDataStream ds(&packet, QIODevice::ReadWrite);
        ds << index << total << sub << frameIdx;
        qint64 ret = sock->writeDatagram(packet,  address, this->port);
        int maxCount = 5;
        while(ret == -1 && maxCount > 0)
        {
            ret = sock->writeDatagram(packet,  address, this->port);
            qDebug()<<"数据失败重试："<<maxCount;
            --maxCount;
        }
        if(ret == -1)
        {
            qDebug()<<"数据最终失败！";
            break;
        }
        static int sockCount = 0;
        ++sockCount;
        if(sockCount > 64 ){
            sockCount = 0;
            QThread::usleep(5);
        }
    }


}

void SocketSend::slotSendDatas(QVector<QByteArray> data)
{
//    static quint64 old= 0;
//    quint64 begin = QDateTime::currentDateTime().toMSecsSinceEpoch();

    for(QVector<QByteArray>::iterator it = data.begin();it != data.end();++it)
    {
        this->slotSendData(*it);
    }
//    qDebug()<<"udp发送时间："<<(QDateTime::currentDateTime().toMSecsSinceEpoch() - begin);
//    qDebug()<<"距离上次时间："<<(QDateTime::currentDateTime().toMSecsSinceEpoch() - old);
//    old =
      QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void SocketSend::slotPortAndHostAddress(quint16 port,QHostAddress address)
{
    this->port = port;
    this->address = address;
}

void SocketSend::setBlockSize(int blockSize)
{
    if(blockSize > sock->socketOption(QUdpSocket::SendBufferSizeSocketOption).toInt())
    {
        blockSize = sock->socketOption(QUdpSocket::SendBufferSizeSocketOption).toInt();
    }
    this->blockSize =blockSize;
}

int SocketSend::getBlockSize()
{
    return this->blockSize;
}


QVector<QHostAddress> SocketSend::getLocalhostBroadcastIps()
{
    QVector<QHostAddress> adds;
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for(QList<QNetworkInterface>::iterator itInterface = list.begin();itInterface != list.end();++itInterface)
    {
        QList<QNetworkAddressEntry> entryList = itInterface->addressEntries();
        for(QList<QNetworkAddressEntry>::iterator itAdd = entryList.begin() ; itAdd != entryList.end();++itAdd)
        {
            QHostAddress broadcast = itAdd->broadcast();
            if((!broadcast.isNull()) && broadcast.protocol() == QAbstractSocket::IPv4Protocol)
            {
                adds.push_back(broadcast);
            }
        }
    }
    return adds;
}
