#include "widget.h"
#include <QApplication>
#include "AudioControl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<AudioPeriodData>("AudioPeriodData");

    Widget w;
    w.show();

    return a.exec();
}
