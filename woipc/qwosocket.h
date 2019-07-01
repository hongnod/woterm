#pragma once

#include "qwoipc.h"
#include "ipchelper.h"

#include <QThread>
#include <QLocalSocket>
#include <QMap>
#include <QPointer>
#include <QWaitCondition>
#include <QDataStream>


class QWoSocket : public QObject
{
    Q_OBJECT
public:
    explicit QWoSocket(FunIpcCallBack cb, QObject *parent=nullptr);
    virtual ~QWoSocket();

    int connect(const QString& name);
    void send(const QStringList& funArgs);
    void close();
signals:
    void ipcConnect(const QString& name);
    void ipcSend(const QStringList& funArgs);
    void ipcClose();

private slots:
    void onConnected();

    void onDisconnected();
    void onError(QLocalSocket::LocalSocketError socketError);
    void onReadyRead();

    void onIpcConnect(const QString& name);
    void onIpcSend(const QStringList &funArgs);
    void onIpcClose();
private:
    const FunIpcCallBack m_cb;
    int m_id;

    QPointer<FunArgReader> m_reader;
    QPointer<FunArgWriter> m_writer;

    QPointer<QLocalSocket> m_socket;
    QWaitCondition m_cond;
};
