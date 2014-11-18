#-------------------------------------------------
#
# Project created by QtCreator 2014-11-14T22:16:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartBluetoothOven
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    temperatureprofiler.cpp

HEADERS  += \
    temperatureprofiler.hpp \
    mainwindow.hpp

FORMS    += mainwindow.ui
