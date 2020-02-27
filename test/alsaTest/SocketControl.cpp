#include "SocketControl.h"
#include "AudioControl.h"
#include <QTime>
#include <QDebug>

SocketControl::SocketControl(QObject *parent)
              : QObject(parent)
              , m_udpSocket(new QUdpSocket(this))
              , m_hostAddress(new QHostAddress("192.168.0.232"))
              , m_port(6000)
              , m_timer(new QTimer(this))
{

    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(m_udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(AudioControl::getInstance(), SIGNAL(sendCaptureData(const char*,uint)), this, SLOT(sendData(const char*,uint)));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(dealTimer()));
    //m_timer->start(1000);
    m_udpSocket->bind(QHostAddress::Any, 6000, QUdpSocket::ShareAddress);
}

void SocketControl::sendData(const char *data, const unsigned int len)
{
    m_udpSocket->writeDatagram(data, len, *m_hostAddress, m_port);
}

void SocketControl::readData()
{
    while (m_udpSocket->hasPendingDatagrams())
    {
       QByteArray datagram;
       datagram.resize(m_udpSocket->pendingDatagramSize());
       m_udpSocket->readDatagram(datagram.data(), datagram.size());
       qDebug() << QTime::currentTime().toString() << " Recv UDP Data :" << datagram.size();
       AudioControl::getInstance()->addToMixerData(2, datagram.data(), datagram.size());
    }
}

void SocketControl::error(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    qDebug() << m_udpSocket->errorString();
}

void SocketControl::dealTimer()
{

}
