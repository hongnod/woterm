#ifndef QWOIPC_H
#define QWOIPC_H

#include "woipc_global.h"

class WOIPCSHARED_EXPORT QWoIpc
{

public:
    QWoIpc();
};

typedef void (*FunIpcCallBack)(int hdl, char *funName, char *argv[]);

extern "C" {
int WOIPCSHARED_EXPORT IpcConnect(const char* name, FunIpcCallBack cb);
int WOIPCSHARED_EXPORT IpcCall(int hdl, const char* funName, char *argv[]);
bool WOIPCSHARED_EXPORT IpcClose(int hdl);
int WOIPCSHARED_EXPORT IpcInit(void);
}

#endif // QWOIPC_H
