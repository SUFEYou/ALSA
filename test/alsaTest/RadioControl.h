#ifndef RADIOCONTROL_H
#define RADIOCONTROL_H

#include <QObject>
#include <QHostAddress>
#include <QAbstractSocket>

class QTcpSocket;
class QUdpSocket;
class QTimer;

class RadioControl : public QObject
{
    Q_OBJECT
public:
    RadioControl(QObject *parent=NULL);
    void start();
    void stop();
    void UDPStart();
    void UDPStop();
    inline void setTcpAddr(QHostAddress addr) { m_TcpAddr = addr; }
    inline void setTcpPort(quint16 port) { m_TcpPort = port; }
    inline void setUdpAddr(QHostAddress addr) { m_UdpAddr = addr; }
    inline void setUdpPort(quint16 port) { m_UdpPort = port; }

protected:

private slots:
    void connected();
    void disconnected();
    void displayError(QAbstractSocket::SocketError err);
    void readMessage();

    void udpError(QAbstractSocket::SocketError err);
    void udpReadMessage();

    void dealTimer();
    void test();

    void udpStartTimerDeal();

private:
    QTcpSocket*         m_TcpSocket;
    QUdpSocket*         m_UdpSocket;
    QTimer*             m_Timer;
    QTimer*             m_Timer1;
    QTimer*             m_UdpStartTimer;

    QHostAddress        m_TcpAddr;
    quint16             m_TcpPort;
    QHostAddress        m_UdpAddr;
    quint16             m_UdpPort;

};

#endif // RADIOCONTROL_H
