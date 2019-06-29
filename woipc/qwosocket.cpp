#include "qwosocket.h"

#include <QLocalSocket>
#include <QDataStream>
#include <QBuffer>


bool qSendTo(QLocalSocket *socket, const QStringList &funArgs)
{
    QByteArray buf;
    QDataStream in(&buf, QIODevice::WriteOnly);
    in << funArgs;
    int length = buf.length();
    socket->write((char*)&length, sizeof(int));
    return socket->write(buf.data(), length) > 0;
}

QStringList qRecvFrom(QLocalSocket *socket)
{
    QByteArray buf;
    int length;
    if(socket->bytesAvailable() < 4) {
        return QStringList();
    }
    if(socket->read((char*)&length, sizeof(int)) != sizeof(int)){
        return QStringList();
    }
    buf.resize(length);
    int trycnt = 10;
    while(socket->bytesAvailable() < length && trycnt > 0) {
        socket->waitForReadyRead(100);
        trycnt--;
    }
    if(socket->read((char*)buf.data(), length) != length) {
        return QStringList();
    }
    QStringList funArgs;
    QDataStream out(buf);
    out >> funArgs;
    return funArgs;
}


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

bool QWoSocket::send(const QStringList& funArgs)
{
    return qSendTo(m_socket, funArgs);
}

void QWoSocket::onConnected()
{
    send(QStringList() << "sendMessage" << "A" << "b" << "cd");
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
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    QStringList data = qRecvFrom(local);
    if(data.length() <= 0) {
        return;
    }
    qDebug() << data;
    static int i = 0;
    i++;
    QStringList funArgs;
    funArgs << "sendMessage" << "a" << "bd" << QString("%1").arg(i);
    qSendTo(local, funArgs);
}
