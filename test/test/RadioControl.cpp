#include "RadioControl.h"
#include <QMutexLocker>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QDebug>

RadioControl::RadioControl()
             : m_stoped(false)
{

}

void RadioControl::stop()
{
    QMutexLocker locker(&m_Mutex);
    m_stoped = true;
}

void RadioControl::run()
{
    if (m_TcpSocket == NULL)
    {
        m_TcpSocket = new QTcpSocket();
        connect(m_TcpSocket, SIGNAL(connected()), this, SLOT(connected()));
        connect(m_TcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(m_TcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
        connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    }
    if (m_UdpSocket == NULL)
    {
        m_UdpSocket = new QUdpSocket();
    }
    m_TcpSocket->connectToHost(QHostAddress("192.168.1.5"), 6060);
    while(!m_stoped)
    {

    }
    m_stoped = false;
}

void RadioControl::connected()
{
    qDebug() << "connected";
    const char str[10] = {"hello"};
    m_TcpSocket->write(str, 10);
}

void RadioControl::disconnected()
{
    qDebug() << "disconnected";
}

void RadioControl::displayError(QAbstractSocket::SocketError err)
{
    qDebug() << m_TcpSocket->errorString();
}

void RadioControl::readMessage()
{
    char data[1024];
    qint64 len = m_TcpSocket->read(data, 1024);
    if (len > 0)
        m_TcpSocket->write(data, len);
}
