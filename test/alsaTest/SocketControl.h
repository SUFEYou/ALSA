#ifndef SOCKETCONTROL_H
#define SOCKETCONTROL_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>

class SocketControl : public QObject
{
    Q_OBJECT
public:
    SocketControl(QObject *parent = NULL);
    SocketControl(QObject *parent = NULL, quint16 port = 6000, quint8 id = 0);

private:
    void init();

public slots:
    void sendData(const char *data, const unsigned int len);
    void tcpClientStart();

private slots:
    void readData();
    void error(QAbstractSocket::SocketError socketError);
    void dealTimer();

    void connected();
    void disconnected();
    void displayError(QAbstractSocket::SocketError err);
    void readMessage();

private:
    QUdpSocket          *m_udpSocket;
    QHostAddress        m_hostAddress;
    quint16             m_port;
    quint8              m_id;
    QTimer              *m_timer;

    QTcpSocket          *m_TcpSocket;
    QHostAddress        m_TcpAddr;
    quint16             m_TcpPort;

};

#endif // SOCKETCONTROL_H
