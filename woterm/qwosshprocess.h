#pragma once

#include "qwoprocess.h"

#include <QPointer>
#include <QLocalSocket>

class QTermWidget;
class QLocalServer;
class QLocalSocket;

class QWoSshProcess : public QWoProcess
{
    Q_OBJECT
public:
    explicit QWoSshProcess();

private slots:
    void onNewConnection();
    void onClientError(QLocalSocket::LocalSocketError socketError);
    void onClientDisconnected();
    void onClientReadyRead();
private:
    void read(char *buf, int len);
private:
    QPointer<QLocalServer> m_server;
    QPointer<QLocalSocket> m_client;
};
