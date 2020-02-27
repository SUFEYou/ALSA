#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class SocketControl;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_up_clicked();

    void on_down_clicked();

private:
    Ui::Widget *ui;
    SocketControl *m_socketControl;
};

#endif // WIDGET_H
