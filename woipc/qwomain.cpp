#include "qwomain.h"

#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>
#include <QLocalSocket>
#include <QDataStream>
#include <QMutex>

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
    QObject::connect(this, SIGNAL(ipcClose(int)), this, SLOT(onIpcClose(int)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(ipcConnect(int,const QString&)), this, SLOT(onIpcConnect(int,const QString&)), Qt::QueuedConnection);
    QObject::connect(this, SIGNAL(ipcSend(int,const QStringList&)), this, SLOT(onIpcSend(int,const QStringList&)), Qt::QueuedConnection);
}

int QWoMain::connect(const QString &name, FunIpcCallBack cb)
{
    static int cntid = 1001;
    int lsid = cntid++;
    m_cbs.insert(lsid, cb);
    emit ipcConnect(lsid, name);
    QMutex mtx;
    mtx.lock();
    m_cond.wait(&mtx, 5000);
    mtx.unlock();
    return lsid;
}

void QWoMain::send(int id, const QStringList &funArgs)
{
    emit ipcSend(id, funArgs);
}

void QWoMain::close(int id)
{
    emit ipcClose(id);
    QMutex mtx;
    mtx.lock();
    m_cond.wait(&mtx, 3000);
    mtx.unlock();
}

void QWoMain::onConnected()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    qSendTo(local, QStringList() << "sendMessage" << "A" << "b" << "cd");
    m_cond.wakeAll();
}

void QWoMain::onDisconnected()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    local->deleteLater();
    int id = local->property(SOCKET_LOCALID).toInt();
    m_locals.remove(id);
    m_cbs.remove(id);
}

void QWoMain::onError(QLocalSocket::LocalSocketError socketError)
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    local->deleteLater();
    int id = local->property(SOCKET_LOCALID).toInt();
    m_locals.remove(id);
    m_cbs.remove(id);
}

void QWoMain::onReadyRead()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    QStringList data = qRecvFrom(local);
    QVariant vid = local->property(SOCKET_LOCALID);
    if(!vid.isValid() || data.isEmpty()) {
        return;
    }
    int id = vid.toInt();
    FunIpcCallBack cb = m_cbs[id];
    char *argv[100] = {};
    for(int i = 0; i < data.count(); i++) {
        std::string v = data.at(i).toStdString();
        argv[i] = reinterpret_cast<char*>(malloc(v.length()+20));
        strcpy_s(argv[i], v.length()+1, v.c_str());
    }
    cb(id, argv, data.count());
    for(int i = 0; i < data.count(); i++) {
        free(argv[i]);
        argv[i] = nullptr;
    }
}

void QWoMain::onIpcConnect(int id, const QString &name)
{
    QLocalSocket *local = new QLocalSocket(this);
    m_locals.insert(id, local);
    local->setProperty(SOCKET_LOCALID, id);
    local->moveToThread(QCoreApplication::instance()->thread());
    QObject::connect(local, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(local, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(local, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onError(QLocalSocket::LocalSocketError)));
    QObject::connect(local, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    if(!m_locals.contains(id)) {
        return;
    }
    local->connectToServer(name);
    m_cond.wakeAll();
}

void QWoMain::onIpcSend(int id, const QStringList &funArgs)
{
    if(!m_locals.contains(id)) {
        return;
    }
    QLocalSocket *local = m_locals[id];
    if(!local->isWritable()) {
        return;
    }
    qSendTo(local, funArgs);
}

void QWoMain::onIpcClose(int id)
{
    if(!m_locals.contains(id)) {
        return;
    }
    QLocalSocket *local = m_locals[id];
    local->disconnectFromServer();
    local->close();
    m_cond.wakeAll();
}
