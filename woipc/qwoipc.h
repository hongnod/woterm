#ifndef QWOIPC_H
#define QWOIPC_H

#include "woipc_global.h"

#include <QThread>
#include <QWaitCondition>

typedef void (*FunIpcCallBack)(int hdl, char *argv[], int argc);

#ifdef __cplusplus
extern "C" {
#endif

int WOIPCSHARED_EXPORT IpcConnect(const char* name, FunIpcCallBack cb);
bool WOIPCSHARED_EXPORT IpcCall(int hdl, char *argv[], int argc);
bool WOIPCSHARED_EXPORT IpcClose(int hdl);
int WOIPCSHARED_EXPORT IpcInit(void);
void WOIPCSHARED_EXPORT IpcExit(void);


#ifdef __cplusplus
}
#endif

class QWoApp : public QThread
{
    Q_OBJECT
public:
    void start();
private:
    virtual void run();
private:
    QWaitCondition m_cond;
};

#endif // QWOIPC_H
