#include "SocketControl.h"
#include <QTime>
#include <QDebug>
#include <stdio.h>

#define UDPADDRESS "192.168.1.15"
#define TCPADDRESS "192.168.1.5"

SocketControl::SocketControl(QObject *parent)
              : QObject(parent)
              , m_udpSocket(new QUdpSocket(this))
              , m_hostAddress(UDPADDRESS)
              , m_port(6000)
              , m_id(0)
              , m_timer(new QTimer(this))
              , m_TcpSocket(new QTcpSocket(this))
              , m_TcpAddr("192.168.1.5")
              , m_TcpPort(6060)
{
    init();
}

SocketControl::SocketControl(QObject *parent, quint16 port, quint8 id)
              : QObject(parent)
              , m_udpSocket(new QUdpSocket(this))
              , m_hostAddress(UDPADDRESS)
              , m_port(port)
              , m_id(id)
              , m_timer(new QTimer(this))
              , m_TcpSocket(new QTcpSocket(this))
              , m_TcpAddr("192.168.1.5")
              , m_TcpPort(6060)
{
    init();
}

void SocketControl::init()
{
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(m_udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    //connect(AudioControl::getInstance(), SIGNAL(sendCaptureData(uint8_t*,uint)), this, SLOT(sendData(uint8_t*,uint)));
    connect(AudioControl::getInstance(), SIGNAL(sendCaptureData(AudioPeriodData)), this, SLOT(sendData(AudioPeriodData)));
    m_udpSocket->bind(QHostAddress::Any, m_port, QUdpSocket::ShareAddress);

    //connect(m_TcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    //connect(m_TcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    //connect(m_TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    //connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    //connect(m_timer, SIGNAL(timeout()), this, SLOT(dealTimer()));
}

//void SocketControl::sendData(uint8_t *data, const unsigned int len)
//{
//    //qDebug() << QTime::currentTime().toString() << " Send Data :" << len;
//    m_udpSocket->writeDatagram(data, len, m_hostAddress, m_port);
//    //m_TcpSocket->write(data, len);
//    printf("len = %d, send Data: ", len);
//    for (unsigned int i = 0; i < len; ++i)
//        printf("%02X ", data[i]);
//    printf("\n");
//}

void SocketControl::sendData(AudioPeriodData data)
{
    m_udpSocket->writeDatagram(data.data, data.dataLen, m_hostAddress, m_port);

//    printf("len = %d, send Data: ", data.dataLen);
//    for (unsigned int i = 0; i < data.dataLen; ++i)
//        printf("%02X ", data.data[i]);
//    printf("\n");
}

void SocketControl::readData()
{
    while (m_udpSocket->hasPendingDatagrams())
    {
       QByteArray datagram;
       datagram.resize(m_udpSocket->pendingDatagramSize());
       m_udpSocket->readDatagram(datagram.data(), datagram.size());
       //qDebug() << QTime::currentTime().toString() << " Recv UDP Data :" << datagram.size();

       //AudioControl::getInstance()->addToMixerData(m_id, datagram.data(), datagram.size());

       AudioControl::getInstance()->decoder(m_id, datagram.data(), datagram.size());
    }
}

void SocketControl::error(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qDebug() << m_udpSocket->errorString();
}

void SocketControl::tcpClientStart()
{
    m_TcpSocket->abort();
    m_TcpSocket->connectToHost(m_TcpAddr, m_TcpPort);
}

void SocketControl::dealTimer()
{
    m_TcpSocket->abort();
    m_TcpSocket->connectToHost(m_TcpAddr, m_TcpPort);
}

void SocketControl::connected()
{
    qDebug() << "connected";
    m_timer->stop();
}

void SocketControl::disconnected()
{
    qDebug() << "disconnected";
    m_timer->start(10000);
}

void SocketControl::displayError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    qDebug() << m_TcpSocket->errorString();
    m_timer->start(10000);
}

void SocketControl::readMessage()
{
    QByteArray array;
    array = m_TcpSocket->readAll();
    if (array.length() > 0)
    {
        qDebug() << QTime::currentTime().toString() << " Recv TCP Data :" << array.size();
        //AudioControl::getInstance()->addToPlaybackDataList(array.data(), array.size());
    }
}
