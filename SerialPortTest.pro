#-------------------------------------------------
#
# Project created by QtCreator 2017-03-23T16:39:57
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPortTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INSTALLS        = target
target.files    = widgettest
target.path     = /home/pi