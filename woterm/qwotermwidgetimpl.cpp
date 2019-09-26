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
#include <QMenu>


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

bool QWoTermWidgetImpl::handleTabMouseEvent(QMouseEvent *ev)
{
    if(ev->buttons().testFlag(Qt::RightButton)) {
        if(m_menu == nullptr) {
            m_menu = new QMenu(this);
            QAction *modify = m_menu->addAction(QIcon(":/qwoterm/resource/skin/linkcfg.png"), tr("Edit"));
            QObject::connect(modify, SIGNAL(triggered()), this, SLOT(onModifyThisSession()));
            QAction *close = m_menu->addAction(tr("Close Session"));
            QObject::connect(close, SIGNAL(triggered()), this, SLOT(onCloseThisSession()));
            QAction *dup = m_menu->addAction(tr("Duplicate In New Window"));
            QObject::connect(dup, SIGNAL(triggered()), this, SLOT(onDuplicateInNewWindow()));
        }
        m_menu->exec(QCursor::pos());
        return true;
    }
    return false;
}

void QWoTermWidgetImpl::onRootSplitterDestroy()
{
    close();
    deleteLater();
}

void QWoTermWidgetImpl::onModifyThisSession()
{
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
    QObject::connect(&dlg, SIGNAL(connect(const QString&)), QWoMainWindow::instance(), SLOT(onSessionReadyToConnect(const QString&)));
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

void QWoTermWidgetImpl::onCloseThisSession()
{
    deleteLater();
}

void QWoTermWidgetImpl::onDuplicateInNewWindow()
{
    QString path = QApplication::applicationFilePath();
    path.append(" ");
    path.append(m_target);
    QProcess::startDetached(path);
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
