QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE=app
include(../qtermwidget/qtermwidget.pri)
include($$PWD/../third/aes/aes.pri)

DESTDIR = $$PROJECT_BINDIR
unix:QMAKE_RPATHDIR+=$$PROJECT_LIBDIR
SOURCES += main.cpp \
    ipchelper.cpp \
    qwotermwidget.cpp \
    qwoshellwidget.cpp \
    qwoshellwidgetimpl.cpp \
    qwotermwidgetimpl.cpp \
    qwoprocess.cpp \
    qwosshprocess.cpp \
    qwoevent.cpp \
    qwosetting.cpp \
    qwomainwindow.cpp \
    qwoshower.cpp \
    qwoshowerwidget.cpp \
    qwowidget.cpp \
    qwosshconf.cpp \
    qwohostinfolist.cpp \
    qwoutils.cpp \
    qwohostinfoedit.cpp \
    qwohostsimplelist.cpp \
    qwohostlistmodel.cpp \
    qwosessionproperty.cpp \
    qwoaes.cpp \
    qwosessionlist.cpp \
    qwosessionmanage.cpp \
    qwotermmask.cpp \
    qwopasswordinput.cpp

HEADERS += \
    ipchelper.h \
    qwotermwidget.h \
    qwoshellwidget.h \
    qwoshellwidgetimpl.h \
    qwotermwidgetimpl.h \
    qwoprocess.h \
    qwosshprocess.h \
    qwoevent.h \
    qwosetting.h \
    qwomainwindow.h \
    qwoshower.h \
    qwoshowerwidget.h \
    qwowidget.h \
    qwosshconf.h \
    qwoglobal.h \
    qwohostinfolist.h \
    qwoutils.h \
    qwohostinfoedit.h \
    qwohostsimplelist.h \
    qwohostlistmodel.h \
    qwosessionproperty.h \
    qwoaes.h \
    qwosessionlist.h \
    qwosessionmanage.h \
    qwotermmask.h \
    qwopasswordinput.h

FORMS += \
    qwohostlist.ui \
    qwohostinfolist.ui \
    qwohostinfo.ui \
    qwomainwindow.ui \
    qwosessionproperty.ui \
    qwosessionmanage.ui \
    qwotermmask.ui \
    qwotermmask.ui \
    qwopasswordinput.ui

RESOURCES += qwoterm.qrc
