#include "qwoshower.h"
#include "qwotermwidgetimpl.h"
#include "qwoshellwidgetimpl.h"
#include "qwosessionproperty.h"

#include <QTabBar>
#include <QResizeEvent>
#include <QMessageBox>
#include <QtGlobal>
#include <QSplitter>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QIcon>

#define TAB_TYPE_NAME ("tabtype")
#define TAB_TARGET_NAME ("target")

QWoShower::QWoShower(QTabBar *tab, QWidget *parent)
    : QStackedWidget (parent)
    , m_tabs(tab)
{
    QObject::connect(tab, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
    QObject::connect(tab, SIGNAL(currentChanged(int)), this, SLOT(onTabCurrentChanged(int)));
    QObject::connect(tab, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(onTabbarDoubleClicked(int)));
    tab->installEventFilter(this);
}

QWoShower::~QWoShower()
{

}

bool QWoShower::openLocalShell()
{
//    QWoShowerWidget *impl = new QWoShellWidgetImpl(this);
//    impl->setProperty(TAB_TYPE_NAME, ETShell);
//    createTab(impl, "local");
    return true;
}

bool QWoShower::openConnection(const QString &target)
{
    QWoShowerWidget *impl = new QWoTermWidgetImpl(target, this);
    impl->setProperty(TAB_TYPE_NAME, ETSsh);
    impl->setProperty(TAB_TARGET_NAME, target);
    createTab(impl, target);
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
    QWoShowerWidget *target = v.value<QWoShowerWidget*>();
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

void QWoShower::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QRect rt(0, 0, width(), height());
    p.fillRect(rt, QColor(Qt::black));
    QFont ft = p.font();
    ft.setPixelSize(190);
    ft.setBold(true);
    p.setFont(ft);
    QPen pen = p.pen();
    pen.setStyle(Qt::DotLine);
    pen.setColor(Qt::lightGray);
    QBrush brush = pen.brush();
    brush.setStyle(Qt::Dense7Pattern);
    pen.setBrush(brush);
    p.setPen(pen);
    p.drawText(rt, Qt::AlignCenter, "WoTerm");
}

bool QWoShower::eventFilter(QObject *obj, QEvent *ev)
{
    switch (ev->type()) {
    case QEvent::MouseButtonPress:
        return tabMouseButtonPress((QMouseEvent*)ev);
    }
    return false;
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

void QWoShower::createTab(QWoShowerWidget *impl, const QString& tabName)
{
    addWidget(impl);
    int idx = m_tabs->addTab(tabName);
    m_tabs->setCurrentIndex(idx);
    m_tabs->setTabData(idx, QVariant::fromValue(impl));
    QObject::connect(impl, SIGNAL(destroyed(QObject*)), this, SLOT(onTermImplDestroy(QObject*)));
    setCurrentWidget(impl);
    qDebug() << "tabCount" << m_tabs->count() << ",implCount" << count();
}

bool QWoShower::tabMouseButtonPress(QMouseEvent *ev)
{
    QPoint pt = ev->pos();
    int idx = m_tabs->tabAt(pt);
    qDebug() << "tab hit" << idx;
    if(idx >= 0 && ev->buttons().testFlag(Qt::RightButton)) {
        QVariant v = m_tabs->tabData(idx);
        QWoShowerWidget *impl = v.value<QWoShowerWidget*>();
        QVariant type = impl->property(TAB_TYPE_NAME);
        if(type.toInt() == ETSsh) {
            QVariant target = impl->property(TAB_TARGET_NAME);
            qDebug() << "target" << target;
            QWoSessionProperty dlg(QWoSessionProperty::ModifySession, idx, this);
            QObject::connect(&dlg, SIGNAL(connect(const QString&)), this, SIGNAL(readyToConnect(const QString&)));
            dlg.exec();
        }

    }

    return true;
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
    QWoShowerWidget *impl = v.value<QWoShowerWidget *>();
    setCurrentWidget(impl);
}

void QWoShower::onTermImplDestroy(QObject *it)
{
    QWidget *target = qobject_cast<QWidget*>(it);
    for(int i = 0; i < m_tabs->count(); i++) {
        QVariant v = m_tabs->tabData(i);
        QWidget *impl = v.value<QWidget *>();
        if(target == impl) {
            removeWidget(target);
            m_tabs->removeTab(i);
            break;
        }
    }
    qDebug() << "tabCount" << m_tabs->count() << ",implCount" << count();
//    if(tabCount() <= 0) {
//        QMessageBox::StandardButton btn = QMessageBox::warning(this, tr("Info"), tr("The Last session had been close, do you want to exit?"), QMessageBox::Ok|QMessageBox::No);
//        if(btn == QMessageBox::No) {
//            openLocalShell();
//            return ;
//        }
//        QApplication::exit();
//    }
}

void QWoShower::onTabbarDoubleClicked(int index)
{
    if(index < 0) {
        openLocalShell();
    }
}
