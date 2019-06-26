TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $$PWD/../bin

LIBS += -L"$$DESTDIR" -l"woipc"

SOURCES += \
        main.c
