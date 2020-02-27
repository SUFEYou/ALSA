#ifndef SOCKETCONTROL_H
#define SOCKETCONTROL_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class SocketControl : public QObject
{
    Q_OBJECT
public:
    SocketControl(QObject *parent = NULL);

public slots:
    void sendData(const char *data, const unsigned int len);

private slots:
    void readData();
    void error(QAbstractSocket::SocketError socketError);
    void dealTimer();

private:
    QUdpSocket          *m_udpSocket;
    QHostAddress        *m_hostAddress;
    quint16             m_port;
    QTimer              *m_timer;
};

#endif // SOCKETCONTROL_H
