#-------------------------------------------------
#
# Project created by QtCreator 2016-06-13T17:01:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SystemMonitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    systeminfo.cpp \
    proc.cpp \
    resourceinfo.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    systeminfo.h \
    proc.h \
    resourceinfo.h \
    qcustomplot.h

FORMS    += mainwindow.ui

RESOURCES += \
    img.qrc
