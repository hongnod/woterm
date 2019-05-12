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
    QStringList env = environment();
    env << "TERM_MSG_CHANNEL="+name;
   // env << "ProgramData="+QApplication::applicationDirPath();
    setEnvironment(env);

    QObject::connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void QWoSshProcess::onNewConnection()
{
    m_client = m_server->nextPendingConnection();
    QObject::connect(m_client, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    QObject::connect(m_client, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onClientError(QLocalSocket::LocalSocketError)));
    QObject::connect(m_client, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));

    updateTermSize();
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
        updateTermSize();
    }
}

void QWoSshProcess::updateTermSize()
{
    if(m_client == nullptr) {
        return;
    }
    int linecnt = m_term->screenLinesCount();
    int column = m_term->screenColumnsCount();
    QString fun = QString("setwinsize(%1,%2)").arg(column).arg(linecnt);
    QByteArray cmd = QString("%1%2").arg(fun.size(), 3, 10, QChar('0')).arg(fun).toUtf8();
    m_client->write(cmd);
}

bool QWoSshProcess::eventFilter(QObject *obj, QEvent *ev)
{
    QEvent::Type t = ev->type();
    if (t == QEvent::Resize) {
        QMetaObject::invokeMethod(this, "updateTermSize",Qt::QueuedConnection);
    }
    return false;
}

void QWoSshProcess::setTermWidget(QTermWidget *widget)
{
    QWoProcess::setTermWidget(widget);
    widget->installEventFilter(this);
}
