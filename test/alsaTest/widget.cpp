#include "widget.h"
#include "ui_widget.h"
#include "AudioControl.h"
#include "SocketControl.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
       : QWidget(parent)
       , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QString status;
    if (AudioControl::getInstance()->audioControlInit(status))
    {
        AudioControl::getInstance()->start();
        for (int i = 0; i < 5; ++i)
        {
            QSharedPointer<SocketControl> tmp(new SocketControl(this, 6000+i, i+10));
            m_socketControl.push_back(tmp);
        }
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
