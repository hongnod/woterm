#ifndef QWOIPC_H
#define QWOIPC_H

#include "woipc_global.h"

class WOIPCSHARED_EXPORT QWoIpc
{

public:
    QWoIpc();
};

int WOIPCSHARED_EXPORT ipc_connect(char *name);

#endif // QWOIPC_H
