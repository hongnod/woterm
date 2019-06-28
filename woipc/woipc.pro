
QT       += network

QT       -= gui

TARGET = woipc
TEMPLATE = lib

DEFINES += WOIPC_LIBRARY

DESTDIR = $$PWD/../bin

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        qwoipc.cpp \
        qwomain.cpp

HEADERS += \
        qwomain.h \
        qwoipc.h \
        woipc_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}
