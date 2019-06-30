#pragma once

#include "qwoipc.h"

#include <QThread>
#include <QLocalSocket>
#include <QMap>
#include <QPointer>


class QWoMain : public QObject
{
    Q_OBJECT
public:
    explicit QWoMain(QObject *parent=nullptr);
    virtual ~QWoMain();

    static QWoMain *instance();

    void init();

    int connect(const QString& name, FunIpcCallBack cb);
    void send(int id, const QStringList& funArgs);
    void close(int id);

private:
signals:
    void ipcReady(int id,const QString& name);
    void ipcSend(int id, const QStringList &funArgs);
    void ipcClose(int id);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QLocalSocket::LocalSocketError socketError);
    void onReadyRead();
    void onIpcReady(int id, const QString& name);

    void onIpcSend(int id, const QStringList &funArgs);
    void onIpcClose(int id);

private:
    QMap<int, QPointer<QLocalSocket>> m_locals;
    QMap<int, FunIpcCallBack> m_cbs;
};


bool qSendTo(QLocalSocket *socket, const QStringList &funArgs);
QStringList qRecvFrom(QLocalSocket *socket);
