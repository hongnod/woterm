QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE=app
include(../qtermwidget/qtermwidget.pri)
DESTDIR = $$PROJECT_BINDIR
unix:QMAKE_RPATHDIR+=$$PROJECT_LIBDIR
SOURCES += main.cpp \
    qwotermwidget.cpp

HEADERS += \
    qwotermwidget.h
