/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created: Fri Feb 28 09:39:12 2020
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QPushButton *up;
    QPushButton *down;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(320, 240);
        up = new QPushButton(Widget);
        up->setObjectName(QString::fromUtf8("up"));
        up->setGeometry(QRect(50, 50, 50, 50));
        down = new QPushButton(Widget);
        down->setObjectName(QString::fromUtf8("down"));
        down->setGeometry(QRect(120, 50, 50, 50));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", 0, QApplication::UnicodeUTF8));
        up->setText(QApplication::translate("Widget", "UP", 0, QApplication::UnicodeUTF8));
        down->setText(QApplication::translate("Widget", "DOWN", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
