#include "qwoipc.h"


QWoIpc::QWoIpc()
{
}


int IpcConnect(const char *name, FunIpcCallBack cb)
{
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
    return 1;
}
