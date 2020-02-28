#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>

namespace Ui {
class Widget;
}

class RadioControl;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

private:
    Ui::Widget                      *ui;
    std::vector<RadioControl*>      m_RadioControl;
};

#endif // WIDGET_H
