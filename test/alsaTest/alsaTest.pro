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

#INCLUDEPATH += /root/test/OpenSource/install/include
#LIBS += -L/lib/x86_64-linux-gnu -lasound -ldl -lm
#LIBS += -L/root/test/OpenSource/install/lib -lbcg729

INCLUDEPATH += /usr/local/arm/alsa/include
INCLUDEPATH += /usr/local/arm/bcg729/include
LIBS += -L/usr/local/arm/alsa/lib -lasound
LIBS += -L/usr/local/arm/bcg729/lib -lbcg729


SOURCES += main.cpp\
        widget.cpp \
    AudioControl.cpp \
    AudioPlayback.cpp \
    AudioCapture.cpp \
    SoundMixer.cpp

HEADERS  += widget.h \
    AudioControl.h \
    AudioPlayback.h \
    AudioCapture.h \
    SoundMixer.h

FORMS    += widget.ui
