#include "qwosocket.h"
#include <QLocalSocket>
#include <QDataStream>
#include <QMutex>
#include <QCoreApplication>
#include <QTimer>

QWoSocket::QWoSocket(FunIpcCallBack cb, QObject *parent)
    :QObject (parent)
    ,m_cb(cb)
{
    static QAtomicInt icnt = 1001;
    m_id = icnt++;
    bool ok = QObject::connect(this, SIGNAL(ipcClose()), this, SLOT(onIpcClose()), Qt::QueuedConnection);
    bool ok2 = QObject::connect(this, SIGNAL(ipcConnect(const QString&)), this, SLOT(onIpcConnect(const QString&)), Qt::QueuedConnection);
    bool ok3 = QObject::connect(this, SIGNAL(ipcSend(const QStringList&)), this, SLOT(onIpcSend(const QStringList&)), Qt::QueuedConnection);
    qDebug() << ok << ok2 << ok3;
}

QWoSocket::~QWoSocket()
{
    if(m_writer) {
        delete m_writer;
    }
    if(m_reader) {
        delete m_reader;
    }
    if(m_socket) {
        delete m_socket;
    }
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
    m_cb(m_id, -1, nullptr, 0);
}

void QWoSocket::onError(QLocalSocket::LocalSocketError socketError)
{
    m_cb(m_id, -2, nullptr, 0);
}

void QWoSocket::onReadyRead()
{
    m_reader->readAll();
    while(1) {
        QStringList data = m_reader->next();
        if(data.isEmpty()) {
            return;
        }
        char *argv[100] = {};
        for(int i = 0; i < data.count(); i++) {
            std::string v = data.at(i).toStdString();
            argv[i] = reinterpret_cast<char*>(malloc(v.length()+20));
#if defined (Q_OS_WIN)
            strcpy_s(argv[i], v.length()+1, v.c_str());
#else
            strcpy(argv[i], v.c_str());
#endif
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
    m_reader = new FunArgReader(m_socket, this);
    m_writer = new FunArgWriter(m_socket, this);

    QObject::connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    QObject::connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QObject::connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onError(QLocalSocket::LocalSocketError)));
    QObject::connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    m_socket->connectToServer(name);
}

void QWoSocket::onIpcSend(const QStringList &funArgs)
{
    m_writer->write(funArgs);
}

void QWoSocket::onIpcClose()
{
    m_socket->disconnectFromServer();
    m_socket->close();
    m_cond.wakeAll();
}
