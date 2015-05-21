#-------------------------------------------------
#
# Project created by QtCreator 2015-01-23T11:51:40
#
#-------------------------------------------------

CODECFORTR = UTF-8
CODECFORSRC = UTF-8@

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = PARTONS
TEMPLATE = lib

OBJECTS_DIR = obj
MOC_DIR = moc

DEFINES += QCUSTOMPLOT_COMPILE_LIBRARY

INCLUDEPATH += /usr/include
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/qt/v4.8.6/include
INCLUDEPATH += /usr/local/root/v5.34.21/include

LIBS += -L/usr/local/root/v5.34.21/lib -lPhysics -lMathMore -lHist
LIBS += -L/usr/lib -lcln

include(sources.pri)
