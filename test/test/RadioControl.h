#ifndef RADIOCONTROL_H
#define RADIOCONTROL_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QAbstractSocket>

class QTcpSocket;
class QUdpSocket;

class RadioControl : public QThread
{
    Q_OBJECT
public:
    RadioControl();
    void stop();

protected:
    virtual void run();

private slots:
    void connected();
    void disconnected();
    void displayError(QAbstractSocket::SocketError err);
    void readMessage();


private:
    volatile bool       m_stoped;
    QMutex              m_Mutex;
    QTcpSocket*         m_TcpSocket;
    QUdpSocket*         m_UdpSocket;

};

#endif // RADIOCONTROL_H
