#include "qwoshellprocess.h"
#include "qwoevent.h"
#include "qwosetting.h"

#include <qtermwidget.h>
#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QClipboard>
#include <QProcess>

QWoShellProcess::QWoShellProcess(QObject *parent)
    :QWoProcess (parent)
{

}

QWoShellProcess::~QWoShellProcess()
{

}
