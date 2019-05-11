#include "qwosshprocess.h"

#include <qtermwidget.h>

#include <QApplication>
#include <QDebug>
#include <QProcess>
#include <QMenu>
#include <QClipboard>


QWoSshProcess::QWoSshProcess()
    : QWoProcess (nullptr)
{
    setProgram("D:\\woterm\\openssh\\win32\\sbin\\x64\\Debug\\ssh.exe");
    QStringList args;
    args << "-F" << "D:\\config" << "jump";
    setArguments(args);
}
