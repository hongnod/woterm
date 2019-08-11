#include "qwotermwidgetimpl.h"
#include "qwosshprocess.h"
#include "qwoshellprocess.h"
#include "qwotermwidget.h"
#include "qwoglobal.h"

#include <QCloseEvent>
#include <QSplitter>
#include <QApplication>


QWoTermWidgetImpl::QWoTermWidgetImpl(QWidget *parent)
    : QWoWidget (parent)
    , m_termType(EShell)
{
    QWoShellProcess *process = new QWoShellProcess(m_root);
    init(process);
}

QWoTermWidgetImpl::QWoTermWidgetImpl(QString target, QWidget *parent)
    : QWoWidget (parent)
    , m_target(target)
    , m_termType(ESsh)
{
    QWoSshProcess *process = new QWoSshProcess(target, m_root);
    init(process);
}

QWoTermWidgetImpl::~QWoTermWidgetImpl()
{

}

QWoTermWidgetImpl::ETermType QWoTermWidgetImpl::termType()
{
    return m_termType;
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
    close();
    deleteLater();
}

void QWoTermWidgetImpl::broadcastMessage(int type, QVariant msg)
{
    for(int i = 0; i < m_terms.count(); i++) {
        QWoTermWidget *term = m_terms.at(i);
        if(term) {
            term->onBroadcastMessage(type, msg);
        }
    }
}

void QWoTermWidgetImpl::addToList(QWoTermWidget *w)
{
    m_terms.append(w);
}

void QWoTermWidgetImpl::removeFromList(QWoTermWidget *w)
{
    m_terms.removeOne(w);
}

void QWoTermWidgetImpl::init(QWoProcess *process)
{
    m_root = new QSplitter(this);
    m_root->setHandleWidth(1);
    m_root->setOpaqueResize(false);
    m_root->setAutoFillBackground(true);
    QPalette pal(Qt::gray);
    m_root->setPalette(pal);
    QWoTermWidget *term = new QWoTermWidget(process, m_root);
    m_root->addWidget(term);
    process->start();

    QObject::connect(m_root, SIGNAL(destroyed(QObject*)), this, SLOT(onRootSplitterDestroy()));
}
