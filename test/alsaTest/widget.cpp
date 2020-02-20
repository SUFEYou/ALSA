#include "widget.h"
#include "ui_widget.h"
#include "AudioControl.h"
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QString status;
    AudioControl::getInstance()->audioControlInit(status);
    qDebug() << status;
}

Widget::~Widget()
{
    delete ui;
}
