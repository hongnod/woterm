#include "qwotermitem.h"
#include "qwotermwidget.h"
#include "qwosshprocess.h"

#include <QQuickWidget>
#include <QDebug>

QWoTermItem::QWoTermItem(QQuickItem *parent)
    : QWoItem(parent)
{

}

QWoTermItem::~QWoTermItem()
{

}


QQuickWidget *QWoTermItem::container()
{
    static QQuickWidget *qWidget = new QQuickWidget();
    return qWidget;
}

void QWoTermItem::connect(const QString &args)
{
    qDebug() << args;
    m_sshRemote = new QWoSshProcess();
    m_term = new QWoTermWidget(m_sshRemote, container());
}
