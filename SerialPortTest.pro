#-------------------------------------------------
#
# Project created by QtCreator 2017-03-23T16:39:57
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPortTest
TEMPLATE = app

//INCLUDEPATH += /usr/include/QModbus
//LIBS += -lQModbus

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    flurosens.h

FORMS    += mainwindow.ui

INSTALLS        = target
target.files    = widgettest
target.path     = /home/pi
