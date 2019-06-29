#include "qwoipc.h"
#include "qwomain.h"

#include <QCoreApplication>

static QWoMain gmain;

QWoIpc::QWoIpc()
{
}

int IpcConnect(const char *name, FunIpcCallBack cb)
{
    return gmain.connect(name, cb);
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
    gmain.start();
    gmain.wait(1000);
    return 1;
}

