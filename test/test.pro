TARGET = test
QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

DESTDIR = $$PWD/../bin

LIBS += -L"$$DESTDIR" -l"woipc"

unix: {
  LIBS +=-ldl
}

SOURCES += \
        main.c
