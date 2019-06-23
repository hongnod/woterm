#include "qwoipctest.h"
#include "qwoipcfunction.h"

#include <QLocalServer>

QWoIpcTest::QWoIpcTest(QObject *parent)
    :QObject (parent)
{


}

QWoIpcTest::~QWoIpcTest()
{

}

void QWoIpcTest::start()
{
    static QLocalServer server;
    QString name = "ipctest";
    server.listen(name);
    QObject::connect(&server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    m_connect = new QLocalSocket(this);
    m_connect->connectToServer(name);
}

void QWoIpcTest::onNewConnection()
{
    QLocalServer *server = qobject_cast<QLocalServer*>(sender());
    QLocalSocket* local = server->nextPendingConnection();
    m_client = new QWoIpcFunction(local, this);
    QObject::connect(local, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    QObject::connect(local, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onClientError(QLocalSocket::LocalSocketError)));
    QObject::connect(local, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));
}

void QWoIpcTest::onClientError(QLocalSocket::LocalSocketError socketError)
{

}

void QWoIpcTest::onClientDisconnected()
{

}

void QWoIpcTest::onClientReadyRead()
{

}
