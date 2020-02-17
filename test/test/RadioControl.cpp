#include "RadioControl.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include <QDebug>

#include <QDate>
#include <QTime>

RadioControl::RadioControl(QObject *parent)
            : QObject(parent)
            , m_TcpSocket(new QTcpSocket(this))
            , m_UdpSocket(new QUdpSocket(this))
            , m_Timer(new QTimer(this))
            , m_UdpStartTimer(new QTimer(this))
            , m_TcpAddr("192.168.1.5")
            , m_TcpPort(6060)
            ,m_UdpAddr("224.1.1.1")
            ,m_UdpPort(65000)
{
    connect(m_TcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_TcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(dealTimer()));

    connect(m_UdpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(udpError(QAbstractSocket::SocketError)));
    connect(m_UdpSocket, SIGNAL(readyRead()), this, SLOT(udpReadMessage()));
    connect(m_UdpStartTimer, SIGNAL(timeout()), this, SLOT(udpStartTimerDeal()));

    m_Timer1 = new QTimer(this);
    connect(m_Timer1, SIGNAL(timeout()), this, SLOT(test()));
}

void RadioControl::test()
{
    QString str = QString("%1 %2").arg(QDate::currentDate().toString()).arg(QTime::currentTime().toString());
    m_TcpSocket->write(str.toStdString().c_str(), str.length());
    m_UdpSocket->writeDatagram(str.toStdString().c_str(), str.length(), m_UdpAddr, m_UdpPort);
}

void RadioControl::start()
{
    m_TcpSocket->abort();
    m_TcpSocket->connectToHost(m_TcpAddr, m_TcpPort);
}
void RadioControl::stop()
{
    m_TcpSocket->close();
}

void RadioControl::dealTimer()
{
    m_TcpSocket->abort();
    m_TcpSocket->connectToHost(m_TcpAddr, m_TcpPort);
}

void RadioControl::connected()
{
    qDebug() << "connected";
    m_Timer->stop();
    m_Timer1->start(1000);
    const char str[10] = {"hello"};
    m_TcpSocket->write(str, 10);
}

void RadioControl::disconnected()
{
    qDebug() << "disconnected";
    m_Timer1->stop();
    m_Timer->start(10000);
}

void RadioControl::displayError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    qDebug() << m_TcpSocket->errorString();
    m_Timer->start(10000);
}

void RadioControl::readMessage()
{
    char data[1024];
    qint64 len = m_TcpSocket->read(data, 1024);
    if (len > 0)
        m_TcpSocket->write(data, len);
}

void RadioControl::UDPStart()
{
    m_UdpSocket->abort();
    //m_UdpSocket->bind(QHostAddress::Any, m_UdpPort, QUdpSocket::ShareAddress);
    m_UdpSocket->bind(QHostAddress::AnyIPv4, m_UdpPort, QUdpSocket::ShareAddress);
    m_UdpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);
    bool result = m_UdpSocket->joinMulticastGroup(m_UdpAddr);
    if (!result)
    {
        qDebug() << "UDP Join Multicast Group Err: " << m_UdpSocket->errorString();
        m_UdpStartTimer->start(10000);
    }
    else
    {
        qDebug() << "UDP Join Multicast Group Succeed!";
    }
}

void RadioControl::UDPStop()
{
    m_UdpSocket->leaveMulticastGroup(m_UdpAddr);
    m_UdpSocket->close();
}

void RadioControl::udpError(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err);
    qDebug() << "UDP Err: " << m_UdpSocket->errorString();
    m_UdpStartTimer->stop();
    m_UdpStartTimer->start(10000);
}

void RadioControl::udpStartTimerDeal()
{
    m_UdpSocket->bind(QHostAddress::Any, m_UdpPort, QUdpSocket::ShareAddress);
    m_UdpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);
    bool result = m_UdpSocket->joinMulticastGroup(m_UdpAddr);
    if (!result)
    {
        qDebug() << "UDP Join Multicast Group Err: " << m_UdpSocket->errorString();
    }
    else
    {
        qDebug() << "UDP Join Multicast Group Succeed!";
        m_UdpStartTimer->stop();
    }
}

void RadioControl::udpReadMessage()
{
    while (m_UdpSocket->hasPendingDatagrams())
   {
       QByteArray datagram;
       datagram.resize(m_UdpSocket->pendingDatagramSize());
       m_UdpSocket->readDatagram(datagram.data(),datagram.size());
       qDebug() << m_TcpPort << " Recv UDP Data :" << datagram;
   }
}
