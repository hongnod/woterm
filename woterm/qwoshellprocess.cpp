#include "qwoshellprocess.h"
#include "qwoevent.h"
#include "qwosetting.h"

#include <qtermwidget.h>
#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QClipboard>
#include <QProcess>
#include <QMessageBox>

QWoShellProcess::QWoShellProcess(QObject *parent)
    :QWoProcess (parent)
{
    QString program = QWoSetting::shellProgramPath();
    if(program.isEmpty()) {
        QMessageBox::critical(m_term, "shell", "can't find any shell program.");
        QApplication::exit(0);
        return;
    }
    m_target = program;
    setProgram(program);
    QStringList args;
    args.append(program);
    setArguments(args);

    QTimer::singleShot(3000, this, SLOT(onTimeout()));
}

QWoShellProcess::~QWoShellProcess()
{

}

void QWoShellProcess::onTimeout()
{
    write("dir\r\n");
}
