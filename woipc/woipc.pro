
QT       += network

QT       -= gui

TARGET = woipc
TEMPLATE = lib

DEFINES += WOIPC_LIBRARY

DESTDIR = $$PWD/../bin

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ipchelper.cpp \
    qwoipc.cpp \
    qwomain.cpp \
    qwosocket.cpp


HEADERS += \
    ipchelper.h \
    qwomain.h \
    qwoipc.h \
    qwosocket.h \
    woipc_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
