#include "qwoshower.h"
#include "qwotermwidgetimpl.h"
#include "qwoshellwidgetimpl.h"

#include <QTabBar>
#include <QResizeEvent>
#include <QMessageBox>
#include <QtGlobal>
#include <QSplitter>
#include <QDebug>

QWoShower::QWoShower(QTabBar *tab, QWidget *parent)
    : QStackedWidget (parent)
    , m_tabs(tab)
{
    QObject::connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
    QObject::connect(tab, SIGNAL(currentChanged(int)), this, SLOT(onTabCurrentChanged(int)));
    QObject::connect(tab, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(onTabbarDoubleClicked(int)));

    setAutoFillBackground(true);
    setBackgroundColor("black");
}

QWoShower::~QWoShower()
{

}

bool QWoShower::openLocalShell()
{
    QWoShowerWidget *impl = new QWoShellWidgetImpl(this);
    addWidget(impl);
    int idx = m_tabs->addTab("Local");
    m_tabs->setCurrentIndex(idx);
    m_tabs->setTabData(idx, QVariant::fromValue(impl));
    QObject::connect(impl, SIGNAL(destroyed(QObject*)), this, SLOT(onTermImplDestroy(QObject*)));
    setCurrentWidget(impl);
    qDebug() << "tabCount" << m_tabs->count() << ",implCount" << count();
    return true;
}

bool QWoShower::openConnection(const QString &target)
{
    QWoShowerWidget *impl = new QWoTermWidgetImpl(target, this);
    addWidget(impl);
    int idx = m_tabs->addTab(target);
    m_tabs->setCurrentIndex(idx);
    m_tabs->setTabData(idx, QVariant::fromValue(impl));
    QObject::connect(impl, SIGNAL(destroyed(QObject*)), this, SLOT(onTermImplDestroy(QObject*)));
    setCurrentWidget(impl);
    qDebug() << "tabCount" << m_tabs->count() << ",implCount" << count();
    return true;
}

void QWoShower::setBackgroundColor(const QColor &clr)
{
    QPalette pal;
    pal.setColor(QPalette::Background, clr);
    pal.setColor(QPalette::Window, clr);
    setPalette(pal);
}

void QWoShower::openFindDialog()
{
    int idx = m_tabs->currentIndex();
    if (idx < 0 || idx > m_tabs->count()) {
        return;
    }
    QVariant v = m_tabs->tabData(idx);
    QSplitter *target = v.value<QSplitter*>();
//    QSplitter *take = m_terms.at(idx);
//    Q_ASSERT(target == take);
    //    take->toggleShowSearchBar();
}

int QWoShower::tabCount()
{
    return m_tabs->count();
}

void QWoShower::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    QRect rt(0, 0, newSize.width(), newSize.height());
}

void QWoShower::syncGeometry(QWidget *widget)
{
    QRect rt = geometry();
    rt.moveTo(0, 0);
    widget->setGeometry(rt);
}

void QWoShower::closeSession(int idx)
{
    if(idx >= m_tabs->count()) {
        return;
    }
    QVariant v = m_tabs->tabData(idx);
    QWoShowerWidget *target = v.value<QWoShowerWidget*>();
    target->deleteLater();
}

void QWoShower::onTabCloseRequested(int idx)
{
    QMessageBox::StandardButton btn = QMessageBox::warning(this, "CloseSession", "Close Or Not?", QMessageBox::Ok|QMessageBox::No);
    if(btn == QMessageBox::No) {
        return ;
    }
    closeSession(idx);
}

void QWoShower::onTabCurrentChanged(int idx)
{
    if(idx < 0) {
        return;
    }
    QVariant v = m_tabs->tabData(idx);
    QWoTermWidgetImpl *impl = v.value<QWoTermWidgetImpl *>();
    setCurrentWidget(impl);
}

void QWoShower::onTermImplDestroy(QObject *it)
{
    QWoShowerWidget *target = qobject_cast<QWoShowerWidget*>(it);
    for(int i = 0; i < m_tabs->count(); i++) {
        QVariant v = m_tabs->tabData(i);
        QWoShowerWidget *impl = v.value<QWoShowerWidget *>();
        if(target == impl) {
            removeWidget(target);
            m_tabs->removeTab(i);
            break;
        }
    }
    qDebug() << "tabCount" << m_tabs->count() << ",implCount" << count();
    if(tabCount() <= 0) {
        emit tabEmpty();
    }
}

void QWoShower::onTabbarDoubleClicked(int index)
{
    if(index < 0) {
        openLocalShell();
    }
}
