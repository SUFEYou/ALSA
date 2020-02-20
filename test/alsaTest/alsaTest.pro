#-------------------------------------------------
#
# Project created by QtCreator 2020-02-19T13:08:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = alsaTest
TEMPLATE = app

QMAKE_CXXFLAGS += -fpermissive

#LIBS += -L/lib/x86_64-linux-gnu -lasound -ldl -lm

INCLUDEPATH += /usr/local/arm/alsa/include
LIBS += -L/usr/local/arm/alsa/lib -lasound
#LIBS += -L/usr/local/arm/4.3.2/arm-none-linux-gnueabi/libc/usr/lib -ldl -lm


SOURCES += main.cpp\
        widget.cpp \
    AudioControl.cpp \
    AudioPlayback.cpp \
    AudioCapture.cpp

HEADERS  += widget.h \
    AudioControl.h \
    AudioPlayback.h \
    AudioCapture.h

FORMS    += widget.ui
