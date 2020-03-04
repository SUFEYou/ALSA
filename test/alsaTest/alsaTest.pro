#-------------------------------------------------
#
# Project created by QtCreator 2020-02-19T13:08:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = alsaTest
TEMPLATE = app

QMAKE_CXXFLAGS += -fpermissive
QMAKE_CXXFLAGS += -Wno-psabi

CONFIG += debug

#INCLUDEPATH += /root/test/OpenSource/install/include
#INCLUDEPATH += /usr/local/include
#LIBS += -L/lib/x86_64-linux-gnu -lasound -ldl -lm
#LIBS += -L/root/test/OpenSource/install/lib -lbcg729
#LIBS += -L/usr/local/lib -lopus

#INCLUDEPATH += /usr/local/arm/alsa/include
#INCLUDEPATH += /usr/local/arm/bcg729/include
#INCLUDEPATH += /root/test/OpenSource/Opus/install/include
#LIBS += -L/usr/local/arm/alsa/lib -lasound
#LIBS += -L/usr/local/arm/bcg729/lib -lbcg729
#LIBS += -L/root/test/OpenSource/Opus/install/lib -lopus

INCLUDEPATH += /usr/local/arm/alsa_4.6.2/include
INCLUDEPATH += /usr/local/arm/bcg729_4.6.2/include
INCLUDEPATH += /root/test/OpenSource/Opus/opus_4.6.2/include
LIBS += -L/usr/local/arm/alsa_4.6.2/lib -lasound
LIBS += -L/usr/local/arm/bcg729_4.6.2/lib -lbcg729
LIBS += -L/root/test/OpenSource/Opus/opus_4.6.2/lib -lopus


SOURCES += main.cpp\
        widget.cpp \
    AudioControl.cpp \
    AudioPlayback.cpp \
    AudioCapture.cpp \
    SoundMixer.cpp \
    SocketControl.cpp \
    RadioControl.cpp

HEADERS  += widget.h \
    AudioControl.h \
    AudioPlayback.h \
    AudioCapture.h \
    SoundMixer.h \
    SocketControl.h \
    RadioControl.h

FORMS    += widget.ui
