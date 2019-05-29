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

void QWoTermItem::connect(const QString &remote)
{
    qDebug() << remote;
    m_sshRemote = new QWoSshProcess(remote);
    m_term = new QWoTermWidget(m_sshRemote, container());
    m_sshRemote->start();
    QRectF rt(x(), y(), width(), height());
    m_term->setGeometry(rt.toRect());
}

void QWoTermItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QRectF newGeom = mapRectToScene(newGeometry);
    qDebug() << newGeometry << oldGeometry << newGeom;
    if(m_term) {
        m_term->setGeometry(newGeometry.toRect());
    }
    QWoItem::geometryChanged(newGeometry, oldGeometry);
}
