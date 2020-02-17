TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#INCLUDEPATH += /home/OpenSource/jthread-1.3.3/install/gcc/include
#INCLUDEPATH += /home/OpenSource/jrtplib-3.11.1/install/gcc/include/jrtplib3

#LIBS += -L/home/OpenSource/jthread-1.3.3/install/gcc/lib  -ljthread
#LIBS += -L/home/OpenSource/jrtplib-3.11.1/install/gcc/lib -ljrtp


INCLUDEPATH += /home/OpenSource/jthread-1.3.3/install/arm/include
INCLUDEPATH += /home/OpenSource/jrtplib-3.11.1/install/arm/include/jrtplib3

LIBS += -L/home/OpenSource/jthread-1.3.3/install/arm/lib -ljthread
LIBS += -L/home/OpenSource/jrtplib-3.11.1/install/arm/lib -ljrtp

SOURCES += main.cpp
