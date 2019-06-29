#include "qwoipc.h"
#include "qwomain.h"
#include "qwosocket.h"

#include <QCoreApplication>

QWoIpc::QWoIpc()
{
}

int IpcConnect(const char *name, FunIpcCallBack cb)
{
    QThread *thread = QCoreApplication::instance()->thread();
    QWoSocket *socket = new QWoSocket(cb, thread);
    socket->moveToThread(thread);
    socket->connect(name);
    return 0;
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
    static QWoMain thread;
    thread.start();
    thread.wait(1000);
    return 1;
}

