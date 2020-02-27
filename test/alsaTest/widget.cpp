#include "widget.h"
#include "ui_widget.h"
#include "AudioControl.h"
#include "SocketControl.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
       : QWidget(parent)
       , ui(new Ui::Widget)
       , m_socketControl(new SocketControl(this))
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QString status;
    if (AudioControl::getInstance()->audioControlInit(status))
    {
        AudioControl::getInstance()->start();
        //m_socketControl->tcpClientStart();
    }
    qDebug() << status;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_up_clicked()
{
    AudioControl::getInstance()->up();
}

void Widget::on_down_clicked()
{
    AudioControl::getInstance()->down();
}
