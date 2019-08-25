#include "qwoshellwidget_win32.h"

#include <QCloseEvent>


QWoWin32ShellWidget::QWoWin32ShellWidget(QWidget *parent)
    : QWidget (parent)
{

}

void QWoWin32ShellWidget::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}
