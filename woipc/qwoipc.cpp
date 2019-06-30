#include "qwoipc.h"
#include "qwomain.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>

#include <QCoreApplication>

static QWoApp woApp;

int IpcConnect(const char *name, FunIpcCallBack cb)
{
    return QWoMain::instance()->connect(name, cb);
}

int IpcCall(int hdl, const char *funName, char *argv[])
{
    return 0;
}

bool IpcClose(int hdl)
{
    return false;
}

int IpcInit()
{
    if(woApp.isRunning()) {
        return 0;
    }
    woApp.start();
    woApp.wait(1000);
    return 1;
}

void QWoApp::run()
{
    char*** argv = __p___argv();
    int* argc = __p___argc();
    QCoreApplication app(*argc, *argv);
    qDebug() << "start app thread";
    QWoMain::instance()->init();
    app.exec();
}
