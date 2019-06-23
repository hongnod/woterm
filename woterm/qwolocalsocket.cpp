#include "qwolocalsocket.h"


QWoLocalSocket::QWoLocalSocket(QLocalSocket *socket, QObject *parent)
    : QObject(parent)
    , m_socket(socket)
{

}

QWoLocalSocket::~QWoLocalSocket()
{

}
