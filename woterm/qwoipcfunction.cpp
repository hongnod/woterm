#include "qwoipcfunction.h"


QWoIpcFunction::QWoIpcFunction(QLocalSocket *socket, QObject *parent)
    :QObject (parent)
    , m_socket(socket)
{

}

QWoIpcFunction::~QWoIpcFunction()
{

}
