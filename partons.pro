#-------------------------------------------------
#
# Project created by QtCreator 2015-01-23T11:51:40
#
#-------------------------------------------------

CODECFORTR = UTF-8
CODECFORSRC = UTF-8@

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += debug

CONFIG(debug, debug|release) {
    DESTDIR = build/debug
} else {
    DESTDIR = build/release
}

OBJECTS_DIR = $${DESTDIR}/obj
MOC_DIR = $${DESTDIR}/moc


TARGET = PARTONS
TEMPLATE = lib

DEFINES += QCUSTOMPLOT_COMPILE_LIBRARY

INCLUDEPATH += /usr/local/cln/v1.3.4/include
INCLUDEPATH += /usr/local/root/v5.34.30/include/root

LIBS += -L/usr/local/cln/v1.3.4/lib -lcln
#LIBS += -L/usr/local/gsl/v1.16/lib -lgsl
LIBS += -L/usr/local/root/v5.34.30/lib/root -lPhysics -lMathMore -lHist


include(sources.pri)
