#include "qwotermitem.h"
#include "qwotermwidget.h"
#include "qwosshprocess.h"

#include <QQuickWidget>
#include <QDebug>

QWoTermItem::QWoTermItem(QQuickItem *parent)
    : QWoItem(parent)
{
    QObject::connect(this, SIGNAL(visibleChanged()), this, SLOT(onVisibleChanged()));
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
    syncTermGeometry();
}

void QWoTermItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QWoItem::geometryChanged(newGeometry, oldGeometry);
    syncTermGeometry();
}

void QWoTermItem::onVisibleChanged()
{
    syncTermGeometry();
}

bool QWoTermItem::event(QEvent *ev)
{
    QEvent::Type t = ev->type();
    qDebug() << "event:" << t;
    return QWoItem::event(ev);
}

void QWoTermItem::syncTermGeometry()
{
    if(m_term) {
        m_term->setVisible(isVisible());
        QPointF lt = mapToScene(QPointF(0,0));
        QRectF rt(lt.x(), lt.y(), width(), height());
        m_term->setGeometry(rt.toRect());
    }
}
