#include "qwoipc.h"
#include "qwomain.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>

#include <QCoreApplication>
#include <QMutex>
#include <QDebug>

static QWoApp woApp;
static QWoMain woMain;

int IpcConnect(const char *name, FunIpcCallBack cb)
{
    return woMain.connect(name, cb);
}

bool IpcCall(int hdl, char *argv[], int argc)
{
    QStringList args;
    for(int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }
    woMain.send(hdl, args);
    return true;
}

bool IpcClose(int hdl)
{
    woMain.close(hdl);
    return false;
}

int IpcInit()
{
    if(woApp.isRunning()) {
        return 0;
    }
    woApp.start();
    return 1;
}

void QWoApp::start()
{
    QMutex mtx;
    mtx.lock();
    QThread::start();
    m_cond.wait(&mtx, 3000);
    mtx.unlock();
}

void QWoApp::run()
{
    char*** argv = __p___argv();
    int* argc = __p___argc();
    QCoreApplication app(*argc, *argv);
    qDebug() << "start app thread";
    m_cond.wakeOne();
    app.exec();
}

void IpcExit()
{
    woApp.exit(0);
    woApp.wait(5000);
}
