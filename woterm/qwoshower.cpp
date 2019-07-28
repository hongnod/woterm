#include "qwoshower.h"
#include "qwosshprocess.h"
#include "qwotermwidget.h"

#include <QTabBar>
#include <QResizeEvent>
#include <QMessageBox>
#include <QtGlobal>

QWoShower::QWoShower(QTabBar *tab, QWidget *parent)
    : QWidget (parent)
    , m_tabs(tab)
{
    QObject::connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
    QObject::connect(tab, SIGNAL(tabMoved(int,int)), this, SLOT(onTabMoved(int,int)));
    QObject::connect(tab, SIGNAL(currentChanged(int)), this, SLOT(onTabCurrentChanged(int)));

    setAutoFillBackground(true);
    setBackgroundColor("black");

}

QWoShower::~QWoShower()
{

}

bool QWoShower::openConnection(const QString &target)
{
    QWoSshProcess *process = new QWoSshProcess(target, this);
    QWoTermWidget *term = new QWoTermWidget(process, this);
    syncGeometry(term);
    process->start();
    m_terms.append(term);
    int idx = m_tabs->addTab(target);
    m_tabs->setCurrentIndex(idx);
    m_tabs->setTabData(idx, QVariant::fromValue(term));
    QObject::connect(process, SIGNAL(finished(int)), this, SLOT(onSshProcessFinished(int)));
    return true;
}

void QWoShower::setBackgroundColor(const QColor &clr)
{
    QPalette pal;
    pal.setColor(QPalette::Background, clr);
    pal.setColor(QPalette::Window, clr);
    setPalette(pal);
//    for(int i = 0; i < m_terms.count(); i++) {
//        QWoTermWidget *term = m_terms.at(i);
    //    }
}

void QWoShower::openFindDialog()
{
    int idx = m_tabs->currentIndex();
    if (idx < 0 || idx > m_tabs->count()) {
        return;
    }
    QVariant v = m_tabs->tabData(idx);
    QWoTermWidget *target = v.value<QWoTermWidget*>();
    QWoTermWidget *take = m_terms.at(idx);
    Q_ASSERT(target == take);
    take->toggleShowSearchBar();
}

void QWoShower::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    QRect rt(0, 0, newSize.width(), newSize.height());
    for(int i = 0; i < m_terms.length(); i++) {
        QWoTermWidget *term = m_terms.at(i);
        term->setGeometry(rt);
    }
}

void QWoShower::syncGeometry(QWidget *widget)
{
    QRect rt = geometry();
    rt.moveTo(0, 0);
    widget->setGeometry(rt);
}

void QWoShower::closeSession(int idx)
{
    if(m_tabs == nullptr) {
        return;
    }
    if(idx >= m_tabs->count()) {
        return;
    }
    QVariant v = m_tabs->tabData(idx);
    QWoTermWidget *target = v.value<QWoTermWidget*>();
    QWoTermWidget *take = m_terms.takeAt(idx);
    Q_ASSERT(target == take);
    m_tabs->removeTab(idx);
    QWoProcess *process = take->process();
    take->close();
    process->kill();
    process->deleteLater();
}

void QWoShower::onTabCloseRequested(int idx)
{
    QMessageBox::StandardButton btn = QMessageBox::warning(this, "CloseSession", "Close Or Not?", QMessageBox::Ok|QMessageBox::No);
    if(btn == QMessageBox::No) {
        return ;
    }
    closeSession(idx);
}

void QWoShower::onTabMoved(int from, int to)
{
    m_terms.move(from, to);
}

void QWoShower::onSshProcessFinished(int code)
{
    QWoProcess *hit = qobject_cast<QWoProcess*>(sender());
    for(int i = 0; i < m_terms.count(); i++) {
        QWoTermWidget *term = m_terms.at(i);
        QWoProcess *ssh = term->process();
        if(ssh == hit) {
            closeSession(i);
            return;
        }
    }
}

void QWoShower::onTabCurrentChanged(int idx)
{
    if(idx < 0) {
        return;
    }
    for(int i = 0; i < m_terms.count(); i++) {
        QWoTermWidget *term = m_terms.at(i);
        if(idx == i) {
            term->show();
            term->setFocus();
        }else{
            term->hide();
        }
    }
}
