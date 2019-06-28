#include "qwosocket.h"

#include <QLocalSocket>
#include <QDataStream>


QWoSocket::QWoSocket(QObject *parent)
    :QObject (parent)
{
    m_socket = new QLocalSocket(parent);
    QObject::connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onError(QLocalSocket::LocalSocketError)));
    QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

QWoSocket::~QWoSocket()
{

}

void QWoSocket::connect(const QString &name)
{
    m_socket->connectToServer(name);
}

void QWoSocket::onConnected()
{
    QDataStream in(m_socket);
    QString funName = "send";
    QStringList args;
    args << "A" << "B" << "C";
    in << funName << args;
}

void QWoSocket::onDisconnected()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    local->deleteLater();
}

void QWoSocket::onError(QLocalSocket::LocalSocketError socketError)
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    local->deleteLater();
}

void QWoSocket::onReadyRead()
{
    QDataStream out(m_socket);
    QString funName;
    QStringList args;
    out >> funName >> args;

    qDebug() << funName << args;
}
