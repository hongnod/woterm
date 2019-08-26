#include "qwoshellwidget_win32.h"
#include "qwosetting.h"
#include "qwoprocess.h"

#include <QCloseEvent>
#include <Windows.h>


QWoWin32ShellWidget::QWoWin32ShellWidget(QWidget *parent)
    : QWidget (parent)
{
    init();
}

void QWoWin32ShellWidget::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}

void QWoWin32ShellWidget::init()
{
    QString path = QWoSetting::shellProgramPath();
    m_shell = new QWoProcess(this);
    m_shell->setProgram(path);
    m_shell->enableDebugConsole(true);
    m_shell->startDetached();
}
