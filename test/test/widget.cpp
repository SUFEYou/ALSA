#include "widget.h"
#include "ui_widget.h"
#include "RadioControl.h"

RadioControl radioControl;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    radioControl.start();
}

Widget::~Widget()
{
    radioControl.stop();
    radioControl.wait();
    delete ui;
}
