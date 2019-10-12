#include "qwoshellwidgetimpl.h"
#include "qwoshellwidget.h"
#include "qwoglobal.h"

#include <QCloseEvent>
#include <QSplitter>
#include <QApplication>


QWoShellWidgetImpl::QWoShellWidgetImpl(QWidget *parent)
    : QWoShowerWidget (parent)
{
    m_root = new QSplitter(this);
    m_root->setHandleWidth(1);
    m_root->setOpaqueResize(false);
    m_root->setAutoFillBackground(true);
    QPalette pal(Qt::gray);
    m_root->setPalette(pal);
    QWoShellWidget *term = new QWoShellWidget(m_root);
    m_root->addWidget(term);

    QObject::connect(m_root, SIGNAL(destroyed(QObject*)), this, SLOT(onRootSplitterDestroy()));
}

QWoShellWidgetImpl::~QWoShellWidgetImpl()
{

}

void QWoShellWidgetImpl::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}

void QWoShellWidgetImpl::resizeEvent(QResizeEvent *event)
{
    QWoWidget::resizeEvent(event);
    QSize newSize = event->size();
    QRect rt(0, 0, newSize.width(), newSize.height());
    m_root->setGeometry(rt);
}

void QWoShellWidgetImpl::onRootSplitterDestroy()
{
    close();
    deleteLater();
}
