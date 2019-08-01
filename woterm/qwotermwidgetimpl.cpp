#include "qwotermwidgetimpl.h"
#include "qwosshprocess.h"
#include "qwotermwidget.h"

#include <QCloseEvent>
#include <QSplitter>


QWoTermWidgetImpl::QWoTermWidgetImpl(QString target, QWidget *parent)
    : QWoWidget (parent)
    , m_target(target)
{
    m_root = new QSplitter(this);
    QWoSshProcess *process = new QWoSshProcess(target, m_root);
    QWoTermWidget *term = new QWoTermWidget(process, m_root);
    m_root->addWidget(term);
    process->start();

    QObject::connect(m_root, SIGNAL(destroyed(QObject*)), this, SLOT(onRootSplitterDestroy()));
}

void QWoTermWidgetImpl::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}

void QWoTermWidgetImpl::resizeEvent(QResizeEvent *event)
{
    QWoWidget::resizeEvent(event);
    QSize newSize = event->size();
    QRect rt(0, 0, newSize.width(), newSize.height());
    m_root->setGeometry(rt);
}

void QWoTermWidgetImpl::onRootSplitterDestroy()
{
    qDebug() << "rootClose()";
}
