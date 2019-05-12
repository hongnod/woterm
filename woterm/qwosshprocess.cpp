#include "qwosshprocess.h"

#include <qtermwidget.h>

#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QClipboard>
#include <QLocalServer>
#include <QLocalSocket>

QWoSshProcess::QWoSshProcess()
    : QWoProcess (nullptr)
{
    setProgram("D:\\woterm\\openssh\\win32\\sbin\\x64\\Debug\\ssh.exe");
    QStringList args;
    args << "-F" << "D:\\config" << "jump";
    setArguments(args);

    QString name = QString("WoTerm%1_%2").arg(QApplication::applicationPid()).arg(quint64(this));
    m_server = new QLocalServer(this);
    m_server->listen(name);
    QStringList env;
    env << "TERM_MSG_CHANNEL="+name;
    setEnvironment(env);

    QObject::connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void QWoSshProcess::onNewConnection()
{
    m_client = m_server->nextPendingConnection();
    QObject::connect(m_client, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    QObject::connect(m_client, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onClientError(QLocalSocket::LocalSocketError)));
    QObject::connect(m_client, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));
}

void QWoSshProcess::onClientError(QLocalSocket::LocalSocketError socketError)
{
    if(m_client) {
        m_client->deleteLater();
    }
}

void QWoSshProcess::onClientDisconnected()
{
    if(m_client) {
        m_client->deleteLater();
    }
}

void QWoSshProcess::onClientReadyRead()
{
    char buf[512];
    if(m_client == nullptr) {
        return;
    }
    buf[3] = '\0';
    m_client->read(buf, 3);
    int len = QString(buf).toInt();
    buf[len] = '\0';
    m_client->read(buf, len);
    QString data(buf);
    qDebug() << data;
    if(data.startsWith("getwinsize")) {

    }
}
