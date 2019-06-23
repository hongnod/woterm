#pragma once

#include <QObject>
#include <QPointer>
#include <QLocalSocket>

class QWoIpcFunction;

class QWoIpcTest : public QObject
{
    Q_OBJECT
public:
    explicit QWoIpcTest(QObject *parent=nullptr);
    virtual ~QWoIpcTest();
    void start();

private slots:
    void onNewConnection();
    void onClientError(QLocalSocket::LocalSocketError socketError);
    void onClientDisconnected();
    void onClientReadyRead();
private:
    QPointer<QLocalSocket> m_connect;
    QPointer<QWoIpcFunction> m_client;
};
