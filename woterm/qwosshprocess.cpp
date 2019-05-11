#include "qwosshprocess.h"

#include <qtermwidget.h>

#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QClipboard>
#include <QLocalServer>

QWoSshProcess::QWoSshProcess()
    : QWoProcess (nullptr)
{
    setProgram("D:\\woterm\\openssh\\win32\\sbin\\x64\\Debug\\ssh.exe");
    QStringList args;
    args << "-F" << "D:\\config" << "jump";
    setArguments(args);

    QString name = QString("WoTerm%1_%2").arg(QApplication::applicationPid()).arg(quint64(this));
    m_serer = new QLocalServer(this);
    m_serer->listen(name);
    QString fullName = m_serer->serverName();
    QStringList env;
    env << "TERM_MSG_CHANNEL="+name;
    setEnvironment(env);
    qDebug() << fullName;
}
