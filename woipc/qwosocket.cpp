#include "qwosocket.h"
#include <QLocalSocket>
#include <QDataStream>
#include <QMutex>
#include <QCoreApplication>


bool qWrite(QLocalSocket *socket, char* data, int len) {
    int nleft = len;
    char *buf = data;
    while(nleft > 0) {
        int n = socket->write(buf, nleft);
        if(n < 0) {
            return false;
        }
        nleft -= n;
        buf += n;
    }
    return true;
}

bool qRead(QLocalSocket *socket, char* data, int len) {
    int nleft = len;
    char *buf = data;
    while(nleft > 0) {
        int n = socket->read(buf, nleft);
        if(n < 0) {
            return false;
        }
        nleft -= n;
        buf += n;
    }
    return true;
}

bool qSendTo(QLocalSocket *socket, const QStringList &funArgs)
{
    QByteArray buf;
    QDataStream in(&buf, QIODevice::WriteOnly);
    in << funArgs;
    int length = buf.length();
    if(!qWrite(socket, (char*)&length, sizeof(int))){
        return false;
    }
    return qWrite(socket, buf.data(), length);
}

QStringList qRecvFrom(QLocalSocket *socket)
{
    QByteArray buf;
    int length;
    if(!qRead(socket, (char*)&length, sizeof(int))) {
        return QStringList();
    }
    buf.resize(length);
    if(!qRead(socket, buf.data(), length)) {
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
    deleteLater();
}

void QWoSocket::onConnected()
{
    static int pingCount = 0;
    QStringList funArgs;
    funArgs << "ping" << QString("%1").arg(pingCount++);
    send(funArgs);
    m_cond.wakeAll();
}

void QWoSocket::onDisconnected()
{
    m_cb(m_id, -3, nullptr, 0);
}

void QWoSocket::onError(QLocalSocket::LocalSocketError socketError)
{
    m_cb(m_id, -2, nullptr, 0);
}

void QWoSocket::onReadyRead()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    while(1) {
        QStringList data = qRecvFrom(local);
        if(data.isEmpty()) {
            if(!m_socket->isValid()) {
                m_cb(m_id, -1, nullptr, 0);
            }
            return;
        }
        char *argv[100] = {};
        for(int i = 0; i < data.count(); i++) {
            std::string v = data.at(i).toStdString();
            argv[i] = reinterpret_cast<char*>(malloc(v.length()+20));
            strcpy_s(argv[i], v.length()+1, v.c_str());
        }
        m_cb(m_id, 0, argv, data.count());
        for(int i = 0; i < data.count(); i++) {
            free(argv[i]);
            argv[i] = nullptr;
        }
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
    if(!qSendTo(m_socket, funArgs)) {
        if(!m_socket->isValid()) {
            m_cb(m_id, -1, nullptr, 0);
        }
    }
}

void QWoSocket::onIpcClose()
{
    m_socket->disconnectFromServer();
    m_socket->close();
    m_cond.wakeAll();
}
