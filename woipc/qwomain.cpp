#include "qwomain.h"

#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>
#include <QLocalSocket>
#include <QDataStream>

#include <stdlib.h>
#include <stdio.h>

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

#define SOCKET_LOCALID ("localid")

QWoMain::QWoMain(QObject *parent)
    : QObject(parent)
{

}

QWoMain::~QWoMain()
{

}

QWoMain *QWoMain::instance()
{
    static QWoMain main;
    return &main;
}

void QWoMain::init()
{
    QObject::connect(this, SIGNAL(ready(int,const QString&)), this, SLOT(onReady(int,const QString&)), Qt::QueuedConnection);
}

int QWoMain::connect(const QString &name, FunIpcCallBack cb)
{
    static int cntid = 1001;
    QLocalSocket *local = new QLocalSocket(this);
    int lsid = cntid++;
    m_locals.insert(lsid, local);
    m_cbs.insert(lsid, cb);
    local->setProperty(SOCKET_LOCALID, lsid);
    local->moveToThread(QCoreApplication::instance()->thread());
    QObject::connect(local, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(local, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(local, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onError(QLocalSocket::LocalSocketError)));
    QObject::connect(local, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    emit ready(lsid, name);
    return lsid;
}

bool QWoMain::send(int id, const QStringList &funArgs)
{
    if(!m_locals.contains(id)) {
        return false;
    }
    QLocalSocket *local = m_locals[id];
    return qSendTo(local, funArgs);
}

void QWoMain::onConnected()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    qSendTo(local, QStringList() << "sendMessage" << "A" << "b" << "cd");
}

void QWoMain::onDisconnected()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    local->deleteLater();
}

void QWoMain::onError(QLocalSocket::LocalSocketError socketError)
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    local->deleteLater();
}

void QWoMain::onReadyRead()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    QStringList data = qRecvFrom(local);

    qDebug() << data;
    static int i = 0;
    i++;
    QStringList funArgs;
    funArgs << "sendMessage" << "a" << "bd" << QString("%1").arg(i);
    qSendTo(local, funArgs);
}

void QWoMain::onReady(int id, const QString &name)
{
    if(!m_locals.contains(id)) {
        return;
    }
    QLocalSocket *local = m_locals[id];
    local->connectToServer(name);
}
