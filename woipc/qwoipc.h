#ifndef QWOIPC_H
#define QWOIPC_H

#include "woipc_global.h"
#include <QThread>

class WOIPCSHARED_EXPORT QWoIpc
{

public:
    QWoIpc();
};

typedef void (*FunIpcCallBack)(int hdl, char *funName, char *argv[]);

#ifdef __cplusplus
extern "C" {
#endif

int WOIPCSHARED_EXPORT IpcConnect(const char* name, FunIpcCallBack cb);
int WOIPCSHARED_EXPORT IpcCall(int hdl, const char* funName, char *argv[]);
bool WOIPCSHARED_EXPORT IpcClose(int hdl);
int WOIPCSHARED_EXPORT IpcInit(void);


#ifdef __cplusplus
}
#endif

#endif // QWOIPC_H
