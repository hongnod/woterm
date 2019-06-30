#ifndef QWOIPC_H
#define QWOIPC_H

#include "woipc_global.h"

#include <QThread>

typedef void (*FunIpcCallBack)(int hdl, char *argv[], int argc);

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

class QWoApp : public QThread
{
    Q_OBJECT
private:
    virtual void run();
};

#endif // QWOIPC_H
