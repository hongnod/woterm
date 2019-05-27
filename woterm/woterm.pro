QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += qml quick quickcontrols2

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS


TEMPLATE=app
include(../qtermwidget/qtermwidget.pri)
DESTDIR = $$PROJECT_BINDIR
unix:QMAKE_RPATHDIR+=$$PROJECT_LIBDIR

SOURCES += main.cpp \
    qwotermwidget.cpp \
    qwoprocess.cpp \
    qwosshprocess.cpp \
    qwoevent.cpp \
    qwosetting.cpp \
    qwomainwindow.cpp \
    qwoitem.cpp

HEADERS += \
    qwotermwidget.h \
    qwoprocess.h \
    qwosshprocess.h \
    qwoevent.h \
    qwosetting.h \
    qwomainwindow.h \
    qwoitem.h

RESOURCES += \
    woterm.qrc

OTHER_FILES += \
    woterm.qml

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =
