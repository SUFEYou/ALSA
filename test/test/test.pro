#-------------------------------------------------
#
# Project created by QtCreator 2020-02-12T18:02:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    RadioControl.cpp

HEADERS  += widget.h \
    RadioControl.h

FORMS    += widget.ui
