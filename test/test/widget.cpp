#include "widget.h"
#include "ui_widget.h"
#include "RadioControl.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    for (int i = 0; i < 1; ++i)
    {
        RadioControl* radio = new RadioControl;
        radio->setTcpPort(6060+i);
        radio->setUdpPort(65000+i);
        m_RadioControl.push_back(radio);
        radio->start();
        radio->UDPStart();
    }

}

Widget::~Widget()
{
    delete ui;
}
