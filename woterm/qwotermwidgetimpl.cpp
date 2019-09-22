#include "qwotermwidgetimpl.h"
#include "qwosshprocess.h"
#include "qwotermwidget.h"
#include "qwosessionproperty.h"
#include "qwoglobal.h"
#include "qwoshower.h"
#include "qwosshconf.h"
#include "qwomainwindow.h"

#include <QCloseEvent>
#include <QSplitter>
#include <QApplication>
#include <QMessageBox>


QWoTermWidgetImpl::QWoTermWidgetImpl(QString target, QWidget *parent)
    : QWoShowerWidget (parent)
    , m_target(target)
    , m_termType(ESsh)
{
    m_root = new QSplitter(this);
    m_root->setHandleWidth(1);
    m_root->setOpaqueResize(false);
    m_root->setAutoFillBackground(true);
    QPalette pal(Qt::gray);
    m_root->setPalette(pal);
    QWoSshProcess *process = new QWoSshProcess(target, m_root);
    QWoTermWidget *term = new QWoTermWidget(process, m_root);
    m_root->addWidget(term);
    process->start();
    QObject::connect(m_root, SIGNAL(destroyed(QObject*)), this, SLOT(onRootSplitterDestroy()));
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

void QWoTermWidgetImpl::handleTabMouseEvent(QMouseEvent *ev)
{
    if(ev->buttons().testFlag(Qt::RightButton)) {
        QVariant target = property(TAB_TARGET_NAME);
        qDebug() << "target" << target;
        if(!target.isValid()) {
            QMessageBox::warning(this, tr("alert"), tr("failed to find host infomation"));
            return;
        }
        int idx = QWoSshConf::instance()->findHost(target.toString());
        if(idx < 0) {
            QMessageBox::warning(this, tr("alert"), tr("failed to find host infomation"));
            return;
        }
        QWoSessionProperty dlg(QWoSessionProperty::ModifySession, idx, this);
        QObject::connect(&dlg, SIGNAL(connect(const QString&)), QWoMainWindow::instance(), SLOT(onSessionReadyToConnec(const QString&)));
        int ret = dlg.exec();
        if(ret == QWoSessionProperty::Save) {
            HostInfo hi = QWoSshConf::instance()->hostInfo(idx);
            for(int i = 0; i < m_terms.length(); i++) {
                QPointer<QWoTermWidget> term = m_terms.at(i);
                if(!term.isNull()) {
                    term->triggerPropertyCheck();
                }
            }
        }
    }
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
