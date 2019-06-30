#include "qwosocket.h"
#include <QLocalSocket>
#include <QDataStream>
#include <QMutex>
#include <QCoreApplication>


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


QWoSocket::QWoSocket(FunIpcCallBack cb, QObject *parent)
    :QObject (parent)
    ,m_cb(cb)
{
    static int icnt = 1001;
    m_id = icnt++;
    bool ok = QObject::connect(this, SIGNAL(ipcClose()), this, SLOT(onIpcClose()), Qt::QueuedConnection);
    bool ok2 = QObject::connect(this, SIGNAL(ipcConnect(const QString&)), this, SLOT(onIpcConnect(const QString&)), Qt::QueuedConnection);
    bool ok3 = QObject::connect(this, SIGNAL(ipcSend(const QStringList&)), this, SLOT(onIpcSend(const QStringList&)), Qt::QueuedConnection);
    qDebug() << ok << ok2 << ok3;
}

QWoSocket::~QWoSocket()
{

}

int QWoSocket::connect(const QString &name)
{
    QThread *thread = QCoreApplication::instance()->thread();
    moveToThread(thread);
    emit ipcConnect(name);
    QMutex mtx;
    mtx.lock();
    m_cond.wait(&mtx, 5000);
    mtx.unlock();
    return m_id;
}

void QWoSocket::send(const QStringList &funArgs)
{
    emit ipcSend(funArgs);
}

void QWoSocket::close()
{
    emit ipcClose();
    QMutex mtx;
    mtx.lock();
    m_cond.wait(&mtx, 5000);
    mtx.unlock();
}

void QWoSocket::onConnected()
{
    QStringList funArgs;
    funArgs << "ping";
    send(funArgs);
    m_cond.wakeAll();
}

void QWoSocket::onDisconnected()
{

}

void QWoSocket::onError(QLocalSocket::LocalSocketError socketError)
{

}

void QWoSocket::onReadyRead()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    QStringList data = qRecvFrom(local);
    if(data.isEmpty()) {
        return;
    }
    char *argv[100] = {};
    for(int i = 0; i < data.count(); i++) {
        std::string v = data.at(i).toStdString();
        argv[i] = reinterpret_cast<char*>(malloc(v.length()+20));
        strcpy_s(argv[i], v.length()+1, v.c_str());
    }
    m_cb(m_id, argv, data.count());
    for(int i = 0; i < data.count(); i++) {
        free(argv[i]);
        argv[i] = nullptr;
    }
}

void QWoSocket::onIpcConnect(const QString &name)
{
    m_socket = new QLocalSocket(this);
    QObject::connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onError(QLocalSocket::LocalSocketError)));
    QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    m_socket->connectToServer(name);
}

void QWoSocket::onIpcSend(const QStringList &funArgs)
{
    if(!m_socket->isWritable()) {
        return;
    }
    qSendTo(m_socket, funArgs);
}

void QWoSocket::onIpcClose()
{
    m_socket->disconnectFromServer();
    m_socket->close();
    m_cond.wakeAll();
}
