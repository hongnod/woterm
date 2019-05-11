#include "qwoprocess.h"

#include <qtermwidget.h>

#include <QApplication>
#include <QDebug>
#include <QProcess>
#include <QMenu>
#include <QClipboard>


QWoProcess::QWoProcess(QObject *parent)
    : QProcess (parent)
{
}

void QWoProcess::setTermWidget(QTermWidget *widget)
{
    m_term = widget;
}
